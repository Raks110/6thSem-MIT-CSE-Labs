#include <stdio.h>
#include <stdlib.h>
#include <string.h>

__global__ void reverse_each_word(char *A, char *B, int len)
{
	int id = blockIdx.x * blockDim.x + threadIdx.x;
	if(id < len)
		B[id] = A[len - id - 1];
}

int main()
{
	int i, size;
	char *A = "GNINNIGEB WEN YM SI SIHT";

	int len = strlen(A);
	char *B = (char *)malloc(sizeof(char) * len);

	char *d_a, *d_b;

	size = len * sizeof(char);

	cudaMalloc((void **)&d_a, size);
	cudaMalloc((void **)&d_b, size);

	cudaMemcpy(d_a, A, size, cudaMemcpyHostToDevice);

	reverse_each_word<<<ceil(len/256.0), 256>>>(d_a, d_b, len);

	cudaMemcpy(B, d_b, size, cudaMemcpyDeviceToHost);

	printf("Original String : \n%s\n",A);
	printf("Desired Output :\n%s\n",B);

	cudaFree(d_a);
	cudaFree(d_b);

	free(B);

	return 0;
}