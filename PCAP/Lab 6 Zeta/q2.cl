__kernel void parallelSort(__global int * A, __global int * res) {
	
	int id = get_global_id(0);
	int n = get_global_size(0);
	
	int pos = 0;
	for (int i = 0; i < n; i++)
		if (A[i] < A[id] || (A[i] == A[id] && i < id))
			pos++;
	res[pos] = A[id];		
}
