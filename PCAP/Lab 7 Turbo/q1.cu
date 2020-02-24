#include<stdio.h>

__global__ void add_variable(int *a, int *b, int *c){

	int id = blockDim.x * blockIdx.x + threadIdx.x;
	c[id] = a[id] + b[id];
}

__global__ void add_single_block(int *a, int *b, int *c){

	int id = threadIdx.x;
	c[id] = a[id] + b[id];
}

__global__ void add_single_thread(int *a, int *b, int *c){

	int id = blockDim.x * blockIdx.x;
	c[id] = a[id] + b[id];
}

int main(){

	int N;

	printf("Enter N: ");
	scanf("%d", &N);

	int vectorA[N], vectorB[N], vectorC[N];
	int *d_a, *d_b, *d_c;

	cudaMalloc((void**) &d_a, sizeof(int)*N);
	cudaMalloc((void**) &d_b, sizeof(int)*N);
	cudaMalloc((void**) &d_c, sizeof(int)*N);

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

	printf("Choose what to do:\n1. Create 1 Thread each Block.\n2. Create 1 Block Only.\n3. Create DynamicTM.\nYour Choice: ");

	int choice = 1;

	scanf("%d", &choice);

	if(choice == 1)
		add_single_thread<<<N,1>>>(d_a, d_b, d_c);

	else if(choice == 2)
		add_single_block<<<1,N>>>(d_a, d_b, d_c);
	
	else
		add_variable<<<ceil(N/256.0),256>>>(d_a, d_b, d_c);

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