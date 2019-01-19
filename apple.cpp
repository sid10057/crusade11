#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include"opencv2/core/core.hpp"
#include<stdlib.h>
#include<iostream>
using namespace cv;
using namespace std;
Mat img1=imread("led.jpg",1);
Mat img2(img1.rows,img1.cols,CV_8UC1,Scalar(0));
int t1l,t1h,t2l,t2h,t3l,t3h;
void call(int t,void *c)
{
	int sum=0;
	for(int i=0;i<img2.rows;i++)
	{
		for(int j=0;j<img2.cols;j++)
		{
			int a=img1.at<Vec3b>(i,j)[0];
            int b=img1.at<Vec3b>(i,j)[1];
             int c=img1.at<Vec3b>(i,j)[2];
			if(a<=t3h&&a>=t3l&&b<=t2h&&b>=t2l&&c<=t1h&&c>=t1l)
					img2.at<uchar>(i,j)=0;
			else
					img2.at<uchar>(i,j)=255;
		}
	}
	for(int i=0; i<img2.rows;i++)
	{
		for(int j=0;j<img2.cols;j++)
		{
			if(img2.at<uchar>(i,j)==0)
				sum++;
		}
	}
cout<<sum;
cout<<endl;
 imshow("win",img2);
}

int main()
{
	namedWindow("win",WINDOW_NORMAL);
	createTrackbar("TrackRedLow","win",&t1l,255,call);
	createTrackbar("TrackRedHigh","win",&t1h,255,call);
	createTrackbar("TrackGreenlow","win",&t2l,255,call);
	createTrackbar("TrackGreenHigh","win",&t2h,255,call);
	createTrackbar("TrackBlueLow","win",&t3l,255,call);
	createTrackbar("TrackBlueHigh","win",&t3h,255,call);
    imshow("win",img2);
	waitKey(0);
	return 0;
}
