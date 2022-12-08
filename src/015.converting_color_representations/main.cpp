#include <iostream>
#include <math.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

class ColorDetector
{
private:

	cv::Vec3b _target_color;
	int _max_distance;

	int getDistance(cv::Vec3b color) const
	{
		/*return abs(color[0] - _target_color[0]) +
			abs(color[1] - _target_color[1]) +
			abs(color[2] - _target_color[2]);*/

		return static_cast<int>(cv::norm<int, 3>(cv::Vec3i(color[0] - _target_color[0], color[1] - _target_color[1], color[2] - _target_color[2])));
	}
public:

	ColorDetector(uchar blue, uchar green, uchar red, int max_distance = 100) : _target_color{blue, green, red}, _max_distance{max_distance}
	{
	}

	ColorDetector(cv::Vec3b target_color, int max_distance = 100) :
			_target_color{target_color}, _max_distance{max_distance}
	{
	}

	void setTargetColor(uchar blue, uchar green, uchar red)
	{
		//_target_color = cv::Vec3b{blue, green, red};
			cv::Mat tmp(1, 1, CV_8UC3);
			tmp.at<cv::Vec3b>(0, 0) = cv::Vec3b(red, green, blue);
			cv::cvtColor(tmp, tmp, cv::COLOR_BGR2Lab);

			_target_color = tmp.at<cv::Vec3b>(0, 0);
	}

	void setMaxDistance(int max_distance)
	{
		if(max_distance < 0)
		{
			max_distance = 0;
		}
		else
		{
			_max_distance = max_distance;
		}
	}


	cv::Mat process(cv::Mat const image)
	{
		cv::Mat result;
		result.create(image.size(), CV_8U);

		cv::Mat converted;
		cv::cvtColor(image, converted, cv::COLOR_BGR2Lab);

		cv::Mat_<cv::Vec3b>::const_iterator it_begin = converted.begin<cv::Vec3b>();
		cv::Mat_<cv::Vec3b>::const_iterator it_end = converted.end<cv::Vec3b>();
/*
		cv::Mat_<cv::Vec3b>::const_iterator it_begin = image.begin<cv::Vec3b>();
		cv::Mat_<cv::Vec3b>::const_iterator it_end = image.end<cv::Vec3b>();
*/		
		cv::Mat_<uchar>::iterator it_output = result.begin<uchar>();

		while(it_begin != it_end)
		{
			cv::Vec3b current_color = *it_begin;

			if(getDistance(current_color) < _max_distance)
			{
				*it_output = 255;
			}
			else
			{
				*it_output = 0;
			}

			it_begin++;
			it_output++;
		}

		return result;
	}

	cv::Mat process_absdiff(cv::Mat const image)
	{
		cv::Mat result;
		cv::absdiff(image, cv::Scalar{_target_color}, result);
		std::vector<cv::Mat> channels;
		cv::split(result, channels);

		result = channels[0] + channels[1] + channels[1];
		cv::threshold(result, result, _max_distance, 255, cv::THRESH_BINARY_INV);

		return result;

	}
	
	cv::Mat process_floodfill(cv::Mat image)
	{
		cv::Mat result;
//		cv::floodFill(image, cv::Point(79, 270), cv::Scalar(255, 255, 255), (cv::Rect *)0, cv::Scalar(5, 5, 5), cv::Scalar(5, 5, 5));
		cv::floodFill(image, cv::Point(79, 95), cv::Scalar(255, 255, 255), (cv::Rect *)0, cv::Scalar(48, 48, 48), cv::Scalar(48, 48, 48), cv::FLOODFILL_FIXED_RANGE);
		cv::floodFill(image, cv::Point(158, 258), cv::Scalar(255, 255, 255), (cv::Rect *)0, cv::Scalar(48, 48, 48), cv::Scalar(48, 48, 48), cv::FLOODFILL_FIXED_RANGE);

		return image;

	}


	cv::Mat operator()(cv::Mat const image)
	{
		//return process_floodfill(image);
		return process(image);
	}



};


int main()
{
	cv::Mat image = cv::imread("puppy-800x452.bmp");

	cv::namedWindow("Original");
	cv::imshow("Original", image);

	ColorDetector color_detector{77, 115, 153, 80};
	color_detector.setTargetColor(77, 115, 153);

	cv::Mat result = color_detector(image);

	//image.copyTo(result, result);



	cv::namedWindow("Result");
	cv::imshow("Result", result);

	cv::waitKey(0);

	return 0;
}
