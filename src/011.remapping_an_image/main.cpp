#include <iostream>
#include <math.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>


int main()
{
	cv::Mat image = cv::imread("puppy-800x452.bmp");
	cv::Mat srcX (image.rows, image.cols, CV_32F);
	cv::Mat srcY (image.rows, image.cols, CV_32F);
	cv::Mat result;

	cv::namedWindow("Original");
	cv::imshow("Original", image);

	for(int y = 0; y < image.rows; y++)
	{
		for(int x = 0; x < image.cols; x++)
		{
			srcX.at<float>(y, x) = x;
			srcY.at<float>(y, x) = y + 3 * sin(x / 6.0);
		}
	}

	cv::remap(image, result, srcX, srcY, cv::INTER_LINEAR);

	cv::namedWindow("Result");
	cv::imshow("Result", result);

	cv::waitKey(0);

	return 0;
}
