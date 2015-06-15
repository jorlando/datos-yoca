#ifndef HASHINGTRICK_H_
#define HASHINGTRICK_H_

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <ctime>
#include <functional>
#include <string>
#include <fstream>
#include <sstream>


using namespace std;

class HashingTrick {
	
	vector<int> sentimiento;
	int dimensions;
	vector<string> ids;
	vector <vector <int> > indices_entr;
	vector <vector <int> > indices_test;

public:
	HashingTrick(int dimensions);
	virtual ~HashingTrick();
	vector<vector<pair<int,int> > > aplicar_The_Hashing_Trick(bool stopwords,int cant_reviews, string archivo, bool bool_ngrams,int n_grams);
	vector<vector<pair<int,int> > > aplicar_The_Hashing_Trick_test(bool stopwords,int cant_reviews, string archivo, bool bool_ngrams,int n_grams);
	vector<int> getSentiment();
	vector<string> getIds();
	int hashear(string palabra);
	vector <vector <int> > getIndices_entr();
	vector <vector <int> > getIndices_test();

private:

};

#endif /* HASHINGTRICK_H_ */
