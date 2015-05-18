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

using namespace std;


template <typename T1, typename T2>
struct less_second {
    typedef pair<T1, T2> type;
    bool operator ()(type const& a, type const& b) const {
        return a.second > b.second;
    }
};


vector<vector<int> > crear_Bag_Of_Words (bool stopwords,int num_bag_of_words){
	//Declaro variables
	vector<string> palabras(num_bag_of_words);
	string line;
	std::map<string,int> mymap;
	std::map<string,int> mapBagOW;
	int cant_reviews = 25000;
	string word;
	vector<vector<int> > bag(cant_reviews);
	std::map<string,int> mapStopw;

	//Abro archivo
	std::ifstream infile("labeledTrainData.tsv");
	//std::ifstream infile("text.txt");

	//Si no se usan stoprwords armo un map con ellas
	if (stopwords){
		std::ifstream stopw("stop-words.txt");
		while(getline(stopw, line)) {
			istringstream iss(line);
			while(iss >> word) {
				mapStopw[word] = 1;
			}
		}
		mapStopw["a"] = 1;
		stopw.close();
	}

	//Leo la primera linea que no me sirve (nombre de campos)
	getline(infile, line);

	//Itero linea a linea, eliminando el id y puntaje de cada una
	while(getline(infile, line)) {
		istringstream iss(line);
		iss >> word;
		iss >> word;
		while(iss >> word) {
			if (!(mapStopw.count(word))){
				mymap[word] += 1;
			}
		}
	}

	//Copio todo lo del map a un vector para ordenar
	vector<pair<string, int> > vector_ordenado(mymap.begin(), mymap.end());
	sort(vector_ordenado.begin(), vector_ordenado.end(), less_second<string, int>());

	//Armo un map con las palabras de mayor frecuncia
	for (int i=0;i<num_bag_of_words; i++ ){
		mapBagOW[vector_ordenado[i].first] = i;
	}

	//Vuelvo al comienzo del archivo de reviews
	infile.clear();
	infile.seekg(0, ios::beg);

	//Leo la primera linea que no me sirve (nombre de campos)
	getline(infile, line);

	int count = 0;
	int cant_mas_bias = num_bag_of_words +1 ;
	//Itero linea a linea, armando el vector de bag of words para cada review
	while(getline(infile, line)) {
		vector<int> unVector(cant_mas_bias);
		istringstream iss(line);
		iss >> word;
		iss >> word;
		while(iss >> word) {
			if (mapBagOW.count(word)){
				unVector[mapBagOW[word]] += 1;
			}
		}
		//agrego el bias
		unVector[num_bag_of_words] = 1;
		bag[count] = unVector;
		count += 1;
	}

	infile.close();

	return bag;
}

/*
//Perceptron


int cant_mas_bias = num_bag_of_words +1 ;
vector<float> pesos(cant_mas_bias);

//Inicializo vector de pesos con todos en 0.5
for (int i = 0; i<cant_mas_bias; i++){
	pesos[i] = 0.5;
}
bool perfecto = false;
while (perfecto != true){
	for (int review=0; review<cant_reviews;review++){
		int suma = 0;
		float resultado = 0;
		for (int elemento = 0; elemento<cant_mas_bias ; elemento++){
			suma += pesos[elemento] * bagofWord[review][element];
		}
		if (suma >0){
			resultado = 1;
		}
	}

}
*/

int main() {
	bool stopwords = false;
	bool bagOfWords = true;
	bool hashingTrick = false;
	int num_bag_of_words = 10000;
	vector<vector<int> > vectorBagOW;

	if (bagOfWords){
		vectorBagOW = crear_Bag_Of_Words(stopwords,num_bag_of_words);
	}
	if (hashingTrick){
		//poner funcion que hace hashing trick
	}

	cout << "YOCA" << endl;
	return 0;
}
