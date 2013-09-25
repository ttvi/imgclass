/*
 * flags.h
 *
 *  Created on: Sep 12, 2013
 *      Author: vinhtt
 */

#ifndef __FLAGS_H__
#define __FLAGS_H__

#include <iostream>
#include <vector>

using namespace std;

//#define VLFEAT_TEST
//#define DISPLAY_IMAGE
//#define CLI
//#define KMEANS_TEST
#define FISHER_VECTOR
//#define MULTITHREADING

typedef double DATATYPE;
#define CLUSTERING_DATATYPE VL_TYPE_DOUBLE

typedef struct _SIZE {
	int numData;	// rows
	int dimension;	// cols
} SIZE;

typedef struct _THREAD_DATA {
	vector<string> img_locs;
	vector<int> img_tags;
	int counter;
	int length;
} THREAD_DATA;

#endif /* FLAGS_H_ */
