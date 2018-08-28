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
#define NUDGEDEBUG 1
#define SHOWCASE 1
#define INPUT 	784
#define HIDDEN 	16	
#define OUTPUT	10

static double learningRate = 0.05;
// static double eta = 0.5, alpha = 0.9;
//The highest double value that can ge generated.
static double max = 2;		
static double biasMax = 2;	


// Implicit initialization to 0
static double	
		W1[HIDDEN][INPUT],
		W2[HIDDEN][HIDDEN],
		W3[HIDDEN][OUTPUT],
		DeltaW3[HIDDEN][OUTPUT],
		DeltaW2[HIDDEN][HIDDEN],
		DeltaW1[HIDDEN][INPUT],
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

// double testInput[INPUT] = {1, 3, 1, 5};





/**************************************************************************/
/*	PRIVATE METHODS														  */
/**************************************************************************/
// https://stackoverflow.com/questions/13408990/how-to-generate-random-double-number-in-c
static void genMatrix(int sizeX, int sizeY, double matrix[sizeX][sizeY]){
	int i, j;

	for(i = 0; i < sizeX; i++){
		for(j = 0; j < sizeY; j++){
			matrix[i][j] = (double)((double)rand()/(double)(RAND_MAX/max) - (max - 1));
		}
	}
}

static void genBias(double bias[], int size){
	int i;

	for(i = 0; i < size; i++){
		bias[i] = (double)((double)rand()/(double)(RAND_MAX/biasMax) - (biasMax - 1));
	}
}

// Cred till:
// http://www.ece.utep.edu/research/webfuzzy/docs/kk-thesis/kk-thesis-html/node72.html
static double sigmoid(double x){
     double exp_value;
     double return_value;

     /*** Exponential calculation ***/
     exp_value = exp((double) -x);

     /*** Final sigmoid value ***/
     return_value = 1 / (1 + exp_value);

     return return_value;
}

static double matrixVectorMult(int i, int j, int matSize, double *vec, double mat[][matSize], double tempSum){
	if(DEBUG)	printf("tempSum[%d] before calc: %f\n", i, tempSum);
	if(DEBUG)	printf("matsize: %d\n", matSize);
	if(DEBUG)	printf("j: %d\n", j);

	tempSum += mat[i][j]*vec[j];

	if(DEBUG)	printf("tempSum %d after calc: %f\n\n", i, tempSum);

	return j == matSize-1		?	tempSum
		:	matrixVectorMult(i, ++j, matSize, vec, mat, tempSum);		
}

static void sumLayer(int sourceSize, int wSize, int destSize, double *sourceL, double weigth[destSize][wSize], double *destL,  double *bias){
	
	for(int i = 0; i < destSize; i++){
		destL[i] = sigmoid(matrixVectorMult(i, 0, wSize, sourceL, weigth, 0.0) + bias[i]);
		// destL[i] = matrixVectorMult(i, 0, wSize, sourceL, weigth, 0.0) - bias[i];

		if(DEBUG)	printf("\ndestL[%d]: %f \t bias[%d]: %f\n\n", i, destL[i], i, bias[i]);
	}
}

// For every weight, bias associated with the nudge. Change it accordingly.
static void backPropagate(double *input, double *nudgesHO){
	int i, j;
	double 	//learningValue = 0.0,
			sumNudges[HIDDEN],
			nudgesHH[HIDDEN] = {0.0},
			// layerNudges[HIDDEN] = {0.0},
			nudgesIH[HIDDEN] = {0.0};	//Will be used to determine if a layer received a negative or positive nudge when summed up. e.g. Out -> H2.
										//Will be used to calculate the next layer, weights and biases.

	// NudgesHO används for W3
	// Prepare nudges
	for(i = 0; i < HIDDEN; i++){
		sumNudges[i] = 0.0;
		for(j = 0; j < OUTPUT; j++){
			// sumNudges[i] += W3[i][j]*nudgesHO[j];
			if(nudgesHO[j] < 0.0)
				sumNudges[i] += -sqrt(pow((W3[i][j]*nudgesHO[j]), 2));
			else
				sumNudges[i] += sqrt(pow((W3[i][j]*nudgesHO[j]), 2));
		}
		nudgesHH[i] = sumNudges[i] * H2[i] * (1.0 - H2[i]);		//Används för W2
	}

	for(i = 0; i < HIDDEN; i++){
		sumNudges[i] = 0.0;
		for(j = 0; j < HIDDEN; j++){
			// sumNudges[i] += W2[j][i]*nudgesHH[i];
			if(nudgesHH[j] < 0.0)
				sumNudges[i] += -sqrt(pow((W2[i][j]*nudgesHH[j]), 2));
			else
				sumNudges[i] += sqrt(pow((W2[i][j]*nudgesHH[j]), 2));
		}
		nudgesIH[i] = sumNudges[i] * H1[i] * (1.0 - H1[i]);		//Används för W1
	}

	// Change biases
	// for(i = 0; i < HIDDEN; i++){
	// 	sumNudges[i] = 0.0;
	// 	for(j = 0; j < OUTPUT; j++){
	// 		// sumNudges[i] += (W3[i][j]*nudgesHO[j]);			//Use sumnudges as a counter
	// 		if(nudgesHO[j] < 0.0)
	// 			sumNudges[i] += -sqrt(pow((W3[i][j]*nudgesHO[j]), 2));
	// 		else
	// 			sumNudges[i] += sqrt(pow((W3[i][j]*nudgesHO[j]), 2));
	// 	}
	// 	BH2[i] += sumNudges[i] * H2[i] * (1.0 - H2[i]) * learningRate;		//Används för W2
	// }

	// for(i = 0; i < HIDDEN; i++){
	// 	sumNudges[i] = 0.0;
	// 	for(j = 0; j < HIDDEN; j++){
	// 		// sumNudges[i] += (W3[i][j]*nudgesHO[j]);			//Use sumnudges as a counter
	// 		if(nudgesHH[j] < 0.0)
	// 			sumNudges[i] += -sqrt(pow((W2[i][j]*nudgesHH[j]), 2));
	// 		else
	// 			sumNudges[i] += sqrt(pow((W2[i][j]*nudgesHH[j]), 2));
	// 	}
	// 	BH1[i] += sumNudges[i] * H1[i] * (1.0 - H1[i]) * learningRate;		//Används för W2
	// }
	
	// for(i = 0; i < HIDDEN; i++){
	// 	sumNudges[i] = 0.0;
	// 	for(j = 0; j < OUTPUT; j++){
	// 		sumNudges[i] += (-W2[i][j]*nudgesHH[j]);			//Use sumnudges as a counter
	// 	}
	// 	BH1[i] += sumNudges[i];		//Används för W
	// }

	if(NUDGEDEBUG){
		for(i = 0; i < OUTPUT; i++){
			printf("NudgeHO[%d]: %f\n", i, nudgesHO[i]);
		}
		printf("\n");
		for(i = 0; i < HIDDEN; i++){
			printf("NudgeHH[%d]: %f\n", i, nudgesHH[i]);
		}
		printf("\n");
		for(i = 0; i < HIDDEN; i++){
			printf("NudgeIH[%d]: %f\n", i, nudgesIH[i]);
		}
	}

	// // W3 
	for(i = 0; i < OUTPUT; i++){
		for(j = 0; j < HIDDEN; j++){
			DeltaW3[j][i] = learningRate * H2[i] * nudgesHO[i] + learningRate * DeltaW3[j][i];
			W3[j][i] += DeltaW3[j][i];
		}
	}
	// // W2
	for(i = 0; i < HIDDEN; i++){
		for(j = 0; j < HIDDEN; j++){
			DeltaW2[i][j] = learningRate * H1[i] * nudgesHH[i] + learningRate * DeltaW2[i][j];
			W2[i][j] += DeltaW2[i][j];
		}
	}

	// // W1
	for(i = 0; i < HIDDEN; i++){
		for(j = 0; j < INPUT; j++){
			DeltaW1[i][j] = learningRate * input[i] * nudgesIH[i] + learningRate * DeltaW1[i][j];
			W1[i][j] += DeltaW1[i][j];
		}
	}
}

// Calculate how much each output was wrong. This will determine how much to change weights, biases associated with this output.
// Send this data to back-propagate
static void initBackPropagate(double *input, int wantNr, int size){
	int i;
	double nudges[OUTPUT];

	for(i = 0; i < size; i++){
		if(i != wantNr)
			nudges[i] = -pow((Out[i] - correctOut[i]), 2) * Out[i] * (1.0 - Out[i]);
		else
			nudges[i] = pow((Out[i] - correctOut[i]), 2) *  Out[i] * (1.0 - Out[i]);
		
		//	DeltaO[k] = (Target[p][k] - Output[p][k]) * Output[p][k] * (1.0 - Output[p][k]) ;   /* Sigmoidal Outputs, SSE */
		// if(NUDGEDEBUG) printf("correctout[%d]: %f\n", i, correctOut[i]);

		// nudges[i] = pow((correctOut[i] - Out[i]), 2) +  Out[i] * (1.0 - Out[i]);
		correctOut[i] = 0;													//Reset

		// if(NUDGEDEBUG) printf("Outputnudges[%d]: %f\n", i, nudges[i]);
	}

	backPropagate(input, nudges);
}

// Check the result of our NN. If wrong begin training.
static void checkResultOfNN(double *input, int inputLabel){
	int i, iGreatestVal;
	double greatestVal = -999999;

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
		initBackPropagate(input, inputLabel, OUTPUT);
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
	// genBias(BH1, HIDDEN);
	// genBias(BH2, HIDDEN);
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
void neuralNetwork(double *input, int inputLabel) {
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

	checkResultOfNN(input, inputLabel);
}

// Körs vid ensam körning av NN.
int main(int argc, char const *argv[]){
	int i, j, k = 0, dataSet = 0, menuchoice;
	mnist_data *data;   
	double input[INPUT];



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
				for(dataSet = 0; dataSet < 1000; dataSet++){
					k = 0;
					for(i = 0; i < MNISTSIZE; i++)
						for(j = 0; j < MNISTSIZE; j++)
							input[k++] = data[dataSet].data[i][j];
					
					neuralNetwork(input, data[dataSet].label);
					printf("Output: \n");
					for(i = 0; i < OUTPUT; i++)
						printf("%d: %f ", i, Out[i]);
					printf("\n");
				}

				
				
			break;

			case 2:
			printf("\n");
			for(i = 0; i < HIDDEN; i++)
				for(j = 0; j < HIDDEN; j++)
					printf("W1[%d][%d]: %f \n", i, j, W1[i][j]);
						
			printf("\n");

			for(i = 0; i < HIDDEN; i++)
				for(j = 0; j < HIDDEN; j++)
					printf("W2[%d][%d]: %f \n", i, j, W2[i][j]);
					
			printf("\n");

			for(i = 0; i < HIDDEN; i++)
				for(j = 0; j < OUTPUT; j++)
					printf("W3[%d][%d]: %f \n", i, j, W3[i][j]);
			printf("\n");
			for (int i = 0; i < HIDDEN; i++){
				printf("H1[%d]: %f \n", i, H1[i]);
			}
			printf("\n");
			for (int i = 0; i < HIDDEN; i++){
				printf("H2[%d]: %f \n", i, H2[i]);
			}
			printf("\n");
			for (int i = 0; i < OUTPUT; i++){
				printf("Out[%d]: %f \n", i, Out[i]);
			}
			printf("\n");
			for (int i = 0; i < HIDDEN; i++){
				printf("BH1[%d]: %f \n", i, BH1[i]);
			}
			printf("\n");
			for (int i = 0; i < HIDDEN; i++){
				printf("BH2[%d]: %f \n", i, BH2[i]);
			}
			break;

			case 0:
				exit(0);
			break;
		}
	}


	return 0;
}




































// V1
// static void backPropagate(double *input, double *nudgesHO){
// 	int i, j;
// 	double 	//learningValue = 0.0,
// 			sumNudges[HIDDEN],
// 			nudgesHH[HIDDEN] = {0.0},
// 			// layerNudges[HIDDEN] = {0.0},
// 			nudgesIH[HIDDEN] = {0.0};	//Will be used to determine if a layer received a negative or positive nudge when summed up. e.g. Out -> H2.
// 										//Will be used to calculate the next layer, weights and biases.

// 	// NudgesHO används for W3
// 	// Prepare nudges
// 	for(i = 0; i < HIDDEN; i++){
// 		sumNudges[i] = 0.0;
// 		for(j = 0; j < OUTPUT; j++){
// 			sumNudges[i] += W3[i][j]*nudgesHO[j];
// 		}
// 		nudgesHH[i] = sumNudges[i] * H2[i] * (1.0 - H2[i]);		//Används för W2
// 	}

// 	for(i = 0; i < HIDDEN; i++){
// 		sumNudges[i] = 0.0;
// 		for(j = 0; j < HIDDEN; j++){
// 			sumNudges[i] += W2[j][i]*nudgesHH[i];
// 		}
// 		nudgesIH[i] = sumNudges[i] * H1[i] * (1.0 - H1[i]);		//Används för W1
// 	}

// 	// Change biases
// 	for(i = 0; i < HIDDEN; i++){
// 		sumNudges[i] = 0.0;
// 		for(j = 0; j < OUTPUT; j++){
// 			sumNudges[i] += W3[i][j]*nudgesHO[j];			//Use sumnudges as a counter
// 		}
// 		BH2[i] += sumNudges[i];		//Används för W2
// 	}
	
// 	for(i = 0; i < HIDDEN; i++){
// 		sumNudges[i] = 0.0;
// 		for(j = 0; j < OUTPUT; j++){
// 			sumNudges[i] += W2[i][j]*nudgesHH[j];			//Use sumnudges as a counter
// 		}
// 		BH1[i] += sumNudges[i];		//Används för W
// 	}

// 	if(NUDGEDEBUG){
// 		for(i = 0; i < OUTPUT; i++){
// 			printf("NudgeHO[%d]: %f\n", i, nudgesHO[i]);
// 		}
// 		printf("\n");
// 		for(i = 0; i < HIDDEN; i++){
// 			printf("NudgeHH[%d]: %f\n", i, nudgesHH[i]);
// 		}
// 		printf("\n");
// 		for(i = 0; i < HIDDEN; i++){
// 			printf("NudgeIH[%d]: %f\n", i, nudgesIH[i]);
// 		}
// 	}

// 	// W3 
// 	for(i = 0; i < OUTPUT; i++){
// 		for(j = 0; j < HIDDEN; j++){
// 			DeltaW3[j][i] = learningRate * H2[i] * nudgesHO[i] + learningRate * DeltaW3[i][j];
// 			W3[j][i] += DeltaW3[j][i];
// 		}
// 	}
// 	// W2
// 	for(i = 0; i < HIDDEN; i++){
// 		for(j = 0; j < HIDDEN; j++){
// 			DeltaW2[j][i] = learningRate * H1[i] * nudgesHH[i] + learningRate * DeltaW2[i][j];
// 			W2[j][i] += DeltaW2[j][i];
// 		}
// 	}

// 	// W1
// 	for(i = 0; i < INPUT; i++){
// 		for(j = 0; j < HIDDEN; j++){
// 			DeltaW1[j][i] = learningRate * input[i] * nudgesIH[i] + learningRate * DeltaW1[j][i];
// 			W1[j][i] += DeltaW1[j][i];
// 		}
// 	}


// 	// W3, BH2 is handled here.
// 	// for(i = 0; i < OUTPUT; i++){
// 	// 	learningValue 		 = sqrt(pow((nudgesHO[i]*learningRate), 2));	//Make a positive number.
// 	// 	// learningValue 		 = 	 (nudges[i]*learningRate);
// 	// 	// if(DEBUG) printf("\nNudge[%d] from output is: %f\n", i, nudges[i]);
		
// 	// 	// Start with W3, H2, BH2
// 	// 	// The greater the influence of the node in H2, the greater the increase/decrease in value to maximize
// 	// 	// the "bang for our bucks" on the weight/bias.
// 	// 	for(j = 0; j < HIDDEN; j++){
// 	// 		if(DEBUG) printf("Weight3 [%d][%d] was: %f\n", j, i, W3[j][i]);

// 	// 		if(nudgesHO[i] < 0.0){							//If the nudge is negative, this is not the desired output. Decrease impact of edges.
// 	// 			if(W3[j][i] < 0.0)
// 	// 				W3[j][i] 		+= learningValue*Out[i];	//Negative becomes less negative.
// 	// 			else 
// 	// 				W3[j][i] 		-= learningValue*Out[i];	//Positive becomes less positive.

// 	// 			// If H2[j] had a huge impact leading to a wrong answer (i.e. negative nudge), increase bias to make it less
// 	// 			// likely to affect to a wrong answer again.
// 	// 			BH2[j]				+= learningValue;
// 	// 			// Remember desired changes
// 	// 			layerNudges[j]		+= learningValue*W3[j][i];
// 	// 		}else{											//If the nudge is positive, this is desired output. Increase positive impact of edges.
// 	// 			W3[j][i] 			+= learningValue*Out[i];	//Positive becomes more positive.
// 	// 			BH2[j]				-= learningValue;
// 	// 			// Remember desired changes in proportion to the impact of a given weight.
// 	// 			layerNudges[j]		+= learningValue*W3[j][i];
// 	// 		}


// 	// 		if(DEBUG) printf("Weight3 [%d][%d] is now: %f\n", j, i, W3[j][i]);
			
// 	// 	}

// 	// 	if(DEBUG) printf("layerNudge[%d] is: %f\n", i, layerNudges[i]);
// 	// }
	
// 	// if(DEBUG) printf("\n\n");

// 	// // W2, H1, BH1 is handled here.
// 	// for(i = 0; i < HIDDEN; i++){
// 	// 	learningValue		 = sqrt(pow((layerNudges[i]*learningRate), 2));
// 	// 	layerNudges[i]		 = 0.0;

// 	// 	for(j = 0; j < HIDDEN; j++){
// 	// 		if(DEBUG) printf("Weight2 [%d][%d] was: %f\n", j, i, W2[j][i]);

// 	// 		if(layerNudges[i] < 0.0){							//If the nudge is negative, this is not the desired output. Decrease impact of edges.
// 	// 			if(W2[j][i] < 0.0)
// 	// 				W2[j][i] 		+= learningValue*H2[j];	//Negative becomes less negative.
// 	// 			else 
// 	// 				W2[j][i] 		-= learningValue*H2[j];	//Positive becomes less positive.

// 	// 			// If H2[j] had a huge impact leading to a wrong answer (i.e. negative nudge), increase bias to make it less
// 	// 			// likely to affect to a wrong answer again.
// 	// 			BH1[j]				+= learningValue;
// 	// 			// Remember desired changes
// 	// 			layerNudges[j]		-= learningValue*W2[j][i];
// 	// 		}else{											//If the nudge is positive, this is desired output. Increase impact of edges.
// 	// 			W2[j][i] 			+= learningValue*H2[j];	//Positive becomes more positive.
// 	// 			BH1[j]				-= learningValue;
// 	// 			// Remember desired changes
// 	// 			layerNudges[j]		+= learningValue*W2[j][i];
// 	// 		}


// 	// 		if(DEBUG) printf("Weight2 [%d][%d] is now: %f\n", j, i, W2[j][i]);
// 	// 	}

// 	// 	if(DEBUG) printf("layerNudge[%d] is: %f\n", i, layerNudges[i]);
// 	// }

// 	// if(DEBUG) printf("\n\n");

// 	// // W1 is handeled here. Can only change W1 on this stage.
// 	// for(i = 0; i < HIDDEN; i++){
// 	// 	learningValue		 = sqrt(pow((layerNudges[i]*learningRate), 2));
// 	// 	layerNudges[i]		 = 0.0;

// 	// 	for(j = 0; j < HIDDEN; j++){
// 	// 		if(DEBUG) printf("Weight1 [%d][%d] was: %f\n", j, i, W1[j][i]);

// 	// 		if(layerNudges[i] < 0.0){
// 	// 			if(W1[j][i] < 0.0)
// 	// 				W1[j][i]		+= learningValue*H1[j];
// 	// 			else
// 	// 				W1[j][i] 		-= learningValue*H1[j];
// 	// 		}else
// 	// 			W1[j][i] 			+= learningValue*H1[j];
			
// 	// 		if(DEBUG) printf("Weight1 [%d][%d] is now: %f\n", j, i, W1[j][i]);
// 	// 	}

// 	// }
// }