#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv){

	int size, rank, x;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if(rank == 0){

		scanf("%d", &x);

		for(int i=1;i<size;i++){
			MPI_Send(&x, 1, MPI_INT, i, i, MPI_COMM_WORLD);
		}
	}
	else{

		MPI_Recv(&x, 1, MPI_INT, 0, rank, MPI_COMM_WORLD, &status);
		printf("Process ID: %d\nReceived: %d\n\n", rank, x);
	}

	MPI_Finalize();
	return 0;
}