//============================================================================
// Name        : Bag.cpp
// Author      : Maxi
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include "FileCleaner.h"
#include "BagOfWords.h"
#include "Perceptron.h"

using namespace std;


void limpiarReviews(string in, string out){
	FILE *origin;
	FILE *destiny;
	FileCleaner fileCleaner = FileCleaner();

	origin = fopen(in.c_str(), "r");
	destiny = fopen(out.c_str(), "w");

	fileCleaner.cleanFile(origin, destiny);

	fclose(origin);
	fclose(destiny);
}

void ejecutarPerceptronNormal(int num_bag_of_words, int cant_reviews_entrenamiento, vector<vector<int> > vectorBagOW,vector<int> sentiment,vector<vector<int> > vectorTestBagOW, int cant_reviews_test, vector<string> ids){
	Perceptron perc = Perceptron(num_bag_of_words, cant_reviews_entrenamiento, vectorBagOW, sentiment,vectorTestBagOW, cant_reviews_test, ids);
	
	perc.perceptronNormal();
}

int main() {
	bool eliminar_stopwords = true;
	bool bagOfWords = true;
	bool hashingTrick = false;
	bool perceptronNormal = true;
	bool ballseptron = false;
	int num_bag_of_words = 10000;
	int cant_reviews_entrenamiento = 25000;
	int cant_reviews_test = 25000;
	bool bool_ngrams = true;
	int ngrams = 3;
	
	vector<vector<int> > vectorBagOW;
	vector<vector<int> > vectorTestBagOW;
	BagOfWords bagW = BagOfWords ();
	
	cout<<"Limpieza de reviews"<<endl;
	limpiarReviews("../Archivos/labeledTrainData.tsv", "../Archivos/edited.tsv");
	limpiarReviews("../Archivos/testData.tsv", "../Archivos/testEdited.tsv");

	cout<<"Se crean los vectores de Bag of Words"<<endl;
	if (bagOfWords){
		vectorBagOW = bagW.crear_Bag_Of_Words(eliminar_stopwords,num_bag_of_words,cant_reviews_entrenamiento, "../Archivos/edited.tsv",bool_ngrams,ngrams);
		vectorTestBagOW = bagW.crear_Bag_Of_Words_test(num_bag_of_words,cant_reviews_test, "../Archivos/testEdited.tsv",bool_ngrams,ngrams);
	}
	if (hashingTrick){
		//poner funcion que hace hashing trick
	}
	
	cout<<"Se ejecuta el Perceptron"<<endl;
	if (perceptronNormal){
		vector<int> sentiment = bagW.getSentiment();
		vector<string> ids = bagW.getIds();
		ejecutarPerceptronNormal(num_bag_of_words,cant_reviews_entrenamiento,vectorBagOW, sentiment,vectorTestBagOW, cant_reviews_test, ids);
	}
	cout << "YOCA" << endl;
	return 0;
}
