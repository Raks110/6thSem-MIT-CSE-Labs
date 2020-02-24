#include<stdio.h>

__global__ void sine(float *a, float *b){

	int id = blockDim.x * blockIdx.x + threadIdx.x;
	b[id] = sin(a[id]);
}

int main(){

	int N;

	printf("Enter N: ");
	scanf("%d", &N);

	float vectorA[N], vectorB[N];
	float *d_a, *d_b;

	cudaMalloc((void**) &d_a, sizeof(float)*N);
	cudaMalloc((void**) &d_b, sizeof(float)*N);

	printf("Enter N Angles (in Radians):\n");
	
	for(int i=0;i<N;i++){
		scanf("%f", &vectorA[i]);
	}

	cudaMemcpy(d_a, &vectorA, sizeof(float)*N, cudaMemcpyHostToDevice);

	sine<<<ceil(N/512.0), 512>>>(d_a, d_b);

	cudaMemcpy(&vectorB, d_b, sizeof(float)*N, cudaMemcpyDeviceToHost);
	
	for(int i=0;i<N;i++){
		printf("sin(%.3f) = %.3f\n", vectorA[i], vectorB[i]);
	}
	
	printf("\n");

	cudaFree(d_a);
	cudaFree(d_b);

	return 0;
}