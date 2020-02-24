__kernel void vector_to_octal(__global int *A, __global int *B){

	int i = get_global_id(0);
	int n = A[i];

	int octalNum[100];
    int j=0;

    while (n != 0) { 
  
        octalNum[j] = (n % 8);
        j++;
        n = n / 8;
    } 

    int sec = 0;
    int k = 0;
    
    while(k!=j){

        int pow = 1;

        for(int i=0;i<k;i++){
            pow *= 10;
        }

    	sec = (sec) + (octalNum[k] * pow);
        k++;
    }

    B[i] = sec;
}