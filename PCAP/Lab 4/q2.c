#include <mpi.h>
#include <math.h>
#include <stdio.h>

int main(){

    int n, rank, size, i;
    double mypi, pi, h, sum, x;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    
    n = size;
	h   = 1.0 / (double) n;
	sum = 0.0;
	for (i = rank + 1; i <= n; i += size) {
		x = h * ((double)i - 0.5);
	    sum += 4.0 / (1.0 + x*x);
	}
	
	mypi = h * sum;
	    
	MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0,
		MPI_COMM_WORLD);
	    
	if (rank == 0){
		printf("Rakshit's Pi is currently values at %.8f\n", pi);
	}
    
    MPI_Finalize();
    return 0;
}