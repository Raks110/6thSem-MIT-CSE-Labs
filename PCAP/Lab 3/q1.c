#include<stdio.h>
#include<mpi.h>

int factorial(int n){

	int ret = 1;
	for(int i=1;i<=n;i++)
		ret *= i;

	return ret;
}

int main(){

	int size,rank;
	int recv;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int arr[size];

	if(!rank){

		printf("Enter %d numbers\n", size);

		for(int i=0;i<size;i++){
			scanf("%d",&arr[i]);
		}
	}

	MPI_Scatter(arr, 1, MPI_INT, &recv, 1, MPI_INT, 0, MPI_COMM_WORLD);
	int ret = factorial(recv);

	MPI_Gather(&ret, 1, MPI_INT, arr, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if(!rank){

		int sum = 0;

		for(int i=0;i<size;i++){

			sum += arr[i];
		}

		printf("Final Sum: %d\n", sum);
	}
	
	MPI_Finalize();
	return 0;
}