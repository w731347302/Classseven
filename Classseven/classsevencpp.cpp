#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	Mat src_l1 = imread("lena.jpg");     //Á·Ï°1
	Mat dst_l1;
	Mat src_l2 = imread("metal-part-distorted-03.png");

	
	Point2f center(src_l1.cols / 2.0, src_l1.rows / 2.0);
	int angle = 10;
	int scale = 1;
	Mat rot = getRotationMatrix2D(center, angle, scale);
	Rect bbox = RotatedRect(center, src_l1.size(), angle).boundingRect();
	rot.at<double>(0, 2) += bbox.width / 2.0 - center.x;
	rot.at<double>(1, 2) += bbox.height / 2.0 - center.y;
	warpAffine(src_l1, dst_l1, rot, bbox.size());
	



	//Á·Ï°2
	Mat src_l22;
	src_l2.copyTo(src_l22);
	Mat canny;
	Canny(src_l2, canny, 100, 255);
	
	vector<Vec2f> lines;
	HoughLines(canny, lines, 1, CV_PI / 180, 100);
	vector<Vec2f>::iterator it = lines.begin();
	for (; it != lines.end(); ++it)
	{
		float rho = (*it)[0], theta = (*it)[1];
		Point pt1, pt2;
		double a = cos(theta);
		double b = sin(theta);
		double x0 = a * rho;
		double y0 = b * rho;
		pt1.x = saturate_cast<int>(x0 + 1000 * (-b));
		pt1.y = saturate_cast<int>(y0 + 1000 * (a));
		pt2.x = saturate_cast<int>(x0 - 1000 * (-b));
		pt2.y = saturate_cast<int>(y0 - 1000 * (a));
		line(src_l2, pt1, pt2, Scalar(0, 0, 255), 1);
	}
	

	//Á·Ï°3
	vector<Vec4i> lines2;
	HoughLinesP(canny, lines2, 1, CV_PI / 180, 50,40.0,10.0);
	for (int i = 0; i < lines2.size();i++)
	{
		Vec4i p = lines2[i];
		line(src_l22, Point(p[0], p[1]), Point(p[2], p[3]), Scalar(0, 0, 255),1);
	}

	imshow("dst_l1", dst_l1);
	imshow("canny", canny); 
	imshow("src_l2", src_l2);
	imshow("src_l22", src_l22);
	waitKey(0);
}