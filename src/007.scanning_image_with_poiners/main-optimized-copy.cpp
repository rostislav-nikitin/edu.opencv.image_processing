#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <random>

void reduceColors(cv::Mat const image, cv::Mat &result, int div_power)
{
	result.create(image.rows, image.cols, image.type());

	uchar mask = 0xff << div_power;
	int div = pow(2, div_power);
	int div_shifted = div >> 1;

	int rows_count, components_count;

	rows_count = image.rows;
    components_count = image.cols * image.channels();

	if(image.isContinuous())
	{
		//std::cout << "Continuous = TRUE" << std::endl;
		components_count *= rows_count;
		rows_count = 1;
	}

	for(int row_index = 0; row_index < rows_count; row_index++)
	{
		uchar const *pixel = image.ptr<uchar>(row_index);
		uchar *result_pixel = result.ptr<uchar>(row_index);

		for(int component_index = 0; component_index < components_count; component_index++)
		{
			//*pixel++ = (*pixel & mask) | div_shifted;
			*result_pixel++ = (*pixel++ & mask) | div_shifted;
		}
	}


}

int main()
{
	cv::Mat image = cv::imread("puppy-800x452.bmp");
	cv::Mat result;

	reduceColors(image, result, 6);

	cv::namedWindow("Original");
	cv::imshow("Original", image);

	cv::namedWindow("Result");
	cv::imshow("Result", result);

	cv::waitKey(0);
	
	return 0;
}
