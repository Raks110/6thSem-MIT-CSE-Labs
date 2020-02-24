#include<stdio.h>
#include<mpi.h>

#include<stdio.h>
#include<mpi.h>

int main(){

	int size, rank, len=1;
	char recv[100];

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if(!rank){
		printf("Enter Length:\n");
		scanf("%d",&len);
	}

	MPI_Bcast(&len, 1, MPI_INT, 0, MPI_COMM_WORLD);	
	char str[len];

	if(!rank){

		for(int i=0;i<len;i++){
			scanf("%c",&str[i]);
		}
	}

	MPI_Scatter(str, len/size, MPI_CHAR, recv, len/size, MPI_CHAR, 0, MPI_COMM_WORLD);

	int num_con = 0;
	int arr[size];

	for(int i=0;i<len/size;i++){

		char temp = recv[i];

		if(temp!='a' && temp!='e' && temp!='i' && temp!='o' && temp!='u'){
			num_con++;
		}
	}

	MPI_Gather(&num_con, 1, MPI_INT, arr, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if(!rank){
		int sum = 0;

		for(int i=0;i<size;i++){
			sum += arr[i];
			printf("Process %d: %d\n", i, arr[i]);
		}

		printf("Total: %d\n", sum);
	}
	
	MPI_Finalize();
	return 0;
}