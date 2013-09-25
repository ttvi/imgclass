/*
 * clustering.h
 *
 *  Created on: Sep 23, 2013
 *      Author: vinhtt
 */

#ifndef __CLUSTERING_H__
#define __CLUSTERING_H__

#include "flags.h"

#include <vl/kmeans.h>
#include <vl/gmm.h>
#include <vl/fisher.h>
#include <vl/svm.h>


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

vector<DATATYPE*> fvlist;
vector<int> labels;

int dms = 0;
int len = 0;

void encode(DATATYPE* data, int numData, int dimension, int numClusters,
		int label) {

	// KMeans
	VlKMeans* kmeans = vl_kmeans_new(CLUSTERING_DATATYPE, VlDistanceL2);
	vl_kmeans_set_algorithm(kmeans, VlKMeansLloyd);	// Use Lloyd algorithm
	vl_kmeans_set_max_num_iterations(kmeans, 100);// Run at most 100 iterations of cluster refinement
	vl_kmeans_init_centers_plus_plus(kmeans, data, dimension, numData,
			numClusters);// Initialize the cluster centers by randomly sampling the data
	vl_kmeans_cluster(kmeans, data, dimension, numData, numClusters);

	// GMM
	VlGMM* gmm = vl_gmm_new(CLUSTERING_DATATYPE, dimension, numClusters);
	vl_gmm_set_max_num_iterations(gmm, 1000);// set the maximum number of EM iterations to 1000
	vl_gmm_set_kmeans_init_object(gmm, kmeans);
	vl_gmm_cluster(gmm, data, numData);

	dms = 2 * numClusters * dimension;

	// run fisher encoding
	DATATYPE* enc = (DATATYPE*) vl_malloc(sizeof(DATATYPE) * dms);// allocate space for the encoding
	vl_fisher_encode(enc, CLUSTERING_DATATYPE, vl_gmm_get_means(gmm), dimension,
			numClusters, vl_gmm_get_covariances(gmm), vl_gmm_get_priors(gmm),
			data, numData,
			VL_FISHER_FLAG_IMPROVED);

	// put into the vector list
	pthread_mutex_lock(&mutex);
	fvlist.push_back(enc);
	labels.push_back(label);
	pthread_mutex_unlock(&mutex);

	// free the memory
	vl_free(enc);
	vl_gmm_delete(gmm);
	vl_kmeans_delete(kmeans);
}

//void svm_classify() {
//	double lambda = 0.01;
//	double bias;
//
//	cout << "allocating data: ";
//	flush(cout);
//	int numData = fvlist.size();
//	cout << numData << "x" << dms << "... ";
//	flush(cout);
//	double data[numData*dms];
//	double label[numData];
//	cout << "copying data... " << numData << "x" << dms << "... ";
//	flush(cout);
//	for (int i = 0; i < numData; i++)
//	{
//		for (int j = 0; j < dms; j++)
//			data[i*dms + j] = (double)(fvlist[i])[j];
////		label[i] = (double)labels[i];
//	}
//
//	cout << "training svm... ";
//	flush(cout);
//	VlSvm* svm = vl_svm_new(VlSvmSolverSgd, data, dms, numData, label, lambda);
//	cout << "start svm... ";
//	flush(cout);
//	vl_svm_train(svm);
//
//	double const * model = vl_svm_get_model(svm);
//	bias = vl_svm_get_bias(svm);
////	printf("model w = [ %f , %f ] , bias b = %f \n", model[0], model[1], bias);
//	vl_svm_delete(svm);
//}

#endif
