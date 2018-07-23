// Good reference: http://www.cs.bham.ac.uk/~jxb/INC/nn.html
// https://www.youtube.com/watch?v=aircAruvnKk
// https://www.youtube.com/watch?v=IHZwWFHWa-w

/**************************************************************************/
/*	INCLUDES	 														  */
/**************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

/**************************************************************************/
/*	IMPORT METHODS	 													  */
/**************************************************************************/
#include "neural-network.h"

/**************************************************************************/
/*	TYPE DEFINITIONS & VARIABLES										  */
/**************************************************************************/
#define DEBUG 	1
#define INPUT 	4
#define HIDDEN 	2
#define OUTPUT	2

float max = 1.0;
float biasMax = 10;

// Implicit initialization to 0
static float	
		W1[INPUT][HIDDEN],
		W2[HIDDEN][HIDDEN],
		W3[HIDDEN][OUTPUT],
		H1[HIDDEN],
		BH1[HIDDEN],
		H2[HIDDEN],
		BH2[HIDDEN],
		Out[OUTPUT],
		BOUT[OUTPUT];

float testInput[INPUT] = {1, 1, 1, 1};




/**************************************************************************/
/*	PRIVATE METHODS														  */
/**************************************************************************/
// https://stackoverflow.com/questions/13408990/how-to-generate-random-float-number-in-c
static void genMatrix(float matrix[][HIDDEN], int sizeY, int sizeX){
	int i, j;

	for(i = 0; i < sizeY; i++){
		for(j = 0; j < sizeX; j++){
			matrix[i][j] = (float)((double)rand()/(double)(RAND_MAX/max));
		}
	}
}

static void genBias(float bias[], int size){
	int i;

	for(i = 0; i < size; i++){
		bias[i] = (float)((double)rand()/(double)(RAND_MAX/biasMax));
	}
}

// http://www.ece.utep.edu/research/webfuzzy/docs/kk-thesis/kk-thesis-html/node72.html
static float sigmoid(float x){
     float exp_value;
     float return_value;

     /*** Exponential calculation ***/
     exp_value = exp((double) -x);

     /*** Final sigmoid value ***/
     return_value = 1 / (1 + exp_value);

     return return_value;
}

static float matrixVectorMult(float *vec, int i, int j, float mat[][HIDDEN], int matSize, float tempSum){
	if(DEBUG)	printf("tempSum: %f\n", tempSum);

	tempSum += mat[j][i]*vec[j];

	return j == matSize		?	tempSum
		:	matrixVectorMult(vec, i, ++j, mat, matSize, tempSum);		
}

static void sumLayer(float sourceL[], int sourceSize, float weigth[][HIDDEN], float destL[], int destSize, float bias[]){
	
	for(int i = 0; i < destSize; i++){
		destL[i] = sigmoid(matrixVectorMult(sourceL, i, 0, weigth, HIDDEN, 0.0) - bias[i]);

		if(DEBUG)	printf("destL[%d]: %f \t bias[%d]: %f\n", i, destL[i], i, bias[i]);
	}
}

static void init(struct matrices *data){

	srand((unsigned int)time(NULL));

	genMatrix(W1, INPUT, HIDDEN);
	genMatrix(W2, HIDDEN, HIDDEN);
	genMatrix(W3, HIDDEN, OUTPUT);
	genBias(BH1, HIDDEN);
	genBias(BH2, HIDDEN);

}


/**************************************************************************/
/*	PUBLIC METHODS														  */
/**************************************************************************/
void neuralNetwork(struct matrices *data) {
	init(data);
	if(DEBUG)	printf("To H1:\n");
	sumLayer(testInput, INPUT, W1, H1, HIDDEN, BH1);
	if(DEBUG)	printf("To H2:\n");
	sumLayer(H1, HIDDEN, W2, H2, HIDDEN, BH2);
	if(DEBUG)	printf("To Out:\n");
	sumLayer(H2, HIDDEN, W3, Out, HIDDEN, BOUT);				// No bias on out nodes, BOUT is 0
}