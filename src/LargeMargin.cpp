#include "LargeMargin.h"

using namespace std;

template <typename T1, typename T2>
struct less_second {
    typedef pair<T1, T2> type;
    bool operator ()(type const& a, type const& b) const {
        return a.second > b.second;
    }
};

LargeMargin::LargeMargin() {
	// TODO Auto-generated constructor stub
}

LargeMargin::LargeMargin(int num_bag_of_words, int cant_reviews_entrenamiento,vector<vector<int> > vectorBagOW,vector<int> sentiment, vector<vector<int> > vectorTestBagOW, int cant_reviews_test, vector<string> ids) {
	this->num_bag_of_words = num_bag_of_words;
	this->cant_reviews_entrenamiento = cant_reviews_entrenamiento;
	this->vectorBagOW = vectorBagOW;
	this->sentiment = sentiment;
	this->vectorTestBagOW = vectorTestBagOW;
	this->cant_reviews_test = cant_reviews_test;
	this->ids = ids;
}

LargeMargin::~LargeMargin() {
	// TODO Auto-generated destructor stub
}


vector<float> LargeMargin::calcular_nuevos_pesos(vector<float> pesos, int sent,int resultado,vector<int> vecBagOfW){
	float n = 0.1;
	float mult = n * float((sent - resultado));
	int cant_mas_bias = num_bag_of_words +1 ;
	vector<float> pesos_nuevo_final(cant_mas_bias);
	
	for (int i = 0; i< cant_mas_bias; i++){
		pesos_nuevo_final[i] = pesos[i] + mult * vecBagOfW[i];
	}
	
	return pesos_nuevo_final;
}

void LargeMargin::LargeMarginCalculo(){
	int cant_mas_bias = num_bag_of_words +1 ;
	vector<float> pesos(cant_mas_bias);
	//Inicializo vector de pesos con todos en 0.5
	for (int i = 0; i<cant_mas_bias; i++){
		pesos[i] = 0.5;
	}
	//Numero de vector guardado
	int num_vector_large = 0;
	//Vector large_margin
	std::map<int, vector<float> > map_pesos;
	//Map de cantidad de veces que sobrevivio
	std::map<int, int>  map_cant_sobrevivi;
	//Cantidad de veces que sobrevivio
	int cant_sobrevivi = 0;
	
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
				map_pesos[num_vector_large] = pesos;
				map_cant_sobrevivi[num_vector_large] = cant_sobrevivi;
				cant_sobrevivi = 0;
				num_vector_large += 1;
				pesos = calcular_nuevos_pesos(pesos,sentiment[review],resultado,vectorBagOW[review]);
				errores += 1;
			}
			else{
				cant_sobrevivi += 1;
			}
		}
		cout << "Hubo "<<errores<< " errores. Pasada n° "<<cuenta << "."<<endl;
		cuenta +=1;
		if (errores == 0){
			perfecto = true;
		}
	}
	map_pesos[num_vector_large] = pesos;
	map_cant_sobrevivi[num_vector_large] = cant_sobrevivi;
	
	
	
	vector<pair<int, int> > vector_ordenado(map_cant_sobrevivi.begin(), map_cant_sobrevivi.end());
	sort(vector_ordenado.begin(), vector_ordenado.end(), less_second<int, int>());

	for (int f = 0; f<cant_mas_bias ; f++){
		cout << map_pesos[vector_ordenado[0].first][f] << " "<< pesos[f]<< endl;
	}
	
	
	//Empiezo a hacer el test
	vector<float> result_test (cant_reviews_test);
	for (int review2=0; review2<cant_reviews_test;review2++){
		for (int vec = 0; vec < 3 ; vec ++){
			float suma = 0;
			for (int elemento2 = 0; elemento2<cant_mas_bias ; elemento2++){
				float d = this->vectorTestBagOW[review2][elemento2];
				suma += map_pesos[vector_ordenado[vec].first][elemento2] * d;
			}
			result_test[review2] += vector_ordenado[vec].second * suma;
		}
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
	
	//Armo el archivo para el submission
	ofstream arch ("res_LargeMargin_2grams_2_pesos_actualizado.csv");
	arch << "id,sentiment\n";
	for (int r = 0; r < cant_reviews_test; r++){
		arch << ids[r];
		arch << ",";
		arch << result_test[r];
		arch << "\n";
	}
	arch.close();
}