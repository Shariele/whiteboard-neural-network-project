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
#include "mnist-master/mnist.h"

// #define USE_MNIST_LOADER
// #define MNIST_DOUBLE
// 

/**************************************************************************/
/*	TYPE DEFINITIONS & VARIABLES										  */
/**************************************************************************/
#define DEBUG 	0
#define SHOWCASE 1
#define INPUT 	784
#define HIDDEN 	16	
#define OUTPUT	10

static float learningRate = 0.05;
//The highest float value that can ge generated.
static float max = 5;		
static float biasMax = 1;			

// Implicit initialization to 0
static float	
		W1[HIDDEN][INPUT],
		W2[HIDDEN][HIDDEN],
		W3[HIDDEN][OUTPUT],
		H1[HIDDEN],
		BH1[HIDDEN],
		H2[HIDDEN],
		BH2[HIDDEN],
		Out[OUTPUT],
		BOUT[OUTPUT],
		correctOut[OUTPUT];

		// Test variables
		// W1[HIDDEN][INPUT] 		= {{0.5, 2, 0.5, 0.5}, {0.5, 0.5, 3, 8}},
		// W2[HIDDEN][HIDDEN] 		= {{0.5, 0.5}, {0, 0.5}},
		// W3[HIDDEN][OUTPUT]		= {{6, 2}, {0, 1}},
		// H1[HIDDEN],
		// BH1[HIDDEN]				= {1, 1},
		// H2[HIDDEN],
		// BH2[HIDDEN]				= {1, 1},
		// Out[OUTPUT],
		// BOUT[OUTPUT],
		// correctOut[OUTPUT];

// float testInput[INPUT] = {1, 3, 1, 5};





/**************************************************************************/
/*	PRIVATE METHODS														  */
/**************************************************************************/
// https://stackoverflow.com/questions/13408990/how-to-generate-random-float-number-in-c
static void genMatrix(int sizeX, int sizeY, float matrix[sizeX][sizeY]){
	int i, j;

	for(i = 0; i < sizeX; i++){
		for(j = 0; j < sizeY; j++){
			matrix[i][j] = (float)((double)rand()/(double)(RAND_MAX/max) - (max-1));
		}
	}
}

static void genBias(float bias[], int size){
	int i;

	for(i = 0; i < size; i++){
		bias[i] = (float)((double)rand()/(double)(RAND_MAX/biasMax));
	}
}

// Cred till:
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

// For every weight, bias associated with the nudge. Change it accordingly.
static void backPropagate(float *nudges){
	int i, j;
	float 	learningValue = 0.0,
			layerNudges[HIDDEN] = {0};	//Will be used to determine if a layer received a negative or positive nudge when summed up. e.g. Out -> H2.
										//Will be used to calculate the next layer, weights and biases.


	// W3, BH2 is handled here.
	for(i = 0; i < OUTPUT; i++){
		learningValue 		 = sqrt(pow((nudges[i]*learningRate), 2));	//Make a positive number.
		// learningValue 		 = 	 (nudges[i]*learningRate);
		// if(DEBUG) printf("\nNudge[%d] from output is: %f\n", i, nudges[i]);
		
		// Start with W3, H2, BH2
		// The greater the influence of the node in H2, the greater the increase/decrease in value to maximize
		// the "bang for our bucks" on the weight/bias.
		for(j = 0; j < HIDDEN; j++){
			if(DEBUG) printf("Weight3 [%d][%d] was: %f\n", j, i, W3[j][i]);

			if(nudges[i] < 0.0){							//If the nudge is negative, this is not the desired output. Decrease impact of edges.
				if(W3[j][i] < 0.0)
					W3[j][i] 		+= learningValue*Out[i];	//Negative becomes less negative.
				else 
					W3[j][i] 		-= learningValue*Out[i];	//Positive becomes less positive.

				// If H2[j] had a huge impact leading to a wrong answer (i.e. negative nudge), increase bias to make it less
				// likely to affect to a wrong answer again.
				BH2[j]				+= learningValue;
				// Remember desired changes
				layerNudges[j]		+= learningValue*W3[j][i];
			}else{											//If the nudge is positive, this is desired output. Increase positive impact of edges.
				W3[j][i] 			+= learningValue*Out[i];	//Positive becomes more positive.
				BH2[j]				-= learningValue;
				// Remember desired changes in proportion to the impact of a given weight.
				layerNudges[j]		+= learningValue*W3[j][i];
			}


			if(DEBUG) printf("Weight3 [%d][%d] is now: %f\n", j, i, W3[j][i]);
			
		}

		if(DEBUG) printf("layerNudge[%d] is: %f\n", i, layerNudges[i]);
	}
	
	if(DEBUG) printf("\n\n");

	// W2, H1, BH1 is handled here.
	for(i = 0; i < HIDDEN; i++){
		learningValue		 = sqrt(pow((layerNudges[i]*learningRate), 2));
		layerNudges[i]		 = 0.0;

		for(j = 0; j < HIDDEN; j++){
			if(DEBUG) printf("Weight2 [%d][%d] was: %f\n", j, i, W2[j][i]);

			if(nudges[i] < 0.0){							//If the nudge is negative, this is not the desired output. Decrease impact of edges.
				if(W2[j][i] < 0.0)
					W2[j][i] 		+= learningValue*H2[j];	//Negative becomes less negative.
				else 
					W2[j][i] 		-= learningValue*H2[j];	//Positive becomes less positive.

				// If H2[j] had a huge impact leading to a wrong answer (i.e. negative nudge), increase bias to make it less
				// likely to affect to a wrong answer again.
				BH1[j]				+= learningValue;
				// Remember desired changes
				layerNudges[j]		-= learningValue*W2[j][i];
			}else{											//If the nudge is positive, this is desired output. Increase impact of edges.
				W2[j][i] 			+= learningValue*H2[j];	//Positive becomes more positive.
				BH1[j]				-= learningValue;
				// Remember desired changes
				layerNudges[j]		+= learningValue*W2[j][i];
			}


			if(DEBUG) printf("Weight2 [%d][%d] is now: %f\n", j, i, W2[j][i]);
		}

		if(DEBUG) printf("layerNudge[%d] is: %f\n", i, layerNudges[i]);
	}

	if(DEBUG) printf("\n\n");

	// W1 is handeled here. Can only change W1 on this stage.
	for(i = 0; i < HIDDEN; i++){
		learningValue		 = sqrt(pow((layerNudges[i]*learningRate), 2));
		layerNudges[i]		 = 0.0;

		for(j = 0; j < HIDDEN; j++){
			if(DEBUG) printf("Weight1 [%d][%d] was: %f\n", j, i, W1[j][i]);

			if(layerNudges[i] < 0.0){
				if(W1[j][i] < 0.0)
					W1[j][i]		+= learningValue*H1[j];
				else
					W1[j][i] 		-= learningValue*H1[j];
			}else
				W1[j][i] 			+= learningValue*H1[j];
			
			if(DEBUG) printf("Weight1 [%d][%d] is now: %f\n", j, i, W1[j][i]);
		}

	}
}

// Calculate how much each output was wrong. This will determine how much to change weights, biases associated with this output.
// Send this data to back-propagate
static void initBackPropagate(int wantNr, int size){
	int i;
	float nudges[OUTPUT];

	for(i = 0; i < size; i++){
		if(i != wantNr)
			nudges[i] = -pow((Out[i] - correctOut[i]), 2);
		else
			nudges[i] = pow((Out[i] - correctOut[i]), 2);

		if(DEBUG) printf("Outputnudges[%d]: %f\n", i, nudges[i]);
	}

	backPropagate(nudges);
}

// Check the result of our NN. If wrong begin training.
static void checkResultOfNN(int inputLabel){
	int i, iGreatestVal;
	float greatestVal = -999999;

	// Find the greatest value and corresponding index for later comparison. 
	// The greatest value is the number the NN think is the correct one.
	for(i = 0; i < OUTPUT; i++){
		if(Out[i] > greatestVal){
			greatestVal = Out[i];	iGreatestVal = i;
		}
	}

	// First check, was NN wrong? If not, all is good. If the index with the greatest value is equal to the number we want,
	// then the NN was right.
	if(iGreatestVal != inputLabel){
		if(SHOWCASE || DEBUG)	printf("\nThe NN got the WRONG number. It guessed %d when it should be %d. Retraining!\n", iGreatestVal, inputLabel);

		correctOut[inputLabel] = 1;	//This is the number we want. All else is 0.
		initBackPropagate(inputLabel, OUTPUT);
	}else{
		if(SHOWCASE || DEBUG)	printf("\nThe NN got the CORRECT number which was: %d \n", iGreatestVal);
		// Return the number here when finished.
	}
}

static void init(){

	srand((unsigned int)time(NULL));

	genMatrix(INPUT, HIDDEN, W1);
	genMatrix(HIDDEN, HIDDEN, W2);
	genMatrix(HIDDEN, OUTPUT, W3);
	genBias(BH1, HIDDEN);
	genBias(BH2, HIDDEN);
}

static mnist_data *importMnistData(){
	mnist_data *data;                              //.data och .label
    unsigned int cnt;
    int ret = 0, x, y, i, currData = 0;

    // Returns 0 when all is good
    if ((ret = mnist_load("traindata/train-images.idx3-ubyte", "traindata/train-labels.idx1-ubyte", &data, &cnt))) {
        printf("An error occured: %d\n", ret);
    } else {
        printf("image count: %d\n", cnt);

        printf("\nLabel for matrix: %u\n", data[currData].label);
        for(i = 0; i < 28; i++)
                printf("%3d ", i);
        printf("\n");
        for(y = 0; y < 28; y++){
                for(x = 0; x < 28; x++){
                        if(data[currData].data[y][x] != 0)
                                printf("1 ");
                        else
                                printf(" ");
                }
                printf("\n");
        }
        printf("\n");
        
        // free(data);
    }
    return data;
}

/**************************************************************************/
/*	PUBLIC METHODS														  */
/**************************************************************************/
void neuralNetwork(float *input, int inputLabel) {
	// init();
	// Change testInput to Data later.
	if(DEBUG)	printf("**********************************\n");
	if(DEBUG)	printf("To H1:\n");
	sumLayer(INPUT, INPUT, HIDDEN, input, W1, H1, BH1);
	if(DEBUG)	printf("**********************************\n");
	if(DEBUG)	printf("**********************************\n");
	if(DEBUG)	printf("To H2:\n");
	sumLayer(HIDDEN, HIDDEN, HIDDEN, H1, W2, H2, BH2);
	if(DEBUG)	printf("**********************************\n");
	if(DEBUG)	printf("**********************************\n");
	if(DEBUG)	printf("To Out:\n");
	sumLayer(HIDDEN, HIDDEN, OUTPUT, H2, W3, Out, BOUT);				// No bias on out nodes, BOUT is only 0

	checkResultOfNN(inputLabel);
}

// Körs vid ensam körning av NN.
int main(int argc, char const *argv[]){
	int i, j, k = 0, dataSet = 0, menuchoice;
	mnist_data *data;   
	float input[INPUT];



	data = importMnistData();
	init();
	

	while(1){
		printf("\n***********************\n");
		printf("1. Train network\n");
		printf("2. See info\n");
		// printf("3. Print \n");
		printf("\n***********************\n");

		scanf("%d", &menuchoice);

		switch(menuchoice){
			case 1: 
				for(dataSet = 0; dataSet < 1; dataSet++){
					k = 0;
					for(i = 0; i < MNISTSIZE; i++)
						for(j = 0; j < MNISTSIZE; j++)
							input[k++] = data[dataSet].data[i][j];
					
					neuralNetwork(input, data[dataSet].label);
					printf("\nOutput: ");
					for(i = 0; i < OUTPUT; i++)
						printf("%f ", Out[i]);
					printf("\n");
				}

				
				
			break;

			case 2:
				for(i = 0; i < HIDDEN; i++)
						for(j = 0; j < HIDDEN; j++)
							printf("W2[%d][%d]: %f \n", i, j, W2[i][j]);
						printf("\n");

				for(i = 0; i < HIDDEN; i++)
						for(j = 0; j < OUTPUT; j++)
							printf("W3[%d][%d]: %f \n", i, j, W3[i][j]);
			break;
		}
	}


	return 0;
}