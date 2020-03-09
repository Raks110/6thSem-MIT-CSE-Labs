#include <stdio.h>
#include <stdlib.h>
#include <string.h>

__global__ void str_repeat(char *A, char *B, int len, int N)
{
	int id = blockIdx.x * blockDim.x + threadIdx.x;
	int i;
	if(id < N)
		for(i=0;i<len;i++)
			B[id*len + i] = A[i];
}

int main()
{
	char str[20];
	char *A, *B;
	int len, N, size1, size2;
	char *d_a, *d_b;

	printf("Enter a string : ");
	scanf("%s", str);

	len = strlen(str);
	A = (char *)malloc(sizeof(char) * len);
	strcpy(A, str);

	printf("Enter the repeat count : ");
	scanf("%d", &N);

	B = (char *)malloc(sizeof(char) * len * N);

	size1 = len * sizeof(char);
	size2 = N * len * sizeof(char);

	cudaMalloc((void **)&d_a, size1);
	cudaMalloc((void **)&d_b, size2);

	cudaMemcpy(d_a, A, size1, cudaMemcpyHostToDevice);

	str_repeat<<<ceil(N/256.0), 256>>>(d_a, d_b, len, N);

	cudaMemcpy(B, d_b, size2, cudaMemcpyDeviceToHost);

	printf("Output string :\n%s\n",B);

	cudaFree(d_a);
	cudaFree(d_b);

	free(A);
	free(B);

	return 0;
}