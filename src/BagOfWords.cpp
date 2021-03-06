#include "BagOfWords.h"

using namespace std;

template <typename T1, typename T2>
struct less_second {
    typedef pair<T1, T2> type;
    bool operator ()(type const& a, type const& b) const {
        return a.second > b.second;
    }
};

BagOfWords::BagOfWords() {
	// TODO Auto-generated constructor stub

}

BagOfWords::~BagOfWords() {
	// TODO Auto-generated destructor stub
}

vector<int> BagOfWords::getSentiment(){
	return sentimiento;
}

vector<string> BagOfWords::getIds(){
	return ids;
}

vector< vector<int> > BagOfWords::getIndices_entr(){
	return this->indices_entr;
}

vector< vector<int> > BagOfWords::getIndices_test(){
	return this->indices_test;
}

vector<vector<int> > BagOfWords::crear_Bag_Of_Words (bool stopwords,int num_bag_of_words, int cant_reviews, string archivo, bool bool_ngrams,int n_grams){
	//Declaro variables
	vector<string> palabras(num_bag_of_words);
	string line;
	std::map<string,int> mymap;
	std::map<string,int> mapBagOW;
	string word;
	vector<vector<int> > bag(cant_reviews);
	std::map<string,int> mapStopw;
	vector<int> sentiment (cant_reviews);

	//Abro archivo
	std::ifstream infile(archivo.c_str());
	//std::ifstream infile("text.txt");

	//Si no se usan stoprwords armo un map con ellas
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
	//Itero linea a linea, eliminando el id y puntaje de cada una
	while(getline(infile, line)) {
		istringstream iss(line);
		iss >> word;
		iss >> word;
		while(iss >> word) {
			if (!(mapStopw.count(word))){
				mymap[word] += 1;
			}
			if (bool_ngrams){
				//hago 2 grams
				ngrams = pal_ant + " " + word;
				if (!(mapStopw.count(ngrams))){
					mymap[ngrams] += 1;
				}
				

				if(n_grams == 3){
					//Hago 3 grams
					ngrams = pal_ant_ant + " " + pal_ant + " ";
					ngrams += word;
					if (!(mapStopw.count(ngrams))){
						mymap[ngrams] += 1;
					}
					pal_ant = word;
					pal_ant_ant = pal_ant;
				}
				pal_ant = word;
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
	
	//Guardo la palabra anterior
	pal_ant = "";
	pal_ant_ant = "";
	int count = 0;
	int cant_mas_bias = num_bag_of_words +1 ;
	//Itero linea a linea, armando el vector de bag of words para cada review
	while(getline(infile, line)) {
		vector<int> index;
		vector<int> unVector(cant_mas_bias);
		istringstream iss(line);
		iss >> word;
		iss >> word;
		sentiment[count] = atoi(word.c_str());
		while(iss >> word) {
			if (mapBagOW.count(word)){
				if(!unVector[mapBagOW[word]]>0){
					//index.push_back(mapBagOW[word]);
				}
				unVector[mapBagOW[word]] += 1;
			}
			if (bool_ngrams){
				//Hago 2 grams
				ngrams = pal_ant + " " + word;
				if (mapBagOW.count(ngrams)){
					if(!unVector[mapBagOW[ngrams]]>0){
						//index.push_back(mapBagOW[ngrams]);
					}
					unVector[mapBagOW[ngrams]] += 1;
				}
				

				if(n_grams == 3){
					// Hago 3 grams
					ngrams = pal_ant_ant +" " + pal_ant;
					ngrams += " " + word;
					if (mapBagOW.count(ngrams)){
						if(!unVector[mapBagOW[ngrams]]>0){
							//index.push_back(mapBagOW[ngrams]);
						}
						unVector[mapBagOW[ngrams]] += 1;
					}
					pal_ant = word;
					pal_ant_ant = pal_ant;
				}
				pal_ant = word;
			}
		}
		//agrego el bias
		unVector[num_bag_of_words] = 1;
		//index.push_back(num_bag_of_words);
		for (int elemento = 0; elemento<cant_mas_bias ; elemento++){
			if (unVector[elemento] > 0){
				index.push_back(elemento);
			}
		}
		bag[count] = unVector;
		this->indices_entr.push_back(index);
		count += 1;
	}
	this->sentimiento = sentiment; 
	
	infile.close();
	
	this->mapBagOfWords = mapBagOW;
	
	return bag;
}

vector<vector<int> > BagOfWords::crear_Bag_Of_Words_test (int num_bag_of_words, int cant_reviews, string archivo, bool bool_ngrams,int n_grams){
	//Declaro variables
	string line;
	std::map<string,int> mapBagOW;
	string word;
	vector<vector<int> > bag(cant_reviews);
	vector<string> ids(cant_reviews);

	//Abro archivo
	std::ifstream infile(archivo.c_str());

	//Leo la primera linea que no me sirve (nombre de campos)
	getline(infile, line);

	int count = 0;
	int cant_mas_bias = num_bag_of_words +1 ;
	
	//Guardo la palabra anterior
	string pal_ant = "";
	string pal_ant_ant = "";
	string ngrams;
	
	//Itero linea a linea, armando el vector de bag of words para cada review
	while((getline(infile, line))&& (count < cant_reviews)) {
		vector<int> index;
		vector<int> unVector(cant_mas_bias);
		istringstream iss(line);
		iss >> word;
		ids[count] = word;
		while(iss >> word) {
			if (this->mapBagOfWords.count(word)){
				if(!unVector[this->mapBagOfWords[word]]>0){
					//index.push_back(this->mapBagOfWords[word]);
				}
				unVector[this->mapBagOfWords[word]] += 1;
			}
			if (bool_ngrams){
				//Hago 2 grams
				ngrams = pal_ant + " " + word;
				if (this->mapBagOfWords.count(ngrams)){
					if(!unVector[this->mapBagOfWords[ngrams]]>0){
						//index.push_back(this->mapBagOfWords[ngrams]);
					}
					unVector[this->mapBagOfWords[ngrams]] += 1;
				}
				
				if(n_grams == 3){
					// Hago 3 grams
					ngrams = pal_ant_ant +" " + pal_ant;
					ngrams += " " + word;
					if (this->mapBagOfWords.count(ngrams)){
						if(!unVector[this->mapBagOfWords[ngrams]]>0){
							//index.push_back(this->mapBagOfWords[ngrams]);
						}
						unVector[this->mapBagOfWords[ngrams]] += 1;
					}
					pal_ant = word;
					pal_ant_ant = pal_ant;
				}
				pal_ant = word;
			}
		}
		//agrego el bias
		unVector[num_bag_of_words] = 1;
		//index.push_back(num_bag_of_words);
		for (int elemento = 0; elemento<cant_mas_bias ; elemento++){
			if (unVector[elemento] > 0){
				index.push_back(elemento);
			}
		}
		bag[count] = unVector;
		this->indices_test.push_back(index);
		count += 1;
	}
	this->ids = ids;

	infile.close();
	

	return bag;
}