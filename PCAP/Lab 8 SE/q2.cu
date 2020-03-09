#include <stdio.h>
#include <stdlib.h>
#include <string.h>

__global__ void reverse_each_word(char *A, int *I, char *B, int spaces)
{
	int id = blockIdx.x * blockDim.x + threadIdx.x;
	if(id < spaces)
	{
		int start = I[id] + 1;
		int end = I[id + 1];
		for(int i=0; i < (start + end)/2; i++)
		{
			B[end + start - 1] = A[i];
			B[i] = A[end + start - 1];
		}
	}
}

int main()
{
	int i, size1, size2, k = 0, spaces = 0;
	char *d_a, *d_b;
	int *d_i;
	char *A = "SIHT SI YM WEN GNINNIGEB";

	int len = strlen(A);
	char *B = (char *)malloc(sizeof(char) * len);

	for(i=0;i<len;i++)
		if(A[i] == ' ')
			spaces++;
	spaces++;

	int *I = (int *)malloc(sizeof(int) * (spaces+1));
	I[k++] = -1;

	for(i=0;i<len;i++)
		if(A[i] == ' ')
			I[k++] = i;
	I[k++] = len;

	size1 = len * sizeof(char);
	size2 = (spaces + 1) * sizeof(int);

	cudaMalloc((void **)&d_a, size1);
	cudaMalloc((void **)&d_i, size2);
	cudaMalloc((void **)&d_b, size1);

	cudaMemcpy(d_a, A, size1, cudaMemcpyHostToDevice);
	cudaMemcpy(d_i, I, size2, cudaMemcpyHostToDevice);

	reverse_each_word<<<ceil(spaces/256.0), 256>>>(d_a, d_i, d_b, spaces);

	cudaMemcpy(B, d_b, size1, cudaMemcpyDeviceToHost);

	printf("Original String : \n%s\n",A);
	printf("Desired Output :\n%s\n",B);

	cudaFree(d_a);
	cudaFree(d_i);
	cudaFree(d_b);

	free(B);
	free(I);

	return 0;
}