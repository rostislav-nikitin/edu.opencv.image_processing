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

	int channels_count = image.channels();

	for(int idx_row = 1; idx_row < image.rows - 1; idx_row++)
	{
		uchar const *previous = image.ptr<const uchar>(idx_row-1);
		uchar const *current = image.ptr<const uchar>(idx_row);
		uchar const *next = image.ptr<const uchar>(idx_row+1);

		uchar *output = image.ptr<uchar>(idx_row);

		for(int idx_col_channel = channels_count; idx_col_channel < ((image.cols - 1) * channels_count); idx_col_channel++)
		{
			*output++ = cv::saturate_cast<uchar>(5.0 * current[idx_col_channel] - 
							current[idx_col_channel - channels_count] - 
							current[idx_col_channel + channels_count] - 
							previous[idx_col_channel] - next[idx_col_channel]);
		}
	}

	image.row(0).setTo(cv::Scalar(0));
	image.row(image.rows - 1).setTo(cv::Scalar(0));
	image.col(0).setTo(cv::Scalar(0));
	image.col(image.cols - 1).setTo(cv::Scalar(0));

	cv::namedWindow("Result");
	cv::imshow("Result", image);

	cv::waitKey(0);

	return 0;
}
