//@uthor Jesus Daniel Neira Lara
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<conio.h>           // it may be necessary to change or remove this line if not using Windows
using namespace cv;
using namespace std;

Mat src, dst_fill;
int thresh = 100;
int R = 255;
int G = 0;
int B = 0;
int brl = 7;
int arc = 0;


void dtcRct(int,void*)
{
	vector<vector<Point>> contours;
	vector<vector<Point>> contours_hollow;
	vector<Vec4i> hierarchy;
	vector<Vec4i> hierarchy_hollow;
	Scalar color = CV_RGB(R, G, B);
	Canny(src, dst_fill, thresh, thresh * 2);
	findContours(dst_fill, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0)); // Buscar contornos de la imagen
	findContours(dst_fill, contours_hollow, hierarchy_hollow, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0)); // Buscar contornos de la imagen
	Mat fill = Mat::zeros(dst_fill.size(), CV_8UC3); // Crear Mat donde iran los contornos a dibujar
	Mat hollow = Mat::zeros(dst_fill.size(), CV_8UC3); // Crear Mat donde iran los contornos a dibujar con huecos
	Mat ccl = Mat::zeros(dst_fill.size(), CV_8UC3); // Crear Mat donde iran los Circulos
	Mat rct = Mat::zeros(dst_fill.size(), CV_8UC3); // Crear Mat donde iran los Rectangulos
	Mat area = Mat::zeros(dst_fill.size(), CV_8UC3);
	///------------ Dibujar los contornos de la imagen
	for (size_t j = 0; j < contours_hollow.size(); j++)
	{
		if (arcLength(contours_hollow[j], false) >= 20.0 && arcLength(contours_hollow[j], false) <= 40.0)
		{
			drawContours(hollow, contours_hollow, (int)j, color, 1, 8, hierarchy_hollow, 0, Point());
		}
		else if (hierarchy_hollow[j][2] == -1)
		{
			drawContours(area, contours_hollow, (int)j, color, 2, 8, hierarchy_hollow, 0, Point());
		}
	}
	for (size_t i = 0; i < contours.size(); i++)
	{
		drawContours(fill, contours, (int)i, color, 1, 8, hierarchy, 0, Point());
		if (contours[i].size() <= brl)
		{
			drawContours(rct, contours, (int)i, color, 1, 8, hierarchy, 0, Point());
		}
		else if(contours[i].size() > brl)
		{
			drawContours(ccl, contours, (int)i, color, 1, 8, hierarchy, 0, Point());
		}
	}
	///--------Separar los huecos------------
	hollow = hollow - fill;
	area = fill - area;
	///--------------------------
	///--------Binarisar la imagenes
	cvtColor(area, area, CV_BGR2GRAY);

	threshold(area, area, 30, 255.0, CV_THRESH_BINARY);
	///--------------------------------------
	///--------Sacar contornos nuevos
	vector<vector<Point>> contCircle;
	vector<vector<Point>> contRect;
	vector<vector<Point>> contArea;
	vector<vector<Point>> circlecont;
	vector<vector<Point>> rectcont;

	Mat nar = Mat::zeros(dst_fill.size(), CV_8UC3);
	Mat hcdr = Mat::zeros(dst_fill.size(), CV_8UC3);
	Mat hcrl = Mat::zeros(dst_fill.size(), CV_8UC3);

	findContours(area, contArea, hierarchy_hollow, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	///----------Separa los cuadrados con huecos y los circulos
	for (size_t i = 0; i < contArea.size(); i++)
	{
		if (arcLength(contArea[i],false) > 100.0)
		{
			drawContours(nar, contArea, (int)i, color, 1, 8, hierarchy, 0, Point());
			if (contArea[i].size() <= 7)
			{
				drawContours(hcdr, contArea, (int)i, color, 1, 8, hierarchy, 0, Point());
			}
			else
			{
				drawContours(hcrl, contArea, (int)i, color, 2, 8, hierarchy, 0, Point());
			}
		}
		else
		{
			drawContours(hcrl, contArea, (int)i, color, 2, 8, hierarchy, 0, Point());
		}
	}

	hcrl = ccl - hcrl;
	///-----------------------------------
	///-----------Binarisar las imagenes
	cvtColor(ccl, ccl, CV_BGR2GRAY);
	cvtColor(rct, rct, CV_BGR2GRAY);
	cvtColor(hollow, hollow, CV_BGR2GRAY);
	cvtColor(hcrl, hcrl, CV_BGR2GRAY);
	cvtColor(hcdr, hcdr, CV_BGR2GRAY);

	threshold(ccl, ccl, 30, 255.0, CV_THRESH_BINARY);
	threshold(rct, rct, 30, 255.0, CV_THRESH_BINARY);
	threshold(hollow, hollow, 30, 255.0, CV_THRESH_BINARY);
	threshold(hcrl, hcrl, 30, 255.0, CV_THRESH_BINARY);
	threshold(hcdr, hcdr, 30, 255.0, CV_THRESH_BINARY);
	///-------------Sacar contornos----------------
	findContours(ccl, contCircle, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	findContours(rct, contRect, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	findContours(hollow, contours_hollow, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	findContours(hcrl, circlecont, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	findContours(hcdr, rectcont, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	///--------Mostrar las cantidad de elementos en las imagenes y las imagenes
	cout << "Resultados: " << endl;
	cout << "Total de Elementos: " << contours.size() << endl;
	cout << "Circulos: " << contCircle.size() << endl;
	cout << "Rectangulos: " << contRect.size() << endl;
	cout << "Huecos: " << contours_hollow.size() << endl;
	cout << "Circulos sin huecos: " << circlecont.size() << endl;
	cout << "Rectangulos con 2 Huecos: " << rectcont.size() << endl;
	cout << "Objetos con huecos: " << contArea.size() << endl;

	imshow("Contours", fill);
	imshow("Huecos", hollow);
	imshow("Areas", area);
	imshow("Dos Huecos", nar);
	imshow("Cuadrados con dos huecos", hcdr);
	imshow("Circulos con sin huecos", hcrl);
	imshow("Circulos",ccl);
	imshow("Rectangulos", rct);
}

int main()
{
	src = imread("blocks1.png", IMREAD_GRAYSCALE);
	namedWindow("Original",WINDOW_AUTOSIZE);
	namedWindow("Contours", WINDOW_AUTOSIZE);
	namedWindow("Circulos", WINDOW_AUTOSIZE);
	namedWindow("Rectangulos", WINDOW_AUTOSIZE);
	namedWindow("Huecos", WINDOW_AUTOSIZE);
	imshow("Original",src);


	createTrackbar("Red", "Contours",&R,255,dtcRct);
	createTrackbar("Green", "Contours", &G, 255, dtcRct);
	createTrackbar("Blue", "Contours", &B, 255, dtcRct);
	createTrackbar("Break Line", "Circulos", &brl,30, dtcRct);
	createTrackbar("Arco", "Circulos", &arc, 200, dtcRct);
	dtcRct(0, 0);

	waitKey();
	return 0;
}
