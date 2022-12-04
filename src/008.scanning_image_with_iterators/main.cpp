#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <random>

void reduceColors(cv::Mat image, int div_power)
{
	uchar mask = 0xff << div_power;
	int div = pow(2, div_power);
	int div_shifted = div >> 1;

	cv::Mat_<cv::Vec3b>::iterator it_begin = image.begin<cv::Vec3b>();
	cv::Mat_<cv::Vec3b>::iterator it_end = image.end<cv::Vec3b>();

	for(;it_begin != it_end; it_begin++)
	{
		(*it_begin)[0] = ((*it_begin)[0] & mask) | div_shifted;
		(*it_begin)[1] = ((*it_begin)[1] & mask) | div_shifted;
		(*it_begin)[2] = ((*it_begin)[2] & mask) | div_shifted;
	}
}

int main()
{
	cv::Mat image = cv::imread("puppy-800x452.bmp");
	cv::namedWindow("Original");
	cv::imshow("Original", image);

	reduceColors(image, 6);

	cv::namedWindow("Result");
	cv::imshow("Result", image);

	cv::waitKey(0);
	
	return 0;
}
