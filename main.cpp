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
  print(lap);
   namedWindow("Te", 1);
    imshow("Te", lap);
  if (lap.at<uchar>(seed)==0)
     return 0;
  else
     return 1;
}

float gradient_magnitude()
{
  
} 
 
int main(int argc, char** argv)
{
    Point start_point;
    Mat bin = Mat(10, 10, CV_8UC1, Scalar(200));

    circle(bin, Point(5, 5), 1, Scalar(255), -1);
    circle(bin, Point(5, 5), 4, Scalar(100), 1);
    

    Mat dst = bin.clone();

    cout << laplassian_zero_crossing(bin, Point(1, 1))<< endl;
    
    namedWindow("Lap",1);    
    namedWindow("Binar", 1);
    imshow("Binar", bin);
    imshow("Lap", dst);

    waitKey(0);
    return 0;
}