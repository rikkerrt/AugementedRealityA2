#include "VisionCalibration.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <chrono>
#include "json.hpp"

using namespace cv;
using namespace std;
using namespace std::chrono;
using json = nlohmann::json;

VisionCalibration::VisionCalibration() {

}

/// <summary>
/// Add a color that w
/// </summary>
/// <param name="name"></param>
void VisionCalibration::addColor(string name) {
	HSVRange range;
	range.name = name;
	colors.push_back(range);
}

void VisionCalibration::capurePhoto(VideoCapture videoCapture) {
	auto startTime = steady_clock::now();
	int countDown = 5;

	while (countDown > 0) {
		auto currentTime = steady_clock::now();
		auto elapsed = currentTime - startTime;

		// Update the countdown every second
		if (elapsed >= seconds(1)) {
			startTime = steady_clock::now();
			countDown--;
		}

		// Read from webcam and show countdown
		videoCapture.read(capture);
		rectangle(capture, Point(25, 10), Point(250, 40), Scalar(255, 255, 255), FILLED);
		putText(capture, "Taking picture in: " + to_string(countDown), Point(30, 30),
			FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0, 0, 0), 1);

		imshow("Calibration", capture);
		waitKey(1);
	}

	// "Take" photo
	videoCapture.read(capture);
	destroyWindow("Calibration");
}

void VisionCalibration::calibrate() {

	Mat captureHSV;
	cvtColor(capture, captureHSV, COLOR_BGR2HSV);

	// Create a window with trackbars for each color
	for (HSVRange& color : colors) {

		string windowName = "Calibration " + color.name;
		namedWindow(windowName);

		createTrackbar(color.name + " VMin", windowName, &color.valueMin, 255);
		createTrackbar(color.name + " VMax", windowName, &color.valueMax, 255);
		createTrackbar(color.name + " SMin", windowName, &color.saturationMin, 255);
		createTrackbar(color.name + " SMax", windowName, &color.saturationMax, 255);
		createTrackbar(color.name + " HMin", windowName, &color.hueMin, 179);
		createTrackbar(color.name + " HMax", windowName, &color.hueMax, 179);
	}

	// Use the trackbars to mask the image
	while (true) {

		for (HSVRange& color : colors) {

			Mat result, mask;

			// create a masks
			Scalar lower(color.hueMin, color.saturationMin, color.valueMin);
			Scalar upper(color.hueMax, color.saturationMax, color.valueMax);
			inRange(captureHSV, lower, upper, mask);

			// apply masks
			result = Mat::zeros(capture.size(), capture.type());
			capture.copyTo(result, mask);

			// Show results
			imshow("Calibration " + color.name, result);

		}

		// Pressing 's' exits the current configuration // TODO turn this into a button of sorts
		if (waitKey(1) == 's')
		{
			break;
		}
	}

	for (HSVRange& color : colors) {
		destroyWindow("Calibration " + color.name);
	}
}

void VisionCalibration::save(string filename) {
	
	ofstream file;
	file.open(filename);
	
	if (!file.is_open()) {
		cerr << "Unable to open file for writing.\n";
		return;
	}
	
	file << "{\n";
	int count = 0;
	
	for (HSVRange color : colors) {
		file << "  \"" << color.name << "\": {\n";
		file << "    \"hueMin\": " << color.hueMin << ",\n";
		file << "    \"hueMax\": " << color.hueMax << ",\n";
		file << "    \"valueMin\": " << color.valueMin << ",\n";
		file << "    \"valueMax\": " << color.valueMax << ",\n";
		file << "    \"saturationMin\": " << color.saturationMin << ",\n";
		file << "    \"saturationMax\": " << color.saturationMax << "\n";
		file << "  }";

		if (++count < colors.size()) {
			file << ",\n";
		}
	}
	
	file << "}\n";
	file.close();
}

int extractValue(const string& line) {
	size_t colon = line.find(":");
	if (colon == string::npos) return 0;
	string numStr = line.substr(colon + 1);
	numStr.erase(remove(numStr.begin(), numStr.end(), ','), numStr.end());  // Remove trailing comma
	return stoi(numStr);
}

string trim(const string& s) {
	size_t start = s.find_first_not_of(" \t\r\n");
	size_t end = s.find_last_not_of(" \t\r\n");
	return (start == string::npos || end == string::npos) ? "" : s.substr(start, end - start + 1);
}

void VisionCalibration::load(string filename) {
	std::vector<HSVRange> ranges;

	try {
		// Read the JSON file
		std::ifstream file(filename);
		json data = json::parse(file);

		// Iterate through each color in the JSON
		for (auto& element : data.items()) {
			HSVRange range;
			range.name = element.key();
			json values = element.value();

			range.hueMin = values["hueMin"];
			range.hueMax = values["hueMax"];
			range.saturationMin = values["saturationMin"];
			range.saturationMax = values["saturationMax"];
			range.valueMin = values["valueMin"];
			range.valueMax = values["valueMax"];

			ranges.push_back(range);
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Error loading JSON file: " << e.what() << std::endl;
	}

	colors = ranges;
}

vector<HSVRange> VisionCalibration::getColors() {
	return colors;
}