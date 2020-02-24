#include <mpi.h>
#include <stdio.h>
#include <string.h>

void toggle(char* buff, int count){

	for(int i=0;i<count;i++){

		if(buff[i] >= 'a' && buff[i]<='z'){
			buff[i] -= 32;
		}
		else if(buff[i] >= 'A' && buff[i] <= 'Z'){
			buff[i] += 32;
		}
	}
}

int main(int argc, char** argv){

	int size, rank, count = 13;
	char buffer[100];
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if(size != 2){
		MPI_Abort(MPI_COMM_WORLD, 1);
	}
	else{

		if(rank == 0){

			printf("Enter String: ");
			scanf("%s", buffer);

			MPI_Ssend(buffer, strlen(buffer), MPI_BYTE, 1, 10, MPI_COMM_WORLD);
			MPI_Recv(buffer, count, MPI_BYTE, 1, 11, MPI_COMM_WORLD, &status);

			printf("Final String: ");
			fprintf(stdout, "%s\n", buffer);
		}
		else{

			MPI_Recv(buffer, count, MPI_BYTE, 0, 10, MPI_COMM_WORLD, &status);
			toggle(buffer, count);
			MPI_Ssend(buffer, count, MPI_BYTE, 0, 11, MPI_COMM_WORLD);
		}
	}

	MPI_Finalize();
	return 0;
}