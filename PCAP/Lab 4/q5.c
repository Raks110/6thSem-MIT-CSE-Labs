#include<mpi.h>
#include<stdio.h>

int main(){

	int rank, size;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int arr[100][size];
	int key;

	if(!rank){

		printf("Enter %d elements\n", size*size);
		for(int i=0;i<size;i++){

			for(int j=0;j<size;j++){

				scanf("%d", &arr[i][j]);
			}
		}
	}

	int recv[size];
	int recv_beta[size];
	int recv_beta2[size];
	
	for(int i=0;i<size;i++){

		MPI_Scatter(arr[i], 1, MPI_INT, &recv_beta[i], 1, MPI_INT, 0, MPI_COMM_WORLD);
	}

	MPI_Alltoall(recv_beta, 1, MPI_INT, recv_beta2, 1, MPI_INT, MPI_COMM_WORLD);
	for(int i=0;i<size;i++){

		MPI_Scan(&recv_beta2[i], &recv[i], 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
	}
	

		printf("Row %d: ", rank);

			for(int i=0;i<size;i++){

				printf("%d ", recv[i]);
			}

			printf("\n");	

	MPI_Finalize();
	return 0;
}