#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <iostream>     // std::cout
#include <algorithm>    // std::stable_sort
#include <vector>  
#include <set>
#include <map>

using std::cout;
using std::cin;
using std::endl;

using namespace cv;
using namespace std;

#define level 127
#define iteration 3
int fixedX = 3;
int fixedY= 10;


Mat src, dst;
#define pll pair<int, int>
#define mp make_pair
int vis[1000][1000];
int regionalval=0;
int region=0;
int limx , limy;

int check(int x, int y)
{
	if(x<0 || y<0 || x>=limx || y>=limy)
		return 0;

	if(src.at<uchar>(x,y)==0 && vis[x][y]==0){
		return 1;
	}
	return 0;
}

void floodFill( int x, int y)
{

	if(x<0 || y<0 || x>=limx || y>=limy || vis[x][y]==1)
		return;
	//cout<<x<<" "<<y<<endl;
	regionalval++;
	
	/*
	if(regionalval>fixedY)
		return ;
	*/

	vis[x][y] =1 ;

	if(check(x-1 , y))
		floodFill(x-1, y);

	if(check(x-1 , y-1))
		floodFill( x-1, y-1);

	if(check(x-1 , y+1))
		floodFill( x-1, y+1);

	if(check(x , y-1))
		floodFill(x, y-1);

	if(check(x , y+1))
		floodFill(x, y+1);

	if(check(x+1 , y))
		floodFill( x+1, y);

	if(check(x+1 , y-1))
		floodFill( x+1, y-1);

	if(check(x+1 , y+1))
		floodFill( x+1, y+1);

	return ;
}

void Thresholding(Mat &src)
{
	for(int i =0; i<src.rows; i+=1)
	{	
		for( int j =0; j<src.cols ; j++)
		{
			int val = src.at<uchar>(i,j);
			if( val > level)
			{
				src.at<uchar>(i,j) = 255;
			}
			else
			{
				src.at<uchar>(i,j) =0;	
			}
		}
		
	}
	imshow("input", src);
}
Mat invert(Mat dst)
{
	for(int i =0; i<dst.rows; i++)
	{
		for(int j =0 ; j<dst.cols; j++)
		{
			if (dst.at<uchar>(i,j) == 255)
				dst.at<uchar>(i,j)  =0;
			else
				dst.at<uchar>(i,j) = 255;
		}

	}
	return dst;
}

Mat procedure(Mat src, Mat kernel, char *num, int *region, int *avg, int firsttime)
{
	vector< pll > positions;

	int value =0;
	for(int i =0; i<src.rows; i+=1)
	{
		int count = 0, ans =0;
		for( int j =0; j<src.cols ; j++)
		{
			int val = src.at<uchar>(i,j);
			if( val == 0)
			{
				positions.push_back(mp(i,j));
			}
		}
	}

	//cout<<positions.size()<<endl;

	
	memset(vis, 0, sizeof vis);
	limx= src.rows, limy = src.cols;
	(*region)=0;
	
	for(int i =0; i<positions.size(); i++)
	{
		int xx = positions[i].first, yy = positions[i].second;
		regionalval=0;
		//cout<<i<<ends<<*region<<endl;;
		//cout<<xx<<" "<<yy<<endl;
		if(check(xx, yy))
			floodFill( xx, yy);

		if(firsttime){
			if( regionalval >= fixedX  && regionalval<= fixedY)
			(*region)++;
		}
		else
			if( regionalval >= fixedX )
			(*region)++;
	}

	
	string format = ".jpg";
	//imwrite("print"+format, src);
	(*num)++;
	//cout<<fixedX<<endl;
	fixedX--;
	fixedY--;

	if(!firsttime){
	(*avg) += (*region);
	}
	cout<<"No of people found in the image : "<< (*region)<<endl;
	

	Mat dst = src.clone();

	Mat pdst;

	dst = invert(dst);

	erode(dst, pdst, kernel);

	pdst = invert(pdst);

	return pdst;
}

int main()
{
	freopen("input.txt" , "r", stdin);
	
	char in[20];
	while(scanf("%s", in)){

	fixedX = 3;
	fixedY = 10;

	src = imread(in, 0);
	
	//test11, test14, test16

	imshow("init", src);
	
	
	Thresholding(src);

	//waitKey(0);
	Mat kernel = Mat::ones(3,3, CV_32F);

	string print="out";
	char num = 'a';
	int avg = 0;
	Mat output[10];
	region = 0;
	
	for(int i =1 ; i<=iteration; i++)
	{
	
	//imshow("eroded", src);
		output[i] = procedure(src, kernel,&num, &region, &avg, ( i==1) ? 1 : 0);
	imshow("special"+num , output[i]);
	src = output[i];
	
	//imwrite("out.jpg" , output[i]);


	}

	cout<<" Final count : "<<avg/(iteration-1)<<endl;
	
	//imshow("output6", src);
	waitKey(0);

	}
}