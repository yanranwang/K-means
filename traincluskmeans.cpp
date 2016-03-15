#include <fstream>
#include <string.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <iomanip>
#include <string>
#include "vl/kmeans.h"
using namespace std;
#define dataType float
#define CLUSTERNUMBER 4000
#define DIM 96
#define LINES 406489
#define MOVEQ 37
dataType datmat[LINES * DIM];
int	labels[LINES];
float* centers;//[4000*DIM];
vector<dataType> splitex(string sr) {
      	vector<dataType> datavec;
      	string strtemp;
            
       	string::size_type pos1, pos2;
       	pos2 = sr.find('\t');
      	pos1 = 0;
	while (string::npos != pos2) {
             	datavec.push_back((float)atof(sr.substr(pos1, pos2 - pos1).c_str()));
             	pos1 = pos2 + 1;
           	pos2 = sr.find('\t', pos1);
    	}
     	datavec.push_back((float)atof(sr.substr(pos1).c_str()));

     	return datavec;
}
int main() {
	ifstream ins("/Joyce/video/youtubesample.txt");
	if(!ins.is_open()) cout << "error:cant't open file" << endl;
	string s;
	int count = 0;
	vector<dataType> featuredata;
	for (int i = 0; i < LINES; i++) {
		getline(ins, s);
		featuredata = splitex(s);
		for (int j = 0; j < DIM; j++) {
			datmat[count] = featuredata[j + MOVEQ];
			count++;
		}
	}
	ins.close();
	cout << "it's get data!" << endl;
	
	VlKMeansAlgorithm algorithm = VlKMeansLloyd ;
 	VlVectorComparisonType distance = VlDistanceL2 ;
 	VlKMeans * kmeans = vl_kmeans_new (VL_TYPE_FLOAT, distance) ;
 	//vl_kmeans_seed_centers_with_rand_data (kmeans, datmat, DIM, LINES, CLUSTERNUMBER) ;
 	vl_kmeans_set_max_num_iterations (kmeans, 100) ;
	vl_kmeans_set_algorithm(kmeans, algorithm);
	vl_kmeans_set_num_repetitions(kmeans, 8);
	vl_kmeans_set_verbosity(kmeans, 1);
	vl_kmeans_set_initialization(kmeans, VlKMeansRandomSelection);
	vl_kmeans_cluster(kmeans, datmat, DIM, LINES, CLUSTERNUMBER);
 	//vl_kmeans_refine_centers (kmeans, datmat, LINES) ;
	
	centers = (float*)vl_kmeans_get_centers(kmeans);

	//CLUTO_VP_ClusterRB(LINES,DIM,NULL,NULL,datmat,CLUTO_SIM_EDISTANCE,CLUTO_CLFUN_I1,CLUTO_ROWMODEL_MAXTF,CLUTO_COLMODEL_NONE,1.0,8,15,10,CLUTO_CSTYPE_BESTFIRST,0,0,CLUSTERNUMBER,labels);
	
	
	ofstream outs;
	outs.open("/Joyce/video/clusterYoutubeHOGkmeans4000.txt");
	if (!outs.is_open()) cout << "error:cant't open file" << endl;
	else {
		//outs<<fixed<<setprecision(6);
		for (int i = 0; i < CLUSTERNUMBER; i++) {
			for (int j = 0; j < DIM; j++) {
				outs << centers[i * DIM + j] << '\t';
			}
			outs << endl;
		}
	}
	return 1;
}

