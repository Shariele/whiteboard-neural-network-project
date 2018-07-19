#define USE_MNIST_LOADER
#define MNIST_DOUBLE
#include "mnist-master/mnist.h"

#include <stdio.h>

/***************************************************************************/
/*   All cred till Nuri Park för hämtning och organisering av MNIST data   */
/*   Samt MNIST för tränings och testdata                                  */
/***************************************************************************/
/*   MNIST loader by Nuri Park - https://github.com/projectgalateia/mnist  */
/***************************************************************************/
/*   http://yann.lecun.com/exdb/mnist/                                     */
/***************************************************************************/

int main(int argc, char **argv){
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
                
                free(data);
        }

        return 0;
}