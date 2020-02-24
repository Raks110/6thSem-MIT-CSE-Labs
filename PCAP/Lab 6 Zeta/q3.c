#include <stdio.h>
#include <CL/cl.h>
#include <stdlib.h>
#define MAX_SOURCE_SIZE (0x100000)
void main()
{
	int n;
	printf("Enter n:\n");
	scanf("%d",&n);
	int *a=(int*)malloc(n*sizeof(int));

	for(int i=0;i<n;i++)
	{
		scanf("%d",&a[i]);
	}

	FILE *f1;
	char *srcstr1;
	size_t srcsize1;
	f1=fopen("q3.cl","r");
	if(!f1)
	{
		fprintf(stderr,"Falied to load kernel\n");
		exit(1);
	}

	srcstr1=(char*)malloc(MAX_SOURCE_SIZE);
	srcsize1=fread(srcstr1,1,MAX_SOURCE_SIZE,f1);
	fclose(f1);

	cl_platform_id platform_id=NULL;
	cl_device_id device_id=NULL;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;

	cl_int ret=clGetPlatformIDs(1,&platform_id,&ret_num_platforms);
	ret=clGetDeviceIDs(platform_id,CL_DEVICE_TYPE_GPU,1,&device_id,&ret_num_devices);

	cl_context context=clCreateContext(NULL,1,&device_id,NULL,NULL,&ret);

	cl_command_queue command_queue=clCreateCommandQueue(context,device_id,NULL,&ret);

	cl_mem a_mem=clCreateBuffer(context,CL_MEM_READ_ONLY,sizeof(int)*n,NULL,&ret);		

	ret=clEnqueueWriteBuffer(command_queue,a_mem,CL_TRUE,0,sizeof(int)*n,a,0,NULL,NULL);

	cl_program prgm1=clCreateProgramWithSource(context,1,(const char**)&srcstr1,(const size_t*)&srcsize1,&ret);

	ret=clBuildProgram(prgm1,1,&device_id,NULL,NULL,NULL);
	ret=clBuildProgram(prgm1,1,&device_id,NULL,NULL,NULL);

	cl_kernel kernelodd=clCreateKernel(prgm1,"oddsort",&ret);
	cl_kernel kerneleven=clCreateKernel(prgm1,"evensort",&ret);

	ret=clSetKernelArg(kernelodd,0,sizeof(cl_mem),(void*)&a_mem);

	ret=clSetKernelArg(kerneleven,0,sizeof(cl_mem),(void*)&a_mem);

	size_t global_item_size=n;
	size_t local_item_size=1;

	cl_event event;
	for(int i=0;i<n/2;i++)
	{	
	ret=clEnqueueNDRangeKernel(command_queue,kernelodd,1,NULL,&global_item_size,&local_item_size,0,NULL,NULL);
	ret=clEnqueueNDRangeKernel(command_queue,kerneleven,1,NULL,&global_item_size,&local_item_size,0,NULL,NULL);
	ret=clFinish(command_queue);
	ret=clEnqueueReadBuffer(command_queue,a_mem,CL_TRUE,0,sizeof(int)*n,a,0,NULL,NULL);
	}

	for(int i=0;i<n;i++)
	{
		printf("%d ",a[i]);

	}

	ret=clFlush(command_queue);
	ret=clReleaseKernel(kernelodd);
	ret=clReleaseKernel(kerneleven);
	ret=clReleaseProgram(prgm1);
	ret=clReleaseMemObject(a_mem);
	ret=clReleaseCommandQueue(command_queue);
	ret=clReleaseContext(context);

	free(a);
	getchar();
}