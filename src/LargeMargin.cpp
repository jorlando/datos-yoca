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

LargeMargin::LargeMargin(int num_bag_of_words, int cant_reviews_entrenamiento,vector<vector<int> > vectorBagOW,vector<int> sentiment, vector<vector<int> > vectorTestBagOW, int cant_reviews_test, vector<string> ids, int cant_vec_pesos, vector< vector<int> > vec_entr, vector< vector<int> > vec_test, bool hashingTrick) {
	this->num_bag_of_words = num_bag_of_words;
	this->cant_reviews_entrenamiento = cant_reviews_entrenamiento;
	this->vectorBagOW = vectorBagOW;
	this->sentiment = sentiment;
	this->vectorTestBagOW = vectorTestBagOW;
	this->cant_reviews_test = cant_reviews_test;
	this->ids = ids;
	this->cant_vec_pesos = cant_vec_pesos;
	this->vec_entr = vec_entr;
	this->vec_test = vec_test;
	this->hashingTrick = hashingTrick;
	this->cant_sob = 0;
}

LargeMargin::LargeMargin(int num_bag_of_words, int cant_reviews_entrenamiento,vector<vector<pair<int,int> > > vectorHash,vector<int> sentiment, vector<vector<pair<int,int> > > vectorHashTest, int cant_reviews_test, vector<string> ids, int cant_vec_pesos, vector< vector<int> > vec_entr, vector< vector<int> > vec_test, bool hashingTrick) {
	this->num_bag_of_words = num_bag_of_words;
	this->cant_reviews_entrenamiento = cant_reviews_entrenamiento;
	this->vectorHash = vectorHash;
	this->sentiment = sentiment;
	this->vectorHashTest = vectorHashTest;
	this->cant_reviews_test = cant_reviews_test;
	this->ids = ids;
	this->cant_vec_pesos = cant_vec_pesos;
	this->vec_entr = vec_entr;
	this->vec_test = vec_test;
	this->hashingTrick = hashingTrick;
	this->cant_sob = 0;
}

LargeMargin::~LargeMargin() {
	// TODO Auto-generated destructor stub
}


vector<float> LargeMargin::calcular_nuevos_pesos(vector<float> pesos, int sent,int resultado,vector<int> vecBagOfW, int review){
	float n = 0.1;
	float mult = n * float((sent - resultado));
	int cant_mas_bias = num_bag_of_words +1 ;
	vector<float> pesos_nuevo_final(cant_mas_bias);
	
	pesos_nuevo_final = pesos;
	vector<int> vecEntr = this->vec_entr[review];
	for (int i = 0; i< vecEntr.size(); i++){
		pesos_nuevo_final[vecEntr[i]] = pesos[vecEntr[i]] + mult * vecBagOfW[vecEntr[i]];
	}
	
	return pesos_nuevo_final;
}

vector<float> LargeMargin::calcular_nuevos_pesos(vector<float> pesos, int sent,int resultado,vector<pair<int, int> > vecHash, int review){
	float n = 0.1;
	float mult = n * float((sent - resultado));
	int cant_mas_bias = num_bag_of_words +1 ;
	vector<float> pesos_nuevo_final(cant_mas_bias);

	pesos_nuevo_final = pesos;
	vector<int> vecEntr = this->vec_entr[review];
	for (int i = 0; i< vecEntr.size(); i++){
		pesos_nuevo_final[this->vec_entr[review][i]] = pesos[this->vec_entr[review][i]] + mult * log(1+this->vectorHash[review][this->vec_entr[review][i]].second);
	}
	return pesos_nuevo_final;
}

vector<float> LargeMargin::LargeMarginCalculo(){
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

			vector<int> vecEntr = this->vec_entr[review];
			for (int elemento = 0; elemento<vecEntr.size() ; elemento++){
				if(!this->hashingTrick){
					d = this->vectorBagOW[review][vecEntr[elemento]];
				}
				else{
					d = this->vectorHash[review][elemento].second;
				}
				suma += pesos[vecEntr[elemento]] * d;
			}
			if (suma >0){
				resultado = 1;
			}
			if (sentiment[review] != resultado){
				if(cant_sobrevivi>(cant_reviews_entrenamiento/5)){
					this->cant_sob += 1;
					//map_pesos[num_vector_large] = pesos;
					map_cant_sobrevivi[num_vector_large] = cant_sobrevivi;
					cant_sobrevivi = 0;
					num_vector_large += 1;
					//Calculo suma para el peso

					vector<float> suma_peso_review (cant_reviews_test);
					for (int review2=0; review2<cant_reviews_test;review2++){
						float suma = 0;
						vector<int> vecTest = this->vec_test[review2];
						for (int elemento2 = 0; elemento2<vecTest.size() ; elemento2++){
							float d;
							if(!this->hashingTrick){
								d = this->vectorTestBagOW[review2][vecTest[elemento2]];
							}
							else{
								d = this->vectorHashTest[review2][elemento2].second;
							}
							suma += pesos[vecTest[elemento2]] * d;
						}
						suma_peso_review[review2] = suma ;
					}
					suma_pesos.push_back(suma_peso_review);
				}
				cant_sobrevivi = 0;
				if (!this->hashingTrick){
					pesos = calcular_nuevos_pesos(pesos,sentiment[review],resultado,vectorBagOW[review],review);
				}
				else{
					pesos = calcular_nuevos_pesos(pesos,sentiment[review],resultado,vectorHash[review],review);
				}
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
	vector<float> suma_peso_review (cant_reviews_test);
	for (int review2=0; review2<cant_reviews_test;review2++){
		float suma = 0;
		vector<int> vecTest = this->vec_test[review2];
		for (int elemento2 = 0; elemento2<vecTest.size() ; elemento2++){
			float d;
			if(!this->hashingTrick){
				d = this->vectorTestBagOW[review2][vecTest[elemento2]];
			}
			else{
				d = this->vectorHashTest[review2][elemento2].second;
			}
			suma += pesos[vecTest[elemento2]] * d;
		}
		suma_peso_review[review2] = suma;
	}
	suma_pesos.push_back(suma_peso_review);
	
	vector<pair<int, int> > vector_ordenado(map_cant_sobrevivi.begin(), map_cant_sobrevivi.end());
	sort(vector_ordenado.begin(), vector_ordenado.end(), less_second<int, int>());

	//Empiezo a hacer el test
	vector<float> result_test (cant_reviews_test);
	for (int review2=0; review2<cant_reviews_test;review2++){
		//cout << review2<<endl;
		for (int vec = 0; vec < this->cant_vec_pesos ; vec ++){
			float suma = 0;
			vector<int> vecTest = this->vec_test[review2];
			result_test[review2] += vector_ordenado[vec].second * suma_pesos[vector_ordenado[vec].first][review2];
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
	
	return result_test;
}