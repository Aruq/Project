#include "stdio.h"
#include "mpi.h"
#include <math.h>

double getTaylorElement(int n){

	double value = 1.0 / ( (2.0 * n) + 1.0);
	return (n % 2 == 0)? value : -value;
}

int main(argc,argv)

int argc; 
char *argv[]; 
{

	
	int numOfTerms; 	
	int nodeId; 		
	int numberOfNodes; 	
    
	double PI25DT = 3.141592653589793238462643; 
	double nodeSum = 0; 
	double totalSum; 	
	double predictedPi; 
	MPI_Init(&argc,&argv);

	MPI_Comm_size(MPI_COMM_WORLD,&numberOfNodes); 
	
	MPI_Comm_rank(MPI_COMM_WORLD,&nodeId);

	if (nodeId == 0) {
	    printf("Enter the number of intervals: (0 quits) ");
	    scanf("%d",&numOfTerms); 
	}

	MPI_Bcast(&numOfTerms, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	if (numOfTerms == 0){
		return 0;
	}

	for(int i = nodeId; i<numOfTerms; i += numberOfNodes){
		nodeSum += getTaylorElement(i);
	}
	printf("NodeId=[%d] Summation=[%.4f]\n", nodeId, nodeSum);
	

	MPI_Reduce(&nodeSum, &totalSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	
	if (nodeId == 0){
		predictedPi = 4.0 * totalSum;
		 printf("pi is approximately %.16f, Error is %.16f\n", predictedPi, fabs(predictedPi - PI25DT));
		
	}
	      
	MPI_Finalize();
	return 0;
}
