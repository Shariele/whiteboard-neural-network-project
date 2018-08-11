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
		// W1[INPUT][HIDDEN],
		// W2[HIDDEN][HIDDEN],
		// W3[HIDDEN][OUTPUT],
		// H1[HIDDEN],
		// BH1[HIDDEN],
		// H2[HIDDEN],
		// BH2[HIDDEN],
		// Out[OUTPUT],
		// BOUT[OUTPUT];

		// Test variables
		W1[HIDDEN][INPUT] 		= {{0.5, 2, 0.5, 0.5}, {0.5, 0.5, 3, 8}},
		W2[HIDDEN][HIDDEN] 		= {{0.5, 0.5}, {0, 0.5}},
		W3[HIDDEN][OUTPUT]		= {{6, 2}, {0, 1}},
		H1[HIDDEN],
		BH1[HIDDEN]				= {1, 1},
		H2[HIDDEN],
		BH2[HIDDEN]				= {1, 1},
		Out[OUTPUT],
		BOUT[OUTPUT],
		correctOut[OUTPUT];



float testInput[INPUT] = {1, 3, 1, 5};
int ShouldBe = 0;





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

static float matrixVectorMult(int i, int j, int matSize, float *vec, float mat[][matSize], float tempSum){
	if(DEBUG)	printf("tempSum[%d] before calc: %f\n", i, tempSum);
	if(DEBUG)	printf("matsize: %d\n", matSize);
	if(DEBUG)	printf("j: %d\n", j);

	tempSum += mat[i][j]*vec[j];

	if(DEBUG)	printf("tempSum %d after calc: %f\n\n", i, tempSum);

	return j == matSize-1		?	tempSum
		:	matrixVectorMult(i, ++j, matSize, vec, mat, tempSum);		
}

static void sumLayer(int sourceSize, int wSize, int destSize, float *sourceL, float weigth[destSize][wSize], float *destL,  float *bias){
	
	for(int i = 0; i < destSize; i++){
		destL[i] = sigmoid(matrixVectorMult(i, 0, wSize, sourceL, weigth, 0.0) - bias[i]);
		// destL[i] = matrixVectorMult(i, 0, wSize, sourceL, weigth, 0.0) - bias[i];

		if(DEBUG)	printf("\ndestL[%d]: %f \t bias[%d]: %f\n\n", i, destL[i], i, bias[i]);
	}
}

static void backPropagate(int wantNr, int size){
	int i;
	float nudges[OUTPUT];

	for(i = 0; i < size; i++){
		if(i != wantNr)
			nudges[i] = -pow((Out[i] - correctOut[i]), 2);
		else
			nudges[i] = pow((Out[i] - correctOut[i]), 2);

		printf("\nNudges[%d]: %f\n", i, nudges[i]);
	}
}

static void initBackPropagation(struct matrices *data){
	int i, iGreatestVal;
	float greatestVal = -999999;

	// Find the greatest value and corresponding index for later comparison.
	for(i = 0; i < OUTPUT; i++){
		if(Out[i] > greatestVal){
			greatestVal = Out[i];	iGreatestVal = i;
		}
	}
	// First check, was NN wrong? If not, all is good. If the index with the greatest value is equal to the number we want,
	// then the NN was right.
	if(iGreatestVal != data->number){
		if(DEBUG) printf("\nThe NN got the WRONG number. It guessed %d when it should be %d. Retraining!\n", iGreatestVal, data->number);

		correctOut[data->number] = 1;	//This is the number we want. All else is 0.
		backPropagate(data->number, OUTPUT);
	}else{
		printf("\nThe NN got the CORRECT number which was: %d \n", iGreatestVal);
	}
}



// static float computateCost(){
// 	correctOut[data->number] = 1;	//This is what we want. All else is 0.
// 		// Calculate cost
// 		for (i = 0; i < OUTPUT; i++){
// 			cost += pow((Out[i] - correctOut[i]), 2);
// 		}

// 		printf("\nCost for NN: %f\n", cost);
// }

static void init(struct matrices *data){

	srand((unsigned int)time(NULL));

	// genMatrix(W1, INPUT, HIDDEN);
	// genMatrix(W2, HIDDEN, HIDDEN);
	// genMatrix(W3, HIDDEN, OUTPUT);
	// genBias(BH1, HIDDEN);
	// genBias(BH2, HIDDEN);

}


/**************************************************************************/
/*	PUBLIC METHODS														  */
/**************************************************************************/
void neuralNetwork(struct matrices *data) {
	init(data);
	// Change testInput to Data later.
	if(DEBUG)	printf("**********************************\n");
	if(DEBUG)	printf("To H1:\n");
	sumLayer(INPUT, INPUT, HIDDEN, testInput, W1, H1, BH1);
	if(DEBUG)	printf("**********************************\n");
	if(DEBUG)	printf("**********************************\n");
	if(DEBUG)	printf("To H2:\n");
	sumLayer(HIDDEN, HIDDEN, HIDDEN, H1, W2, H2, BH2);
	if(DEBUG)	printf("**********************************\n");
	if(DEBUG)	printf("**********************************\n");
	if(DEBUG)	printf("To Out:\n");
	sumLayer(HIDDEN, HIDDEN, OUTPUT, H2, W3, Out, BOUT);				// No bias on out nodes, BOUT is only 0

	initBackPropagation(data);
}