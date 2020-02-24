#include<stdio.h>

__global__ void linear_op(int *a, int *b, int *c, int *scalar){

	int id = blockDim.x * blockIdx.x + threadIdx.x;
	c[id] = (*scalar * a[id]) + b[id];
}

int main(){

	int N, alpha;

	printf("Enter ⍺: ");
	scanf("%d", &alpha);

	printf("Enter N: ");
	scanf("%d", &N);

	int vectorA[N], vectorB[N], vectorC[N];
	int *d_a, *d_b, *d_c, *d_d;

	cudaMalloc((void**) &d_a, sizeof(int)*N);
	cudaMalloc((void**) &d_b, sizeof(int)*N);
	cudaMalloc((void**) &d_c, sizeof(int)*N);
	cudaMalloc((void**) &d_d, sizeof(int));

	printf("Enter N Elements for A:\n");
	
	for(int i=0;i<N;i++){
		scanf("%d", &vectorA[i]);
	}

	printf("Enter N Elements for B:\n");
	for(int i=0;i<N;i++){
		scanf("%d", &vectorB[i]);
	}

	cudaMemcpy(d_a, &vectorA, sizeof(int)*N, cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, &vectorB, sizeof(int)*N, cudaMemcpyHostToDevice);
	cudaMemcpy(d_d, &alpha, sizeof(int), cudaMemcpyHostToDevice);

	linear_op<<<ceil(N/512.0), 512>>>(d_a, d_b, d_c, d_d);

	cudaMemcpy(&vectorC, d_c, sizeof(int)*N, cudaMemcpyDeviceToHost);
	
	for(int i=0;i<N;i++){
		printf("%d ", vectorC[i]);
	}
	
	printf("\n");

	cudaFree(d_a);
	cudaFree(d_b);
	cudaFree(d_c);

	return 0;
}