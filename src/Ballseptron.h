#ifndef BALLSEPTRON_H_
#define BALLSEPTRON_H_


#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cmath>
using namespace std;

class Ballseptron {
	
	int num_bag_of_words;
	int cant_reviews_entrenamiento;
	vector<vector<int> > vectorBagOW;
	vector<int> sentiment;
	vector<vector<int> > vectorTestBagOW;
	int cant_reviews_test;
	vector<string> ids;
	
public:
	Ballseptron();
	Ballseptron(int num_bag_of_words, int cant_reviews_entrenamiento, vector<vector<int> > vectorBagOW, vector<int> sentiment,vector<vector<int> > vectorTestBagOW, int cant_reviews_test,vector<string> ids);
	virtual ~Ballseptron();
	void ballseptron();

private:
	vector<float> calcular_nuevos_pesos(vector<float> pesos, int sentiment,int resultado,vector<int> vecBagOfW);
	vector<float> calcular_pesos_ballseptron(vector<float> pesos,float mod_pesos,float r,int yt,vector<int> vecBagOfW, int cant_mas_bias, int sentiment,int resultado);

};

#endif 