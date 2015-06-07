#ifndef LARGEMARGIN_H_
#define LARGEMARGIN_H_


#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
using namespace std;

class LargeMargin {
	
	int num_bag_of_words;
	int cant_reviews_entrenamiento;
	vector<vector<int> > vectorBagOW;
	vector<int> sentiment;
	vector<vector<int> > vectorTestBagOW;
	int cant_reviews_test;
	vector<string> ids;
	
public:
	LargeMargin();
	LargeMargin(int num_bag_of_words, int cant_reviews_entrenamiento, vector<vector<int> > vectorBagOW, vector<int> sentiment,vector<vector<int> > vectorTestBagOW, int cant_reviews_test,vector<string> ids);
	virtual ~LargeMargin();
	void LargeMarginCalculo();

private:
	vector<float> calcular_nuevos_pesos(vector<float> pesos, int sentiment,int resultado,vector<int> vecBagOfW);

};

#endif 