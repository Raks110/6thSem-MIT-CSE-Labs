#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int j=0;
char strings[20][20];
int func(char str[])
{
	for(int i=0;i<j;i++)
		if(strcmp(str,strings[i])==0)
			return 0;
	return 1;
}
int main()
{
	char c,buf[10];
	int i,flag;
	FILE *fp = fopen("hi.c","r");
	FILE *fb = fopen("q1.txt","w");
	c = fgetc(fp);
	if(fp==NULL)
	{
		printf("Cannot open file\n");
		exit(0);
	}

	char str1[10];
	char str2[10];
	char str3[10];
	char str4[10];
	strcpy(str4,"void");
	strcpy(str1,"int");
	strcpy(str2,"float");
	strcpy(str3,"double");
	char str[100];
	int l=1,cl=1;
	while(c!=EOF)
	{
		flag=0;
		while(c=='\n')
		{
			c=fgetc(fp);
			if(c=='#')
			{
				while(c!='\n')
					c=fgetc(fp);
			}
			l++;
			cl=1;
		}
		if(c=='/')
		{
			c=fgetc(fp);
			if(c=='/')
			{
				while(c!='\n')
					c=fgetc(fp);
				l++;
			}
			else if(c=='*')
			{
				while(!(c=='*' && fgetc(fp)=='/'))
				{
					if(c=='\n')
						l++;
					c=fgetc(fp);
				}
			}
		}
		if(c=='(')
			fprintf(fb,"<(,%d,%d,LB>\n",cl,l);
		else if(c==')')
			fprintf(fb,"<),%d,%d,RB>\n",cl,l);
		else if(c==';')
			fprintf(fb,"<;,%d,%d,SS>\n",cl,l);
		else if(c=='{')
			fprintf(fb,"<{,%d,%d,LC>\n",cl,l);
		else if(c=='}')
			fprintf(fb,"<},%d,%d,RC>\n",cl,l);
		else if(c=='*')
			fprintf(fb,"<*,%d,%d,AO>\n",cl,l);
		else if(c=='/')
			fprintf(fb,"</,%d,%d,AO>\n",cl,l);
		else if(c==',')
			fprintf(fb,"<,,%d,%d,SS>\n",cl,l);
		else if(c==' ')
			cl++;
		else if(c=='"')
		{
			i=0;
			c=fgetc(fp);
			while(c!='"')
			{
				str[i++]=c;
				c=fgetc(fp);
			}
			str[i]='\0';
			fprintf(fb,"<\"%s\",%ld,%d,STRING LITERAL>\n",str,cl-strlen(str),l);
		}
		else if(c=='^')
			fprintf(fb,"<^,%d,%d,LO>",cl,l);

		if(c=='+')
		{
			i=0;
			buf[i++]=c;
			c=fgetc(fp);
			cl++;
			if(c=='+')
			{
				buf[i++]=c;
				buf[i]='\0';
				fprintf(fb,"<%s,%d,%d,AO>\n",buf,cl,l);
			}
			else
			{
				fprintf(fb, "<+,%d,%d,ASSIGNMENT OPERATOR>\n",cl,l);
			}
		}

		if(c=='&')
		{
			i=0;
			buf[i++]=c;
			c=fgetc(fp);
			cl++;
			if(c=='&')
			{
				buf[i++]=c;
				buf[i]='\0';
				fprintf(fb,"<%s,%d,%d,LO>\n",buf,cl,l);
			}
			else
			{
				fprintf(fb, "<&,%d,%d,LO>\n",cl,l);
			}
		}

		if(c=='|')
		{
			i=0;
			buf[i++]=c;
			c=fgetc(fp);
			cl++;
			if(c=='|')
			{
				buf[i++]=c;
				buf[i]='\0';
				fprintf(fb,"<%s,%d,%d,AO>\n",buf,cl,l);
			}
			else
			{
				fprintf(fb, "<|,%d,%d,ASSIGNMENT OPERATOR>\n",cl,l);
			}
		}

		if(c=='-')
		{
			i=0;
			buf[i++]=c;
			c=fgetc(fp);
			cl++;
			if(c=='-')
			{
				buf[i++]=c;
				buf[i]='\0';
				fprintf(fb,"<%s,%d,%d,AO>\n",buf,cl,l);
			}
			else
			{
				fprintf(fb, "<-,%d,%d,ASSIGNMENT OPERATOR>\n",cl,l);
			}
		}


		
		if(c=='=')
		{
			i=0;
			buf[i++]=c;
			c = fgetc(fp);
			cl++;
			if(c=='=')
			{
				buf[i++]=c;
				buf[i]='\0';
				fprintf(fb,"<==,%d,%d,RO>\n",cl,l);
			}
			else
			{
				buf[i]='\0';
				fprintf(fb,"<=,%d,%d,ASSIGNMENT OPERATOR>\n",cl,l);
			}
		}
		else
		{
			i=0;
			if(c=='<'||c=='>'||c=='!')
			{
				buf[i++]=c;
				c = fgetc(fp);
				cl++;
				if(c=='=')
				{
					buf[i++]=c;
				}
				buf[i]='\0';
				fprintf(fb,"<%s,%d,%d,LB>\n",buf,cl,l);
			}
			else
			{
				buf[i]='\0';
			}
		}
		
		i=0;
		while(c>='0' && c<='9')
		{
			str[i++]=c;
			c=fgetc(fp);
			cl++;
			flag=1;
		}
		if(c=='.' && flag==1)
		{
		  str[i++]=c;
		  c=fgetc(fp);
		  while(c>='0' && c<='9')
		  { 
		  	str[i++]=c;
			c=fgetc(fp);
			cl++;
			flag=1;
		  }	
		}
		str[i]='\0';
		if(strlen(str))
			fprintf(fb,"<%s,%ld,%d,NUMERICAL CONSTANTS>\n",str,cl-strlen(str),l);

		i=0;
		while((c>='A'&& c<='Z') || (c>='a' && c<='z') || (c>='0' && c<='9')|| c=='_')
		{
			str[i++]=c;
			c=getc(fp);
			flag=1;
			cl++;
		}
		if(c==EOF)
			break;
		str[i]='\0';
		i=0;
		if(strcmp(str1,str)==0)
			fprintf(fb,"<int,%d,%d,KEYWORD>\n",cl-4,l);
		else if(strcmp(str2,str)==0)
			fprintf(fb,"<float,%d,%d,KEYWORD>\n",cl-6,l);
		else if(strcmp(str3,str)==0)
			fprintf(fb,"<double,%d,%d,KEYWORD>\n",cl-7,l);
		else if(strcmp(str4,str)==0)
			fprintf(fb,"<void,%d,%d,KEYWORD>\n",cl-5,l);
		else if(strlen(str)>0)
			if(func(str)==1)
			{
				strcpy(strings[j++],str);
				fprintf(fb,"<%s,%ld,%d,IDENTIFIER>\n",str,cl-strlen(str),l);
			}


		if(flag==0)
			c=fgetc(fp);
	}

	fclose(fp);
	fclose(fb);
	return 0;
}