#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include"opencv2/core/core.hpp"
#include<iostream>
#include<math.h>
#include<stdlib.h>
#include<bits/stdc++.h>
using namespace cv;
using namespace std;
VideoCapture vid("test.webm");
Mat img;
double variance(int x1,int x2,int x3) 
{ 
    // Compute mean (average of elements) 
    float mean = (x1+x2+x3)/3;
  	
    // Compute sum squared  
    // differences with mean. 
    double sqDiff ;   
        sqDiff = ((x1 - mean) * (x1 - mean))+((x2 - mean) * (x2 - mean))+((x2 - mean) * (x2 - mean)); 
    return (sqDiff/3); 
} 		
int main()
{
	while(1)
	{
	vid>>img;
	if(img.empty())
		break;
	Mat img1(img.rows,img.cols,CV_8UC1,Scalar(0));
Mat img2(img.rows,img.cols,CV_8UC1,Scalar(0));
Mat img3(img.rows,img.cols,CV_8UC1,Scalar(0));
int i;int x=0;
for( i=img.rows-1;i>=0;i--)
		{
			int flag=1;
			for(int j=0;j<img.cols;j++)
			{
				if(!((img.at<Vec3b>(i,j)[0]<110)&&(img.at<Vec3b>(i,j)[0]<110)&&(img.at<Vec3b>(i,j)[0]<110)&&(variance(img.at<Vec3b>(i,j)[0],img.at<Vec3b>(i,j)[1],img.at<Vec3b>(i,j)[2])<10)))
					flag=0;		
			}
			if(flag==1)
			{
				for(int k=0;k<img.cols;k++)
				{
					if(img.at<Vec3b>(i,k)[0]>x)
						x=x+img.at<Vec3b>(i,k)[0];
				}	
				break;
			}
		}
	for( ;i>=0;i--)
	{
		for(int j=0;j<img.cols;j++)
		{
			img.at<Vec3b>(i,j)[0]=x/img.cols;
			img.at<Vec3b>(i,j)[1]=x/img.cols;
			img.at<Vec3b>(i,j)[2]=x/img.cols;
		}
	}	
	namedWindow("colour",WINDOW_NORMAL);
	imshow("colour",img);
	for(int i=0;i<img.rows;i++)
		{
			for(int j=0;j<img.cols;j++)
			{
				img1.at<uchar>(i,j)=(int)(0.21*img.at<Vec3b>(i,j)[2]+0.72*img.at<Vec3b>(i,j)[1]+0.07*img.at<Vec3b>(i,j)[0]);
			}
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
	//imshow("grey",img1);
	Canny(img1,img2,25,25*3,3);      //Threshold value(255) determined by inspection 
	//imshow("canny",img2);
	vector<Vec4i>lines;
	HoughLinesP(img2, lines, 1, CV_PI/180, 50, 50, 10 );
	vector<float>distance;
	vector<float>ang;                 //storing all angles in one vector array 'angle'
	vector<float>finalang;
	if(lines.size()==0)
	{
		printf("lines=0\n");
		continue;
	}
	for(int i=0;i<lines.size();i++)
	{
		line(img3,Point(lines[i][0],lines[i][1]),Point(lines[i][2],lines[i][3]), Scalar(255), 1, LINE_4);
		//cout<<lines[i][0]<<" "<<lines[i][1]<<" "<<lines[i][2]<<" "<<lines[i][3]<<endl;
		if(lines[i][1]!=lines[i][3])
			ang.push_back((float)((atan(((float)(lines[i][1]-lines[i][3])/(lines[i][2]-lines[i][0]))))*180/3.14));
	}
	int *arr;
	arr=(int *)malloc((lines.size())*(sizeof(int)));
	for(int i=0;i<ang.size();i++)
		arr[i]=0;
	namedWindow("hough",WINDOW_NORMAL);
	imshow("hough",img3);
	vector<float>::iterator ptr;
	for(int i=0;i<ang.size();i++)
	{
		float x=(*(ang.begin()+i));
		float sum=0;
		int count=0;
		for(int p=i;p<ang.size();p++)
		{
			if((abs((*(ang.begin()+p))-x)<=5)&&(arr[p]==0))	//the final angle array will contain only "really" distinct lines
			{
				sum=sum+(*(ang.begin()+p));					//5 is threshold of angle, determined by inspection
				arr[p]=1;
				count++;
			}	
		}
		if(count>0)finalang.push_back(sum/count);
	}
	sort(finalang.begin(),finalang.end());
	for(ptr=ang.begin();ptr!=ang.end();ptr++)
	{
		//cout<<*ptr<<" ";
	}
	cout<<endl;
	for(ptr=finalang.begin();ptr!=finalang.end();ptr++)
	{
		cout<<*ptr<<" ";
	}
	cout<<endl;
	/*if(finalang.size()==1)									//rotate the bot along the seen edge
	{
		 if(finalang[0]<0)
			//cout<<"ROTATE "<<90+finalang[0]<<" DEGREE ANTICLOCKWISE"<<endl;
		else															
			//cout<<"ROTATE "<<90-finalang[0]<<" DEGREE CLOCKWISE"<<endl;
	}
	else if(finalang.size()==2)								//in a corner ,rotate the bot along most 'Horizontal' type line
	{
		if(abs(finalang[0])<abs(finalang[1]))
		{
			//cout<<"ROTATE "<<90-finalang[0]<<" DEGREE CLOCKWISE"<<endl;
		}
		else
		{
			//cout<<"ROTATE "<<90+finalang[1]<<" DEGREE ANTICLOCKWISE"<<endl;
		}
	}
	else 													//here the bot needs to travel along side walls
	{														//thus neglect the angles which are 'nearly	horizontal'		
			float sum=0;
			int count=0;
			for(int i=0;i<3;i++)
			{
				if(abs(finalang[i])<6)continue;
				if(finalang[i]<0)
				{
					sum+=(-1*(90+finalang[i]));
				}
				else
					sum+=(90-finalang[i]);
				count++;
			}
			float ans=sum/count;
			if(ans<0)
				//cout<<"ROTATE "<<abs(ans)<<" DEGREE ANTICLOCKWISE"<<endl;
			else
				//cout<<"ROTATE "<<abs(ans)<<" DEGREE CLOCKWISE"<<endl;
	}*/
	waitKey(250);
}
vid.release();
destroyAllWindows();
return 0;
}
       