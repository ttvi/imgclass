/*
 * sift_detector.cpp
 *
 *  Created on: Sep 11, 2013
 *      Author: vinhtt
 */

#ifndef __FEATURE_DETECTOR__
#define __FEATURE_DETECTOR__

#include "flags.h"

#include <stdlib.h>

#include <vl/generic.h>

#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/features2d/features2d.hpp>

using namespace cv;

SIZE detect_features(const char* imfile, DATATYPE** data) {

	// read the image from file
	Mat img = imread(imfile, CV_LOAD_IMAGE_GRAYSCALE);

	// detect the keypoint and descriptor lists
	SurfFeatureDetector detector;
	Mat descriptors;
	vector<KeyPoint> keypoints;

	detector.detect(img, keypoints);	// keypoint list
	detector.compute(img, keypoints, descriptors);	// descriptor list

	// copy the data to the output
	int length = descriptors.cols * descriptors.rows;
	*data = new DATATYPE[length];
	uchar* tmp = descriptors.data;
	for (int i = 0; i < length; i++)
		(*data)[i] = (DATATYPE) tmp[i];

	SIZE size;
	size.dimension = descriptors.cols;
	size.numData = descriptors.rows;

	// free memory
	descriptors.~Mat();
	img.~Mat();

	return size;
}

#endif
