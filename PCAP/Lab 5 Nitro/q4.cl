__kernel void vector_swap(__global int *A){

	int i = get_global_id(0);
	
	int temp = A[2*i];
	A[2*i] = A[2*i + 1];
	A[2*i + 1] = temp;
}