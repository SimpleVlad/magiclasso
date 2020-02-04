#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <math.h>
#include <list>
#include <vector>
#include <algorithm>

using namespace cv;
using namespace std;


// /*void CallBackFunc(int event, int x, int y, int flags, void* userdata)
// {
//       if ((event == EVENT_MOUSEMOVE) && (flags & EVENT_FLAG_LBUTTON)){
//         if ((startX == NULL)&&(startY == NULL)){
//            startX=x;
//            startY=y;
//         }
//         line(dst,Point(x,y),Point(startX,startY),Scalar(255,255,255),1);
//         line(mask,Point(x,y),Point(startX,startY),Scalar(255,255,255),1);
//         imshow("DST", dst);
//         imshow("Mask", mask);
//         startX=x;курс 
//         startY=y;
//       }
// }*/

struct veght_pixel
{
  Point pixel;
  double cost;
};

// /*class Node
// {
//   public:
//     vector<veght_pixel> val; // in progress
//     veght_pixel back;        // to
// }*/

void neighbors(const Point &seed, vector <Point> &arr , int max_x, int max_y)
{
   cout<< "Output points:"<<endl;
   for (int i = -1; i <= 1; i++)
       {
         for(int j = -1; j <= 1; j++)
          {
             if ((i==0)&&(j==0))
             {
               continue;
             }
             else
             {
              if ((seed.x + i >= 0) && (seed.y + j >= 0) && (seed.x + i < max_x)
                 && (seed.y + j < max_y)) 
               { 
                   arr.push_back(Point(seed.x + i, seed . y + j));
                   cout << seed.x + i<< " "<<seed.y + j  <<endl;
               }
             }
          }
       }
}



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
    float grad = (1 / cos(dp) + 1/cos(dq)) / M_PI; // cost under zero;
    cout <<"Grad: " <<grad<<endl<<endl<<endl;
    return wlaplasian * laplasian.at<float>(q) + wdirection * grad + wmagitude
           * magnitude.at<float>(q);
}

bool comp( veght_pixel a, veght_pixel b)
{
 return a.cost < b.cost;
}

int main(int argc, char** argv)
{
  // --generate strart value--
  Point start_point(31, 48);
  Point end_point(31, 47);
  Mat bin = imread("/home/dizheninv/Desktop/test.png", IMREAD_GRAYSCALE);
  Mat cost_map(bin.rows, bin.cols, CV_32FC1, cv::Scalar(2147483647));
  Mat expanded_map(bin.rows, bin.cols, CV_8UC1, cv::Scalar(0));

  vector <veght_pixel> L;
  vector <Point> path;
  veght_pixel P;
  vector<Point> processed;

  Mat laplasian = laplassian_zero_crossing(bin);
  Mat dx;
  Mat dy;
  Mat magnitude;
  gradient_magnitude(bin, dx, dy, magnitude);
  
  // --start algorithm--
  P.pixel = start_point;
  P.cost = 0;
  processed.push_back(P.pixel); 
  L.push_back(P);
  cost_map.at<float>(start_point) = P.cost; 
  while (!L.empty())
  {
    sort(L.begin(), L.end(), comp);
    P = L.back(); 
    L.pop_back();

  }
 
 
 
  // --test block--
  // circle(cost_map, P.pixel, 10, 0);
  // cout<< cost_map;
  namedWindow("Cost",1);
  imshow("Cost", cost_map);
  waitKey(0);
  return 0;
}





















// int main(int argc, char** argv)

// {
//     Point start_point(331, 48);
//     Point end_point(331, 47);
//     vector <veght_pixel> L;
//     vector <Point> path;
//     veght_pixel P;
//     vector<Point> processed;

//     Mat bin = imread("/home/dizheninv/Desktop/test.png", IMREAD_GRAYSCALE);
//     Mat laplasian = laplassian_zero_crossing(bin);
//     Mat dx;
//     Mat dy;
//     Mat magnitude;
//     gradient_magnitude(bin, dx, dy, magnitude);

//     P.pixel = start_point;
//     P.cost = 0;
//     // processed.push_back(P.pixel); 
//     L.push_back(P);

//     while (!L.empty())
//     {
//        vector<Point> arr;
//        sort(L.begin(), L.end(), comp);
//        P = L.back();
//        cout << "F" << endl;
//        L.pop_back();
//        processed.push_back(P.pixel);
//        neighbors( P.pixel, arr, laplasian.rows, laplasian.cols);
//        cout<<arr<<endl;
//        while  (!arr.empty())
//          {
//            Point st;
//            st = arr.back();
//            arr.pop_back();
//            auto neighbor = find(processed.begin(), processed.end(), st);
//            if (neighbor == processed.end())
//             {
//               double total_weight = P.cost + local_cost(P.pixel,st, laplasian, magnitude, dx, dy);  // rework
//             //cout << "Total_weight " << total_weight <<endl;
//             }
           
//          }
//     }

//     namedWindow("Lap",1);
//     namedWindow("Binar", 1);
//     namedWindow("Mag",1);
//     imshow("Binar", magnitude);
//     imshow("Mag", laplasian);
//     waitKey(0);
//     return 0;
// }