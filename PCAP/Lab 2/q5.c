#include <mpi.h>
#include <stdio.h>

#define MCW MPI_COMM_WORLD

int factorial(int n){

	int ret = 1;
	for(int i=1;i<=n;i++)
		ret *= i;

	return ret;
}

int other(int rank){

	int m = 0;

	if(rank%2 == 0){
		m = (rank * (rank+1) / 2); 
	}
	else{
		m = factorial(rank);
	}

	return m;
}

int main(int argc, char** argv){

	int size, rank, x = 0;
	MPI_Status status;
	int TAG = 10;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MCW, &size);
	MPI_Comm_rank(MCW, &rank);

	if(rank == 0){

		if(size%2 == 0)
			x += rank;
		else
			x += other(rank);

		MPI_Send(&x, 1, MPI_INT, rank+1, TAG, MCW);
		MPI_Recv(&x, 1, MPI_INT, size-1, TAG, MCW, &status);

		printf("Finally Received Back: %d\n", x);
	}
	else{

		MPI_Recv(&x, 1, MPI_INT, rank-1, TAG, MCW, &status);

		if(size%2 == 0)
			x += rank;
		else
			x += other(rank);

		MPI_Send(&x, 1, MPI_INT, (rank+1)%size, TAG, MCW);
	}

	MPI_Finalize();
	return 0;
}