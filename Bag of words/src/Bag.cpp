//============================================================================
// Name        : Bag.cpp
// Author      : Maxi
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;


template <typename T1, typename T2>
struct less_second {
    typedef pair<T1, T2> type;
    bool operator ()(type const& a, type const& b) const {
        return a.second > b.second;
    }
};

int main() {
	//Declaro variables
	int num_bag_of_words = 1000;
	vector<string> palabras(num_bag_of_words);
	string line;
	std::map<string,int> mymap;

	//Abro archivo
	std::ifstream infile("labeledTrainData.tsv");
	//std::ifstream infile("text.txt");

    //Leo la primera linea que no me sirve (nombre de campos)
    getline(infile, line);

    //Itero linea a linea, eliminando la primera linea (nombres de campos) y el id y puntaje de cada una
	while(getline(infile, line)) {
		istringstream iss(line);
		string word;
		iss >> word;
		iss >> word;
		while(iss >> word) {
			mymap[word] += 1;
		}
	}

	//Copio todo lo del map a un vector para ordenar
	vector<pair<string, int> > mapcopy(mymap.begin(), mymap.end());
	sort(mapcopy.begin(), mapcopy.end(), less_second<string, int>());

	//Me quedo solo con las palabras que mayor frecuencia tienen
	for (int i=0;i<num_bag_of_words; i++ ){
		palabras[i] = mapcopy[i].first;
	}

	infile.close();

	cout << "YOCA" << endl;
	return 0;
}
