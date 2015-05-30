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

public:
	BagOfWords();
	virtual ~BagOfWords();
	vector<vector<int> > crear_Bag_Of_Words (bool stopwords,int num_bag_of_words);

private:


};

#endif 