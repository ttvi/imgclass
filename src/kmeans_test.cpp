/*
 * kmeans_test.cpp
 *
 *  Created on: Sep 11, 2013
 *      Author: vinhtt
 */

#include "flags.h"
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/ml/ml.hpp>
#include <vl/kmeans.h>

#ifdef KMEANS_TEST

using namespace cv;

int main( int argc, char** argv )
{
	const char* fname = "res/images/butterfly.jpg";
	Mat src = imread( fname, 1 );
	imshow("image", src);

	SiftFeatureDetector detector;
	vector<KeyPoint> keypoints;
	std::cout << "detecting keypoints..." << std::endl;
	detector.detect( src, keypoints );

//  Mat samples(src.rows * src.cols, 3, CV_32F);
//  for( int y = 0; y < src.rows; y++ )
//    for( int x = 0; x < src.cols; x++ )
//      for( int z = 0; z < 3; z++)
//        samples.at<float>(y + x*src.rows, z) = src.at<Vec3b>(y,x)[z];

	int length = keypoints.size();
	std::cout << "number of keypoints: " << length << std::endl;

	Mat samples(length, 4, CV_32F);
	Mat max_sample(1, 4, CV_32F);
	max_sample.at<float>(0, 0) = 0;
	max_sample.at<int>(0, 1) = 0;
	max_sample.at<float>(0, 2) = 0;
	max_sample.at<float>(0, 3) = 0;
	for( int i = 0; i < length; i++ )
	{
		samples.at<float>(i, 0) = keypoints[i].angle;
		samples.at<int>(i, 1) = keypoints[i].octave;
		samples.at<float>(i, 2) = keypoints[i].response;
		samples.at<float>(i, 3) = keypoints[i].size;

		if (samples.at<float>(i, 0) > max_sample.at<float>(0, 0))
		max_sample.at<float>(0, 0) = samples.at<float>(i, 0);
		if (samples.at<int>(i, 1) > max_sample.at<int>(0, 1))
		max_sample.at<int>(0, 1) = samples.at<int>(i, 1);
		if (samples.at<float>(i, 2) > max_sample.at<float>(0, 2))
		max_sample.at<float>(0, 2) = samples.at<float>(i, 2);
		if (samples.at<float>(i, 3) > max_sample.at<float>(0, 3))
		max_sample.at<float>(0, 3) = samples.at<float>(i, 3);
	}

//  std::cout << "before normalization: " << samples << std::endl;
	// normalization
	for( int i = 0; i < length; i++ )
	for (int j = 0; j < 4; j++)
	samples.at<float>(i, j) /= max_sample.at<float>(0, j);
//  std::cout << "max sample: " << max_sample << std::endl;

	int clusterCount = 15;
	Mat labels;
	int attempts = 4;
	Mat centers;
//  std::cout << "after normalization" << samples << std::endl;
	std::cout << "clustering... ";
	std::flush(std::cout);
	kmeans(samples, clusterCount, labels, TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers );
	std::cout << "done" << std::endl;
//  std::cout << labels << std::endl << centers << std::endl;
//  length = centers.cols * centers.rows;
//  for (int i = 0; i < centers.rows; i++)
//	  for (int j = 0; j < centers.cols; j++)
//		  centers.at<float>(i, j) *= max_sample.at<float>(0, j);
//  std::cout << labels << std::endl << centers << std::endl;

	EM model;
	Mat logs, probs;

	model.set("nclusters", clusterCount);
	model.set("maxIters", 10000);
	model.set("epsilon", 0.0001);

	std::cout << "training GMM... ";
	std::flush(std::cout);
	model.trainE(samples, centers, noArray(), noArray(), logs, labels, probs);
	std::cout << "done" << std::endl;
//  std::cout << logs << std::endl;
//  std::cout << labels << std::endl;
//  std::cout << probs << std::endl;

//  std::cout << "model trained" << std::endl;
	Mat means = model.getMat("means");
//  std::cout << means << std::endl;
	std::vector<Mat> covs = model.getMatVector("covs");
//  for (int i = 0; i < covs.size(); i++)
//	  std::cout << "cov " << covs[i] << std::endl;
	std::vector<Mat> weights = model.getMatVector("covs");
//  for (int i = 0; i < weights.size(); i++)
//	  std::cout << "weight " << weights[i] << std::endl;

//  Mat new_image( src.size(), src.type() );
//  for( int y = 0; y < src.rows; y++ )
//    for( int x = 0; x < src.cols; x++ )
//    {
//      int cluster_idx = labels.at<int>(y + x*src.rows,0);
//      new_image.at<Vec3b>(y,x)[0] = centers.at<float>(cluster_idx, 0);
//      new_image.at<Vec3b>(y,x)[1] = centers.at<float>(cluster_idx, 1);
//      new_image.at<Vec3b>(y,x)[2] = centers.at<float>(cluster_idx, 2);
//    }
//  imshow( "clustered image", new_image );
//  imshow("original image", src);
	waitKey( 0 );
}

#endif
