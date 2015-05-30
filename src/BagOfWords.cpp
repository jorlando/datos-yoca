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



vector<vector<int> > BagOfWords::crear_Bag_Of_Words (bool stopwords,int num_bag_of_words){
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
	std::ifstream infile("../Archivos/edited.tsv");
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