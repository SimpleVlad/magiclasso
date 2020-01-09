#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <math.h>
#include <list>
#include <vector>
#include <algorithm>

using namespace cv;
using namespace std;

// Mat dst, mask;
// int startX = NULL;
// int startY = NULL;

/*void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{

      if ((event == EVENT_MOUSEMOVE) && (flags & EVENT_FLAG_LBUTTON)){
        if ((startX == NULL)&&(startY == NULL)){
           startX=x;
           startY=y;
        }
        line(dst,Point(x,y),Point(startX,startY),Scalar(255,255,255),1);
        line(mask,Point(x,y),Point(startX,startY),Scalar(255,255,255),1);
        imshow("DST", dst);
        imshow("Mask", mask);
        startX=x;
        startY=y;
      }
}*/

 void np(const Point &seed, vector <Point> &arr , int max_x, int max_y)
 {
   for (int i = -1; i <= 1; i++)
       {
         for(int j = -1; j <= 1; j++)
          {
            if ((seed.x + i  => 0) || (seed.y + i => 0 )) && (seed.y - i  >  )
               { 
                   arr.push_back(Point(seed.x + i, seed . y + j));
                   cout << seed.x - i<< endl;
               }
          }
       }
 }


struct veght_pixel
{
  Point pixel;
  double tmp;
  bool processed;
};

Mat laplassian_zero_crossing(const Mat &src)
{
  Mat lap;
  Laplacian( src, lap, CV_32F);
  threshold(lap, lap, 0, 1, THRESH_BINARY);
  return lap;
}

void gradient_magnitude(const Mat &src, Mat &dx, Mat &dy, Mat &mag)
{
  double maxVal;
  double minVal;
  Point pointMax, pointMin;

  Sobel(src, dx, CV_32F, 1, 0);
  Sobel(src, dy, CV_32F, 0, 1);
  magnitude (dx, dy, mag);
  minMaxLoc(mag, &minVal, &maxVal);
  mag = 1 - mag / maxVal;
}

float local_cost(const Point &p, const Point &q, const Mat &laplasian, 
                const Mat &magnitude, const Mat &dx, const Mat &dy)
{
    static const float wlaplasian = 0.43f;
    static const float wdirection = 0.43f;
    static const float wmagitude = 0.14f;
    float dp;
    float dq;

    if (dy.at<float>(p) * (q.x - p.x) - dx.at<float>(p) * (q.y - p.y) >= 0)
    {
        dp = dy.at<float>(p) * (q.x - p.x) - dx.at<float>(p) * (q.y - p.y);
        dq = dy.at<float>(q) * (q.x - p.x) - dx.at<float>(q) * (q.y - p.y);
    }
    else
    {
        dp = dy.at<float>(p) * (p.x - q.x) - dx.at<float>(p) * (p.y - q.y);
        dq = dy.at<float>(q) * (p.x - q.x) - dx.at<float>(q) * (p.y - q.y);
    }
    float grad = (1 / cos(dp) + 1/cos(dq)) / M_PI;
    return wlaplasian * laplasian.at<float>(q) + wdirection * grad + wmagitude
           * magnitude.at<float>(q);
}

bool comp( veght_pixel a, veght_pixel b)
{
return a.tmp < b.tmp;
}

int main(int argc, char** argv)
{
    Point start_point(331, 48);
    Point end_point(331, 47);
    vector <veght_pixel> L;
    vector <Point> path;
    veght_pixel P;

    Mat bin = imread("/home/dizheninv/Desktop/test.png", IMREAD_GRAYSCALE);
    // circle(bin, Point(331, 48), 20, Scalar(0), -1);
    // circle(bin, Point(396, 85), 20, Scalar(0), -1);
    Mat laplasian = laplassian_zero_crossing(bin);
    Mat dx;
    Mat dy;
    Mat magnitude;
    gradient_magnitude(bin, dx, dy, magnitude);

    P.pixel = start_point;
    P.tmp = 0;
    P.processed = false; 
    L.push_back(P);

    // while (!L.empty())
    // {
    //   sort(L.begin(), L.end(), comp);
    //   P = L.back();
    //   cout << P.tmp;
    //   L.pop_back();
    //   P.processed = true;
      
    // // for 
    // //   e.push_back(P.pixel);
    // //   cout << P.tmp << " " << P.pixel;
    // //   use_pixel.push_back(P.pixel);
    // //   for (int i=0; i < 9; i++)
    // //      {
    // //        if()
    // //      }
    // }
    vector <Point> arr;
    np(Point(0,0), arr, 10, 10); 
    cout<<"Start"<<endl;
    while (!arr.empty())
    {
      start_point = arr.back();
      arr.pop_back();
      cout << start_point.x << " "<<start_point.y << endl;
    }
      
    namedWindow("Lap",1);
    namedWindow("Binar", 1);
    namedWindow("Mag",1);
    imshow("Binar", magnitude);
    //imshow("Lap", dst);
    imshow("Mag", laplasian);

    waitKey(0);
    return 0;
}