#!/usr/bin/env python

#  Author: Angela Chapman
#  Date: 8/6/2014
#
#  This file contains code to accompany the Kaggle tutorial
#  "Deep learning goes to the movies".  The code in this file
#  is for Part 1 of the tutorial on Natural Language Processing.
#
# *************************************** #

import sys
import csv
import os
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.ensemble import RandomForestClassifier
from sklearn import linear_model
from KaggleWord2VecUtility import KaggleWord2VecUtility
import pandas as pd
import numpy as np

def calcular_nuevo_peso(pesos,esperado,resultado_obtenido,vector_review):
    #print pesos[0]
    cambia = False
    vector= vector_review.tolist()
    vector.append(1)
    n = 0.1
    mult = n * float((esperado - resultado))
    lista = []
    for x in vector:
        lista.append (mult * x)
    pesos_nuevo_final = []
    for i in xrange(0,len(pesos)):
        pesos_nuevo_final.append(pesos[i] + lista[i])
        #if i==1:
            #print pesos[i], lista[i], pesos_nuevo_final[i]
    #for d in xrange(0,len(pesos)):
        #if pesos[d] != 0.5:
            #print "asd"
    return pesos_nuevo_final


if __name__ == '__main__':

    train = pd.read_csv(os.path.join(os.path.dirname(__file__), 'data', 'labeledTrainData.tsv'), header=0, \
                    delimiter="\t", quoting=3)
    test = pd.read_csv(os.path.join(os.path.dirname(__file__), 'data', 'testData.tsv'), header=0, delimiter="\t", \
                   quoting=3 )

    print 'The first review is:'
    print train["review"][0]

    raw_input("Press Enter to continue...")


    print 'Download text data sets. If you already have NLTK datasets downloaded, just close the Python download window...'
    #nltk.download()  # Download text data sets, including stop words

    # Initialize an empty list to hold the clean reviews
    clean_train_reviews = []

    # Loop over each review; create an index i that goes from 0 to the length
    # of the movie review list

    print "Cleaning and parsing the training set movie reviews...\n"
    for i in xrange( 0, len(train["review"])):
        clean_train_reviews.append(" ".join(KaggleWord2VecUtility.review_to_wordlist(train["review"][i], True)))


    # ****** Create a bag of words from the training set
    #
    print "Creating the bag of words...\n"


    # Initialize the "CountVectorizer" object, which is scikit-learn's
    # bag of words tool.
    vectorizer = CountVectorizer(analyzer = "word",   \
                             tokenizer = None,    \
                             preprocessor = None, \
                             stop_words = None,   \
                             max_features = 10000)

    # fit_transform() does two functions: First, it fits the model
    # and learns the vocabulary; second, it transforms our training data
    # into feature vectors. The input to fit_transform should be a list of
    # strings.
    train_data_features = vectorizer.fit_transform(clean_train_reviews)
    del clean_train_reviews
    # Numpy arrays are easy to work with, so convert the result to an
    # array
    train_data_features = train_data_features.toarray()
   

   #cmienzo el perceptron
    pesos = []
    for w in xrange(0,10001):
        pesos.append(0.5)
    print len(pesos)
    perfecto = False
    resultado = 0
    errores = 0
    cuenta = 0
    while ((perfecto != True) or (cuenta == 30)):
        cuenta +=1
        errores = 0
        algo= False
        for review in xrange(0,25000):
                #print pesos[1]
                suma = 0
        	for elemento in xrange (0, 10000):
        	   	
        	   	suma += pesos[elemento]*train_data_features[review][elemento]
                #bias
        	#print suma
        	suma += pesos [10000]
        	
        	#if suma != 0.5:
        		#print suma
        	if suma>= 0:
        		resultado = 1
        	else:
        		resultado = 0
        	if train["sentiment"][review] != resultado:
        		pesos = calcular_nuevo_peso(pesos,train["sentiment"][review],resultado,train_data_features[review])
        		
        		#print pesos[0]
        		errores += 1
        	if((review == 10000)or(review == 20000)or(review == 24999)):
        	   	print review


        if errores == 0:
        	perfecto = True
        else:
        	print "Hay erorres:"
        	print errores
        	print "hacemos una nueva pasada"
        	
        	
        	
    	
    #comienzo el test
    print "Comienza test"
    # Create an empty list and append the clean reviews one by one
    clean_test_reviews = []

    print "Cleaning and parsing the test set movie reviews...\n"
    for i in xrange(0,len(test["review"])):
        clean_test_reviews.append(" ".join(KaggleWord2VecUtility.review_to_wordlist(test["review"][i], True)))

    # Get a bag of words for the test set, and convert to a numpy array
    test_data_features = vectorizer.transform(clean_test_reviews)
    del clean_test_reviews
    test_data_features = test_data_features.toarray()
    res_final =[]
    for review in xrange(0,25000):
	suma = 0
        for elemento in xrange (0, 10000):
            
            suma += pesos[elemento]*test_data_features[review][elemento]
        if suma>= 0:
            res_final.append(1)
        else:
            res_final.append(0)
        if((review == 10000)or(review == 20000)or(review == 24999)):
            print review
    print "Termine"
    output = pd.DataFrame( data={"id":test["id"], "sentiment":res_final} )

    # Use pandas to write the comma-separated output file
    output.to_csv(os.path.join(os.path.dirname(__file__), 'data', 'Bag_of_Words_PercPRopioXPAsadas.csv'), index=False, quoting=3)


