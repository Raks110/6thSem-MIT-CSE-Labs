#include<stdio.h>
#include<mpi.h>

int main(){

	int size, rank, m=1;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int arr[size];

	if(!rank){

		printf("%s\n", "Enter all elements");
		for(int i=0;i<size;i++){
			scanf("%d", &arr[i]);
		}
	}

	MPI_Scatter(arr, 1, MPI_INT, &m, 1, MPI_INT, 0, MPI_COMM_WORLD);

	int result;
	MPI_Scan(&m, &result, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

	MPI_Gather(&result, 1, MPI_INT, arr, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if(!rank){

		printf("%s\n", "After Scanning");
		for(int i=0;i<size;i++){
			printf("%d ", arr[i]);
		}
		printf("\n");
	}

	MPI_Finalize();
	return 0;
}