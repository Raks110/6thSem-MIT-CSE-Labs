#include<stdio.h>
#include<mpi.h>

#include<stdio.h>
#include<mpi.h>

int main(){

	int size, rank, len=1;
	char recv1[100], recv2[100];

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if(!rank){
		printf("Enter Length:\n");
		scanf("%d",&len);
	}

	MPI_Bcast(&len, 1, MPI_INT, 0, MPI_COMM_WORLD);	
	char str1[len], str2[len];

	if(!rank){

		printf("String 1:\n");
		
		for(int i=0;i<len;i++){
			scanf("%c",&str1[i]);
		}

		printf("String 2:\n");
char t;
		scanf("%c",&t);

		for(int i=0;i<len;i++){
			scanf("%c",&str2[i]);
		}
	}

	MPI_Scatter(str1, len/size, MPI_CHAR, recv1, len/size, MPI_CHAR, 0, MPI_COMM_WORLD);
	MPI_Scatter(str2, len/size, MPI_CHAR, recv2, len/size, MPI_CHAR, 0, MPI_COMM_WORLD);

	int num_con = 0;
	char arr[2*len];

	char res[200];

	for(int i=0;i<len/size;i++){

		res[2*i] = recv1[i];
		res[2*i+1] = recv2[i];
	}

	MPI_Gather(res, 2*len/size, MPI_CHAR, arr, 2*len/size, MPI_CHAR, 0, MPI_COMM_WORLD);

	if(!rank){
		printf("Final String: ");

		for(int i=0;i<2*len;i++){
			printf("%c",arr[i]);
		}
		printf("\n");
	}
	
	MPI_Finalize();
	return 0;
}