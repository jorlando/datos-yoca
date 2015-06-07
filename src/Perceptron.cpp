#include "Perceptron.h"

using namespace std;


Perceptron::Perceptron() {
	// TODO Auto-generated constructor stub

}

Perceptron::Perceptron(int num_bag_of_words, int cant_reviews_entrenamiento,vector<vector<int> > vectorBagOW,vector<int> sentiment, vector<vector<int> > vectorTestBagOW, int cant_reviews_test, vector<string> ids) {
	this->num_bag_of_words = num_bag_of_words;
	this->cant_reviews_entrenamiento = cant_reviews_entrenamiento;
	this->vectorBagOW = vectorBagOW;
	this->sentiment = sentiment;
	this->vectorTestBagOW = vectorTestBagOW;
	this->cant_reviews_test = cant_reviews_test;
	this->ids = ids;
}

Perceptron::~Perceptron() {
	// TODO Auto-generated destructor stub
}


vector<float> Perceptron::calcular_nuevos_pesos(vector<float> pesos, int sent,int resultado,vector<int> vecBagOfW){
	float n = 0.1;
	float mult = n * float((sent - resultado));
	int cant_mas_bias = num_bag_of_words +1 ;
	vector<float> pesos_nuevo_final(cant_mas_bias);
	
	for (int i = 0; i< cant_mas_bias; i++){
		pesos_nuevo_final[i] = pesos[i] + mult * vecBagOfW[i];
	}
	
	return pesos_nuevo_final;
}

vector<float> Perceptron::entrenamiento(vector<float> pesos, int cant_mas_bias){
	//Inicializo vector de pesos con todos en 0.5
	for (int i = 0; i<cant_mas_bias; i++){
		pesos[i] = 0.5;
	}
	bool perfecto = false;
	int cuenta = 0;
	while (perfecto != true){
		int errores = 0;
		int d;
		for (int review=0; review<cant_reviews_entrenamiento;review++){
			int suma = 0;
			float resultado = 0;
			for (int elemento = 0; elemento<cant_mas_bias ; elemento++){
				d = this->vectorBagOW[review][elemento];
				suma += pesos[elemento] * d;
			}
			if (suma >0){
				resultado = 1;
			}
			if (sentiment[review] != resultado){
				pesos = calcular_nuevos_pesos(pesos,sentiment[review],resultado,vectorBagOW[review]);
				errores += 1;
			}
		}
		cout << "Hubo "<<errores<< " errores. Pasada n° "<<cuenta << "."<<endl;
		cuenta +=1;
		if (errores == 0){
			perfecto = true;
		}
	}
	return pesos;
}

vector<float> Perceptron::clasificoTests(vector<float> result_test, int cant_mas_bias, vector<float> pesos){
	for (int review2=0; review2<cant_reviews_test;review2++){
		float suma = 0;
		for (int elemento2 = 0; elemento2<cant_mas_bias ; elemento2++){
			float d = this->vectorTestBagOW[review2][elemento2];
			suma += pesos[elemento2] * d;
		}
		result_test[review2] = suma;
	}

	//Busco el minimo y el maximo para luego normalizar
	float minimo = result_test[0];
	float maximo = result_test[0];
	for (int res=0; res<cant_reviews_test;res++){
		if (result_test[res] > maximo){
			maximo = result_test[res];
		}
		if (result_test[res] < minimo){
			minimo = result_test[res];
		}
	}
	
	//normalizo
	for (int j=0; j<cant_reviews_test;j++){
		float nuevo = (result_test[j] - minimo) / (maximo - minimo);
		result_test[j] = nuevo;
	}
	return result_test;
}

void Perceptron::saveResults(vector<float> result_test){
	//Armo el archivo para el submission
	ofstream arch ("res_perceptron_3grams.csv");
	arch << "id,sentiment\n";
	for (int r = 0; r < cant_reviews_test; r++){
		arch << ids[r];
		arch << ",";
		arch << result_test[r];
		arch << "\n";
	}
	arch.close();
}

vector<float> Perceptron::perceptronNormal(){
	int cant_mas_bias = num_bag_of_words +1 ;
	vector<float> pesos(cant_mas_bias);
	pesos = entrenamiento(pesos,cant_mas_bias);
	
	//Empiezo a hacer el test
	vector<float> result_test (cant_reviews_test);
	result_test = clasificoTests(result_test, cant_mas_bias, pesos);
	return result_test;
}