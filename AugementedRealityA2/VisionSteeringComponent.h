#pragma once

#include "SteeringComponent.h"
#include "visionCalibration.h"
#include <iostream>

class VisionSteeringComponent : public SteeringComponent
{

private:
    float angleOffset;
    float colorDeviation;
    int minimalMarkerSize;
    float roundnessDeviation;
    bool debugMode;

	VideoCapture webCam;
	vector<HSVRange> colors;
	
    void detectAndDrawLine();
    void processColors(const cv::Mat& imgHSV, cv::Mat& outputImage, std::vector<cv::Point>& points, float minDev, float maxDev);
    void findValidContour(const cv::Mat& mask, std::vector<cv::Point>& validContour);
    void rotatePoint(Point& point, Point origin, float angle);
    void computeAngleAndHeight(const cv::Point& point1, const cv::Point& point2, int frameHeight, float& angle, float& height);
    void drawResults(cv::Mat& image, const cv::Point& point1, const cv::Point& point2, float angle, float height);

public:
	VisionSteeringComponent(VideoCapture webCam, vector<HSVRange> colors);
	~VisionSteeringComponent();

	virtual void update(float elapsedTime) override;
    
    float getAngleOffset() const;
    void setAngleOffset(float value);

    float getColorDeviation() const;
    void setColorDeviation(float value);

    int getMinimalMarkerSize() const;
    void setMinimalMarkerSize(int value);

    float getRoundnessDeviation() const;
    void setRoundnessDeviation(float value);

    bool getDebugMode() const;
    void setDebugMode(bool value);
};

