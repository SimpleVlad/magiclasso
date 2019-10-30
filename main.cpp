#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
// #include <scissor.h> 
using namespace cv;
using namespace std;

Mat dst, mask;
int startX = NULL;
int startY = NULL;

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
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
}
 
int main(int argc, char** argv)
{
    Mat img = imread("test.jpg");
    mask = Mat(img.rows, img.cols, CV_8UC3, Scalar(0, 0, 0));
    dst = img;
    if ( img.empty() ) 
    { 
        cout << "Error loading the image" << endl;
        return -1; 
    }
    namedWindow("DST", 1);
    namedWindow("Mask", 1);
    imshow("DST", dst);
    imshow("Mask", mask);
    setMouseCallback("DST", CallBackFunc, NULL);
    waitKey(0);
    return 0;
}