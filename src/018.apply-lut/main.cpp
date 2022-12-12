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

	cv::Mat contrastize(cv::Mat const &image, cv::Mat const &hist, float const percentile)
	{
		float pixelsCount = image.total() * percentile;


		std::cout << "Pixels count=" << pixelsCount << std::endl;

		int iMin = 0;
		for(float count = 0.0; iMin < 256; iMin++)
		{
			if((count += hist.at<float>(iMin)) >= pixelsCount)
				break;
		}
		std::cout << "iMin=" << iMin << std::endl;

		int iMax = 255;
		for(float count = 0.0; iMax >= 0; iMax--)
		{
			if((count += hist.at<float>(iMax)) >= pixelsCount)
				break;
		}
		std::cout << "iMax=" << iMax << std::endl;

		int dim[1] = {256};
		cv::Mat lookup(1, dim, CV_8U);
		for(int i = 0; i < 256; i++)
		{
			if(i < iMin)
				lookup.at<uchar>(i) = 0;
			else if(i > iMax)
				lookup.at<uchar>(i) = 255;
			else
				lookup.at<uchar>(i) = cvRound((255.0 / (iMax - iMin)) * (i - iMin));
		}


		cv::Mat result = applyLookUp(image, lookup);

		return result;

	}

	void colorReduce(cv::Mat const &image, int div = 64)
	{
		cv::Mat lookup(1, 256, CV_8U);
		for(int i = 0; i < 256; i++)
		{
			lookup.at<uchar>(i) = i / div * div + div / 2;
		}

		cv::LUT(image, lookup, image);
	}


	cv::Mat applyLookUp(cv::Mat const &image, cv::Mat const &lookUp)
	{
		cv::Mat result;

		cv::LUT(image, lookUp, result);

		return result;
	}

	cv::Mat qualize(cv::Mat const &image)
	{
		cv::Mat result;
		cv::equalizeHist(image, result);

		return result;
	}
};


int main()
{
	cv::Mat image = cv::imread("puppy-800x452.bmp", cv::IMREAD_GRAYSCALE);

	cv::namedWindow("Original");
	cv::imshow("Original", image);



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


	cv::Mat histImage = histogram.getImageOfHistogram(result, 1);

	cv::namedWindow("Image histogram");
	cv::imshow("Image histogram", histImage);

	cv::Mat equalizedImage = histogram.qualize(image);
	cv::namedWindow("Equalized image");
	cv::imshow("Equalized image", equalizedImage);

	cv::Mat equalizedHist = histogram.getHistogram(equalizedImage);
	cv::Mat equalizedHistImage = histogram.getImageOfHistogram(equalizedHist);
	cv::namedWindow("Equalized hist image");
	cv::imshow("Equalized hist image", equalizedHistImage);


/*
	cv::Mat thresholdedImage;
	cv::threshold(image, thresholdedImage, 175, 255, cv::THRESH_BINARY);
	
	cv::namedWindow("Thresholded image");
	cv::imshow("Thresholded image", thresholdedImage);

	cv::Mat lut(256, 1, CV_8U);
	for(int idx_row = 0; idx_row < lut.rows; idx_row++)
	{
		lut.at<uchar>(idx_row) = 255 - idx_row;
	}

	cv::Mat imageAfterLUT = histogram.applyLookUp(image, lut);
	cv::namedWindow("Image after LUT");
	cv::imshow("Image after LUT", imageAfterLUT);
*/
	/*
	cv::Mat contrastImage = histogram.contrastize(image, result, 0.05);
	cv::namedWindow("Contrast Image");
	cv::imshow("Contrast Image", contrastImage);

	cv::Mat contrastHist = histogram.getHistogram(contrastImage);
	cv::Mat contrastHistImage = histogram.getImageOfHistogram(contrastHist);
	cv::namedWindow("Contrast Hist Image");
	cv::imshow("Contrast Hist Image", contrastHistImage);

	cv::Mat colorImage = cv::imread("puppy-800x452.bmp");

	cv::namedWindow("Color Image");
	cv::imshow("Color Image", colorImage);
	*/

/*	std::vector<cv::Mat> channels;
	cv::split(colorImage, channels);

	histogram.colorReduce(channels[0], 192);
	histogram.colorReduce(channels[1], 192);
	histogram.colorReduce(channels[2], 192);

	cv::merge(channels, colorImage);*/

/*
	histogram.colorReduce(colorImage, 192);
	cv::namedWindow("Reduced Colors Image");
	cv::imshow("Reduced Colors Image", colorImage);
*/
	cv::waitKey(0);

	return 0;
}
