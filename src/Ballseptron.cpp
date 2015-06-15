#include "Ballseptron.h"

using namespace std;


Ballseptron::Ballseptron() {
	// TODO Auto-generated constructor stub

}

Ballseptron::Ballseptron(float r, int num_bag_of_words, int cant_reviews_entrenamiento,vector<vector<int> > vectorBagOW,vector<int> sentiment, vector<vector<int> > vectorTestBagOW, int cant_reviews_test, vector<string> ids, vector< vector<int> > vec_entr, vector< vector<int> > vec_test, bool hashingTrick) {
	this->num_bag_of_words = num_bag_of_words;
	this->cant_reviews_entrenamiento = cant_reviews_entrenamiento;
	this->vectorBagOW = vectorBagOW;
	this->sentiment = sentiment;
	this->vectorTestBagOW = vectorTestBagOW;
	this->cant_reviews_test = cant_reviews_test;
	this->ids = ids;
	this->vec_entr = vec_entr;
	this->vec_test = vec_test;
	this->cambiar_mod_pesos = true;
	this->hashingTrick = hashingTrick;
	this->r = r;
}

Ballseptron::Ballseptron(float r, int num_bag_of_words, int cant_reviews_entrenamiento,vector<vector<pair<int,int> > > vectorHash,vector<int> sentiment, vector<vector<pair<int,int> > > vectorHashTest, int cant_reviews_test, vector<string> ids, vector< vector<int> > vec_entr, vector< vector<int> > vec_test, bool hashingTrick) {
	this->num_bag_of_words = num_bag_of_words;
	this->cant_reviews_entrenamiento = cant_reviews_entrenamiento;
	this->vectorHash = vectorHash;
	this->sentiment = sentiment;
	this->vectorHashTest = vectorHashTest;
	this->cant_reviews_test = cant_reviews_test;
	this->ids = ids;
	this->vec_entr = vec_entr;
	this->vec_test = vec_test;
	this->cambiar_mod_pesos = true;
	this->hashingTrick = hashingTrick;
	this->r = r;
}

Ballseptron::~Ballseptron() {
	// TODO Auto-generated destructor stub
}


vector<float> Ballseptron::calcular_nuevos_pesos(vector<float> pesos, int sent,int resultado,vector<int> vecBagOfW, int review){
	float n = 0.1;
	float mult = n * float((sent - resultado));
	int cant_mas_bias = num_bag_of_words +1 ;
	vector<float> pesos_nuevo_final(cant_mas_bias);
	pesos_nuevo_final = pesos;
	vector<int> vecEntr = this->vec_entr[review];
	for (int i = 0; i< vecEntr.size(); i++){
		pesos_nuevo_final[vecEntr[i]] = pesos[vecEntr[i]] + mult * vecBagOfW[vecEntr[i]];
		this->mod_pesos = this->mod_pesos -  pesos[vecEntr[i]] *  pesos[vecEntr[i]] + pesos_nuevo_final[vecEntr[i]] * pesos_nuevo_final[vecEntr[i]];
	}
	return pesos_nuevo_final;
}

vector<float> Ballseptron::calcular_nuevos_pesos(vector<float> pesos, int sent,int resultado,vector<pair<int, int> > vecHash, int review){
	float n = 0.1;
	float mult = n * float((sent - resultado));
	int cant_mas_bias = num_bag_of_words +1 ;
	vector<float> pesos_nuevo_final(cant_mas_bias);

	pesos_nuevo_final = pesos;
	vector<int> vecEntr = this->vec_entr[review];
	for (int i = 0; i< vecEntr.size(); i++){
		pesos_nuevo_final[this->vec_entr[review][i]] = pesos[this->vec_entr[review][i]] + mult * log(1+this->vectorHash[review][this->vec_entr[review][i]].second);
		this->mod_pesos = this->mod_pesos -  pesos[this->vec_entr[review][i]] *  pesos[this->vec_entr[review][i]] + pesos_nuevo_final[this->vec_entr[review][i]] * pesos_nuevo_final[this->vec_entr[review][i]];
	}
	
	return pesos_nuevo_final;
}


vector<float> Ballseptron::calcular_pesos_ballseptron(vector<float> pesos,float mod_pesos,float r,int yt,vector<int> vec, int cant_mas_bias, int sent,int resultado){
	vector<float> xt(cant_mas_bias);
	float suma = 0;
	float algo = 0;
	float n = 0.1;
	if (resultado == 0) resultado = -1;
	float mult = n * resultado;
	vector<float> pesos_nuevo_final(cant_mas_bias);
	std::map<int,int> indices_usados;
	if(this->hashingTrick){
		for (int i = 0; i<vec.size() ; i++){
			indices_usados[vec[i]] = 1;
		}
	}
	for (int elemento = 0; elemento<cant_mas_bias ; elemento++){
		if(!this->hashingTrick){
			xt[elemento] = vec[elemento] - yt * r * pesos[elemento] / sqrt(mod_pesos);
		}
		else{
			if (indices_usados.count(elemento)){
				xt[elemento] = 1 - yt * r * pesos[elemento] /sqrt(mod_pesos);
			}
			else{
				xt[elemento] = 0 - yt * r * pesos[elemento] / sqrt(mod_pesos);
			}
		}
		pesos_nuevo_final[elemento] = pesos[elemento] + mult * xt[elemento];
		this->mod_pesos = this->mod_pesos -  pesos[elemento] *  pesos[elemento] + pesos_nuevo_final[elemento] * pesos_nuevo_final[elemento];
	}
	
	return pesos_nuevo_final;
}


vector<float> Ballseptron::ballseptron(){
	int cant_mas_bias = num_bag_of_words +1 ;
	vector<float> pesos(cant_mas_bias);
	//Inicializo vector de pesos con todos en 0
	for (int i = 0; i<cant_mas_bias; i++){
		pesos[i] = 0.5;
	}
	bool perfecto = false;
	int cuenta = 0;
	//float r = 0.007;
	//con r=0.009 llega a 3 errores
	//con r = 0.01 llega a 6 errores en la iteracion 1000. podria lleagr a mas parece
	while ((perfecto != true)&&(cuenta<1000)){
		int errores = 0;
		int errores_ball = 0;
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
			//Fin nueva forma de calculo
			if (suma >0){
				resultado = 1;
			}
			//mod_pesos = sqrt(mod_pesos);
			
			int yt = 1;
			//cout << suma << endl;
			if (suma<0) yt = -1;
			suma = yt * suma;
			//cout << suma << " " << mod_pesos << " "<< ((float (suma)/mod_pesos)) << endl;
			if (sentiment[review] != resultado){
				if (!this->hashingTrick){
					pesos = calcular_nuevos_pesos(pesos,sentiment[review],resultado,vectorBagOW[review],review);
				}
				else{
					pesos = calcular_nuevos_pesos(pesos,sentiment[review],resultado,vectorHash[review],review);
				}
				errores += 1;
			}
			//Bola de ballseptron
			if(this->cambiar_mod_pesos){
				this->mod_pesos = 0;
				for (int elemento = 0; elemento<cant_mas_bias ; elemento++){
					this->mod_pesos += pesos[elemento] * pesos[elemento];
				}
				this->cambiar_mod_pesos = false;
				//this->mod_pesos = sqrt(mod_pesos);
			}

			if (((float (suma)/sqrt(this->mod_pesos)) < r)&&(suma != 0) && ((sentiment[review] == resultado))){
				if (!this->hashingTrick){
					pesos = calcular_pesos_ballseptron(pesos,this->mod_pesos,r,yt,vectorBagOW[review],cant_mas_bias,sentiment[review],resultado);
				}
				else{
					pesos = calcular_pesos_ballseptron(pesos,this->mod_pesos,r,yt,vec_entr[review],cant_mas_bias,sentiment[review],resultado);
				}
				errores_ball += 1;
			}
			else if ((sentiment[review] == resultado)){
				this->cambiar_mod_pesos = false;
			}
		}
		cout << "Hubo "<<errores<< " errores. Hubo " << errores_ball << " errores_ball. Pasada n° "<<cuenta << "."<<endl;
		cuenta +=1;
		if ((errores == 0)&&(errores_ball == 0)){
			perfecto = true;
		}
	}
	
	//Empiezo a hacer el test

	vector<float> result_test (cant_reviews_test);
	for (int review2=0; review2<cant_reviews_test;review2++){
		float suma = 0;

		vector<int> vecTest = this->vec_test[review2];
		for (int elemento2 = 0; elemento2<this->vec_test[review2].size() ; elemento2++){
			float d;
			if(!this->hashingTrick){
				d = this->vectorTestBagOW[review2][this->vec_test[review2][elemento2]];
			}
			else{
				d = this->vectorHashTest[review2][elemento2].second;
			}
			suma += pesos[this->vec_test[review2][elemento2]] * d;
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