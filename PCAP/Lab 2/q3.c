#include <stdio.h>
#include <mpi.h>

#define MCW MPI_COMM_WORLD

int main(int argc, char** argv){

	int size, rank;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MCW, &size);
	MPI_Comm_rank(MCW, &rank);

	int x[100];

	MPI_Buffer_attach(&x, 100);

	if(rank == 0){

		printf("Enter Numbers\n");
		int a[size-1];
		
		for(int i=0;i<size-1;i++){
			scanf("%d", &a[i]);
		}

		for(int i=1;i<size;i++)
			MPI_Bsend(&a[i-1], 1, MPI_INT, i, i, MCW);
	}
	else{

		int p;

		MPI_Recv(&p, 1, MPI_INT, 0, rank, MCW, &status);
		if(rank%2 == 0){
			printf("Process: %d, Processed to: %d\n", rank, p*p);
		}
		else{
			printf("Process: %d, Processed to: %d\n", rank, p*p*p);	
		}
	}

	MPI_Finalize();
	return 0;
}