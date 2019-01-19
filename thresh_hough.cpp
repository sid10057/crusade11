#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include"opencv2/core/core.hpp"
#include<iostream>
#include<math.h>
#include<stdlib.h>
#include<bits/stdc++.h>
using namespace cv;
using namespace std;
VideoCapture vid(1);
Mat img, hough;
vector<Vec4i>lines;
vector<float>distance;
vector<float>ang;                 //storing all angles in one vector array 'angle'
vector<float>finalang;
int th_canny, th_hough_thrdlast, th_hough_seclast;
void callback(int t,void *c)
{
	    Mat img3(img.rows,img.cols,CV_8UC1,Scalar(0));
    HoughLinesP(hough, lines, 1, CV_PI/180, th_hough_thrdlast, th_hough_seclast, 10);
    for(int i=0;i<lines.size();i++)
    {
        line(img3,Point(lines[i][0],lines[i][1]),Point(lines[i][2],lines[i][3]), Scalar(255), 1, LINE_4);
        if(lines[i][1]!=lines[i][3])
            ang.push_back((float)((atan(((float)(lines[i][1]-lines[i][3])/(lines[i][2]-lines[i][0]))))*180/3.14));
    }
    imshow("Houghlines", img3);
}
int main()
{
	while(1)
	{
		vid>>img;
		namedWindow("Hough", WINDOW_NORMAL);
		createTrackbar("Threshold 3rd last","Hough",&th_hough_thrdlast,300,callback);
		createTrackbar("Threshold sec last","Hough",&th_hough_seclast,300,callback);
		waitKey(50);
	}
	return 0;
}