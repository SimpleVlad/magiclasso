#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

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

int laplassian_zero_crossing(Mat src, Point seed)
{
  Mat lap = src.clone();
  Laplacian( src, lap, 0.5);
  // print(lap);

  if (lap.at<uchar>(seed)==0)
     return 0;
  else
     return 1;
}

float gradient_magnitude(Mat src, Mat clone, Point seed)
{
  Mat tmp = src.clone();
  // cvtColor(tmp, tmp, COLOR_BGR2GRAY); for colored img
  Mat dx, dy;
  Sobel(tmp, dx, CV_32F, 1, 0);
  Sobel(tmp, dy, CV_32F, 0, 1);

  Mat angle, mag;
  cartToPolar (dx, dy, clone, angle);
  return clone.at<uchar>(seed);
}

float gradient_direction(Mat src, Point seed, Point second)
{
      
}

int main(int argc, char** argv)
{
    Point start_point;
    Mat bin = Mat(100, 100, CV_8UC1, Scalar(200));

    circle(bin, Point(50, 50), 40, Scalar(255), -1);
    circle(bin, Point(50, 50), 30, Scalar(100), 1);
    
    Mat dst = bin.clone();
    Mat clone = bin.clone();

    gradient_magnitude(dst, clone, Point(1,1)); 
    cout << laplassian_zero_crossing(bin, Point(1, 1))<< endl;
    
    namedWindow("Lap",1);    
    namedWindow("Binar", 1);
    namedWindow("Mag",1);
    imshow("Binar", bin);
    imshow("Lap", dst);
    imshow("Mag", clone);

    waitKey(0);
    return 0;
}