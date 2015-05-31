/*
 * FileCleaner.cpp
 *
 *  Created on: 19/5/2015
 *      Author: pola17
 */

#include "FileCleaner.h"
using namespace std;
FileCleaner::FileCleaner() {
	// TODO Auto-generated constructor stub

}

FileCleaner::~FileCleaner() {
	// TODO Auto-generated destructor stub
}

/**
 * para las validaciones simples, donde solo se tiene que ignorar el caracter
 */
bool FileCleaner::ignore(int current){

	if(current >= 33 && current <= 47 && current != 34 ){
		return true;
	}
	else if(current >= 91 && current <= 96 && current != 92 && current != 95){
		return true;
	}
	else if(current >= 123 && current <= 126){
		return true;
	}
	else if(current >= 128 && current <= 255){
		return true;
	}

	return false;

}

/**
 * funcion que deja el archivo de origen limpio en el de destino
 */
void FileCleaner::cleanFile(FILE *origin, FILE *destiny){

	int current; // current char
	bool valid;
	int enter = 0;

	while(!feof(origin)){

		valid = true;

		current = fgetc(origin);

		if(current >= 65 && current <= 90){ // si es mayuscula
			current += 32; // lo pasa a minuscula
		}
		else if(ignore(current)){
			valid = false;
		}
		else if(current == 92){
			fgetc(origin); // leo uno mas porque el 92 es el caracter para escapear -> \ <-
			valid = false;
		}
		else if(current >= 58 && current <= 64){ // caso de un < el cual indica que es una etiqueta HTML
			if(current == 60){
				bool seguir = true;
				while((current!=62)&&(seguir)){ // hasta un > que cierre la etiqueta HTML
					current = fgetc(origin);
					if (current == 10){
						fputc(current, destiny);
						seguir = false;
					}
				}
			}
			valid = false;
		}

		if(valid){
			fputc(current, destiny);
		}
		
	}
}

