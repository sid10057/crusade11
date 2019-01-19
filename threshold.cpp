#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include"opencv2/core/core.hpp"
#include<iostream>
#include<math.h>
#include<stdlib.h>
#include<bits/stdc++.h>
using namespace cv;
using namespace std;
Mat img_col=imread("con3.jpg"); 
double PI = 3.1426;
Mat img(img_col.rows,img_col.cols,CV_8UC1,Scalar(0));
Mat img2(img.rows,img.cols,CV_8UC1,Scalar(0));
Mat img1(img.rows,img.cols,CV_8UC1,Scalar(0));
Mat img_temp(img.rows,img.cols,CV_8UC1,Scalar(0));
double thresh = 10000;
int main()
{
    GaussianBlur(img,img_temp,Size(9,9),0);
    medianBlur(img_temp,img,9);
    vector<vector<Point> > contours1;
        vector<Vec4i> hierarchy;
         double tan,theta,thetad;
       // namedWindow("win1",WINDOW_NORMAL);
         for(int i=0;i<img_col.rows;i++)
        {
            for(int j=0;j<img_col.cols;j++)
            {
                img.at<uchar>(i,j)=(int)(0.21*img_col.at<Vec3b>(i,j)[2]+0.72*img_col.at<Vec3b>(i,j)[1]+0.07*img_col.at<Vec3b>(i,j)[0]);
            }
        }   
         Canny(img,img1,25,25*3,3);      //Th
         // threshold value(255); // determined by inspection
         imshow("cann55y",img1);
        // namedWindow("canny",WINDOW_NORMAL);
        findContours(img1,contours1,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE,Point(0,0));
        Mat drawing1=Mat::zeros(img1.size(),CV_8UC3);
           for( int i = 0; i< contours1.size(); i++ )
            {
                Scalar color( 255, 255 , 255 );
                drawContours(drawing1, contours1, i, color, 2, 8, hierarchy, 0, Point() );
            }
        imshow("win1",drawing1);
        Mat grey1,binary1;
        cvtColor( drawing1, grey1, COLOR_BGR2GRAY );
        threshold( grey1, binary1, 50,255,THRESH_BINARY );
        Moments m1 = moments(binary1,true);
        int cx1=m1.m10/m1.m00;
        int cy1=m1.m01/m1.m00;
        tan=(double)2*(binary1.cols/2-cx1)/binary1.rows;
        theta=atan(tan);
        thetad=theta*180/PI;
        float area=0 ;
        for (unsigned int i = 0;  i < contours1.size();  i++)
        {
            printf("%d\n",contours1.size());
            area =  area+contourArea(contours1[i]); 
            printf("%f\n",area);
        } 
        waitKey(0);
        return 0;
}
