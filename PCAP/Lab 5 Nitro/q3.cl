__kernel void vector_complement(__global int *A, __global int *B){
	
	int i = get_global_id(0);

	int bin = A[i];

	int binL[100];
	int j=0;

	while(bin!=0){

		binL[j] = ((bin%10) == 0);
		bin = bin/10;
		j++;
	}

	int k=0, sec=0;

	while(k!=j){

        int pow = 1;

        for(int i=0;i<k;i++){
            pow *= 10;
        }

    	sec = (sec) + (binL[k] * pow);
        k++;
    }

	B[i] = sec;
}