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

int const maxBinary = 255;
int const maxBlur = 31;
int const maxBlurEdge = 3;
int const maxLowThres = 100;
int const maxValue = 255;
int const max_type = 4;

int blurEdgeValue = 0;
int blurValue = 0;
int edgeThresh = 1;
int intensity = 3;
int kernelSize = 3;
int lowThreshold;
int thresType = 3;
int thresValue = 0;

Mat src, srcGray, output, edge, detectEdges, tmp;
const string windowName = "Artistic Threshold";
const string trackbar = "Trackbars";
const string trackbarType = "Tipo: \n 0: Binário \n 1: Binário Invertido \n 2: Truncado \n 3: Com zero \n 4: Com zero invertido";
const string trackbarValue = "Threshold";
const string trackbarBlur = "Blur";
const string trackbarEdge = "Borda";
const string trackbarBlurEdge = "Blur on Edge";

Mat openFile();

void artisticThreshold( int, void* );
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
  threshold( srcGray, output, thresValue, maxBinary, thresType );

  if(blurValue)
  {
    if(blurValue%2==0)
    {
      blurValue+=1;
    }
    medianBlur ( output, output, blurValue);
  }

  if(lowThreshold)
  {
    blur( srcGray, detectEdges, Size(3,3) );
    Canny( detectEdges, detectEdges, lowThreshold+1, lowThreshold*intensity+1, kernelSize );
    edge = Scalar::all(0);

    src.copyTo( edge, detectEdges);
    cvtColor( edge, edge, COLOR_RGB2GRAY );
    if(blurEdgeValue)
    {
      if(blurEdgeValue%2==0)
      {
        blurEdgeValue+=1;
      }
      GaussianBlur( edge, edge, Size( blurEdgeValue, blurEdgeValue ), 0, 0 );
    }

    /*if(thresType==0||thresType==3)
    {
      bitwise_not ( edge, edge );
    }*/
    output = output + edge;
  }

  imshow( windowName, output );
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
  cvtColor( src, srcGray, COLOR_RGB2GRAY );
  edge.create( srcGray.size(), srcGray.type() );
  namedWindow( windowName, WINDOW_AUTOSIZE );
  namedWindow( trackbar, WINDOW_AUTOSIZE);
  createTrackbar( trackbarType, trackbar, &thresType, max_type, artisticThreshold );
  createTrackbar( trackbarValue, trackbar, &thresValue, maxValue, artisticThreshold );
  createTrackbar( trackbarBlur, trackbar, &blurValue, maxBlur, artisticThreshold );
  createTrackbar( trackbarEdge, trackbar, &lowThreshold, maxLowThres, artisticThreshold );
  createTrackbar( trackbarBlurEdge, trackbar, &blurEdgeValue, maxBlurEdge, artisticThreshold );

  artisticThreshold( 0, 0 );
}
