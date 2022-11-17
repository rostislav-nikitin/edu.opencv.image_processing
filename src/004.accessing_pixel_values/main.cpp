#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <random>

void salt(cv::Mat image, int n)
{
	std::default_random_engine generator;
	std::uniform_int_distribution<int> randomRow{0, image.rows - 1};
	std::uniform_int_distribution<int> randomCol{0, image.cols - 1};

	int i = 0, j = 0;

	for(int k = 0; k < n; k++)
	{
		i = randomCol(generator);
		j = randomRow(generator);
/*		if(i < image.cols) 
			i++;
		if(i < image.rows) 
			j++;
*/
//		std::cout << "i=" << i << ", j=" << j << std::endl;


		if(image.type() == CV_8UC1)
		{
			//image.at<uchar>(j, i) = 255;
			cv::Mat_<uchar> img {image};
			img(j, i) = 255;
		}
		else if(image.type() == CV_8UC3) 
		{
			image.at<cv::Vec3b>(j, i)[0] = 255;
			image.at<cv::Vec3b>(j, i)[1] = 255;
			image.at<cv::Vec3b>(j, i)[2] = 255;

		}
	}

}

int main(int argc, char *argv[])
{
//	std::cout << "argc=" << argc << std::endl;
//	std::cout << "argv=" << argv[1] << std::endl;

	if(argc > 1)
	{
		std::string fileName{argv[1]};
		cv::Mat image = cv::imread(fileName);
		salt(image, 3000);

		cv::namedWindow("Result :: salt & pepper");
		cv::imshow("Result :: salt & pepper", image);

		cv::waitKey(0);
	}
	else
	{
		std::cout << "Please, specify image file name as a first argument";
	}
	return 0;
}
