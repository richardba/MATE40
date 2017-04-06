#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#ifdef _WIN32
  #include <windows.h>
#endif // _WIN32

using namespace cv;
using namespace std;

int const max_value = 255;
int const max_type = 4;
int const max_lowThreshold = 100;
int const max_BINARY_value = 255;
int const max_blur = 31;
int const max_blurEdge = 3;

int threshold_value = 0;
int threshold_type = 3;
int blurEdge_value = 0;
int blur_value = 0;
int edgeThresh = 1;
int lowThreshold;
int ratio = 3;
int kernel_size = 3;

Mat src, src_gray, dst, edge, detected_edges, tmp;
const string window_name = "Artistic Threshold";
const string trackbar = "Trackbars";
const string trackbar_type = "Tipo: \n 0: Binário \n 1: Binário Invertido \n 2: Truncado \n 3: Com zero \n 4: Com zero invertido";
const string trackbar_value = "Value";
const string trackbar_blur = "Blur";
const string trackbar_edge = "Min Threshold";
const string trackbar_blurEdge = "Blur on Edge";

void artisticThreshold( int, void* );
Mat openFile();
void cvMain(int, void*);

int main( int, char** argv )
{
  cvMain( 0, 0);
  for(;;)
    {
      int c;
      c = waitKey( 20 );
      if( (char)c == 27 )
        { return 0; }
      if( (char)c == 'f')
      {
       cvMain( 0, 0);
      }
    }
  return 0;
}
void artisticThreshold( int, void* )
{
  threshold( src_gray, dst, threshold_value, max_BINARY_value, threshold_type );

  if(blur_value)
  {
    if(blur_value%2==0)
    {
      blur_value+=1;
    }
    medianBlur ( dst, dst, blur_value);
  }

  if(lowThreshold)
  {
    blur( src_gray, detected_edges, Size(3,3) );
    Canny( detected_edges, detected_edges, lowThreshold+1, lowThreshold*ratio+1, kernel_size );
    edge = Scalar::all(0);

    src.copyTo( edge, detected_edges);
    cvtColor( edge, edge, COLOR_RGB2GRAY );
    if(blurEdge_value)
    {
      if(blurEdge_value%2==0)
      {
        blurEdge_value+=1;
      }
      GaussianBlur( edge, edge, Size( blurEdge_value, blurEdge_value ), 0, 0 );
    }

    /*if(threshold_type==0||threshold_type==3)
    {
      bitwise_not ( edge, edge );
    }*/
    dst = dst + edge;
  }

  imshow( window_name, dst );
}

Mat openFile()
{
  #ifdef _WIN32
    HWND hwnd;
    HANDLE hf;
    OPENFILENAME ofn_test = {0};
    std::string tempString;

    ZeroMemory(&ofn_test, sizeof(ofn_test));
    ofn_test.lStructSize = sizeof(ofn_test);

    char filename_cstring[2048] = {0};
    ofn_test.lpstrFile = filename_cstring;
    ofn_test.nMaxFile = 2048;
    ofn_test.lpstrTitle = tempString.c_str();
    GetOpenFileName(&ofn_test);
    char* filename = ofn_test.lpstrFile;
    return imread(filename);
  #elif __APPLE__
    string filename;
    cout << "Insira o nome do arquivo" << endl;
    cin >> filename;
    return imread(filename);
  #else
    FILE *in;
    if (!(in = popen("zenity  --title=\"Select an image\" --file-selection","r")))
    {
      Mat nil;
      return nil;
    }

    char buff[512];
    std::string selectFile = "";
    while (fgets(buff, sizeof(buff), in) != NULL)
    {
        selectFile += buff;
    }
    pclose(in);

    selectFile.erase(std::remove(selectFile.begin(), selectFile.end(), '\n'), selectFile.end());
    return imread(selectFile);
  #endif // _WIN32
  return Mat();
}

void cvMain(int, void*)
{
  src = openFile();
  if(src.empty())
  {
    exit(0);
  }
  cvtColor( src, src_gray, COLOR_RGB2GRAY );
  edge.create( src_gray.size(), src_gray.type() );
  namedWindow( window_name, WINDOW_AUTOSIZE );
  namedWindow( trackbar, WINDOW_AUTOSIZE);
  createTrackbar( trackbar_type, trackbar, &threshold_type, max_type, artisticThreshold );
  createTrackbar( trackbar_value, trackbar, &threshold_value, max_value, artisticThreshold );
  createTrackbar( trackbar_blur, trackbar, &blur_value, max_blur, artisticThreshold );
  createTrackbar( trackbar_edge, trackbar, &lowThreshold, max_lowThreshold, artisticThreshold );
  createTrackbar( trackbar_blurEdge, trackbar, &blurEdge_value, max_blurEdge, artisticThreshold );

  artisticThreshold( 0, 0 );
}
