#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include"opencv2/core/core.hpp"
#include "rs232.h"
#include<iostream>
#include<math.h>
#include<stdlib.h>
#include<bits/stdc++.h>
using namespace cv;
using namespace std;
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
/*All the thresholds:
1. Thresholds for segmentation of LED colour
2. pix_thresh: the threshold for how many white pixels there have to be there in segmented image for the LED to come close enough
3. threshold angle for horizon in lines=1,2 and 3 cases
4. end of the code waitKey according to the timne the bot will take to turn a given quanta angle    */
int flag=0, a=0,low_red,high_red,low_blue,high_blue,low_green,high_green,pix_red,pix_green,pix_blue,pix_thresh;       
/*flag gives tells whether there is LED or no. a will give the status of number of LEDs detected. pix_colour gives a pixel value.
pix_thresh gives the threshold pixel value above which we will consider bot to be close enough to LED. The other 2 variables are used to segment the LED*/
VideoCapture vid(1);			
int left_cnt=0;
int right_cnt=0;
int white_cnt=0;  
int main()
{
	
    int cport_nr=24;   /* /dev/ttyACM0 (COM1 on windows) */
    int bdrate=9600;       /* 9600 baud */

  	char c, mode[]={'8','N','1',0};
	while(1)
	{
		Mat img_col;		//coloured image used to detect LED
		vid>>img_col;
		Mat img1(img_col.rows,img_col.cols,CV_8UC1,Scalar(0));		//greyscale image used for path detection
		Mat img_temp(img1.rows,img1.cols,CV_8UC1,Scalar(0));		//temporary image used for erosion dialation
		Mat img2(img1.rows,img1.cols,CV_8UC1,Scalar(0));		//used for canny
		Mat img3(img1.rows,img1.cols,CV_8UC1,Scalar(0));		//used for hough
		//greyscale conversion
		for(int i=0;i<img_col.rows;i++)
		{
			for(int j=0;j<img_col.cols;j++)
			{
				img1.at<uchar>(i,j)=(int)(0.21*img_col.at<Vec3b>(i,j)[2]+0.72*img_col.at<Vec3b>(i,j)[1]+0.07*img_col.at<Vec3b>(i,j)[0]);
			}
		}	
		for(int i=0;i<img_col.rows;i++)
		{
			for(int j=0;j<img_col.cols;j++)
			{
				if(img1.at<uchar>(i,j)>50)	//shadow removal & segmentation
					img1.at<uchar>(i,j)=255;
			}
		}
		//Segmentation of LED colour
        for(int i=0;i<img_col.rows;d++)
        {
            for(int j=0;j<img_col.cols;e++)
            {
                pix_blue=img_col.at<Vec3b>(i,j)[0];
                pix_green=img_col.at<Vec3b>(i,j)[1];
                pix_red=img_col.at<Vec3b>(i,j)[2];
                if(pix_blue<=high_blue && pix_blue>=low_blue && pix_green<=high_green && pix_green>=low_green && pix_red<=high_red && pix_red>=low_red)
                {
                    seg.at<uchar>(i,j)=255;
                    white_cnt++;
                }
            }
        }
        if(white_cnt>=pix_thresh)
        {
            if(flag==0)
                a++;
            flag=1;
        }
        if(flag==1 && white_cnt==0)            //white_cnt should be 0 or  less than some min threshold
        {
            if(RS232_OpenComport(cport_nr,bdrate,mode))        //check if port is open
                continue;
            c='S';
            RS232_cputs(cport_nr,&c);
            c='K'
            RS232_cputs(cport_nr,&c);
            flag=0;
        }
		erosion(&img1,&img_temp);
		dilation(&img_temp,&img1);
		Canny(img1,img2,255,255*3,3);      //Threshold value(255) determined by inspection 
		namedWindow("canny",WINDOW_NORMAL);
		imshow("canny",img2);
		vector<Vec4i>lines;
		HoughLinesP(img2, lines, 1, CV_PI/180, 100, 50, 10 );
		vector<float>length;
		vector<float>ang;                 //storing all angles in one vector array 'angle'
		vector<float>finalang;				//stores the final angles
		vector<float>finallen;
		for(int i=0;i<lines.size();i++)
		{
			line(img3,Point(lines[i][0],lines[i][1]),Point(lines[i][2],lines[i][3]), Scalar(255), 1, LINE_4);
			cout<<lines[i][0]<<" "<<lines[i][1]<<" "<<lines[i][2]<<" "<<lines[i][3]<<endl;
			length.push_back((float)(sqrt((lines[i][0]-lines[i][2])*(lines[i][0]-lines[i][2])+(lines[i][1]-lines[i][3])*(lines[i][1]-lines[i][3]))));
			ang.push_back((float)((atan(((float)(lines[i][1]-lines[i][3])/(lines[i][2]-lines[i][0]))))*180/3.14));
		}
		int *arr;
		arr=(int *)malloc((lines.size())*(sizeof(int)));
		for(int i=0;i<ang.size();i++)
			arr[i]=0;
		namedWindow("hough",WINDOW_NORMAL);
		imshow("hough",img3);
		vector<float>::iterator ptr;
		//condition for no lines detected
		if(ang.size()==0)
		{
			printf("no line\n");
			continue;
		}
		for(int i=0;i<ang.size();i++)
		{
			float x=(*(ang.begin()+i));
			float sum=0;
			float sum2=0;
			int count=0;
			for(int p=i;p<ang.size();p++)
			{
				if((abs((*(ang.begin()+p))-x)<=5)&&(arr[p]==0))	//the final angle array will contain only "really" distinct lines
				{
					sum=sum+(*(ang.begin()+p));					//5 is threshold of angle, determined by inspection
					sum2=sum2+(*(length.begin()+p));
					arr[p]=1;
					count++;
				}	
			}
			if(count>0)
				{
					finalang.push_back(sum/count);
					finallen.push_back(sum2);
				}
		}
		for(ptr=ang.begin();ptr!=ang.end();ptr++)
		{
			cout<<*ptr<<" ";
		}
		cout<<endl;
		for(ptr=finalang.begin();ptr!=finalang.end();ptr++)
		{
			cout<<*ptr<<" ";
		}
		cout<<endl;
		if(finalang.size()==3)
		{
			if(RS232_OpenComport(cport_nr,bdrate,mode))		//check if port is open
				continue;
			int j=0;
			if(abs(finalang[1])<abs(finalang[j]))
				j=1;
			if(abs(finalang[2])<abs(finalang[j]))
				j=2;
			int i;int k;
			if((finalang[0]>0)&&(j!=0))
				i=0;	
			if((finalang[1]>0)&&(j!=1))
				i=1;
			if((finalang[2]>0)&&(j!=2))
				i=2;
			
			if(abs(finalangle[j])<15)						//15 is threshold 
			{
				if(finallen[0]>finallen[2])
					right_cnt++;
				else
					left_cnt++;
				c='F';
				RS232_cputs(cport_nr,&c);
			}
		}
		//we have to find logic for another case
		else if(finalang.size()==1)									//rotate the bot along the seen edge
		{
			c='S';
		 	RS232_cputs(cport_nr,&c);	
			 if(abs(finalang[0])<10)
			 {
			 	if(right_cnt>left_cnt)
			 	{
			 		c='R';
			 		RS232_cputs(cport_nr,&c);
			 		waitKey(2000);		//tbd
			 		c='S';
			 		RS232_cputs(cport_nr,&c);		
			 	}
			 	else
			 	{
			 		c='L';
			 		RS232_cputs(cport_nr,&c);
			 		waitKey(2000);		//tbd
			 		c='S';
			 		RS232_cputs(cport_nr,&c);		
			 	}
			 	left_cnt=0;
			 	right_cnt=0;
			 }
			else															
			{
				if(finalang[0]>0)
				{
			 		c='R';
			 		RS232_cputs(cport_nr,&c);
			 		waitKey(1000);		//tbd
			 		c='S';
			 		RS232_cputs(cport_nr,&c);		
			 	}
			 	else
			 	{
			 		c='L';
			 		RS232_cputs(cport_nr,&c);
			 		waitKey(1000);		//tbd
			 		c='S';
			 		RS232_cputs(cport_nr,&c);		
			 	}	

			}
		}
		else if(finalang.size()==2)								//in a corner ,rotate the bot along most 'Horizontal' type line
		{

			if(RS232_OpenComport(cport_nr,bdrate,mode))
				continue;
			c='S';
			RS232_cputs(cport_nr,&c);
			waitKey(1000);
			int j;
			if(abs(finalang[0])>15)      //15 is a threshold
				j=0;
			else
				j=1;
			int i;
			if(finalang[j]>0)
			{
				c='R';
				RS232_cputs(cport_nr,&c);
				waitKey(1000);
				c='S';
			 	RS232_cputs(cport_nr,&c);				//threshold
			}
			else
			{
				c='L';
				RS232_cputs(cport_nr,&c);
				waitKey(1000);	
				c='S';
			 	RS232_cputs(cport_nr,&c);			//threshold
			}	
			
		waitKey(2000);
	}
	return 0;
}