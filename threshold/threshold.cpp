#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <string>

using namespace cv;
int threshold_value = 0;
int threshold_type = 3;
int const max_value = 255;
int const max_type = 4;
int const max_BINARY_value = 255;
int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;

Mat src, src_gray, dst, edge, detected_edges, tmp;
const char* window_name = "Artistic Threshold";
const char* trackbar_type = "Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted";
const char* trackbar_value = "Value";
void Threshold_Demo( int, void* );
Mat openFile();
void cvMain(int, void*);
void CannyThreshold(int, void*);

int main( int, char** argv )
{
  cvMain( 0, 0);
  //CannyThreshold( 0, 0 );
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
void Threshold_Demo( int, void* )
{
  /* 0: Binary
     1: Binary Inverted
     2: Threshold Truncated
     3: Threshold to Zero
     4: Threshold to Zero Inverted
   */
  threshold( src_gray, dst, threshold_value, max_BINARY_value,threshold_type );
  imshow( window_name, dst );
}

void CannyThreshold(int, void*)
{
  Threshold_Demo( 0, 0 );
  /// Reduce noise with a kernel 3x3
  blur( src_gray, detected_edges, Size(3,3) );

  /// Canny detector
  Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );

  /// Using Canny's output as a mask, we display our result
  edge = Scalar::all(0);

  src.copyTo( edge, detected_edges);

  cvtColor( edge, edge, COLOR_RGB2GRAY );

  dst = dst + edge;

  imshow( window_name, dst );
 }


Mat openFile()
{
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
}

void cvMain(int, void*)
{
  src = openFile();
  cvtColor( src, src_gray, COLOR_RGB2GRAY );
  edge.create( src_gray.size(), src_gray.type() );
  namedWindow( window_name, WINDOW_AUTOSIZE );
  createTrackbar( trackbar_type, window_name, &threshold_type, max_type, Threshold_Demo );
  createTrackbar( trackbar_value, window_name, &threshold_value, max_value, Threshold_Demo );
  createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );
  Threshold_Demo( 0, 0 );
}
