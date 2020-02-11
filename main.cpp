#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <queue>

using namespace cv;
using namespace std;

bool g_flag;
Mat img_draw , img_pre_draw;
Point start;

Scalar colored_line(255,0,0);

struct Pix
{
    double value;
    Point next_point;

    Pix(){}

    bool operator > (const Pix &b) const
    {
        return value > b.value;
    }

    bool operator < (const Pix &b) const
    {
        return value < b.value;
    }
};

Mat img, zero_crossing, gradient_magnitude, pre, cost, expand, Ix, Iy;

Mat lap_zer_cross(Mat canny)
{
  Mat lap;
  lap.create(img.rows, img.cols, CV_64FC1);
  threshold(canny, zero_crossing, 254, 1, THRESH_BINARY);
  return lap;
}


// Mat lap_zer_cross(const Mat& canny)
// {
//     namedWindow("Lap");
//     for (int i = 0; i < img.rows; i++)
//     {
//         for (int j = 0; j < img.cols; j++){
//             if (canny.at<uchar>(i, j) == 255)
//                 zero_crossing.at<uchar>(i, j) = 0;
//             else
//                 zero_crossing.at<uchar>(i, j) = 1;
//         }
//     }
//       imshow("lap", zero_crossing);
// }

void val_Ix(const Mat& value)
{
  for (int i = 0; i < img.rows; i++){
        for (int j = 0; j < img.cols - 1; j++){
            Ix.at<double>(i, j) = (value.at<uchar>(i, j + 1) - value.at<uchar>(i, j)) / 255.0;
        }
        Ix.at<double>(i, img.cols - 1) = Ix.at<double>(i, img.cols - 2);
    }
}


void val_Iy(const Mat& value)
{
  for (int j = 0; j < img.cols; j++){
        for (int i = 0; i < img.rows - 1; i++){
            Iy.at<double>(i, j) = (value.at<uchar>(i + 1, j) - value.at<uchar>(i, j)) / 255.0;
        }
        Iy.at<double>(img.rows - 1, j) = Iy.at<double>(img.rows - 2, j);
    }
}

void grad_mag()
{
    Mat G;
    G.create(img.rows, img.cols, CV_64FC1);
    double max_val = 0.0;
      for (int i = 0; i < Ix.rows; i++)
      {
        for (int j = 0; j < Ix.cols; j++)
        {
            G.at<double>(i, j) = sqrt(Ix.at<double>(i, j) * Ix.at<double>(i, j) + Iy.at<double>(i, j) * Iy.at<double>(i, j));
            max_val = max(max_val, G.at<double>(i, j));
        }
      }
    for (int i = 0; i < gradient_magnitude.rows; i++)
    {
        for (int j = 0; j < gradient_magnitude.cols; j++)
        {
            gradient_magnitude.at<double>(i, j) = 1.0 - G.at<double>(i, j) / max_val;
        }
    }
}

double local_cost(Point p, Point q, bool diag)
{
    double  fG = 0.0;
    fG = gradient_magnitude.at<double>(q.y, q.x);
    double dp;
    double dq;

    if ((Iy.at<double>(p.y, p.x) * (q.x - p.x) + (-Ix.at<double>(p.y, p.x)) * (q.y - p.y)) >= 0){
        dp = Iy.at<double>(p.y, p.x) * (q.x - p.x) + (-Ix.at<double>(p.y, p.x)) * (q.y - p.y);
        dq = (q.x - p.x) * Iy.at<double>(q.y, q.x) + (q.y - p.y) * (-Ix.at<double>(q.y, q.x));
    }

    else{
        dp = Iy.at<double>(p.y, p.x) * (p.x - q.x) + (-Ix.at<double>(p.y, p.x)) * (p.y - q.y);
        dq = (p.x - q.x) * Iy.at<double>(q.y, q.x) + (p.y - q.y) * (-Ix.at<double>(q.y, q.x));
    }
    if (!diag){
        dp /= sqrt(2);
        dq /= sqrt(2);
    }
    double pi = acos(-1.0);
    if (diag)
        fG /= sqrt(2);
    return  0.43 * zero_crossing.at<uchar>(q.y, q.x) + 0.43 * (acos(dp) + acos(dq)) / pi + 0.14 * fG;
}


void find_min_path(Point start)
{
    Mat in_que;
    Mat skip;
    in_que.create(img.rows, img.cols, CV_8UC1);
    skip.create(img.rows, img.cols, CV_8UC1);
    int neighbor[2][8] = {{1, 0, -1, 0, 1, 1, -1, -1},
                          {0, 1, 0, -1, 1, -1, 1, -1}};

    expand.setTo(0);
    cost.setTo(INT_MAX);
    in_que.setTo(0);
    skip.setTo(0);
    cost.at<double>(start.y, start.x) = 0;
    in_que.at<uchar>(start.y, start.x) = 1;
    priority_queue < Pix, vector<Pix>, greater<Pix> > L;
    Pix begin;
    begin.value=0;
    begin.next_point = start;
    L.push(begin);
    while (!L.empty()){
        Pix P = L.top();
        L.pop();
        Point p = P.next_point;
        in_que.at<uchar>(p.y, p.x) = 0;
        if (skip.at<uchar>(p.y, p.x) == 1)
            continue;
        expand.at<uchar>(p.y, p.x) = 1;
        for (int i = 0; i < 8; i++){
            int tx = p.x + neighbor[0][i];
            int ty = p.y + neighbor[1][i];
            if (tx < 0 || tx >= img.cols || ty < 0 || ty >= img.rows)
                continue;
            if (expand.at<uchar>(ty, tx) == 1)
                continue;
            Point q = Point(tx, ty);
            double tmp;
            if (i <= 3)
            {
                tmp = cost.at<double>(p.y, p.x) + local_cost(p, q, true);
            }
            else
            {
                tmp = cost.at<double>(p.y, p.x) + local_cost(p, q, false);
            }
            if (in_que.at<uchar>(q.y, q.x) == 1 && tmp < cost.at<double>(q.y, q.x))
            {
                skip.at<uchar>(q.y, q.x) = 1;
                continue;
            }
            
            if (in_que.at<uchar>(q.y, q.x) == 0)
            {
                cost.at<double>(q.y, q.x) = tmp;
                pre.at<Vec2i>(q.y, q.x)[0] = p.x;
                pre.at<Vec2i>(q.y, q.x)[1] = p.y;
                in_que.at<uchar>(q.y, q.x) = 1;
                Pix val;
                val.value = cost.at<double>(q.y, q.x);
                val.next_point = q;
                L.push(val);
            }
        }
    }
}

void onMouse(int event, int x, int y, int flags, void *param)
{
    if (event == EVENT_LBUTTONDOWN)
    {
        start = Point(x, y);
        g_flag = true;
        find_min_path(start);
        img_draw.copyTo(img_pre_draw);
        imshow("example", img_draw);
    }
    else
       if (event == EVENT_MOUSEMOVE && g_flag)
       {
          img_pre_draw.copyTo(img_draw);
          Point cur = Point(x, y);
          Point tmp;
          while (cur != start)
          {
              tmp = Point(pre.at<Vec2i>(cur.y, cur.x)[0], pre.at<Vec2i>(cur.y, cur.x)[1]);
              line(img_draw, cur, tmp, colored_line, 2);
              if (tmp == start) break;
              cur = tmp;
          }
          imshow("example", img_draw);
       }
    else 
       if (event == EVENT_RBUTTONDOWN)
       {
        g_flag = false;
        img_pre_draw.copyTo(img_draw);
        imshow("example", img_pre_draw);
       }
}

int main(){
    Mat grayscale;
    Mat value;
    Mat img_canny;
    string path = "/home/dizheninv/Pictures/1512.jpg";
    namedWindow("example");
    
    img = imread(path);
    gradient_magnitude.create(img.rows, img.cols, CV_64FC1);
    pre.create(img.rows, img.cols, CV_32SC2);
    cost.create(img.rows, img.cols, CV_64FC1);
    expand.create(img.rows, img.cols, CV_8UC1);
    zero_crossing.create(img.rows, img.cols, CV_64FC1);
    Ix.create(img.rows, img.cols, CV_64FC1);
    Iy.create(img.rows, img.cols, CV_64FC1);
    cvtColor(img, grayscale, COLOR_BGR2GRAY);
    grayscale.copyTo(value);
    img.copyTo(img_draw);
    img.copyTo(img_pre_draw);
    // GaussianBlur(value, value, Size(3, 3), 0, 0, BORDER_DEFAULT);
    Canny(grayscale, img_canny, 50, 100);
    lap_zer_cross(img_canny);
    val_Ix(value);
    val_Iy(value);
    grad_mag();

    cout << "completed!" << endl;
    imshow("lap", zero_crossing);
    setMouseCallback("example", onMouse, 0);
    imshow("example", img);
    waitKey(0);
}