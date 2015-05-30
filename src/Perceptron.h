#ifndef PERCEPTRON_H_
#define PERCEPTRON_H_


#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

class Perceptron {
	
	int num_bag_of_words;
	int cant_reviews_entrenamiento;
	vector<vector<int> > vectorBagOW;
	
public:
	Perceptron();
	Perceptron(int num_bag_of_words, int cant_reviews_entrenamiento, vector<vector<int> > vectorBagOW);
	virtual ~Perceptron();
	void perceptronNormal();

private:


};

#endif 