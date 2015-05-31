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

public:
	BagOfWords();
	virtual ~BagOfWords();
	vector<vector<int> > crear_Bag_Of_Words (bool stopwords,int num_bag_of_words, int cant_reviews, string archivo);
	vector<vector<int> > crear_Bag_Of_Words_test (int num_bag_of_words, int cant_reviews, string archivo);
	vector<int> getSentiment();
	vector<string> getIds();

private:


};

#endif 