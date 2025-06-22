#include "VisionSteeringComponent.h"
#include "GameObject.h"
#include "visionCalibration.h"
#include <iostream>

/// <summary>
/// Constructor for VisionSteeringComponent.
/// Initializes webcam and color tracking parameters.
/// </summary>
/// <param name="webCam">VideoCapture object to read frames from</param>
/// <param name="colors">List of HSV color ranges to detect</param>
VisionSteeringComponent::VisionSteeringComponent(VideoCapture webCam, vector<HSVRange> colors)
{
    this->webCam = webCam;
    this->colors = colors;

    angleOffset = 90.0f;
    colorDeviation = 20.0f;
    minimalMarkerSize = 1000;
    roundnessDeviation = 30.0f;
    debugMode = true;
}

/// <summary>
/// Destructor for VisionSteeringComponent.
/// </summary>
VisionSteeringComponent::~VisionSteeringComponent()
{
}

/// <summary>
/// Called each frame to update the vision-based steering angle and height.
/// </summary>
/// <param name="elapsedTime">Time elapsed since last update (not used)</param>
void VisionSteeringComponent::update(float elapsedTime)
{
    detectAndDrawLine();

    std::cout << "Height (Vision): " << height << std::endl;
    std::cout << "Angle (Vision): " << SteeringComponent:: angle << std::endl;
}

/// <summary>
/// Detects color markers and calculates the line, angle, and height between them.
/// Draws debug visuals and updates steering values.
/// </summary>
// Main function
void VisionSteeringComponent::detectAndDrawLine() 
{
    Mat webCamCapture, imgHSV;
    vector<Point> points;

    float angle, height;

    float minDev = 1 - colorDeviation / 200;
    float maxDev = 1 + colorDeviation / 200;

    webCam.read(webCamCapture);
    cvtColor(webCamCapture, imgHSV, COLOR_BGR2HSV);

    processColors(imgHSV, webCamCapture, points, minDev, maxDev);

    if (points.size() > 1) 
    {
        Point origin((points[0].x + points[1].x) / 2, (points[0].y + points[1].y) / 2);
        rotatePoint(points[0], origin, angleOffset);
        rotatePoint(points[1], origin, angleOffset);

        computeAngleAndHeight(points[0], points[1], webCamCapture.rows, angle, height);
        drawResults(webCamCapture, points[0], points[1], angle, height);

        this->angle = angle;
        this->height =height;
    }

    if (debugMode) 
    {
        cv::imshow("Steering", webCamCapture);
        cv::waitKey(1);
    }
}

/// <summary>
/// Processes each specified HSV color range to detect relevant contours,
/// validates them based on size and shape, and appends their center points to the list.
/// </summary>
/// <param name="imgHSV">HSV image for color thresholding</param>
/// <param name="outputImage">Image to draw debug contours on</param>
/// <param name="points">Vector to store detected center points of valid contours</param>
/// <param name="minDev">Minimum deviation multiplier for HSV thresholds</param>
/// <param name="maxDev">Maximum deviation multiplier for HSV thresholds</param>
void VisionSteeringComponent::processColors(
    const Mat& imgHSV,
    Mat& outputImage,
    vector<Point>& points,
    float minDev,
    float maxDev) 
{
    for (const HSVRange& color : colors) 
    {
        Mat mask;
        Scalar lower(color.hueMin * minDev, color.saturationMin * minDev, color.valueMin * minDev);
        Scalar upper(color.hueMax * maxDev, color.saturationMax * maxDev, color.valueMax * maxDev);
        inRange(imgHSV, lower, upper, mask);

        vector<Point> validContour;
        findValidContour(mask, validContour);

        if (!validContour.empty()) 
        {
            drawContours(outputImage, vector<vector<Point>>{validContour}, 0, Scalar(255, 0, 0), 2);
            Rect boundingBox = boundingRect(validContour);
            Point center(boundingBox.x + boundingBox.width / 2, boundingBox.y + boundingBox.height / 2);
            points.push_back(center);
        }
    }
}

/// <summary>
/// Finds a valid contour within the given binary mask that matches the area and roundness criteria.
/// </summary>
/// <param name="mask">Binary mask to search for contours</param>
/// <param name="validContour">Output parameter to store the most valid contour found</param>
void VisionSteeringComponent::findValidContour(const Mat& mask, vector<Point>& validContour) 
{
    vector<vector<Point>> contours;
    findContours(mask, contours, RETR_LIST, CHAIN_APPROX_NONE);

    for (const vector<Point>& contour : contours) 
    {
        double area = contourArea(contour);
        if (area > minimalMarkerSize && contour.size() > 10) 
        {
            Rect boundingBox = boundingRect(contour);
            float ellipseArea = (CV_PI * boundingBox.width * boundingBox.height) / 4.0;
            float ratio = ellipseArea / area;
            if (ratio > 1 - roundnessDeviation / 200 && ratio < 1 + roundnessDeviation / 200) 
            {
                validContour = contour;
            }
        }
    }
}

/// <summary>
/// Rotates a point around a specified origin by a given angle.
/// The rotation is performed in 2D space using standard rotation matrix transformation.
/// </summary>
/// <param name="point">Reference to the point to rotate (modified in-place)</param>
/// <param name="origin">The center of rotation</param>
/// <param name="angle">Angle in degrees to rotate the point</param>
void VisionSteeringComponent::rotatePoint(Point& point, Point origin, float angle)
{
    float sine = sin(angle * CV_PI / 180.0);
    float cosine = cos(angle * CV_PI / 180.0);

    point = Point(point.x - origin.x, point.y - origin.y);

    float xnew = point.x * cosine - point.y * sine;
    float ynew = point.x * sine + point.y * cosine;

    point = Point(xnew + origin.x, ynew + origin.y);
}

/// <summary>
/// Computes the angle between two points and the normalized height of their midpoint
/// relative to the frame height.
/// </summary>
/// <param name="point1">First point</param>
/// <param name="point2">Second point</param>
/// <param name="frameHeight">Height of the camera frame</param>
/// <param name="angle">Output parameter for computed angle in degrees</param>
/// <param name="height">Output parameter for normalized vertical midpoint position</param>
void VisionSteeringComponent::computeAngleAndHeight(const Point& point1, const Point& point2, int frameHeight, float& angle, float& height) 
{
    double deltaX = point1.x - point2.x;
    double deltaY = point1.y - point2.y;
    angle = atan2(deltaY, deltaX) * 180.0 / CV_PI;

    Point midPoint((point1.x + point2.x) / 2, (point1.y + point2.y) / 2);
    height = midPoint.y / static_cast<float>(frameHeight);
}

/// <summary>
/// Draws a line between two detected points and annotates the image with calculated
/// steering angle and height information.
/// </summary>
/// <param name="image">Image to draw on</param>
/// <param name="point1">First point</param>
/// <param name="point2">Second point</param>
/// <param name="angle">Steering angle to annotate</param>
/// <param name="height">Normalized height to annotate</param>
void VisionSteeringComponent::drawResults(Mat& image, const Point& point1, const Point& point2, float angle, float height) 
{
    line(image, point1, point2, Scalar(0, 0, 0), 3, LINE_AA);

    string angleText = "Angle: " + to_string(angle) + " degrees";
    putText(image, angleText, Point(25, 50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 0), 2);

    string heightText = "Height: " + to_string(height);
    putText(image, heightText, Point(25, 80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 0), 2);
}

// Getters and Setters
float VisionSteeringComponent::getAngleOffset() const { return angleOffset; }
void VisionSteeringComponent::setAngleOffset(float value) { angleOffset = value; }

float VisionSteeringComponent::getColorDeviation() const { return colorDeviation; }
void VisionSteeringComponent::setColorDeviation(float value) { colorDeviation = value; }

int VisionSteeringComponent::getMinimalMarkerSize() const { return minimalMarkerSize; }
void VisionSteeringComponent::setMinimalMarkerSize(int value) { minimalMarkerSize = value; }

float VisionSteeringComponent::getRoundnessDeviation() const { return roundnessDeviation; }
void VisionSteeringComponent::setRoundnessDeviation(float value) { roundnessDeviation = value; }

bool VisionSteeringComponent::getDebugMode() const { return debugMode; }
void VisionSteeringComponent::setDebugMode(bool value) { debugMode = value; }