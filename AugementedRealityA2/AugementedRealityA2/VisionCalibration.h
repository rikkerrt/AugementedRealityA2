#pragma once
#include<Vector>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

struct HSVRange {
    string name;
    int hueMin = 0, saturationMin = 0, valueMin = 0;
    int hueMax = 179, saturationMax = 255, valueMax = 255;
};

class VisionCalibration
{
private:
    vector<HSVRange> colors;
    Mat capture;
public: 
    VisionCalibration();
    void addColor(string name);
    void capurePhoto(VideoCapture videoCapture);
    void calibrate();
    void save(string filename);
    void load(string filename);
    vector<HSVRange> getColors();
};