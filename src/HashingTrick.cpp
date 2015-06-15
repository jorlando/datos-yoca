#include "HashingTrick.h"

using namespace std;

HashingTrick::HashingTrick(int dimensions) {
	this->dimensions = dimensions;
}

HashingTrick::~HashingTrick() {
	// TODO Auto-generated destructor stub
}

vector<int> HashingTrick::getSentiment(){
	return sentimiento;
}

vector<string> HashingTrick::getIds(){
	return ids;
}

vector <vector <int> > HashingTrick::getIndices_entr(){
	return indices_entr;
}

vector <vector <int> > HashingTrick::getIndices_test(){
	return indices_test;
}

int HashingTrick::hashear(string palabra){
    int h = 2166136261U;
    for(int i = 0; i < palabra.length(); i++){
        h = h ^ (palabra[i]);       /* xor  the low 8 bits */
        h = h * 16777619;  /* multiply by the magic number */
    }
    if (h<0){
    	h = -h;
    }
    return h;
}


vector<vector<pair<int,int> > > HashingTrick::aplicar_The_Hashing_Trick(bool stopwords,int cant_reviews, string archivo, bool bool_ngrams,int n_grams) {

	//Declaro variables
	string line;
	string word;
	std::map<string,int> mapStopw;
	vector<int> sentiment (cant_reviews);
	int index;
	vector<vector<pair<int,int> > > vectores_reviews(cant_reviews);

	//Abro archivo
	std::ifstream infile(archivo.c_str());

	//Si no se usan stopwords armo un map con ellas
	if (stopwords){
		std::ifstream stopw("../Archivos/stop-words.txt");
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

	//Guardo la palabra anterior
	string pal_ant = "";
	string pal_ant_ant = "";
	string ngrams;
	
	int count = 0;
	
	//Itero linea a linea, eliminando el id y puntaje de cada una, haciendo el vector de hashing trick para cada review
	while(getline(infile, line)) {
		//Declaro vector a utilizar
		vector<pair<int,int> > mi_vector;
		vector<int> vec_entr;
		//Declaro map que voy a usar para hacer hash de palabras ya hasheadas
		std::map<string,int> mapPalabrasUsadas;
		
		istringstream iss(line);
		iss >> word;
		iss >> word;
		sentiment[count] = atoi(word.c_str());
		while(iss >> word) {
			if ((!(mapStopw.count(word)))&&(!(mapPalabrasUsadas.count(word)))){
				index = hashear(word)%this->dimensions;
				mi_vector.push_back(make_pair(index,1));
				vec_entr.push_back(index);
				mapPalabrasUsadas[word] = 1;
			}
			if (bool_ngrams){
				//hago 2 grams
				ngrams = pal_ant + " " + word;
				if ((!(mapStopw.count(ngrams)))&&(!(mapPalabrasUsadas.count(ngrams)))){
					index = hashear(ngrams)%this->dimensions;
					mi_vector.push_back(make_pair(index,1));
					vec_entr.push_back(index);
					mapPalabrasUsadas[ngrams] = 1;
				}

				if(n_grams == 3){
					//Hago 3 grams
					ngrams = pal_ant_ant +" " + pal_ant + " ";
					ngrams += word;
					if ((!(mapStopw.count(ngrams)))&&(!(mapPalabrasUsadas.count(ngrams)))){
						index = hashear(ngrams)%this->dimensions;
						mi_vector.push_back(make_pair(index,1));
						vec_entr.push_back(index);
						mapPalabrasUsadas[ngrams] = 1;
					}
					pal_ant = word;
					pal_ant_ant = pal_ant;
				}
				pal_ant = word;
			}
		}
		vectores_reviews[count] = mi_vector;
		this->indices_entr.push_back(vec_entr);
		count += 1;
	}

	this->sentimiento = sentiment; 
		
	infile.close();
	
	return vectores_reviews;
}

vector<vector<pair<int,int> > > HashingTrick::aplicar_The_Hashing_Trick_test(bool stopwords,int cant_reviews, string archivo, bool bool_ngrams,int n_grams) {
	
	//Declaro variables
	string line;
	string word;
	std::map<string,int> mapStopw;
	int index;
	vector<vector<pair<int,int> > > vectores_reviews(cant_reviews);
	vector<string> ids(cant_reviews);
	
	//Abro archivo
	std::ifstream infile(archivo.c_str());

	//Si no se usan stopwords armo un map con ellas
	if (stopwords){
		std::ifstream stopw("../Archivos/stop-words.txt");
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

	int count = 0;
	
	//Guardo la palabra anterior
	string pal_ant = "";
	string pal_ant_ant = "";
	string ngrams;
	
	//Itero linea a linea, armando el vector de bag of words para cada review
	while((getline(infile, line))&& (count < cant_reviews)) {
		//Declaro vector a utilizar
		vector<pair<int,int> > mi_vector;
		vector<int> vec_test;
		//Declaro map que voy a usar para hacer hash de palabras ya hasheadas
		std::map<string,int> mapPalabrasUsadas;

		istringstream iss(line);
		iss >> word;
		ids[count] = word;
		while(iss >> word) {
			if ((!(mapStopw.count(word)))&&(!(mapPalabrasUsadas.count(word)))){
				index = hashear(word)%this->dimensions;
				mi_vector.push_back(make_pair(index,1));
				vec_test.push_back(index);
				mapPalabrasUsadas[word] = 1;
			}
			if (bool_ngrams){
				//hago 2 grams
				ngrams = pal_ant + " " + word;
				if ((!(mapStopw.count(ngrams)))&&(!(mapPalabrasUsadas.count(ngrams)))){
					index = hashear(ngrams)%this->dimensions;
					mi_vector.push_back(make_pair(index,1));
					vec_test.push_back(index);
					mapPalabrasUsadas[ngrams] = 1;
				}
				

				if(n_grams == 3){
					//Hago 3 grams
					ngrams = pal_ant_ant + " " + pal_ant + " ";
					ngrams += word;
					if ((!(mapStopw.count(ngrams)))&&(!(mapPalabrasUsadas.count(ngrams)))){
						index = hashear(ngrams)%this->dimensions;
						mi_vector.push_back(make_pair(index,1));
						vec_test.push_back(index);
						mapPalabrasUsadas[ngrams] = 1;
					}
					pal_ant = word;
					pal_ant_ant = pal_ant;
				}
				pal_ant = word;
			}
		}
		vectores_reviews[count] = mi_vector;
		this->indices_test.push_back(vec_test);
		count += 1;
	}
	this->ids = ids;

	infile.close();
	
	return vectores_reviews;
}
