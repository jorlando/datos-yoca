#ifndef LARGEMARGIN_H_
#define LARGEMARGIN_H_


#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <math.h> 
using namespace std;

class LargeMargin {
	
	int num_bag_of_words;
	int cant_reviews_entrenamiento;
	vector<vector<int> > vectorBagOW;
	vector<int> sentiment;
	vector<vector<int> > vectorTestBagOW;
	int cant_reviews_test;
	vector<string> ids;
	int cant_vec_pesos;
	vector< vector<int> > vec_entr;
	vector< vector<int> > vec_test;
	vector<vector<pair<int,int> > > vectorHash;
	vector<vector<pair<int,int> > > vectorHashTest;
	bool hashingTrick;
	vector<vector<float> > suma_pesos;
	int cant_sob;
	
public:
	LargeMargin();
	LargeMargin(int num_bag_of_words, int cant_reviews_entrenamiento, vector<vector<int> > vectorBagOW, vector<int> sentiment,vector<vector<int> > vectorTestBagOW, int cant_reviews_test,vector<string> ids, int cant_vec_pesos, vector< vector<int> > vec_entr, vector< vector<int> > vec_test, bool hashingTrick);
	LargeMargin(int num_bag_of_words, int cant_reviews_entrenamiento, vector<vector<pair<int,int> > > vectorHash, vector<int> sentiment,vector<vector<pair<int,int> > > vectorHashTest, int cant_reviews_test,vector<string> ids, int cant_vec_pesos, vector< vector<int> > vec_entr, vector< vector<int> > vec_test, bool hashingTrick);
	virtual ~LargeMargin();
	vector<float> LargeMarginCalculo();

private:
	vector<float> calcular_nuevos_pesos(vector<float> pesos, int sentiment,int resultado,vector<int> vecBagOfW, int review);
	vector<float> calcular_nuevos_pesos(vector<float> pesos, int sent,int resultado,vector<pair<int, int> > vecHash, int review);

};

#endif 