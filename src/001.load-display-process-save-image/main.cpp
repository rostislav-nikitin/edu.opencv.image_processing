#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

int main()
{
	cv::Mat image;
	std::cout << "This image is " << image.rows << "x" << image.cols << std::endl;

	image = cv::imread("puppy-800x452.bmp");

	if(image.empty())
	{
	}


	cv::namedWindow("Original Image");
	cv::imshow("Original Image", image);

	cv::Mat result;
	cv::flip(image, result, 1);

	cv::imwrite("output.bmp", result);

	cv::namedWindow("Output Image");
	cv::imshow("Output Image", result);


	cv::waitKey(0);


	return 0;
}
