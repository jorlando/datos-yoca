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
#include <math.h> 
using namespace std;

class Ballseptron {
	
	int num_bag_of_words;
	int cant_reviews_entrenamiento;
	vector<vector<int> > vectorBagOW;
	vector<int> sentiment;
	vector<vector<int> > vectorTestBagOW;
	int cant_reviews_test;
	vector<string> ids;
	vector< vector<int> > vec_entr;
	vector< vector<int> > vec_test;
	bool cambiar_mod_pesos;
	float mod_pesos;
	vector<vector<pair<int,int> > > vectorHash;
	vector<vector<pair<int,int> > > vectorHashTest;
	bool hashingTrick;
	float r;
	
public:
	Ballseptron();
	Ballseptron(float r, int num_bag_of_words, int cant_reviews_entrenamiento, vector<vector<int> > vectorBagOW, vector<int> sentiment,vector<vector<int> > vectorTestBagOW, int cant_reviews_test,vector<string> ids, vector< vector<int> > vec_entr, vector< vector<int> > vec_test, bool hashing_trick);
	Ballseptron(float r, int num_bag_of_words, int cant_reviews_entrenamiento, vector<vector<pair<int,int> > > vectorHash, vector<int> sentiment,vector<vector<pair<int,int> > > vectorHashTest, int cant_reviews_test,vector<string> ids, vector< vector<int> > vec_entr, vector< vector<int> > vec_test, bool hashing_trick);
	virtual ~Ballseptron();
	vector<float> ballseptron();

private:
	vector<float> calcular_nuevos_pesos(vector<float> pesos, int sentiment,int resultado,vector<int> vecBagOfW, int review);
	vector<float> calcular_pesos_ballseptron(vector<float> pesos,float mod_pesos,float r,int yt,vector<int> vecBagOfW, int cant_mas_bias, int sentiment,int resultado);
	vector<float> calcular_nuevos_pesos(vector<float> pesos, int sent,int resultado,vector<pair<int, int> > vecHash, int review);

};

#endif 