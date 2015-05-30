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


vector<vector<int> > crear_Bag_Of_Words(bool stopwords,int num_bag_of_words){
	BagOfWords bagW;
	return bagW.crear_Bag_Of_Words(stopwords,num_bag_of_words);
}

void limpiarReviews(){
	FILE *origin;
	FILE *destiny;
	FileCleaner fileCleaner = FileCleaner();

	origin = fopen("../Archivos/labeledTrainData.tsv", "r");
	destiny = fopen("../Archivos/edited.tsv", "w");

	fileCleaner.cleanFile(origin, destiny);

	fclose(origin);
	fclose(destiny);
}

void ejecutarPerceptronNormal(int num_bag_of_words, int cant_reviews_entrenamiento, vector<vector<int> > vectorBagOW){
	Perceptron perc = Perceptron(num_bag_of_words, cant_reviews_entrenamiento, vectorBagOW);
	
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
	
	vector<vector<int> > vectorBagOW;
	
	limpiarReviews();

	if (bagOfWords){
		vectorBagOW = crear_Bag_Of_Words(eliminar_stopwords,num_bag_of_words);
	}
	if (hashingTrick){
		//poner funcion que hace hashing trick
	}

	if (perceptronNormal){
		ejecutarPerceptronNormal(num_bag_of_words,cant_reviews_entrenamiento,vectorBagOW);
	}
	cout << "YOCA" << endl;
	return 0;
}
