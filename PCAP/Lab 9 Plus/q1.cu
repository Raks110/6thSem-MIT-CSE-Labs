#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

__global__ void alter(int *a, int * t){
	int n = threadIdx.x, m=blockIdx.x, size = blockDim.x;
	t[m*size + n] = 1;
	int d = m;
	while(d>=0){
		t[m*size+n]*=a[m*size+n];
		d--;
	}
}

int main(void){
	int *a,*t,m,n,i,j;
	int *d_a,*d_t;

	printf("Enter the value of m : ");
	scanf("%d",&m);
	printf("Enter the value of n : ");
	scanf("%d",&n);

	int size = sizeof(int) * m * n;

	a = (int*)malloc(m*n*sizeof(int));
	t = (int*)malloc(m*n*sizeof(int));
	printf("Enter input matrix : \n");
	for(i=0;i<m*n;i++){
		scanf("%d",&a[i]);
	}
	cudaMalloc((void**)&d_a,size);
	cudaMalloc((void**)&d_t,size);

	cudaMemcpy(d_a,a,size,cudaMemcpyHostToDevice);

	alter<<<m,n>>>(d_a,d_t);

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
