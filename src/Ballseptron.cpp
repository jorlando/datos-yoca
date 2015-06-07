#include "Ballseptron.h"

using namespace std;


Ballseptron::Ballseptron() {
	// TODO Auto-generated constructor stub

}

Ballseptron::Ballseptron(int num_bag_of_words, int cant_reviews_entrenamiento,vector<vector<int> > vectorBagOW,vector<int> sentiment, vector<vector<int> > vectorTestBagOW, int cant_reviews_test, vector<string> ids) {
	this->num_bag_of_words = num_bag_of_words;
	this->cant_reviews_entrenamiento = cant_reviews_entrenamiento;
	this->vectorBagOW = vectorBagOW;
	this->sentiment = sentiment;
	this->vectorTestBagOW = vectorTestBagOW;
	this->cant_reviews_test = cant_reviews_test;
	this->ids = ids;
}

Ballseptron::~Ballseptron() {
	// TODO Auto-generated destructor stub
}


vector<float> Ballseptron::calcular_nuevos_pesos(vector<float> pesos, int sent,int resultado,vector<int> vecBagOfW){
	float n = 0.1;
	float mult = n * float((sent - resultado));
	int cant_mas_bias = num_bag_of_words +1 ;
	vector<float> pesos_nuevo_final(cant_mas_bias);
	
	for (int i = 0; i< cant_mas_bias; i++){
		pesos_nuevo_final[i] = pesos[i] + mult * vecBagOfW[i];
	}
	
	return pesos_nuevo_final;
}

vector<float> Ballseptron::calcular_pesos_ballseptron(vector<float> pesos,float mod_pesos,float r,int yt,vector<int> vecBagOfW, int cant_mas_bias, int sent,int resultado){
	vector<float> xt(cant_mas_bias);
	float suma = 0;
	float algo = 0;
	float n = 0.1;
	if (resultado == 0) resultado = -1;
	float mult = n * resultado;
	vector<float> pesos_nuevo_final(cant_mas_bias);
	for (int elemento = 0; elemento<cant_mas_bias ; elemento++){
		xt[elemento] = vecBagOfW[elemento] - yt * r * pesos[elemento] / mod_pesos;
		pesos_nuevo_final[elemento] = pesos[elemento] + mult * xt[elemento];
	}
	
	return pesos_nuevo_final;
}


void Ballseptron::ballseptron(){
	int cant_mas_bias = num_bag_of_words +1 ;
	vector<float> pesos(cant_mas_bias);
	//Inicializo vector de pesos con todos en 0.5
	for (int i = 0; i<cant_mas_bias; i++){
		pesos[i] = 0;
	}
	bool perfecto = false;
	int cuenta = 0;
	float r = 0.007;
	//con r=0.009 llega a 3 errores
	//con r = 0.01 llega a 6 errores en la iteracion 1000. podria lleagr a mas parece
	while ((perfecto != true)&&(cuenta<1000)){
		int errores = 0;
		int errores_ball = 0;
		int d;
		for (int review=0; review<cant_reviews_entrenamiento;review++){
			int suma = 0;
			float resultado = 0;
			float mod_pesos = 0;
			
			for (int elemento = 0; elemento<cant_mas_bias ; elemento++){
				d = this->vectorBagOW[review][elemento];
				suma += pesos[elemento] * d;
				mod_pesos += pesos[elemento] * pesos[elemento];
			}
			if (suma >0){
				resultado = 1;
			}
			mod_pesos = sqrt(mod_pesos);
			
			int yt = 1;
			//cout << suma << endl;
			if (suma<0) yt = -1;
			suma = yt * suma;
			//cout << suma << " " << mod_pesos << " "<< ((float (suma)/mod_pesos)) << endl;
			if (sentiment[review] != resultado){
				pesos = calcular_nuevos_pesos(pesos,sentiment[review],resultado,vectorBagOW[review]);
				errores += 1;
			}
			//Bola de ballseptron
			else if (((float (suma)/mod_pesos) < r)&&(suma != 0)){
				//cout<<mod_pesos<<endl;
				pesos = calcular_pesos_ballseptron(pesos,mod_pesos,r,yt,vectorBagOW[review],cant_mas_bias,sentiment[review],resultado);
				errores_ball += 1;
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
	
	//Armo el archivo para el submission
	ofstream arch ("res_ballseptron_r_0.007_0_errorBall.csv");
	arch << "id,sentiment\n";
	for (int r = 0; r < cant_reviews_test; r++){
		arch << ids[r];
		arch << ",";
		arch << result_test[r];
		arch << "\n";
	}
	arch.close();

	
}