
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include<stdio.h>
#include <stdio.h>
int match_method = 4; //Template Matching Algoritma Secimi 1-5
using namespace std;
using namespace cv;
Mat img; Mat templ; Mat result;
char* image_window = "Source Image";
char* result_window = "Result window";

void MatchingMethod(int, void*);
Mat src_base, hsv_base;
Mat src_test1, hsv_test1;
Mat src_test2, hsv_test2;
Mat hsv_half_down;
/**
* @function main
*/
int x;
int main(int argc, char** argv)
{
	printf("Lutfen yapmak istediginiz islemin numarasini giriniz \n 1-Template Matching \n 2-Histogram Compare\n");
	scanf("%d",&x);
	//template
	if (x == 1) {
		printf("template matching metodu calisiyor");
		namedWindow(image_window, CV_WINDOW_AUTOSIZE);
		namedWindow(result_window, CV_WINDOW_AUTOSIZE);
		img = imread("D:/1/11.png", 1);
		templ = imread("D:/1/11.png", 1);
		MatchingMethod(0, 0);
		waitKey(0);
		return 0;
	}
	//histogram
	else if (x == 2) {
		printf("Histogram Compare Metodu calisiyor");
		src_base = imread("D:/1/11.png", 1);
		src_test1 = imread("D:/1/22.png", 1);
		
		/// Convert to HSV
		cvtColor(src_base, hsv_base, COLOR_BGR2HSV);
		cvtColor(src_test1, hsv_test1, COLOR_BGR2HSV);


		hsv_half_down = hsv_base(Range(hsv_base.rows / 2, hsv_base.rows - 1), Range(0, hsv_base.cols - 1));

		/// Using 50 bins for hue and 60 for saturation
		int h_bins = 50; int s_bins = 60;
		int histSize[] = { h_bins, s_bins };

		// hue varies from 0 to 179, saturation from 0 to 255
		float h_ranges[] = { 0, 180 };
		float s_ranges[] = { 0, 256 };

		const float* ranges[] = { h_ranges, s_ranges };

		// Use the o-th and 1-st channels
		int channels[] = { 0, 1 };

		/// Histograms
		MatND hist_base;
		MatND hist_half_down;
		MatND hist_test1;

		/// Calculate the histograms for the HSV images
		calcHist(&hsv_base, 1, channels, Mat(), hist_base, 2, histSize, ranges, true, false);
		normalize(hist_base, hist_base, 0, 1, NORM_MINMAX, -1, Mat());

		calcHist(&hsv_half_down, 1, channels, Mat(), hist_half_down, 2, histSize, ranges, true, false);
		normalize(hist_half_down, hist_half_down, 0, 1, NORM_MINMAX, -1, Mat());

		calcHist(&hsv_test1, 1, channels, Mat(), hist_test1, 2, histSize, ranges, true, false);
		normalize(hist_test1, hist_test1, 0, 1, NORM_MINMAX, -1, Mat());

		/// Apply the histogram comparison methods
		//for (int i = 0; i < 4; i++)

		int compare_method = 2;
		double base_base = compareHist(hist_base, hist_base, compare_method);
		double base_half = compareHist(hist_base, hist_half_down, compare_method);
		double base_test1 = compareHist(hist_base, hist_test1, compare_method);
		printf(" Methot Chi-Square Kullanildi Referans, Yari foto, Kiyaslanan : %f, %f, %f \n", base_base, base_half, base_test1);
		printf(" Resimlerin benzerlik yuzdesi: %.2f \n ", base_test1 * 100 / 14.083775);
		int x = (base_test1 * 100 / 14.083775);
		if (x > 50) {
			printf("Bu fotograflar ayni bolgede cekilmistir.\n");
		}
		else {
			printf(" GPS i kontrol ediniz. Fotograflar arasinda benzerlik cok az \n");
		}
		waitKey(0);
		return 0;
	}
	//cikis
	else {
		printf("lutfen gecerli bir deger giriniz");

	}


}

void MatchingMethod(int, void*)
{
	/// Source image to display
	Mat img_display;
	img.copyTo(img_display);

	/// Create the result matrix
	int result_cols = img.cols - templ.cols + 1;
	int result_rows = img.rows - templ.rows + 1;

	result.create(result_rows, result_cols, CV_32FC1);

	/// Do the Matching and Normalize
	matchTemplate(img, templ, result, match_method);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	/// Localizing the best match with minMaxLoc
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
	}
	else
	{
		matchLoc = maxLoc;
	}

	/// Show me what you got
	rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
	rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);

	imshow(image_window, img_display);
	imshow(result_window, result);

	return;
}

