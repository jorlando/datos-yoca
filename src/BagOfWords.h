#ifndef BAGOFWORDS_H_
#define BAGOFWORDS_H_


#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

class BagOfWords {
	
	vector<int> sentimiento;
	std::map<string,int> mapBagOfWords;
	vector<string> ids;
	vector< vector<int> > indices_entr;
	vector< vector<int> > indices_test;

public:
	BagOfWords();
	virtual ~BagOfWords();
	vector<vector<int> > crear_Bag_Of_Words (bool stopwords,int num_bag_of_words, int cant_reviews, string archivo,bool bool_ngrams,int ngrams);
	vector<vector<int> > crear_Bag_Of_Words_test (int num_bag_of_words, int cant_reviews, string archivo,bool bool_ngrams,int ngrams);
	vector<int> getSentiment();
	vector<string> getIds();
	vector< vector<int> > getIndices_test();
	vector< vector<int> > getIndices_entr();

private:


};

#endif 