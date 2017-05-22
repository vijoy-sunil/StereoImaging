
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

#define ESC_KEY (27)

int main( int argc, char** argv )
{  
    CvCapture *capture_l;
    CvCapture *capture_r;
    IplImage *frame_l, *frame_r;
    int devl=0, devr=1;

    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PXM_BINARY);
    compression_params.push_back(1);
    
    printf("Will open DUAL video devices %d and %d\n", devl, devr);
    capture_l = cvCreateCameraCapture(devl);
    capture_r = cvCreateCameraCapture(devr);
    cvSetCaptureProperty(capture_l, CV_CAP_PROP_FRAME_WIDTH, HRES_COLS);
    cvSetCaptureProperty(capture_l, CV_CAP_PROP_FRAME_HEIGHT, VRES_ROWS);
    cvSetCaptureProperty(capture_r, CV_CAP_PROP_FRAME_WIDTH, HRES_COLS);
    cvSetCaptureProperty(capture_r, CV_CAP_PROP_FRAME_HEIGHT, VRES_ROWS);
	
	Mat gray_frame_l,gray_frame_r;

    while(1)
    {
        frame_l=cvQueryFrame(capture_l);
        frame_r=cvQueryFrame(capture_r);

		Mat mat_frame_l(frame_l);
		cvtColor(mat_frame_l, gray_frame_l, CV_RGB2GRAY);
		imshow("Gray Left", gray_frame_l);
		
		Mat mat_frame_r(frame_r);
		cvtColor(mat_frame_r, gray_frame_r, CV_RGB2GRAY);
		imshow("Gray Right", gray_frame_r);		
		

        // Set to pace frame display and capture rate
        char c = cvWaitKey(10);
        if(c == ESC_KEY)
        {
            imwrite("../left.pgm", gray_frame_l,compression_params);
            imwrite("../right.pgm", gray_frame_r,compression_params);
            
 			imwrite("left.png", mat_frame_l);           
            imwrite("right.png", mat_frame_r);
            printf("Image saved ...\n");
            break;
        }
        else if((c == 'q') || (c == 'Q'))
        {
            printf("Exiting ...\n");
            cvReleaseCapture(&capture_l);
            cvReleaseCapture(&capture_r);
            break;
        }
    }

    cvReleaseCapture(&capture_l);
    cvReleaseCapture(&capture_r);
    cvDestroyWindow("Capture LEFT");
    cvDestroyWindow("Capture RIGHT");

}
