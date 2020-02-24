#include<stdio.h>
#include<CL/cl.h>
#include<stdlib.h>
#include<string.h>

#define MAX_SOURCE_SIZE 0x100000
#define DATA_SIZE (1024)

int main(){

	int i;

	char *data = "Hello";
	unsigned int count = strlen(data);

	int n = 10;

	printf("Enter N: ");
	scanf("%d",&n);

	char *results = (char *)malloc(count * n * sizeof(char));
	
	FILE *kernel_code_file = fopen("q1.cl", "r");
	if (kernel_code_file == NULL) {
		printf("Kernel loading failed.");
		exit(1);
	}
	
	char *source_str = (char *)malloc(DATA_SIZE);
	size_t source_size = fread(source_str, 1, DATA_SIZE, kernel_code_file);
	source_str[source_size] = '\0';
	
	fclose(kernel_code_file);

	// Get platform and device information

	cl_platform_id platform_id = NULL;
	cl_device_id device_id = NULL;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;

	cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);

	cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
	cl_command_queue command_queue = clCreateCommandQueue(context, device_id, NULL, &ret);

	cl_mem mem_input = clCreateBuffer(context,  CL_MEM_READ_ONLY,  sizeof(char) * count, NULL, NULL);
	cl_mem mem_output = clCreateBuffer(context,  CL_MEM_WRITE_ONLY,  sizeof(char) * count * n, NULL, NULL);

	ret = clEnqueueWriteBuffer(command_queue, mem_input, CL_TRUE, 0, sizeof(char) * count, data, 0, NULL, NULL);

	cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t*) &source_size, &ret);

	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	
	// Create OpenCL Kernel Object
	cl_kernel kernel = clCreateKernel(program, "multiply", &ret);

	// Set the arguments of the Kernel
	ret = 0;
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), &mem_input);
	ret = clSetKernelArg(kernel, 1, sizeof(int), &n);
	ret = clSetKernelArg(kernel, 2, sizeof(int), &count);
	ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), &mem_output);

	// Execute the OpenCL Kernel on the array
	size_t global_item_size = count;
	size_t local_item_size = 1;

	// Execute the Kernel on the device
	cl_event event;
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);
	ret = clFinish(command_queue);

	ret = clEnqueueReadBuffer(command_queue, mem_output, CL_TRUE, 0, sizeof(char) * count * n, results, 0, NULL, NULL );

	printf("Initial: %s\n", data);
	printf("Count: %d\n", n);	
	printf("Final String: %s\n", results);

	//Clean Up
	clReleaseMemObject(mem_input);
	clReleaseMemObject(mem_output);
	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(command_queue);

	return 0;
}
