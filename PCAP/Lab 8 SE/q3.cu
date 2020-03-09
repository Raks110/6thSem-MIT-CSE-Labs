#include <stdio.h>
#include <stdlib.h>
#include <string.h>

__global__ void word_count(char *A, char *B, int *I, int *d_s, int spaces)
{
	int id = blockIdx.x * blockDim.x + threadIdx.x;
	if(id < spaces)
	{
		int flag = 1;
		int start = I[id] + 1;
		int end = I[id + 1];
		for(int i=0; i < end-start; i++)
			if(A[start + i] != B[i])
			{
				flag = 0;
				break;
			}
		if(flag == 1)
			atomicAdd(d_s, 1);
	}
}

int main()
{
	int i, size1, size2, size3, k = 0, spaces = 0;
	char *d_a, *d_b;
	int *d_i;
	int *search = 0, *d_s, *result;
	char *A = "PEOPLE NEED A NEW KING AND A NEW KING NEEDS A NEW CROWN";
	char *B = "NEW";
	int len = strlen(A);
	int len2 = strlen(B);

	for(i=0;i<len;i++)
		if(A[i] == ' ')
			spaces++;
	spaces++;

	int *I = (int *)calloc(spaces+1, sizeof(int));
	I[k++] = -1;

	for(i=0;i<len;i++)
		if(A[i] == ' ')
			I[k++] = i;
	I[k++] = len;

	size1 = len * sizeof(char);
	size2 = (spaces + 1) * sizeof(int);
	size3 = len2 * sizeof(char);

	cudaMalloc((void **)&d_a, size1);
	cudaMalloc((void **)&d_b, size3);
	cudaMalloc((void **)&d_i, size2);
	cudaMalloc((void **)&d_s, sizeof(unsigned int));

	cudaMemcpy(d_a, A, size1, cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, B, size3, cudaMemcpyHostToDevice);
	cudaMemcpy(d_i, I, size2, cudaMemcpyHostToDevice);
	cudaMemcpy(d_s, search, sizeof(unsigned int), cudaMemcpyHostToDevice);

	word_count<<<ceil(spaces/256.0), 256>>>(d_a, d_b, d_i, d_s, spaces);

	cudaMemcpy(result, d_s, sizeof(unsigned int), cudaMemcpyDeviceToHost);

	printf("String : \n%s\n",A);
	printf("Total occurances of NEW :%d\n",result[0]);

	cudaFree(d_a);
	cudaFree(d_i);
	cudaFree(d_s);
	cudaFree(d_b);

	free(A);
	free(I);
	free(B);

	return 0;
}