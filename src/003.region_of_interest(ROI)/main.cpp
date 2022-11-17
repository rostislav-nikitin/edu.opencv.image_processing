#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

cv::Mat createMat()
{
	return cv::Mat {500, 500, CV_8U, 50};
}

int main()
{
	cv::Mat image = cv::imread("puppy-800x452.bmp");
	cv::Mat logo = cv::imread("OpenCV_Logo-162x200.png");
	cv::Mat mask = cv::imread("OpenCV_Logo-162x200-mask.png");

	cv::Mat roi {image, cv::Rect{image.cols - logo.cols - 16, image.rows - logo.rows - 16, logo.cols, logo.rows}};
//	cv::Mat roi {image(cv::Range{image.rows - logo.rows, image.rows}, cv::Range{image.cols - logo.cols, image.cols})};


//	cv::Mat mask{logo};

	logo.copyTo(roi, mask);


	cv::namedWindow("result");
	cv::imshow("result", image);
	cv::waitKey(0);



	return 0;
}
