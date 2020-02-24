#include <mpi.h>
#include <stdio.h>

#define MCW MPI_COMM_WORLD

int main(int argc, char** argv){

	int size, rank, x;
	MPI_Status status;
	int TAG = 10;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MCW, &size);
	MPI_Comm_rank(MCW, &rank);

	if(size == 1)
		MPI_Abort(MCW, 1);

	if(rank == 0){

		printf("Enter a number: ");
		scanf("%d", &x);

		MPI_Send(&x, 1, MPI_INT, rank+1, TAG, MCW);
		MPI_Recv(&x, 1, MPI_INT, size-1, TAG, MCW, &status);

		printf("Finally Received Back: %d\n", x);
	}
	else{

		MPI_Recv(&x, 1, MPI_INT, rank-1, TAG, MCW, &status);
		printf("Process %d received %d.\n", rank, x);
		x++;
		MPI_Send(&x, 1, MPI_INT, (rank+1)%size, TAG, MCW);
	}

	MPI_Finalize();
	return 0;
}