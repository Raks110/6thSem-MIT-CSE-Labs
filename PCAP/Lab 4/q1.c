#include<mpi.h>
#include<stdio.h>

int main(){

	int rank, size;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int num = rank+1;
	int prod;
	int result;

	MPI_Scan(&num, &prod, 1, MPI_INT, MPI_PROD, MPI_COMM_WORLD);
	MPI_Scan(&prod, &result, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

	if(rank == size-1){
		printf("Final Result: %d.\n", result);
	}

	MPI_Finalize();
	return 0;
}