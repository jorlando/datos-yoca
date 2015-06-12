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
#include "Ballseptron.h"
#include "LargeMargin.h"
#include "HashingTrick.h"

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
	/*Perceptron perc = Perceptron(num_bag_of_words, cant_reviews_entrenamiento, vectorBagOW, sentiment,vectorTestBagOW, cant_reviews_test, ids);
	vector<float> result_test_clasif (cant_reviews_test);
	result_test_clasif=perc.perceptronNormal();
	perc.saveResults(result_test_clasif)*/
}

void clasificacionSarcasmo(vector<float> &result_test_clasif, vector<float> &result_test_sarcasm, int cant_reviews_test){
	for (int r = 0; r < cant_reviews_test; r++){
		if(result_test_sarcasm[r]>=0.8){
			result_test_clasif[r]=1-result_test_clasif[r];
		}
	}
}

void ejecutarBallseptron(int num_bag_of_words, int cant_reviews_entrenamiento, vector<vector<int> > vectorBagOW,vector<int> sentiment,vector<vector<int> > vectorTestBagOW, int cant_reviews_test, vector<string> ids){
	Ballseptron ball = Ballseptron(num_bag_of_words, cant_reviews_entrenamiento, vectorBagOW, sentiment,vectorTestBagOW, cant_reviews_test, ids);
	ball.ballseptron();
}

void ejecutarLargeMargin(int num_bag_of_words, int cant_reviews_entrenamiento, vector<vector<int> > vectorBagOW,vector<int> sentiment,vector<vector<int> > vectorTestBagOW, int cant_reviews_test, vector<string> ids){
	LargeMargin large = LargeMargin(num_bag_of_words, cant_reviews_entrenamiento, vectorBagOW, sentiment,vectorTestBagOW, cant_reviews_test, ids);
	large.LargeMarginCalculo();
}
int main() {
	bool eliminar_stopwords = true;
	bool bagOfWords = true;
	bool hashingTrick = false;
	bool perceptronNormal = true;
	bool ballseptron = false;
	bool large_margin = false;
	int num_bag_of_words = 12000;
	int cant_reviews_entrenamiento = 25000;
	int cant_reviews_test = 25000;
	int cant_reviews_sarcasm = 1244;
	bool bool_ngrams = true;
	int ngrams = 2;
	bool analizeSarcasm = true;
	
	vector<vector<int> > vectorBagOW;
	vector<vector<int> > vectorTestBagOW;
	vector<vector<int> > vectorBagOWSarcasm;
	BagOfWords bagW = BagOfWords ();
	BagOfWords bagWSarcasm = BagOfWords ();
	HashingTrick hashing = HashingTrick(num_bag_of_words); //probar con otras dimensiones
	
	cout<<"Limpieza de reviews"<<endl;
	limpiarReviews("../Archivos/labeledTrainData.tsv", "../Archivos/edited.tsv");
	limpiarReviews("../Archivos/testData.tsv", "../Archivos/testEdited.tsv");


	cout<<"Se crean los vectores de Bag of Words"<<endl;
	if (bagOfWords){
		vectorBagOW = bagW.crear_Bag_Of_Words(eliminar_stopwords,num_bag_of_words,cant_reviews_entrenamiento, "../Archivos/edited.tsv",bool_ngrams,ngrams);
		vectorTestBagOW = bagW.crear_Bag_Of_Words_test(num_bag_of_words,cant_reviews_test, "../Archivos/testEdited.tsv",bool_ngrams,ngrams);
	}
	if (hashingTrick){
		hashing.aplicar_The_Hashing_Trick(vectorBagOW, cant_reviews_entrenamiento, num_bag_of_words+1);
		hashing.aplicar_The_Hashing_Trick(vectorTestBagOW, cant_reviews_test, num_bag_of_words+1);
		
	}
	
	if(analizeSarcasm){
		cout<<"Limpio reviews Sarcasmo"<<endl;
		limpiarReviews("../Archivos/SarcasmReview.txt", "../Archivos/editedSarcasm.tsv");
		cout<<"Creo Bag of Words Sarcasmo"<<endl;
		vectorBagOWSarcasm = bagWSarcasm.crear_Bag_Of_Words(eliminar_stopwords,num_bag_of_words,cant_reviews_sarcasm, "../Archivos/editedSarcasm.tsv",bool_ngrams,ngrams);
		if(hashingTrick){
			hashing.aplicar_The_Hashing_Trick(vectorBagOWSarcasm, cant_reviews_sarcasm, num_bag_of_words+1); 
		}
	}

	cout<<"Se ejecuta el Perceptron"<<endl;
	if (perceptronNormal){
		vector<int> sentiment = bagW.getSentiment();
		vector<string> ids = bagW.getIds();
		//ejecutarPerceptronNormal(num_bag_of_words, cant_reviews_entrenamiento, vectorBagOW, sentiment, vectorTestBagOW, cant_reviews_test, ids);
		Perceptron perc = Perceptron(num_bag_of_words, cant_reviews_entrenamiento, vectorBagOW, sentiment,vectorTestBagOW, cant_reviews_test, ids);
		vector<float> result_test_clasif (cant_reviews_test);
		result_test_clasif=perc.perceptronNormal();
		
		if(analizeSarcasm){
			vector<int> sentimentSarcasm = bagWSarcasm.getSentiment();
			vector<string> idsSarcasm = bagWSarcasm.getIds();
			Perceptron perceptronSarcasm = Perceptron(num_bag_of_words,cant_reviews_sarcasm,vectorBagOWSarcasm, sentimentSarcasm,vectorTestBagOW, cant_reviews_test, idsSarcasm);
			vector<float> result_test_sarcasm (cant_reviews_test);
			result_test_sarcasm=perceptronSarcasm.perceptronNormal();
			clasificacionSarcasmo(result_test_clasif,result_test_sarcasm,cant_reviews_test);
		}
		perc.saveResults(result_test_clasif);
	}
	else if (ballseptron){
		vector<int> sentiment = bagW.getSentiment();
		vector<string> ids = bagW.getIds();
		ejecutarBallseptron(num_bag_of_words,cant_reviews_entrenamiento,vectorBagOW, sentiment,vectorTestBagOW, cant_reviews_test, ids);
	}
	else if (large_margin){
		vector<int> sentiment = bagW.getSentiment();
		vector<string> ids = bagW.getIds();
		ejecutarLargeMargin(num_bag_of_words,cant_reviews_entrenamiento,vectorBagOW, sentiment,vectorTestBagOW, cant_reviews_test, ids);
	}
	cout << "YOCA" << endl;
	return 0;
}
