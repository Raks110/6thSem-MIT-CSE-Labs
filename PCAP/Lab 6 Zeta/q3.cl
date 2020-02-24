__kernel void oddsort(__global int *a)
{
	int id=get_global_id(0);
	int n=get_global_size(0);
	int temp;
	if(id%2!=0 && id+1!=n)
	{
		if(a[id]>a[id+1])
		{
				temp=a[id];
				a[id]=a[id+1];
				a[id+1]=temp;
	
		}
	}
}

__kernel void evensort(__global int *a)
{
	int id=get_global_id(0);
	int n=get_global_size(0);
	int temp;
	if(id%2==0 && id+1!=n)
	{
		if(a[id]>a[id+1])
		{
				temp=a[id];
				a[id]=a[id+1];
				a[id+1]=temp;
			
		}
	}
}