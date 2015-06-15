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

void ejecutarPerceptronNormal(int num_bag_of_words, int cant_reviews_entrenamiento, vector<vector<int> > vectorBagOW,vector<int> sentiment,vector<vector<int> > vectorTestBagOW, int cant_reviews_test, vector<string> ids, vector< vector<int> > vec_entr, vector< vector<int> > vec_test, bool hashingTrick){
	Perceptron perc = Perceptron(num_bag_of_words, cant_reviews_entrenamiento, vectorBagOW, sentiment,vectorTestBagOW, cant_reviews_test, ids, vec_entr, vec_test, hashingTrick);
	perc.perceptronNormal();
}


void clasificacionSarcasmo(vector<float> &result_test_clasif, vector<float> &result_test_sarcasm, int cant_reviews_test, float porcentaje){
	int x = 0;
	for (int r = 0; r < cant_reviews_test; r++){
		if(result_test_sarcasm[r]>porcentaje){
			x += 1;
			cout<<x<<endl;
			result_test_clasif[r]=1-result_test_clasif[r];
		}
	}
}


void ejecutarPerceptronNormal(int num_bag_of_words, int cant_reviews_entrenamiento, vector<vector<pair<int,int> > > vectorHash,vector<int> sentiment,vector<vector<pair<int,int> > > vectorHashTest, int cant_reviews_test, vector<string> ids, vector< vector<int> > vec_entr, vector< vector<int> > vec_test, bool hashingTrick){
	Perceptron perc = Perceptron(num_bag_of_words, cant_reviews_entrenamiento, vectorHash, sentiment,vectorHashTest, cant_reviews_test, ids, vec_entr, vec_test, hashingTrick);
	perc.perceptronNormal();
}

vector<float> ejecutarBallseptron(float radio_ball, int num_bag_of_words, int cant_reviews_entrenamiento, vector<vector<int> > vectorBagOW,vector<int> sentiment,vector<vector<int> > vectorTestBagOW, int cant_reviews_test, vector<string> ids, vector< vector<int> > vec_entr, vector< vector<int> > vec_test, bool hashingTrick){
	Ballseptron ball = Ballseptron(radio_ball, num_bag_of_words, cant_reviews_entrenamiento, vectorBagOW, sentiment,vectorTestBagOW, cant_reviews_test, ids, vec_entr, vec_test, hashingTrick);
	return ball.ballseptron();
}

vector<float> ejecutarBallseptron(float radio_ball, int num_bag_of_words, int cant_reviews_entrenamiento, vector<vector<pair<int,int> > > vectorHash,vector<int> sentiment,vector<vector<pair<int,int> > > vectorHashTest, int cant_reviews_test, vector<string> ids, vector< vector<int> > vec_entr, vector< vector<int> > vec_test, bool hashingTrick){
	Ballseptron ball = Ballseptron(radio_ball, num_bag_of_words, cant_reviews_entrenamiento, vectorHash, sentiment,vectorHashTest, cant_reviews_test, ids, vec_entr, vec_test, hashingTrick);
	return ball.ballseptron();
}

vector<float> ejecutarLargeMargin(int num_bag_of_words, int cant_reviews_entrenamiento, vector<vector<int> > vectorBagOW,vector<int> sentiment,vector<vector<int> > vectorTestBagOW, int cant_reviews_test, vector<string> ids, int cant_vec_pesos, vector< vector<int> > vec_entr, vector< vector<int> > vec_test, bool hashingTrick){
	LargeMargin large = LargeMargin(num_bag_of_words, cant_reviews_entrenamiento, vectorBagOW, sentiment,vectorTestBagOW, cant_reviews_test, ids, cant_vec_pesos, vec_entr, vec_test, hashingTrick);
	return large.LargeMarginCalculo();
}

vector<float> ejecutarLargeMargin(int num_bag_of_words, int cant_reviews_entrenamiento, vector<vector<pair<int,int> > > vectorHash,vector<int> sentiment,vector<vector<pair<int,int> > > vectorHashTest, int cant_reviews_test, vector<string> ids, int cant_vec_pesos, vector< vector<int> > vec_entr, vector< vector<int> > vec_test, bool hashingTrick){
	LargeMargin large = LargeMargin(num_bag_of_words, cant_reviews_entrenamiento, vectorHash, sentiment,vectorHashTest, cant_reviews_test, ids, cant_vec_pesos, vec_entr, vec_test, hashingTrick);
	return large.LargeMarginCalculo();
}

void escribirResultados(int cant_reviews_test, vector<string> ids, vector<float> result_test, string nombre_resultado){
	//Armo el archivo para el submission
	ofstream arch (nombre_resultado.c_str());
	arch << "id,sentiment\n";
	for (int r = 0; r < cant_reviews_test; r++){
		arch << ids[r];
		arch << ",";
		arch << result_test[r];
		arch << "\n";
	}
	arch.close();
}

int main() {
	int cant_reviews_test = 25000;
	int cant_reviews_sarcasm = 1244;

	//Menu
	cout << "Bienvenido al programa para realizar submission para la competicion 'Bag of Words Meets Bags of Popcorn'" << endl;
	
	bool eliminar_stopwords = true;
	string no_stop;
	cout << "Eliminar stopwords? [S/N]: ";
	cin >> no_stop;
	if ((no_stop == string("N"))||(no_stop == string("n"))){
		eliminar_stopwords = false;
	}
	else if ((!(no_stop == string("S")))&&(!(no_stop == string("s")))){
		cout << "Valor incorrecto. Por defecto, se eliminaran las stopwords." << endl;
	}
	
	int cant_reviews_entrenamiento = 25000;
	string archivo_reviews = string("../Archivos/labeledTrainData.tsv");
	string reviews;
	cout << "Que set de reviews de entrenamiento usar? [1/2]: " << endl;
	cout << "1 - LabeledTrainData" << endl;
	cout << "2 - UnLabeledTrainData clasificadas con apis + LabeledTrainData" << endl;
	cin >> reviews;
	if (reviews == string("2")){
		cant_reviews_entrenamiento = 72689;
		archivo_reviews = string("../Archivos/unlabeledTrainDataClassifiedReview_Mas_labeled_train_data.tsv");
	}
	else if (!(reviews == string("1"))){
		cout << "Valor incorrecto. Por defecto, se utilizara LabeledTrainData." << endl;
	}
	
	
	bool bagOfWords = true;
	bool hashingTrick = false;
	string bagHash;
	cout << "Usar Bag Of Words normal o con Hashing Trick? [B/H]: ";
	cin >> bagHash;
	if ((bagHash == string("H"))||(bagHash == string("h"))){
		bagOfWords = false;
		hashingTrick = true;
	}
	else if ((!(bagHash == string("B")))&&(!(bagHash == string("b")))){
		cout << "Valor incorrecto. Por defecto, se utilizara Bag of Words normal." << endl;
	}
	
	
	int num_bag_of_words = 10000;
	string cant_bag;
	cout << "De que tamaño construirlo? ";
	cin >> cant_bag;
	num_bag_of_words = atoi(cant_bag.c_str());
	
	
	bool bool_ngrams = true;
	int ngrams = 2;
	string resp_ngrams;
	string cuantos_ngrams;
	cout << "Usar ngrams? [S/N]: ";
	cin >> resp_ngrams;
	if ((resp_ngrams == string("N"))||(resp_ngrams == string("n"))){
		bool_ngrams = false;
	}
	else{
		if ((!(resp_ngrams == string("S")))&&(!(resp_ngrams == string("s")))){
			cout << "Valor incorrecto. Por defecto, se utilizaran ngrams." << endl;
		}
		cout << "Cuantos ngrmas usar? [2/3]: ";
		cin >> cuantos_ngrams;
		ngrams = atoi(cuantos_ngrams.c_str());
	}
	
	
	bool analizeSarcasm = true;
	float porcentaje = 0.8;
	string resp_sarcasm;
	string que_porc;
	cout << "Usar el detector de sarcasmo? [S/N]: ";
	cin >> resp_sarcasm;
	if ((resp_sarcasm == string("N"))||(resp_sarcasm == string("n"))){
		analizeSarcasm = false;
	}
	else{
		if ((!(resp_sarcasm == string("S")))&&(!(resp_sarcasm == string("s")))){
			cout << "Valor incorrecto. Por defecto, se utilizaran el detector de sarcasmo." << endl;
		}
		cout << "Que porcentaje de sarcasmo utilizar? (se recomienda 0.5 para HashingTrick, 0.95 para Bag Of Words): ";
		cin >> que_porc;
		porcentaje = atof(que_porc.c_str());
	}
	
	
	bool perceptronNormal = true;
	bool ballseptron = false;
	bool large_margin = false;
	float radio_ball = 0.007;
	int cant_vec_pesos = 1;  //hasta 60 mas o menos
	string tipo;
	cout << "Que tipo de clasificador usar? [1/2/3]" << endl;
	cout << "1 - Perceptron Clasico " << endl;
	cout << "2 - Ballseptron" << endl;
	cout << "3 - Large Margin" << endl;
	cin >> tipo;
	if (tipo == string("2")){
		perceptronNormal = false;
		ballseptron = true;
		string radio;
		cout << "Que radio de Ballseptron utilizar? (se recomienda de 0.005 a 0.01) " << endl;
		cin >> radio;
		radio_ball = atof(radio.c_str());
	}
	else if (tipo == string("3")){
		perceptronNormal = false;
		large_margin = true;
		string cant_large;
		cout << "Que cantidad de vectores de pesos utilizar? (se recomienda usar como maximo0 60) " << endl;
		cin >> cant_large;
		cant_vec_pesos = atoi(cant_large.c_str());
	}
	else if (!(tipo == string("1"))){
		cout << "Valor incorrecto. Por defecto, se utilizara Percepton clasico." << endl;
	}

	
	string nombre_result;
	cout << "Nombre del archivo de resultados: ";
	cin >> nombre_result;
	string nombre_resultado = "../Resultados/" + nombre_result + ".csv";
	
	
	
	
	vector<vector<int> > vectorBagOW;
	vector<vector<int> > vectorTestBagOW;
	vector<vector<int> > vectorBagOWSarcasm;
	vector<vector<pair<int,int> > > vectorHash;
	vector<vector<pair<int,int> > > vectorHashSarcasm;
	vector<vector<pair<int,int> > > vectorHashTest;
	BagOfWords bagW = BagOfWords ();
	BagOfWords bagWSarcasm = BagOfWords ();
	HashingTrick hashing = HashingTrick(num_bag_of_words); //probar con otras dimensiones
	HashingTrick hashingSarcasm = HashingTrick(num_bag_of_words); 
	
	cout<<"Limpieza de reviews"<<endl;
	limpiarReviews(archivo_reviews, "../Archivos/edited.tsv");
	limpiarReviews("../Archivos/testData.tsv", "../Archivos/testEdited.tsv");

	if (bagOfWords){
		cout<<"Se crean los vectores de Bag of Words"<<endl;
		vectorBagOW = bagW.crear_Bag_Of_Words(eliminar_stopwords,num_bag_of_words,cant_reviews_entrenamiento, "../Archivos/edited.tsv",bool_ngrams,ngrams);
		vectorTestBagOW = bagW.crear_Bag_Of_Words_test(num_bag_of_words,cant_reviews_test, "../Archivos/testEdited.tsv",bool_ngrams,ngrams);
	}
	if (hashingTrick){
		cout<<"Se crean los vectores con HashingTrick"<<endl;
		vectorHash = hashing.aplicar_The_Hashing_Trick(eliminar_stopwords,cant_reviews_entrenamiento, "../Archivos/edited.tsv",bool_ngrams,ngrams);
		vectorHashTest = hashing.aplicar_The_Hashing_Trick_test(eliminar_stopwords,cant_reviews_test, "../Archivos/testEdited.tsv",bool_ngrams,ngrams);
	}
	if(analizeSarcasm){
		cout<<"Limpio reviews Sarcasmo"<<endl;
		limpiarReviews("../Archivos/SarcasmReview.txt", "../Archivos/editedSarcasm.tsv");
		
		if (bagOfWords){
			cout<<"Creo Bag of Words Sarcasmo"<<endl;
			vectorBagOWSarcasm = bagWSarcasm.crear_Bag_Of_Words(eliminar_stopwords,num_bag_of_words,cant_reviews_sarcasm, "../Archivos/editedSarcasm.tsv",bool_ngrams,ngrams);
		}
		else{
			cout<<"Creo vectores con HashingTrick Sarcasmo"<<endl;
			vectorHashSarcasm = hashingSarcasm.aplicar_The_Hashing_Trick(eliminar_stopwords,cant_reviews_sarcasm, "../Archivos/editedSarcasm.tsv",bool_ngrams,ngrams); 
		}
	}
	
	if (perceptronNormal){
		cout<<"Se ejecuta el Perceptron"<<endl;
		if(!hashingTrick){
			vector<int> sentiment = bagW.getSentiment();
			vector<string> ids = bagW.getIds();
			vector< vector<int> > vec_entr = bagW.getIndices_entr();
			vector< vector<int> > vec_test = bagW.getIndices_test();
			Perceptron perc = Perceptron(num_bag_of_words, cant_reviews_entrenamiento, vectorBagOW, sentiment,vectorTestBagOW, cant_reviews_test, ids, vec_entr, vec_test, hashingTrick);
			vector<float> result_test_clasif (cant_reviews_test);
			result_test_clasif=perc.perceptronNormal();
			if(analizeSarcasm){
				vector<int> sentimentSarcasm = bagWSarcasm.getSentiment();
				vector<string> idsSarcasm = bagWSarcasm.getIds();
				vector< vector<int> > vec_entr_sarcasm = bagWSarcasm.getIndices_entr();
				Perceptron perceptronSarcasm = Perceptron(num_bag_of_words,cant_reviews_sarcasm,vectorBagOWSarcasm, sentimentSarcasm,vectorTestBagOW, cant_reviews_test, idsSarcasm, vec_entr_sarcasm, vec_test, hashingTrick);
				vector<float> result_test_sarcasm (cant_reviews_test);
				result_test_sarcasm=perceptronSarcasm.perceptronNormal();
				clasificacionSarcasmo(result_test_clasif,result_test_sarcasm,cant_reviews_test, porcentaje);
			}
			escribirResultados(cant_reviews_test, ids, result_test_clasif,nombre_resultado);
		}
		else{
			vector<int> sentiment = hashing.getSentiment();
			vector<string> ids = hashing.getIds();
			vector< vector<int> > vec_entr = hashing.getIndices_entr();
			vector< vector<int> > vec_test = hashing.getIndices_test();
			Perceptron perc = Perceptron(num_bag_of_words, cant_reviews_entrenamiento, vectorHash, sentiment,vectorHashTest, cant_reviews_test, ids, vec_entr, vec_test, hashingTrick);
			vector<float> result_test_clasif (cant_reviews_test);
			result_test_clasif=perc.perceptronNormal();
			if(analizeSarcasm){
				vector<int> sentimentSarcasm = hashingSarcasm.getSentiment();
				vector<string> idsSarcasm = hashingSarcasm.getIds();
				vector< vector<int> > vec_entr_sarcasm = hashingSarcasm.getIndices_entr();
				Perceptron perceptronSarcasm = Perceptron(num_bag_of_words, cant_reviews_sarcasm, vectorHashSarcasm, sentimentSarcasm,vectorHashTest, cant_reviews_test, idsSarcasm, vec_entr_sarcasm, vec_test, hashingTrick);
				vector<float> result_test_sarcasm (cant_reviews_test);
				result_test_sarcasm=perceptronSarcasm.perceptronNormal();
				clasificacionSarcasmo(result_test_clasif,result_test_sarcasm,cant_reviews_test, porcentaje);
			}
			escribirResultados(cant_reviews_test, ids, result_test_clasif,nombre_resultado);
		}
		
	}
	else if (ballseptron){
		cout<<"Se ejecuta el Ballseptron"<<endl;
		if(!hashingTrick){
			vector<int> sentiment = bagW.getSentiment();
			vector<string> ids = bagW.getIds();
			vector< vector<int> > vec_entr = bagW.getIndices_entr();
			vector< vector<int> > vec_test = bagW.getIndices_test();
			vector<float> result_test_clasif (cant_reviews_test);
			result_test_clasif = ejecutarBallseptron(radio_ball, num_bag_of_words,cant_reviews_entrenamiento,vectorBagOW, sentiment,vectorTestBagOW, cant_reviews_test, ids, vec_entr, vec_test, hashingTrick);
			if(analizeSarcasm){
				vector<int> sentimentSarcasm = bagWSarcasm.getSentiment();
				vector<string> idsSarcasm = bagWSarcasm.getIds();
				vector< vector<int> > vec_entr_sarcasm = bagWSarcasm.getIndices_entr();
				Perceptron perceptronSarcasm = Perceptron(num_bag_of_words,cant_reviews_sarcasm,vectorBagOWSarcasm, sentimentSarcasm,vectorTestBagOW, cant_reviews_test, idsSarcasm, vec_entr_sarcasm, vec_test, hashingTrick);
				vector<float> result_test_sarcasm (cant_reviews_test);
				result_test_sarcasm=perceptronSarcasm.perceptronNormal();
				clasificacionSarcasmo(result_test_clasif,result_test_sarcasm,cant_reviews_test, porcentaje);
			}
			escribirResultados(cant_reviews_test, ids, result_test_clasif,nombre_resultado);
		}
		else{
			vector<int> sentiment = hashing.getSentiment();
			vector<string> ids = hashing.getIds();
			vector< vector<int> > vec_entr = hashing.getIndices_entr();
			vector< vector<int> > vec_test = hashing.getIndices_test();
			vector<float> result_test_clasif (cant_reviews_test);
			result_test_clasif = ejecutarBallseptron(radio_ball, num_bag_of_words,cant_reviews_entrenamiento,vectorHash, sentiment,vectorHashTest, cant_reviews_test, ids, vec_entr, vec_test, hashingTrick);
			if(analizeSarcasm){
				vector<int> sentimentSarcasm = hashingSarcasm.getSentiment();
				vector<string> idsSarcasm = hashingSarcasm.getIds();
				vector< vector<int> > vec_entr_sarcasm = hashingSarcasm.getIndices_entr();
				Perceptron perceptronSarcasm = Perceptron(num_bag_of_words, cant_reviews_sarcasm, vectorHashSarcasm, sentimentSarcasm,vectorHashTest, cant_reviews_test, idsSarcasm, vec_entr_sarcasm, vec_test, hashingTrick);
				vector<float> result_test_sarcasm (cant_reviews_test);
				result_test_sarcasm=perceptronSarcasm.perceptronNormal();
				clasificacionSarcasmo(result_test_clasif,result_test_sarcasm,cant_reviews_test, porcentaje);
			}
			escribirResultados(cant_reviews_test, ids, result_test_clasif,nombre_resultado);
		}
	}
	else if (large_margin){
		cout<<"Se ejecuta el Large Margin"<<endl;
		if(!hashingTrick){
			vector<int> sentiment = bagW.getSentiment();
			vector<string> ids = bagW.getIds();
			vector< vector<int> > vec_entr = bagW.getIndices_entr();
			vector< vector<int> > vec_test = bagW.getIndices_test();
			vector<float> result_test_clasif (cant_reviews_test);
			result_test_clasif = ejecutarLargeMargin(num_bag_of_words,cant_reviews_entrenamiento,vectorBagOW, sentiment,vectorTestBagOW, cant_reviews_test, ids, cant_vec_pesos, vec_entr, vec_test, hashingTrick);
			if(analizeSarcasm){
				vector<int> sentimentSarcasm = bagWSarcasm.getSentiment();
				vector<string> idsSarcasm = bagWSarcasm.getIds();
				vector< vector<int> > vec_entr_sarcasm = bagWSarcasm.getIndices_entr();
				Perceptron perceptronSarcasm = Perceptron(num_bag_of_words,cant_reviews_sarcasm,vectorBagOWSarcasm, sentimentSarcasm,vectorTestBagOW, cant_reviews_test, idsSarcasm, vec_entr_sarcasm, vec_test, hashingTrick);
				vector<float> result_test_sarcasm (cant_reviews_test);
				result_test_sarcasm=perceptronSarcasm.perceptronNormal();
				clasificacionSarcasmo(result_test_clasif,result_test_sarcasm,cant_reviews_test, porcentaje);
			}
			escribirResultados(cant_reviews_test, ids, result_test_clasif,nombre_resultado);
		}
		else{
			vector<int> sentiment = hashing.getSentiment();
			vector<string> ids = hashing.getIds();
			vector< vector<int> > vec_entr = hashing.getIndices_entr();
			vector< vector<int> > vec_test = hashing.getIndices_test();
			vector<float> result_test_clasif (cant_reviews_test);
			result_test_clasif = ejecutarLargeMargin(num_bag_of_words,cant_reviews_entrenamiento,vectorHash, sentiment,vectorHashTest, cant_reviews_test, ids, cant_vec_pesos, vec_entr, vec_test, hashingTrick);
			if(analizeSarcasm){
				vector<int> sentimentSarcasm = hashingSarcasm.getSentiment();
				vector<string> idsSarcasm = hashingSarcasm.getIds();
				vector< vector<int> > vec_entr_sarcasm = hashingSarcasm.getIndices_entr();
				Perceptron perceptronSarcasm = Perceptron(num_bag_of_words, cant_reviews_sarcasm, vectorHashSarcasm, sentimentSarcasm,vectorHashTest, cant_reviews_test, idsSarcasm, vec_entr_sarcasm, vec_test, hashingTrick);
				vector<float> result_test_sarcasm (cant_reviews_test);
				result_test_sarcasm=perceptronSarcasm.perceptronNormal();
				clasificacionSarcasmo(result_test_clasif,result_test_sarcasm,cant_reviews_test, porcentaje);
			}
			escribirResultados(cant_reviews_test, ids, result_test_clasif,nombre_resultado);
		}
	}
	cout << "YOCA" << endl;
	return 0;
}
