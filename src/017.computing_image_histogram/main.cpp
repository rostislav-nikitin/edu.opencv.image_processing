#include <iostream>
#include <math.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

class Histogram1D
{
private:
	int histSize[1];
	float hranges[2];
	float const* ranges[1];
	int channels[1];
public:
	Histogram1D()
	{
		histSize[0] = 256;
		hranges[0] = 0.0;
		hranges[1] = 255.0;
		ranges[0] = hranges;
		channels[0] = 1;		
	}

	cv::Mat getHistogram(cv::Mat const &image)
	{
		cv::Mat hist;

		cv::calcHist(&image, 1, 
						0, cv::Mat(), 
						hist, 1, histSize, ranges);

		return hist;
	}

	cv::Mat getImageOfHistogram(cv::Mat const &hist, int zoom = 1)
	{
		double maxValue = 0.0;
		double minValue = 0.0;

		cv::minMaxLoc(hist, &minValue, &maxValue, 0, 0);

		int histSize = hist.rows;

		cv::Scalar BlackColor(0);
		cv::Scalar WhiteColor(255);

		cv::Mat histImage(histSize * zoom, histSize * zoom, CV_8U, WhiteColor);
		int hpt = static_cast<int>(histSize * 0.9);

		for(int idx_row = 0; idx_row < histSize; idx_row++)
		{
			float binValue = hist.at<float>(idx_row);
			if(binValue > 0)
			{
				int intensity = static_cast<int>(binValue * hpt / maxValue);
				cv::line(histImage, cv::Point(idx_row * zoom, histSize * zoom), cv::Point(idx_row * zoom, (histSize - intensity) * zoom), BlackColor, zoom);

			}
		}

		return histImage;
	}
};


int main()
{
	cv::Mat image = cv::imread("puppy-800x452.bmp", cv::IMREAD_GRAYSCALE);

	cv::namedWindow("Original");
	cv::imshow("Original", image);

	//cv::waitKey(0);

	//return 0;

	Histogram1D histogram{};
	cv::Mat result = histogram.getHistogram(image);

	float sum = 0.0;

	for(int idx_row = 0; idx_row < result.rows; idx_row++)
	{
		float histValue = result.at<float>(idx_row);
		if(histValue > 0 || histValue < 0)
		{
			std::cout << "Color=" << idx_row << "; Value=" << histValue << std::endl;
			sum += histValue;
		}
	}

	std::cout << std::endl << "Totals" << std::endl;
	std::cout << "\tPixels count=" << image.rows * image.cols << std::endl;
	std::cout << "\tCalculated pixels count=" << sum << std::endl;

	//return 0;

	cv::Mat histImage = histogram.getImageOfHistogram(result, 1);

	cv::namedWindow("Image histogram");
	cv::imshow("Image histogram", histImage);


	cv::Mat thresholdedImage;
	cv::threshold(image, thresholdedImage, 175, 255, cv::THRESH_BINARY);
	
	cv::namedWindow("Thresholded image");
	cv::imshow("Thresholded image", thresholdedImage);

	cv::waitKey(0);

	return 0;
}
