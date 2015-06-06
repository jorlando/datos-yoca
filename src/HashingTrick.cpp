#include "HashingTrick.h"

HashingTrick::HashingTrick(int dimensions) {
	this->dimensions = dimensions;
}

HashingTrick::~HashingTrick() {
	// TODO Auto-generated destructor stub
}

void HashingTrick::aplicar_The_Hashing_Trick(vector<vector<int> > &vectorBagOW, int cant_reviews, int cant_mas_bias) {

	std::srand(std::time(0));
	vector<int> hashing(cant_mas_bias);
	//Armo funcion de hashing aleatoria
	for(int i=0; i<cant_mas_bias; i++) {
	    hashing[i] = std::rand() % dimensions;
	}

	//Armo el nuevo BagOW con HashingTrick
	for(int i=0; i<cant_reviews; i++) {
		vector<int> unVector(dimensions);

		//Realizo la reduccion de dimensiones
		for(int j=0; j<cant_mas_bias; j++) {
			int numHash = hashing[j];
			unVector[numHash] += vectorBagOW[i][j];
		}

		vectorBagOW[i] = unVector;
	}
}
