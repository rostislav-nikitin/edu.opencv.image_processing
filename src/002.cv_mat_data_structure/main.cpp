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
	cv::Mat image1 {240, 320, CV_8U, 100};
	

	image1.create(200, 200, CV_8U);
	image1 = 100;
	cv::namedWindow("New Image");
	cv::imshow("New Image", image1);


	cv::Mat image2 {240, 320, CV_8UC3, cv::Scalar(0, 0, 255)};
	cv::namedWindow("New Color Image");
	cv::imshow("New Color Image", image2);


	cv::Mat image3 = cv::imread("./puppy-800x452.bmp");
	cv::Mat image4 {image3};

	image1 = image4;

	image3.copyTo(image2);

	cv::Mat image5 = image3.clone();


	cv::flip(image3, image3, 1);



	cv::namedWindow("Flipped Puppy Image");
	cv::imshow("Flipped Puppy Image", image3);

	cv::namedWindow("Assigned Puppy Image");
	cv::imshow("Assigned Puppy Image", image4);




	cv::namedWindow("Copied Puppy Image");
	cv::imshow("Copied Puppy Image", image2);

	cv::namedWindow("Cloned Puppy Image");
	cv::imshow("Cloned Puppy Image", image5);
	



	cv::Mat greyImage = createMat();

	image1 = cv::imread("./puppy-800x452.bmp", cv::IMREAD_GRAYSCALE);
	image1.convertTo(image2, CV_32F, 1/255.0, 0.0);

	cv::Matx33d matrix33 {2.0, 3.0, 1.0,
							2.0, 1.0, 3.0,
							1.0, 2.0, 3.0};
	cv::Matx31d vector3 {1.0, 2.0, 3.0};

	cv::Matx31d result = matrix33 * vector3;

	 

	cv::waitKey(0);


	return 0;
}
