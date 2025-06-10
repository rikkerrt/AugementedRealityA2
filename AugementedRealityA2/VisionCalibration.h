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
    /// <summary>
    /// Constructor
    /// </summary>
    VisionCalibration();

    /// <summary>
    /// Add a color that will be included in the calibration
    /// </summary>
    /// <param name="name">Name of the color to be added</param>
    void addColor(string name);

    /// <summary>
    /// Open a window that waits a few seconds to take a photo and saves this photo
    /// </summary>
    /// <param name="videoCapture">videocapture from which the photo will be taken</param>
    void capurePhoto(VideoCapture videoCapture);

    /// <summary>
    /// Open a window with sliders for each added color to calibrate the color ranges 
    /// </summary>
    void calibrate();

    /// <summary>
    /// Save the current calibration settings into .json
    /// </summary>
    /// <param name="filename">Name of the file</param>
    void save(string filename);

    /// <summary>
    /// Load in calibration settings based on a .json file
    /// </summary>
    /// <param name="filename">Name of the file</param>
    void load(string filename);

    /// <summary>
    /// Retreive the calibrated colors
    /// </summary>
    /// <returns>List of color ranges for each of the calibrated colors</returns>
    vector<HSVRange> getColors();
};