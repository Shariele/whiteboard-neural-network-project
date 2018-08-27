/**************************************************************************/
/*	INCLUDES	 														  */
/**************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**************************************************************************/
/*	IMPORT METHODS	 													  */
/**************************************************************************/
#include "neural-network.h"
// #include "mnist-master/mnist.h"

/**************************************************************************/
/*	TYPE DEFINITIONS & VARIABLES										  */
/**************************************************************************/
#define DEBUG 0
// #define USE_MNIST_LOADER
// #define MNIST_DOUBLE




static matrices trainData[1];

/**************************************************************************/
/*	METHODS														  		  */
/**************************************************************************/

mnist_data *importMnistData(){
	mnist_data *data;                              //.data och .label
    unsigned int cnt;
    int ret = 0, x, y, i, currData = 1;

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

int main(int argc, char const *argv[]){
	mnist_data *data;   
	int i, x, y, countTD = 0;
	trainData[0].number = 1;
	trainData[0].id = 0;

	if(DEBUG){

		// Print matrix
		printf("\nCountTD: %d\n", countTD);
		printf("\nId for matrix: %d \tNumber for matrix: %d\n", trainData[countTD].id, trainData[countTD].number);
		for(i = 0; i < MATRIXSIZE; i++)
			printf("%2d ", i);
		printf("\n");
		for(y = 0; y < MATRIXSIZE; y++){
			for(x = 0; x < MATRIXSIZE; x++){
				printf("%2d ", trainData[countTD].matrix[x][y]);
			}
			printf("\n");
		}
		printf("\n");
	}

	data = importMnistData();
	neuralNetwork(data);

	return 0;
}