#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>


int main()
{
	cv::Mat image = cv::imread("puppy-800x452.bmp");

	cv::namedWindow("Original");
	cv::imshow("Original", image);

	cv::Mat kernel(3, 3, CV_32F, cv::Scalar(0));
	kernel.at<float>(0, 1) = -1.0;	
	kernel.at<float>(1, 0) = -1.0;	
	kernel.at<float>(1, 1) = 5.0;	
	kernel.at<float>(1, 2) = -1.0;	
	kernel.at<float>(2, 1) = -1.0;	

	cv::Mat result;

	cv::filter2D(image, result, image.depth(), kernel);

	cv::namedWindow("Result");
	cv::imshow("Result", result);

	cv::waitKey(0);

	return 0;
}
