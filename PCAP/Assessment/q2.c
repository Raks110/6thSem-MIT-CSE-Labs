#include<mpi.h>
#include<stdlib.h>
#include<stdio.h>

#define siz 100;

int main(){

	int size, rank;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if(size == 4){

		int mat[4][4];
		char str[17];

		if(rank == 0){

			printf("Enter string:\n");
			scanf("%s", str);

			printf("\n");

			str[16] = '\0';

			/*printf("Enter matrix:\n");
			for(int i=0; i<size;i++){

				for(int j=0; j<size;j++){
					
					scanf("%d", &mat[i][j]);
				}
			}*/

			mat[0][0] = 1;
			mat[0][1] = 2;
			mat[0][2] = 3;
			mat[0][3] = -2;
			mat[1][0] = 2;
			mat[1][1] = 3;
			mat[1][2] = 1;
			mat[1][3] = 4;
			mat[2][0] = 2;
			mat[2][1] = 1;
			mat[2][2] = 3;
			mat[2][3] = 2;
			mat[3][0] = 2;
			mat[3][1] = 4;
			mat[3][2] = 3;
			mat[3][3] = 5;
		}

		char sub[5]; sub[5] = '\0';
		MPI_Scatter(str, 4, MPI_CHAR, sub, 4, MPI_CHAR, 0, MPI_COMM_WORLD);

		int row[4];
		MPI_Scatter(mat, 4, MPI_INT, row, 4, MPI_INT, 0,MPI_COMM_WORLD);

		int col[4];
		MPI_Alltoall(row, 1, MPI_INT, col, 1, MPI_INT, MPI_COMM_WORLD);

		int dig_sum = 0, min_col = col[0];
		for(int i=0;i<size;i++){
			if(sub[i] >= '0' && sub[i] <= '9'){

				dig_sum += (sub[i]-48);
			}
		}

		for(int i=0;i<size;i++){

			if(col[i] < min_col){
				min_col = col[i];
			}
		}

		printf("Digit Sum (Rank %d) is: %d.\n", rank, dig_sum);
		printf("Rank %d: Digit Sum (%d) + Minimum Column Sum (%d) = %d.\n\n", rank, dig_sum, min_col, dig_sum + min_col);
	}
	else
		if(rank == 0)
			printf("Please use 4 processes.\n");

	MPI_Finalize();
	return 0;
}

//Input: Ab56s231H342DE23