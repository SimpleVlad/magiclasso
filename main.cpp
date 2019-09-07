#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;
int main( int argc, char** argv )
{
 char* imageName = argv[1];
 Mat image;
 image = imread( "/home/dizheninv/Pictures/675554.jpg", IMREAD_COLOR );
 if( argc != 2 || !image.data )
 {
   cout<< " No image data \n ";
   return -1;
 }
 Mat gray_image;
 for (int i=0;i<1000;++i){
   Canny(image, gray_image, 1, 200000 - i * 1000 , 7, true);
   imwrite( "/home/dizheninv/magicLassoOpenCV/build/t1.jpg", gray_image );
   namedWindow( imageName, WINDOW_AUTOSIZE);
   namedWindow( "Gray image", WINDOW_AUTOSIZE);
   imshow( imageName, image );
   imshow( "Canny", gray_image);
   waitKey(0);
 }
 return 0;
}
