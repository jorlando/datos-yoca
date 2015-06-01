/*
 * FileCleaner.h
 *
 *  Created on: 19/5/2015
 *      Author: pola17
 */

#ifndef FILECLEANER_H_
#define FILECLEANER_H_

#include <iostream>
#include <stdio.h>

class FileCleaner {

public:
	FileCleaner();
	virtual ~FileCleaner();
	void cleanFile(FILE *origin, FILE *destiny);

private:
	bool ignore(int current);

};

#endif /* FILECLEANER_H_ */
