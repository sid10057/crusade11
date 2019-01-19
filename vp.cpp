#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include"opencv2/core/core.hpp"
#include "rs232.h"
#include<iostream>
#include<math.h>
#include<stdlib.h>
#include<bits/stdc++.h>
#include<unistd.h>
using namespace cv;
using namespace std;
double PI = 3.1416;
double thresh = 50;
int flag=0,a=0,pix_red,pix_green,pix_blue,white_cnt = 0;       
VideoCapture vid(1);
void erosion(Mat *img3,Mat *img4)
{
    for(int i=0;i<(*img3).rows;i++)
    {
        for(int j=0;j<(*img3).cols;j++)
        {
            int arr[10];
            int sum=0;
            int count=0;
            int ans;
            for(int x=0;x<3;x++)
            {
                for(int y=0;y<3;y++)
                {
                    if(i-1+x>=0&&j-1+y>=0&&i-1+x<(*img3).rows&&j-1+y<(*img3).cols)
                    {
                        arr[count]=(*img3).at<uchar>(i-1+x,j-1+y);
                        count++;
                    }
                }
            }
            ans=arr[0];
            for(int x=1;x<count;x++)
            {
                if(arr[x]<ans)
                    ans=arr[x];
            }   
            (*img4).at<uchar>(i,j)=ans;
        }
    }
}
void dilation(Mat *img3,Mat *img4)
{
    for(int i=0;i<(*img3).rows;i++)
    {
        for(int j=0;j<(*img3).cols;j++)
        {
            int arr[10];
            int sum=0;
            int count=0;
            int ans;
            for(int x=0;x<3;x++)
            {
                for(int y=0;y<3;y++)
                {
                    if(i-1+x>=0&&j-1+y>=0&&i-1+x<(*img3).rows&&j-1+y<(*img3).cols)
                    {
                        arr[count]=(*img3).at<uchar>(i-1+x,j-1+y);
                        count++;
                    }
                }
            }
            ans=arr[0];
            for(int x=1;x<count;x++)
            {
                if(arr[x]>ans)
                    ans=arr[x];
            }   
            (*img4).at<uchar>(i,j)=ans;
        }
    }
}
int main()
{
   
    int cport_nr=24;   /* /dev/ttyACM0 (COM1 on windows) */
    int bdrate=9600;       /* 9600 baud */
    char c, mode[]={'8','N','1',0};
    if(RS232_OpenComport(cport_nr, bdrate, mode))
  	{
    	printf("Can not open comport\n");
		return(0);
  	}
  	while(1)
  	{
  		Mat img_col;        //coloured image used to detect LED
        vid>>img_col;
        double tan,theta,thetad;
         Mat img(img_col.rows,img_col.cols,CV_8UC1,Scalar(0));
        Mat img_temp(img_col.rows,img_col.cols,CV_8UC1,Scalar(0));
        Mat img_dest(img_col.rows,img_col.cols,CV_8UC1,Scalar(0));
        Mat img1(img_col.rows,img_col.cols,CV_8UC1,Scalar(0));
        Mat seg(img_col.rows,img_col.cols,CV_8UC1,Scalar(0));        //will give segmented image
        int i,j;
         for(int i=0;i<img_col.rows;i++)
        {
            for(int j=0;j<img_col.cols;j++)
            {
                img.at<uchar>(i,j)=(int)(0.21*img_col.at<Vec3b>(i,j)[2]+0.72*img_col.at<Vec3b>(i,j)[1]+0.07*img_col.at<Vec3b>(i,j)[0]);
            }
        }   
    	for( i=0;i<img.rows;i++)
   		{
        	for( j=0;j<img.cols;j++)
        	{
            	if(img.at<uchar>(i,j)>100)   //TBD
              	  img.at<uchar>(i,j)=255;
            	else
                	img.at<uchar>(i,j)=0;
        	}
    	}
    	erosion(&img,&img_temp);
        dilation(&img_temp,&img);
        GaussianBlur(img,img1,Size(15,15),0);       //TBD
        //Segmentation of LED colour
        for(int i=0;i<img_col.rows;i++)
        {
            for(int j=0;j<img_col.cols;j++)
            {
                pix_blue=img_col.at<Vec3b>(i,j)[0];
                pix_green=img_col.at<Vec3b>(i,j)[1];
                pix_red=img_col.at<Vec3b>(i,j)[2];
                if(pix_blue<=253 && pix_blue>=221 && pix_green<=255 && pix_green>=205 && pix_red<=255 && pix_red>=237)
                {
                    seg.at<uchar>(i,j)=255;
                    white_cnt++;
                }
            }
        }
        if(white_cnt>=200)
        {
            if(flag==0)
                a++;
            flag=1;
        }
        if(flag==1 && white_cnt==0)            //white_cnt should be 0 or  less than some min threshold
        {
            c='S';
            RS232_cputs(cport_nr,&c);
            c='K';
            RS232_cputs(cport_nr,&c);
            flag=0;
            usleep(200000);
            c='F';
            RS232_cputs(cport_nr,&c);
            usleep(200000);
        }
        vector<vector<Point> > contours1;
        vector<Vec4i> hierarchy;
        namedWindow("win1",WINDOW_NORMAL);
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
         	area =  area+contourArea(contours1[i]); 
         	printf("%f",area);
      	} 
      	if(area<thresh)
      	{
      		c ='S';
      		RS232_cputs(cport_nr,&c);
      		cout<<"backwards "<<endl;
            c='B';
            RS232_cputs(cport_nr,&c);
            usleep(250000);              //TBD
      	}
        if(thetad<-10)
        {
        	c ='S';
        	RS232_cputs(cport_nr,&c);
        	cout<<"right by "<<-thetad<<endl;
            c='R';
            RS232_cputs(cport_nr,&c);
            int x=(int)(7.22*abs(thetad));
            usleep(x);
             c='S';
            RS232_cputs(cport_nr,&c);
        } 
        else if(thetad>10)
        {
        	c ='S';
        	RS232_cputs(cport_nr,&c);
            cout<<"left by "<<thetad<<endl;
            c='L';
            RS232_cputs(cport_nr,&c);
            int x=(int)(7.22*abs(thetad));
            usleep(x);
             c='S';
            RS232_cputs(cport_nr,&c);
        }
        else 
        {
        	cout<<"forward "<<endl;
            c='F';
            RS232_cputs(cport_nr,&c);
            usleep(250000);
        }
       	//usleep(500000);
  	}
    return 0;
}