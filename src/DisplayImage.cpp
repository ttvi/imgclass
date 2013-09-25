/*
 * DisplayImage.cpp
 *
 *  Created on: Sep 11, 2013
 *      Author: vinhtt
 */

#include "flags.h"

#ifdef DISPLAY_IMAGE

#include <cv.h>
#include <highgui.h>

using namespace cv;

int main( int argc, char** argv )
{
	if(argc != 2)
	{
		printf( "usage: ./DisplayImage <img>\n" );
		return -1;
	}

	Mat image;
	image = imread( argv[1], 1 );

	if( argc != 2 || !image.data )
	{
		printf( "No image data \n" );
		return -1;
	}

	namedWindow( "Display Image", CV_WINDOW_AUTOSIZE );
	imshow( "Display Image", image );

	waitKey(0);

	return 0;
}
#endif
