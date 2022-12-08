#include <iostream>
#include <math.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;


void detect(cv::Mat const &image, double minHue, double maxHue, double minSat, double maxSat, cv::Mat &mask)
{
	Mat hsv;
	cvtColor(image, hsv, COLOR_BGR2HSV);
	std::vector<Mat> channels;
	
	split(hsv, channels);

	Mat hueMaxMask;
	threshold(channels[0], hueMaxMask, maxHue, 255, THRESH_BINARY_INV);

	Mat hueMinMask;
	threshold(channels[0], hueMinMask, minHue, 255, THRESH_BINARY);

	Mat hueMask;
	if(minHue < maxHue)
	{
		hueMask = hueMinMask & hueMaxMask;
	}
	else
	{
		hueMask = hueMinMask | hueMaxMask;
	}
	
	//cv::namedWindow("Hue Min Mask");
	//cv::imshow("Hue Min Mask", hueMinMask);
	//cv::namedWindow("Hue Max Mask");
	//cv::imshow("Hue Max Mask", hueMaxMask);
	//cv::namedWindow("Hue Mask");
	//cv::imshow("Hue Mask", hueMask);

	Mat satMaxMask;
	threshold(channels[1], satMaxMask, maxSat, 255, THRESH_BINARY_INV);
	
	Mat satMinMask;
	threshold(channels[1], satMinMask, minSat, 255, THRESH_BINARY);

	Mat satMask = satMaxMask & satMinMask;
	//cv::namedWindow("Sat Mask");
	//cv::imshow("Sat Mask", satMask);

	mask = hueMask & satMask;
}

int main()
{
	cv::Mat image = cv::imread("puppy-800x452.bmp");

	cv::namedWindow("Original");
	cv::imshow("Original", image);

	Mat mask;
	detect(image, 26.0 / 2.0, 43.1 / 2.0, 40.0 * 2.55, 79.6 * 2.55,  mask);
	
	Mat result;
	image.copyTo(result, mask);

	cv::namedWindow("Result");
	cv::imshow("Result", result);

	cv::waitKey(0);

	return 0;
}
