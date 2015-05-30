#include "Perceptron.h"

using namespace std;


Perceptron::Perceptron() {
	// TODO Auto-generated constructor stub

}

Perceptron::Perceptron(int num_bag_of_words, int cant_reviews_entrenamiento,vector<vector<int> > vectorBagOW) {
	num_bag_of_words = num_bag_of_words;
	cant_reviews_entrenamiento = cant_reviews_entrenamiento;
	vectorBagOW = vectorBagOW;
}
Perceptron::~Perceptron() {
	// TODO Auto-generated destructor stub
}

void Perceptron::perceptronNormal(){

	int cant_mas_bias = num_bag_of_words +1 ;
	vector<float> pesos(cant_mas_bias);

	//Inicializo vector de pesos con todos en 0.5
	for (int i = 0; i<cant_mas_bias; i++){
		pesos[i] = 0.5;
	}
	bool perfecto = false;
	while (perfecto != true){
		for (int review=0; review<cant_reviews_entrenamiento;review++){
			int suma = 0;
			float resultado = 0;
			for (int elemento = 0; elemento<cant_mas_bias ; elemento++){
				suma += pesos[elemento] * vectorBagOW[review][elemento];
			}
			if (suma >0){
				resultado = 1;
			}
		}
	}
}