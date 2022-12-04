#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>


int main()
{
	cv::Mat image1 = cv::imread("puppy-800x452.bmp");
	cv::Mat image2 = cv::imread("rain-800x452.bmp", 0);

	cv::namedWindow("Original");
	cv::imshow("Original", image1);

	std::vector<cv::Mat> channels;
	cv::split(image1, channels);
	channels[0] += image2;

	cv::Mat result;
	cv::merge(channels, result);


	cv::namedWindow("Result");
	cv::imshow("Result", result);

	cv::waitKey(0);

	return 0;
}
