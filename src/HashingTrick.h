#ifndef HASHINGTRICK_H_
#define HASHINGTRICK_H_

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <ctime>
using namespace std;

class HashingTrick {

	int dimensions;

public:
	HashingTrick(int dimensions);
	virtual ~HashingTrick();
	void aplicar_The_Hashing_Trick(vector<vector<int> > &vectorBagOW, int cant_reviews, int cant_mas_bias);

private:

};

#endif /* HASHINGTRICK_H_ */
