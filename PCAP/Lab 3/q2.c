#include<stdio.h>
#include<mpi.h>

int main(){

	int size, rank, m=1;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if(!rank){
		printf("Enter M:\n");
		scanf("%d",&m);
	}

	MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);

	int a[m*size];
	int recv[m];

	if(!rank){

		printf("Enter %d elements:\n", m*size);

		for(int i=0;i<m*size;i++){
			scanf("%d",&a[i]);
		}
	}

	MPI_Scatter(a, m, MPI_INT, recv, m, MPI_INT, 0, MPI_COMM_WORLD);
	int avg = 0;

	for(int i=0;i<m;i++){

		avg += recv[i];
	}

	avg /= m;

	int averages[size];

	MPI_Gather(&avg, 1, MPI_INT, averages, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if(!rank){

		int avg2 = 0;
		for(int i=0;i<size;i++){

			avg2 += averages[i];
		}

		avg2 /= size;

		printf("Final Average: %d\n", avg2);
	}

	MPI_Finalize();
	return 0;
}