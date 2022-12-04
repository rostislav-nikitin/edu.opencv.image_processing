#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <random>

int main()
{
	int div = 64;
	cv::Mat image = cv::imread("puppy-800x452.bmp");


	int rows_count, components_count;

	rows_count = image.rows;
    components_count = image.cols *  image.channels();

	for(int row_index = 0; row_index < rows_count; row_index++)
	{
		uchar *pixel = image.ptr<uchar>(row_index);

		for(int component_index = 0; component_index < components_count; component_index++)
		{
			pixel[component_index] = pixel[component_index] / div * div + div / 2;
		}
	}

	cv::namedWindow("Result");
	cv::imshow("Result", image);
	cv::waitKey(0);
	
	return 0;
}
