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
		channels[0] = 0;		
	}

	void setSize(int size)
	{
		histSize[0] = size;
	}

	cv::Mat getHistogram(cv::Mat const &image)
	{
		cv::Mat hist;

		cv::Mat hsv;
		cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
		std::vector<cv::Mat> hsvChannels;
		cv::split(hsv, hsvChannels);


		cv::calcHist(&hsvChannels[0], 1, 
						channels, cv::Mat(), 
						hist, 1, histSize, ranges, true, false);

		return hist;
	}

	cv::Mat getImageOfHistogram(cv::Mat const &hist, int zoom = 1)
	{
		double maxValue = 0.0;
		double minValue = hist.total();

		/*std::vector<cv::Mat> histChannels;
		cv::split(hist, histChannels);

		std::cout << "histChannels. Rows="  << histChannels[0].rows << "; Cols=" << histChannels[0].cols << "; Channels=" << histChannels[0].channels() << "; Dims=" << histChannels[0].dims << std::endl;
*/
		cv::minMaxLoc(hist, &minValue, &maxValue, 0, 0);
/*
		for(int idx_row = 0; idx_row < 256; idx_row++)
		{
			float binValue = hist.at<float>(0, idx_row, 0);
			if(minValue > binValue)
				minValue = binValue;
			if(maxValue < binValue)
				maxValue = binValue;
		}
*/
		int histSize = hist.rows;
		//int histSize = 256;

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

	cv::Mat invert(cv::Mat const &image)
	{
		cv::Mat inverse (256, 1, CV_8U);

		for(int i = 0; i < 256; i++)
		{
			inverse.at<uchar>(i, 0) = 255 - i;
		}

		return applyLookUp(image, inverse);
	}
};

class ContentFinder
{
private:
	cv::Mat histogram;
	float hranges[2];
	float const* ranges[3];
	int channels[3];
	float threshold;
public:

	ContentFinder() : threshold{0.1f}
	{
		ranges[0] = hranges;
		ranges[1] = hranges;
		ranges[2] = hranges;
	}

	void setThreshold(float value)
	{
		threshold = value;
	}

	void setHistogram(cv::Mat const &hist)
	{
		cv::normalize(hist, histogram, 1.0);
	}

	cv::Mat find(cv::Mat const &image)
	{
		cv::Mat result;

		hranges[0] = 0.0;
		hranges[1] = 255.0;

		ranges[0] = hranges;

		channels[0] = 0;
		channels[1] = 1;
		channels[2] = 2;

		for(int i = 0; i < histogram.dims; i++)
		{
			cv::calcBackProject(&image, 1, channels, histogram, result, ranges, 255.0);
		}

		if(threshold > 0.0)
			cv::threshold(result, result, 255.0 * threshold, 255.0, cv::THRESH_BINARY);
			//cv::threshold(result, result, 10, 255.0, cv::THRESH_BINARY);

		return result;
	}
};

class ImageComparator
{
private:
	Histogram1D _hist;

	cv::Mat _refHistogram;
public:
	ImageComparator()
	{
	}

	void setBinsCount(int const binsCount)
	{
		_hist.setSize(binsCount);
	}

	void setReferenceImage(cv::Mat const &image)
	{
		_refHistogram = _hist.getHistogram(image);
	}

	double compare(cv::Mat const &image)
	{
		cv::Mat hist =_hist.getHistogram(image);

		double result = cv::compareHist(_refHistogram, hist, cv::HISTCMP_INTERSECT);

		return result;
	}
};

int main()
{
	Histogram1D histogram{};
	cv::Mat image = cv::imread("images/scan.jpg", cv::IMREAD_GRAYSCALE);
	
	cv::namedWindow("Original");
	cv::imshow("Original", image);

	cv::Mat thImage;
	cv::threshold(image, thImage, 190, 255, cv::THRESH_BINARY);
	cv::namedWindow("Th Image");
	cv::imshow("TH Image", thImage);

	cv::Mat intImage;
	cv::integral(image, intImage, CV_32S);

	cv::Mat result(image.rows, image.cols, CV_8U);

	int blockSize = 21;
	int threshold = 10;
	int halfSize = blockSize / 2;

	for(int idx_row = halfSize; idx_row < intImage.rows - halfSize - 1; idx_row++)
	{
		uchar *data = image.ptr<uchar>(idx_row);
		int *idata1 = intImage.ptr<int>(idx_row - halfSize);
		int *idata2 = intImage.ptr<int>(idx_row + halfSize + 1);

		for(int idx_col = halfSize; idx_col < intImage.cols - halfSize - 1; idx_col++)
		{
			int sum = idata2[idx_col + halfSize + 1] - idata2[idx_col - halfSize] - idata1[idx_col + halfSize + 1] + idata1[idx_col - halfSize];
			int avg = sum / (blockSize * blockSize);
			if(data[idx_col] < (avg - threshold))
			{
				data[idx_col] = 0;
			}
			else
			{
				data[idx_col] = 255;
			}
		}
	}

	cv::namedWindow("Result");
	cv::imshow("Result", image);

/*	int x0 = 990, y0 = 506, x1 = 1054, y1 = 531;
	cv::Rect rect(cv::Point(x0, y0), cv::Point(x1, y1));
	cv::Mat imageROI = image(rect);
	
	cv::namedWindow("Image ROI");
	cv::imshow("Image ROI", imageROI);


	cv::Scalar sum = cv::sum(imageROI);
	std::cout << sum << std::endl;


	cv::Mat integralImage;
	cv::integral(image, integralImage, CV_32S);

	int integralSum = integralImage.at<int>(y1, x1) - integralImage.at<int>(y1, x0) - integralImage.at<int>(y0, x1) + integralImage.at<int>(y0, x0);
	std::cout << integralSum << std::endl;*/

	cv::waitKey(0);

	return 0;
}