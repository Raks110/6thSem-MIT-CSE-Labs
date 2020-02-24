__kernel void str_chgcase(__global int * A, __global int * B) {
	//Get the index of the current work item
	int i = get_global_id(0);
	if (A[i] >= 'A' && A[i] <= 'Z')
		B[i] = A[i] + 32;
	else
		B[i] = A[i] - 32;	
}