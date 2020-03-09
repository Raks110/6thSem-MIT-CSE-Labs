#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

__global__ void rowadd(int *a, int *b, int * t){
	int rows = blockDim.x, n=threadIdx.x;
	int c = gridDim.x; 
	for(int i=0;i<c;i++){
		t[n*c + i] = a[n*c + i] + b[n*c + i];
	}
}

__global__ void coladd(int *a, int *b, int * t){
	int col = blockDim.x, n=threadIdx.x;
	int r = gridDim.x;
	for(int i=0;i<r;i++){
		t[i*col + n] = a[i*col + n] + b[i*col + n];
	}
}

__global__ void eleadd(int *a, int *b, int * t){
	int n = threadIdx.x;
	t[n] = a[n]+b[n];
}

int main(void){
	int *a,*t,*b,m,n,i,j;
	int *d_a,*d_b,*d_t;

	printf("Enter the value of m : ");
	scanf("%d",&m);
	printf("Enter the value of n : ");
	scanf("%d",&n);

	int size = sizeof(int) * m * n;

	a = (int*)malloc(m*n*sizeof(int));
	b = (int*)malloc(m*n*sizeof(int));
	t = (int*)malloc(m*n*sizeof(int));

	printf("Enter input A matrix : \n");
	for(i=0;i<m*n;i++){
		scanf("%d",&a[i]);
	}

	printf("Enter input B matrix : \n");
	for(i=0;i<m*n;i++){
		scanf("%d",&b[i]);
	}

	cudaMalloc((void**)&d_a,size);
	cudaMalloc((void**)&d_b,size);
	cudaMalloc((void**)&d_t,size);
	
	cudaMemcpy(d_a,a,size,cudaMemcpyHostToDevice);
	cudaMemcpy(d_b,b,size,cudaMemcpyHostToDevice);

	rowadd<<<n,m>>>(d_a,d_b,d_t);
	cudaMemcpy(t,d_t,size,cudaMemcpyDeviceToHost);
	printf("Result vector is : \n");
	for(i=0;i<m;i++){
		for(j=0;j<n;j++){
			printf("%d\t",t[i*n+j]);
		}
		printf("\n");
	}

	coladd<<<m,n>>>(d_a,d_b,d_t);
	cudaMemcpy(t,d_t,size,cudaMemcpyDeviceToHost);
	printf("Result vector is : \n");
	for(i=0;i<m;i++){
		for(j=0;j<n;j++){
			printf("%d\t",t[j*m+i]);
		}
		printf("\n");
	}

	eleadd<<<1,m*n>>>(d_a,d_b,d_t);
	cudaMemcpy(t,d_t,size,cudaMemcpyDeviceToHost);
	printf("Result vector is : \n");
	for(i=0;i<m;i++){
		for(j=0;j<n;j++){
			printf("%d\t",t[i*n+j]);
		}
		printf("\n");
	}

	getchar();
	cudaFree(d_a);
	cudaFree(d_t);
	return 0;
}
