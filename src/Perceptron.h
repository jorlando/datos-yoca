#ifndef PERCEPTRON_H_
#define PERCEPTRON_H_


#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <math.h> 
using namespace std;

class Perceptron {
	
	int num_bag_of_words;
	int cant_reviews_entrenamiento;
	vector<vector<int> > vectorBagOW;
	vector<int> sentiment;
	vector<vector<int> > vectorTestBagOW;
	int cant_reviews_test;
	vector<string> ids;
	vector< vector<int> > vec_entr;
	vector< vector<int> > vec_test;
	vector<vector<pair<int,int> > > vectorHash;
	vector<vector<pair<int,int> > > vectorHashTest;
	bool hashingTrick;
	
public:
	Perceptron();
	Perceptron(int num_bag_of_words, int cant_reviews_entrenamiento, vector<vector<int> > vectorBagOW, vector<int> sentiment,vector<vector<int> > vectorTestBagOW, int cant_reviews_test,vector<string> ids, vector< vector<int> > vec_entr, vector< vector<int> > vec_test, bool hashing_trick);
	Perceptron(int num_bag_of_words, int cant_reviews_entrenamiento,vector<vector<pair<int,int> > > vectorHash,vector<int> sentiment, vector<vector<pair<int,int> > > vectorHashTest, int cant_reviews_test, vector<string> ids, vector< vector<int> > vec_entr, vector< vector<int> > vec_test, bool hashingTrick);
	virtual ~Perceptron();
	vector<float> perceptronNormal();
	void saveResults(vector<float> &result_test);

private:
	vector<float> calcular_nuevos_pesos(vector<float> pesos, int sentiment,int resultado,vector<int> vecBagOfW, int review);
	vector<float> calcular_nuevos_pesos(vector<float> pesos, int sent,int resultado,vector<pair<int, int> > vecHash, int review);
	void entrenamiento(vector<float> &pesos, int cant_mas_bias);
	void clasificoTests(vector<float> &result_test, int cant_mas_bias, vector<float> &pesos);
};

#endif 