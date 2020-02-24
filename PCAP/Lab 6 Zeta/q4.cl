__kernel void reverse(__global char *A,__global char *B,__global int *l)
{
	int k=0,s=0,j=0,len;
	int i = get_global_id(0);

	for(int j=0;j<i;j++)
	{
		s=s+l[j]+1;
	}
	
	int len=l[i];
	
	for(int k=0;k<len;k++)
	{
		B[s+len-k-1]=A[s+k];
	}
	
	B[s+len]=' ';
}