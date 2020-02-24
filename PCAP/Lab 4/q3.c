#include<mpi.h>
#include<stdio.h>

int errs = 0, calls = 0;

void eh(MPI_Comm *comm, int* err, ...){

	if (*err != MPI_ERR_OTHER) {
        errs++;
        printf( "Unexpected error code\n" );fflush(stdout);
    }
    if (*comm != MPI_COMM_WORLD) {
        errs++;
        printf( "Unexpected communicator\n" );fflush(stdout);
    }
    calls++;
	printf("Error No. %d and Call No. %d.\n", errs, calls);
    return;
}

int main(){

	int rank, size;
	MPI_Errhandler newerr;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Comm_create_errhandler(eh, &newerr );
    MPI_Comm_set_errhandler(MPI_COMM_WORLD, newerr );
    MPI_Comm_call_errhandler(MPI_COMM_WORLD, rank );

	int arr[100][size];
	int key;

	if(!rank){

		printf("Enter %d elements\n", size*size);
		for(int i=0;i<size;i++){

			for(int j=0;j<size;j++){

				scanf("%d", &arr[i][j]);
			}
		}

		printf("Enter Search Key: ");
		scanf("%d", &key);
	}

	MPI_Bcast(&key, 1, MPI_INT, 0, MPI_COMM_WORLD);
	int recv[size];
	
	for(int i=0;i<size;i++){

		MPI_Scatter(arr[i], 1, MPI_INT, &recv[i], 1, MPI_INT, 0, MPI_COMM_WORLD);
	}

	int res = 0;
	for(int i=0;i<size;i++){

		if(recv[i] == key){
			res++;
		}
	}

	int fin;
	MPI_Reduce(&res, &fin, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if(!rank){

		printf("Count: %d\n", fin);
	}

	MPI_Finalize();
	return 0;
}