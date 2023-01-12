# include "PulseMonitor.h"

PulseMonitor::PulseMonitor() {

}

PulseMonitor::PulseMonitor(int frameRate, int realWidth, int realHeight) {
	videoFrameRate = frameRate;
	this->realHeight = realHeight;
	this->realWidth = realWidth;
	InitializeGaussianPyramid(16);
	SetBandpassFilter();
	SetMask();
	SetBPMBuffer();
}

void PulseMonitor::InitializeGaussianPyramid(int frameType) {
	cv::Mat firstFrame = cv::Mat::zeros(videoHeight, videoWidth, frameType);
	currentDetectionFrame = firstFrame;
	cv::Mat firstGauss = BuildGauss(firstFrame, levels+1)[levels];
	for (int i = 0; i < bufferSize; ++i) {
		cv::Mat_<cv::Vec3b> _G = firstGauss.clone();
		videoGauss.push_back(_G);
	}
	fourierTransformAvg = std::vector<double>(bufferSize, 0);
}

void PulseMonitor::SetBandpassFilter() {
	float floatFPS = (float)videoFrameRate;
	float floatBufferSize = (float)bufferSize;
	for (int i = 0; i < bufferSize; ++i) {
		frequencies.push_back(floatFPS * i / floatBufferSize);
	}
}

void PulseMonitor::SetBPMBuffer() {
	bpmBuffer = std::vector<int>(bpmBufferSize, 0);
}

void PulseMonitor::SetMask() {
	for (int i = 0; i < frequencies.size(); ++i) {
		mask.push_back(frequencies[i] >= minFrequency && frequencies[i] <= maxFrequency);
	}
}

std::vector<cv::Mat> PulseMonitor::BuildGauss(cv::Mat& frame, int lvls) {
	cv::Mat _frame = frame.clone();
	std::vector<cv::Mat> pyramid{ _frame };
	for (int i = 0; i < lvls; ++i) {
		cv::pyrDown(_frame.clone(), _frame);
		pyramid.push_back(_frame);
	}
	return pyramid;
}

void PulseMonitor::GrabPulse(cv::Mat& frame) {
	cv::Rect myROI(floor((realWidth - videoWidth) / 2), floor((realHeight - videoHeight) / 2), videoWidth, videoHeight);
	cv::Mat detectionFrame = frame.clone()(myROI);
	currentDetectionFrame = detectionFrame;
	videoGauss[bufferIndex] = BuildGauss(detectionFrame, levels + 1)[levels];
	std::vector<float> aft = AFT();
	if (bufferIndex % bpmCalculationFrequency == 0) {
		float hz = frequencies[ArgMax(aft)];
		bpmBuffer[bpmBufferIndex] = hz * 60;
		bpmBufferIndex = (bpmBufferIndex + 1) % bpmBufferSize;
	}
	bufferIndex = (bufferIndex + 1) % bufferSize;
}

std::vector<float> PulseMonitor::AFT() {
	std::vector<std::vector<float>> ft(bufferSize);

	for (int i = 0; i < videoGauss[0].rows; ++i) {
		for (int j = 0; j < videoGauss[0].cols; ++j) {
			for (int c = 0; c < 3; ++c) {
				std::vector<float> pixelArray;
				for (int b = 0; b < bufferSize; ++b) {
					pixelArray.push_back((float)videoGauss[b](i, j)[c]);
				}
				cv::dft(pixelArray, pixelArray);
				for (int k = 0; k < bufferSize; ++k) {
					ft[k].push_back(pixelArray[k]);
				}
			}
		}
	}
	
	std::vector<float> aft;
	for (int i = 0; i < bufferSize; ++i) {
		if (mask[i]) {
			aft.push_back(accumulate(ft[i].begin(), ft[i].end(), 0.0) / ft[i].size());
		}
		else
		{
			aft.push_back(0);
		}
	}
	return aft;
}

int PulseMonitor::GetPulse() {
	float pulse = accumulate(bpmBuffer.begin(), bpmBuffer.end(), 0.0) / bpmBuffer.size();
	pulse = (int)pulse;
	return pulse;
}

int PulseMonitor::ArgMax(std::vector<float> v) {
	float MaxValue = -FLT_MAX;
	int index = 0;
	for (int i = 0; i < v.size(); ++i) {
		if (v[i] >= MaxValue) {
			MaxValue = v[i];
			index = i;
		}
	}
	return index;
}

cv::Mat PulseMonitor::GetDetectionFrame() {
	return currentDetectionFrame;
}
