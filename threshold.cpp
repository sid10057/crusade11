#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include"opencv2/core/core.hpp"
#include<iostream>
#include<math.h>
#include<stdlib.h>
#include<bits/stdc++.h>
using namespace cv;
using namespace std;
Mat img=imread("test4.jpg",1);
Mat img2(img.rows,img.cols,CV_8UC1,Scalar(0));
Mat img1(img.rows,img.cols,CV_8UC1,Scalar(0));

vector<Vec4i>lines;
vector<float>distance;
vector<float>ang;                 //storing all angles in one vector array 'angle'
vector<float>finalang;
int th_canny=250, th_hough_thrdlast=50, th_hough_last=50;
void callback_canny(int tl_can, void *c)
{
    Canny(img1,img2,tl_can,tl_can*3,3);     
    imshow("Canny Thresh",img2);
}
void callback_hough(int t,void *c)
{
    Mat img3(img.rows,img.cols,CV_8UC1,Scalar(0));
    HoughLinesP(img2, lines, 1, CV_PI/180, th_hough_thrdlast, 50, th_hough_last);
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
    namedWindow("colour",WINDOW_NORMAL);
    imshow("colour",img);
    for(int i=0;i<img.rows;i++)
    {
        for(int j=0;j<img.cols;j++)
            img1.at<uchar>(i,j)=(int)(0.21*img.at<Vec3b>(i,j)[2]+0.72*img.at<Vec3b>(i,j)[1]+0.07*img.at<Vec3b>(i,j)[0]);
    }
    for(int i=0;i<img.rows;i++)
    {
        for(int j=0;j<img.cols;j++)
        {
            if(img1.at<uchar>(i,j)>50)
                img1.at<uchar>(i,j)=255;
            else
                img1.at<uchar>(i,j)=0;
        }
    }
    namedWindow("Canny Thresh",WINDOW_NORMAL);
    createTrackbar("Canny","Canny Thresh",&th_canny,300,callback_canny);
    namedWindow("Houghlines",WINDOW_NORMAL);
    createTrackbar("Third Last","Houghlines",&th_hough_thrdlast,150,callback_hough);
    createTrackbar("Last","Houghlines",&th_hough_thrdlast,150,callback_hough);
    waitKey(0);
    return 0;
}
