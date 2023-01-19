#pragma once
#include <vector>
#include <numeric>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core/core.hpp"


class PulseMonitor {
private:
	int levels = 3;
	int minFrequency = 1.0;
	int maxFrequency = 2.0;
	int bufferSize = 100;
	int bufferIndex = 0;
	int videoWidth = 160;
	int videoHeight = 120;
	int videoFrameRate = 10;
	int bpmCalculationFrequency = 5;
	int bpmBufferIndex = 0;
	int bpmBufferSize = 10;
	int realHeight;
	int realWidth;
	cv::Mat currentDetectionFrame;
	std::vector<cv::Mat_<cv::Vec3b>> videoGauss;
	std::vector<double> fourierTransformAvg;
	std::vector<float> frequencies;
	std::vector<int> bpmBuffer;
	std::vector<bool> mask;

	void InitializeGaussianPyramid(int frameType);
	void SetBandpassFilter();
	void SetBPMBuffer();
	void SetMask();
	std::vector<float> AFT();
	std::vector<cv::Mat> BuildGauss(cv::Mat& frame, int lvls);
	int ArgMax(std::vector<float> v);

public:
	PulseMonitor();
	PulseMonitor(int frameRate, int realHeight, int realWidth);
	void GrabPulse(cv::Mat& frame);
	int GetPulse();
	cv::Mat GetDetectionFrame();
};