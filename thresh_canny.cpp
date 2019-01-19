#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include"opencv2/core/core.hpp"
#include<iostream>
#include<math.h>
#include<stdlib.h>
#include<bits/stdc++.h>
using namespace cv;
using namespace std;
VideoCapture vid(0);
Mat img;
Mat canny(img.rows,img.cols,CV_8UC1,Scalar(0));

void callback(int tl,void *c)
{
	Canny(img,canny, tl,tl*3,3);
	imshow("Canny", canny);
}
int main()
{
	while(1)
	{
		vid>>img;
		imshow("ga", img);
		int t=0;
		for(int i=0;i<img.rows;i++)
        {
            for(int j=0;j<img.cols;j++)
            {
                img.at<uchar>(i,j)=(int)(0.21*img.at<Vec3b>(i,j)[2]+0.72*img.at<Vec3b>(i,j)[1]+0.07*img.at<Vec3b>(i,j)[0]);
            }
        }   
		namedWindow("Canny", WINDOW_NORMAL);
		createTrackbar("Threshold","Canny",&t,300,callback);
		waitKey(50);
	}
	return 0;
}