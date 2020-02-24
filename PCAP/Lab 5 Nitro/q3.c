#include<stdio.h>
#include<CL/cl.h>
#include<stdlib.h>

#define MAX_SOURCE_SIZE 0x100000

int power(int base, int power){

	int num = 1;

	while(power--){
		num = num*base;
	}

	return num;
}

int decToBinary(int n) 
{ 
    
    int binary = 0; 
    int i=0;

    while (n > 0) { 
  
  		int num = power(10, i) * (n % 2);
        binary = binary + num;
        n = n / 2; 
        i++;
    }

    return binary;
} 

int main(){

	//Create the two input vectors

	int i;
	int LIST_SIZE;

	printf("%s: ", "Enter List Size: ");
	scanf("%d", &LIST_SIZE);

	// Initialize the decimal vector

	int *A = (int*)malloc(sizeof(int) * LIST_SIZE);

	for(int i=0;i<LIST_SIZE;i++){

		A[i] = decToBinary(i);
	}

	// Load the kernel source code into the array source_str

	FILE *fp;
	char *source_str;
	size_t source_size;

	fp = fopen("q3.cl", "r");

	if(!fp){

		fprintf(stderr, "Failed to load the Kernelos Bestos.\n");
		getchar();
		exit(1);
	}

	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);

	fclose(fp);

	// Get platform and device information

	cl_platform_id platform_id = NULL;
	cl_device_id device_id = NULL;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;

	cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);

	cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
	cl_command_queue command_queue = clCreateCommandQueue(context, device_id, NULL, &ret);

	cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, LIST_SIZE * sizeof(int), NULL, &ret);
	cl_mem b_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, LIST_SIZE * sizeof(int), NULL, &ret);

	ret = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0, LIST_SIZE * sizeof(int), A, 0, NULL, NULL);

	cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t*) &source_size, &ret);

	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	
	// Create OpenCL Kernel Object
	cl_kernel kernel = clCreateKernel(program, "vector_complement", &ret);

	// Set the arguments of the Kernel
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*) &a_mem_obj);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*) &b_mem_obj);

	// Execute the OpenCL Kernel on the array
	size_t global_item_size = LIST_SIZE;
	size_t local_item_size = 1;

	// Execute the Kernel on the device
	cl_event event;
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);
	ret = clFinish(command_queue);

	int *B = (int*) malloc(sizeof(int) * LIST_SIZE);
	ret = clEnqueueReadBuffer(command_queue, b_mem_obj, CL_TRUE, 0, LIST_SIZE * sizeof(int), B, 0, NULL, NULL);

	for(int i=0;i<LIST_SIZE;i++){
		printf("%d -> %d\n", A[i], B[i]);
	}

	//Clean Up
	ret = clFlush(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(a_mem_obj);
	ret = clReleaseMemObject(b_mem_obj);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);

	free(A);
	free(B);
	getchar();

	return 0;
}