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

// Implicit initialization to 0
float	W1[INPUT][HIDDEN],
		W2[HIDDEN][HIDDEN],
		W3[HIDDEN][OUTPUT],
		H1[HIDDEN],
		H2[HIDDEN],
		Out[OUTPUT];





/**************************************************************************/
/*	PRIVATE METHODS														  */
/**************************************************************************/



/**************************************************************************/
/*	PUBLIC METHODS														  */
/**************************************************************************/
void neuralNetwork(struct matrices *data) {

}