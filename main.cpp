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
 
int main(int argc, char** argv)
{
    Point start_point;
    Mat bin = Mat(90, 90, CV_8UC1, Scalar(0));
    
        circle(bin, Point(45, 45), 20, Scalar(255), -1);
     
    // print(bin);

    namedWindow("Binar", 1);
    imshow("Binar", bin);
  
    waitKey(0);
    return 0;
}