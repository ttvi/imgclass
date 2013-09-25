/*
 * vlfeat_test.cpp
 *
 *  Created on: Sep 12, 2013
 *      Author: vinhtt
 */

#include "flags.h"
#include <iostream>
#include <vl/kmeans.h>

#ifdef VLFEAT_TEST

int main(int argc, char** args)
{
	double energy;
	vl_int32* centers;
	int dimension = 2;
	int numData = 10000;
	int numCenters = 15;
	vl_int32* data = new vl_int32[numData];
	VlRand * rand = vl_get_rand();
	for (int i = 0; i < numData; i++)
	data[i] = vl_rand_int31(rand);

	// Use float data and the L2 distance for clustering
	VlKMeans * kmeans = vl_kmeans_new (VL_TYPE_FLOAT, VlDistanceL2);
	// Use Lloyd algorithm
	vl_kmeans_set_algorithm (kmeans, VlKMeansLloyd);
	// initialize the kmeans using kmeans++ approach
	std::cout << "initializing" << std::endl;
	vl_kmeans_init_centers_plus_plus(kmeans, data, dimension, numData, numCenters);
	std::cout << "init done" << std::endl;
	// Run at most 100 iterations of cluster refinement using Lloyd algorithm
	vl_kmeans_set_max_num_iterations (kmeans, 100);
	vl_kmeans_refine_centers (kmeans, data, numData);
	// Obtain the energy of the solution
	energy = vl_kmeans_get_energy(kmeans);
	// Obtain the cluster centers
	centers = (vl_int32*) vl_kmeans_get_centers(kmeans);
	for (int i = 0; i < numCenters; i++)
	std::cout << centers[i] << " ";
}
#endif
