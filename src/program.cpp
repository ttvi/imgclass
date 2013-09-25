/*
 * VLFeat_Fisher_Vector_test.cpp
 *
 *  Created on: Sep 12, 2013
 *      Author: vinhtt
 */

#include "clustering.h"
#include "feature_detector.h"

#include <string>
#include <pthread.h>
#include <fstream>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>

#ifdef FISHER_VECTOR

//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

vector<string> *img_locs;
vector<int> *img_tags;
int mutex_counter;
int mutex_length;

void test_img_size();
void classify();
void load_data(vector<string> *images, vector<int> *labels);
void *threading(void* args);


int main(int argc, char** args) {

	classify();

//	test_img_size();

	return 0;
}

void test_img_size() {
	// read the image from file
	const char* imfile = "res/images/box.png";
	Mat img = imread(imfile, CV_LOAD_IMAGE_GRAYSCALE);
	cout << "cols: " << img.cols << ", rows: " << img.rows << endl;

	// detect the keypoint and descriptor lists
	SurfFeatureDetector detector;
	Mat descriptors;
	vector<KeyPoint> keypoints;

	detector.detect(img, keypoints);	// keypoint list
	detector.compute(img, keypoints, descriptors);	// descriptor list
	cout << keypoints.size() << " to " << descriptors.cols << " and "
			<< descriptors.rows << endl;

	for (int i = 0; i < descriptors.rows; i++) {
		for (int j = 0; j < descriptors.cols; j++)
			cout << (DATATYPE) descriptors.data[i * descriptors.cols + j] << " ";
		cout << endl;
	}

	int length = descriptors.cols * descriptors.rows;
	DATATYPE* data = new DATATYPE[length];

	uchar* tmp = descriptors.data;
	for (int i = 0; i < length; i++)
		data[i] = (DATATYPE) tmp[i];

	imshow("Display Image", img);

	waitKey(0);
}

void classify() {
	// load the image and label lists
	img_locs = new vector<string>();
	img_tags = new vector<int>();
	load_data(img_locs, img_tags);

	cout << "images loading done" << endl;
	flush(cout);

	mutex_counter = 0;
	mutex_length = img_locs->size();
	cout << "images size: " << mutex_length << endl;
	flush(cout);

	int NUM_THREADS = sysconf( _SC_NPROCESSORS_ONLN);
	pthread_t threads[NUM_THREADS];

	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_create(&threads[i], NULL, threading, NULL);
		cout << "thread " << i << " started" << endl;
		flush(cout);
	}

	for (int i = 0; i < NUM_THREADS; i++)
		pthread_join(threads[i], NULL);

//	cout << "classifying... ";
//	flush(cout);
//	svm_classify();
//	cout << "done" << endl;
//	flush(cout);

	pthread_exit(NULL);
}

void *threading(void* args) {
	const char* img_name =
			"/home/vinhtt/Downloads/practical-image-classification/data/images/%s.jpg";

	vl_size numClusters = 15;

	while (mutex_counter < mutex_length) {
		int store;
		pthread_mutex_lock(&mutex);
		store = mutex_counter++;
		const char* id = (*img_locs)[store].c_str();
		pthread_mutex_unlock(&mutex);

		char* filename = new char[1000];
		sprintf(filename, img_name, id);

		// detect feature and descriptor lists
		DATATYPE** data = new DATATYPE*[1];
		SIZE size = detect_features(filename, data);

		// encode
		if (size.numData > numClusters)
			encode(*data, size.numData, size.dimension, numClusters, 0);

		cout << store << " : " << id << endl;

		free(data);
		free(filename);
	}

	pthread_exit(NULL);
}

void load_data(vector<string> *images, vector<int> *labels) {
	const char* classes[] = { "aeroplane", "background", "car", "horse",
			"motorbike", "person" };
	const char* class_name =
			"/home/vinhtt/Downloads/practical-image-classification/data/%s_train.txt";
	const char* img_name =
			"/home/vinhtt/Downloads/practical-image-classification/data/images/%s.jpg";

	for (int i = 0; i < 6; i++) {
		char* filename = new char[1000];
		sprintf(filename, class_name, classes[i]);
		cout << filename << endl;

		ifstream myfile(filename);
		if (myfile.is_open()) {
			string line;
			while (getline(myfile, line)) {
				images->push_back(line);
				labels->push_back(i);
			}
			myfile.close();
		}
	}
}

#endif
