#include <opencv2/opencv.hpp>
#include <iostream>
#include "visioncalibration.h"

using namespace cv;
using namespace std;

void detectAndDrawLine(vector<HSVRange> colors, float angleOffset = 0, float colorDeviation = 10, int minimalMarkerSize = 1000, float roundnessDeviation = 20.0f);

// Init webcam // TODO: to be removed
VideoCapture webCam(0);

void main() {

	VisionCalibration cal;

	/// CALIBRATION ///

	//cal.addColor("Yellow");
	//cal.addColor("Blue");
	//
	//cal.capurePhoto(webCam);
	//cal.calibrate();
	//cal.save("calibration_settings.json");

	/// LOADING IN ///
	cal.load("calibration_settings.json");
	detectAndDrawLine(cal.getColors(), 90);

	webCam.set(CAP_PROP_FRAME_WIDTH, 852);
	webCam.set(CAP_PROP_FRAME_HEIGHT, 480);
}

/// <summary>
/// rotate a point about a given origin by a given numer of degrees
/// </summary>
/// <param name="point">Point to rotate</param>
/// <param name="origin">Origin to rotate about</param>
/// <param name="angle">Angle in degrees to rotate with</param>
/// <returns>Rotated point about the origin</returns>
Point rotate_point(Point point, Point origin, float angle)
{
	float sine = sin(angle * CV_PI / 180.0);
	float cosine = cos(angle * CV_PI / 180.0);

	// translate point back to origin:
	Point transformedPoint(point.x - origin.x, point.y - origin.y);

	// rotate point
	float xnew = transformedPoint.x * cosine - transformedPoint.y * sine;
	float ynew = transformedPoint.x * sine + transformedPoint.y * cosine;

	// translate point back and return
	Point newPoint(xnew + origin.x, ynew + origin.y);
	return newPoint;
}


// TODO: this should eventually become a function that can be polled and return an angle in degrees/radians.

/// <summary>
/// get steering angle from vision
/// </summary>
/// <param name="colors">Colors ranges of the markers</param>
/// <param name="colorDeviation">Amount of color deviation allowed (in %)</param>
/// <param name="markerSize">The minimum size the marker should be (in pixels squared)</param>
void detectAndDrawLine(vector<HSVRange> colors, float angleOffset, float colorDeviation, int minimalMarkerSize, float roundnessDeviation)
{
	Mat webCamCapture, imgHSV, mask;
	vector<Point> points;

	// deviations ((0, 100) -> (0, 50))
	float minDev = 1 - colorDeviation / 200;
	float maxDev = 1 + colorDeviation / 200;

	while (true) 
	{
		points.clear();

		// Convert webcamframe to HSV
		webCam.read(webCamCapture);
		cvtColor(webCamCapture, imgHSV, COLOR_BGR2HSV);

		for (HSVRange color : colors) {

			// Create a mask given the color range
			Scalar lower(color.hueMin * minDev, color.saturationMin * minDev, color.valueMin * minDev);
			Scalar upper(color.hueMax * maxDev, color.saturationMax * maxDev, color.valueMax * maxDev);
			inRange(imgHSV, lower, upper, mask);

			vector<vector<Point>> contours;
			vector<Point> validContour;
			Rect boundingBox;

			// Find all contours in the mask
			findContours(mask, contours, RETR_LIST, CHAIN_APPROX_NONE);

			// Go through all contours and select only 1 valid contour
			for (vector<Point> contour : contours)
			{
				// Calculate contour area
				double area = contourArea(contour);

				// Only allow contours larger than minimalMarkerSize (pixels squared) and a minimal number of points
				if (area > minimalMarkerSize && contour.size() > 10)
				{
					// calculate the area of the marker (assuming an ellipse)
					boundingBox = boundingRect(contour);
					float ellipseArea = (CV_PI * boundingBox.width * boundingBox.height) / 4.0;
					float ratio = ellipseArea / area;

					// Filter contours that are not circulair enough
					if (ratio > 1 - roundnessDeviation / 200 && ratio < 1 + roundnessDeviation / 200)
					{
						validContour = contour;
					}
				}
			}

			if (!validContour.empty())
			{
				// Draw the countour and calculate the centerpoint of the marker
				drawContours(webCamCapture, vector<vector<Point>>{validContour}, 0, Scalar(255, 0, 0), 2);
				Rect boundingBoxBlue = boundingRect(validContour);
				
				Point point(boundingBoxBlue.x + boundingBoxBlue.width / 2, boundingBoxBlue.y + boundingBoxBlue.height / 2);
				points.push_back(point);
			}
		}

		if (points.size() > 1)
		{

			Point origin((points[0].x + points[1].x) / 2, (points[0].y + points[1].y) / 2);

			Point point1 = rotate_point(points[0], origin , angleOffset);
			Point point2 = rotate_point(points[1], origin, angleOffset);

			Point midPoint((point1.x + point2.x) / 2, (point1.y + point2.y) / 2);


			// Draw a line between the two markers
			line(webCamCapture, point1, point2, Scalar(0, 0, 0), 3, LINE_AA);

			// Calculate the angle between the markers and show te result on screen
			double deltaX = point1.x - point2.x;
			double deltaY = point1.y - point2.y;
			double angle = atan2(deltaY, deltaX) * 180.0 / CV_PI;
			
			//string angleText = "Hoek: " + to_string(angle) + " graden";
			//putText(webCamCapture, angleText, Point(50, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0), 2);

			

			string hightText = "hoogte: " + to_string((midPoint.y/(float)webCamCapture.rows));

			putText(webCamCapture, hightText, Point(50, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0), 2);

		}

		// Show the result
		cv::imshow("Steering", webCamCapture);
		cv::waitKey(1);
	}
}