#include <opencv2/opencv.hpp>
#include <iostream>
#include "visioncalibration.h"

using namespace cv;
using namespace std;

void detectAndDrawLine();

// Init some variables
VideoCapture webCam(0);
VisionCalibration cal;

void main() {
	//cal.addColor("Green");
	//cal.addColor("Blue");
	//
	//cal.capurePhoto(webCam);
	//cal.calibrate();
	//cal.save("calibration_settings.json");

	cal.load("calibration_settings.json");
	detectAndDrawLine(0.05);
}

void detectAndDrawLine(float deviation)
{
	Mat webCamCapture, imgHSV;
	vector<HSVRange> colors = cal.getColors();
	vector<Point> points;

	float minDev = 1 - deviation;
	float maxDev = 1 + deviation;

	while (true) {

		points.clear();

		webCam.read(webCamCapture);
		cvtColor(webCamCapture, imgHSV, COLOR_BGR2HSV);

		for (HSVRange color : colors) {

			Mat mask;

			Scalar lower(color.hueMin * minDev, color.saturationMin * minDev, color.valueMin * minDev);
			Scalar upper(color.hueMax * maxDev, color.saturationMax * maxDev, color.valueMax * maxDev);
			inRange(imgHSV, lower, upper, mask);

			vector<vector<Point>> contours;
			vector<Point> validContour;
			Rect boundingBox;

			findContours(mask, contours, RETR_LIST, CHAIN_APPROX_NONE);

			for (vector<Point> contour : contours)
			{
				double area = contourArea(contour);

				if (area > 1000 && contour.size() > 10)
				{
					boundingBox = boundingRect(contour);
					float ellipseArea = (CV_PI * boundingBox.width * boundingBox.height) / 4.0;
					float ratio = ellipseArea / area;

					if (ratio > 0.8 && ratio < 1.2)
					{
						validContour = contour;
					}
				}
			}

			if (!validContour.empty())
			{
				drawContours(webCamCapture, vector<vector<Point>>{validContour}, 0, Scalar(255, 0, 0), 2);
				Rect boundingBoxBlue = boundingRect(validContour);
				points.push_back(Point(boundingBoxBlue.x + boundingBoxBlue.width / 2, boundingBoxBlue.y + boundingBoxBlue.height / 2));
			}
		}

		if (points.size() > 1)
		{
			line(webCamCapture, points[0], points[1], Scalar(0, 0, 0), 3, LINE_AA);

			double deltaX = points[0].x - points[1].x;
			double deltaY = points[0].y - points[1].y;
			double angle = atan2(deltaY, deltaX) * 180.0 / CV_PI;

			string angleText = "Hoek: " + to_string(angle) + " graden";

			putText(webCamCapture, angleText, Point(50, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0), 2);
		}

		cv::imshow("Steering", webCamCapture);
		cv::waitKey(1);
	}
}