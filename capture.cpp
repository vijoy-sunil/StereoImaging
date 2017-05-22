/*
 *
 *  Example by Sam Siewert 
 *
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/contrib/contrib.hpp"

using namespace cv;
using namespace std;

#define HRES_COLS (320)
#define VRES_ROWS (240)

int main( int argc, char** argv )
{

    CvCapture* capture;
    IplImage* frame;
    int dev=0;

    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PXM_BINARY);
    compression_params.push_back(1);
    
    capture = cvCreateCameraCapture(0);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, HRES_COLS);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, VRES_ROWS);
	Mat gray_frame;
    while(1)
    {
        frame=cvQueryFrame(capture);
        Mat mat_frame(frame);
		cvtColor(mat_frame, gray_frame, CV_RGB2GRAY);
		imshow("Gray Left", gray_frame);
     


        char c = cvWaitKey(10);
        if( c == 27 ) 
        {
            imwrite("../left.pgm", gray_frame,compression_params);  
            imwrite("left.png", mat_frame);                  
            printf("\nLeft image write");         
            break;
        }
    }
    
        while(1)
    {
        frame=cvQueryFrame(capture);
        Mat mat_frame(frame);
		cvtColor(mat_frame, gray_frame, CV_RGB2GRAY);
		imshow("Gray Right", gray_frame);
     


        char c = cvWaitKey(10);
        if( c == 27 ) 
        {
            imwrite("../right.pgm", gray_frame,compression_params);
            imwrite("right.png", mat_frame);
            printf("\nRight image write");            
            break;
        }
    }
	printf("\nImage saved ...\n");
    cvReleaseCapture(&capture);
}
