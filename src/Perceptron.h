#ifndef PERCEPTRON_H_
#define PERCEPTRON_H_


#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
using namespace std;

class Perceptron {
	
	int num_bag_of_words;
	int cant_reviews_entrenamiento;
	vector<vector<int> > vectorBagOW;
	vector<int> sentiment;
	vector<vector<int> > vectorTestBagOW;
	int cant_reviews_test;
	vector<string> ids;
	
public:
	Perceptron();
	Perceptron(int num_bag_of_words, int cant_reviews_entrenamiento, vector<vector<int> > vectorBagOW, vector<int> sentiment,vector<vector<int> > vectorTestBagOW, int cant_reviews_test,vector<string> ids);
	virtual ~Perceptron();
	vector<float> perceptronNormal();
	void saveResults(vector<float> &result_test);

private:
	vector<float> calcular_nuevos_pesos(vector<float> pesos, int sentiment,int resultado,vector<int> vecBagOfW);
	void entrenamiento(vector<float> &pesos, int cant_mas_bias);
	void clasificoTests(vector<float> &result_test, int cant_mas_bias, vector<float> &pesos);
	

};

#endif 