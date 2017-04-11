//@uthor Jesus Daniel Neira Lara
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<conio.h>           // it may be necessary to change or remove this line if not using Windows
using namespace cv;
using namespace std;

Mat src, dst;
int thresh = 100;

int main()
{
	src = imread("block1.png", IMREAD_GRAYSCALE);

	namedWindow("Original",WINDOW_AUTOSIZE);
	namedWindow("Elementos", WINDOW_AUTOSIZE);

	imshow("Original", src);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	Scalar color = CV_RGB(255,0,0);

	Canny(src, dst, thresh, thresh * 2);
	findContours(dst, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0)); // Buscar contornos de la imagen

	Mat fill = Mat::zeros(dst.size(), CV_8UC3); // Crear Mat donde iran los contornos a dibujar

	for (size_t i = 0; i < contours.size(); i++)
	{
		drawContours(fill, contours, (int)i, color, 1, 8, hierarchy, 0, Point());
	}

	cout << "Numero de Elementos: " << contours.size() << endl;

	imshow("Elementos", fill);

	waitKey();
	return 0;
}