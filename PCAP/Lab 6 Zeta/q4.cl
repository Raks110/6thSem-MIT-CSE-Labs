__kernel void reverse(__global char *A,__global char *B,__global int *l)
{
	int i = get_global_id(0);
	int s=0;

	int len=l[i];
	

	for(int j=0;j<i;j++)
	{
		s=s+l[j]+1;
	}
	
	for(int k=0;k<len;k++)
	{
		B[s+len-k-1]=A[s+k];
	}
	
	B[s+len]=' ';
}
