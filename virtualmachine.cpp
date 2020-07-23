#include<stdio.h>
#include<string.h>
#define MAXCHARPERLINE 100
FILE *fin,*fout;
char buffer[80];
char filename[20];
char firstcomponent[100];
char secondcomponent[100];
int index=0;
int sp=256;
int gtc=0,eqc=0,ltc=0,ifgotoc=0,retc=0;
void add()
{
	fputs("@SP\nA=M-1\nD=M\nA=A-1\nM=M+D\n@SP\nM=M-1\n",fout);
	sp--;
	return;
}
void sub()
{
	fputs("@SP\nA=M-1\nD=M\nA=A-1\nM=M-D\n@SP\nM=M-1\n",fout);
	sp--;
	return;
}
void neg()
{
	fputs("@SP\nA=M-1\nM=-M\n",fout);
	return;
}
void eq()
{
	fprintf(fout,"@SP\nA=M-1\nD=M\nA=A-1\nD=D-M\n@EQUALEQ%d\nD;JEQ\n@SP\nA=M-1\nA=A-1\nM=0\n@SP\nM=M-1\n@ENDEQ%d\n0;JMP\n(EQUALEQ%d)\n@SP\nA=M-1\nA=A-1\nM=-1\n@SP\nM=M-1\n@ENDEQ%d\n0;JMP\n(ENDEQ%d)\n",eqc,eqc,eqc,eqc,eqc);
	sp--;
	eqc++;
	return;
}
void gt()
{
	fprintf(fout,"@SP\nA=M-1\nD=M\nA=A-1\nD=D-M\n@GT%d\nD;JLT\n@SP\nA=M-1\nA=A-1\nM=0\n@SP\nM=M-1\n@ENDGT%d\n0;JMP\n(GT%d)\n@SP\nA=M-1\nA=A-1\nM=-1\n@SP\nM=M-1\n@ENDGT%d\n0;JMP\n(ENDGT%d)\n",gtc,gtc,gtc,gtc,gtc);
	sp--;
	gtc++;
	return;
}
void or1()
{
	fputs("@SP\nA=M-1\nD=M\nA=A-1\nM=M|D\n@SP\nM=M-1\n",fout);
	sp--;
	return;
}
void lt()
{
	fprintf(fout,"@SP\nA=M-1\nD=M\nA=A-1\nD=D-M\n@LT%d\nD;JGT\n@SP\nA=M-1\nA=A-1\nM=0\n@SP\nM=M-1\n@ENDLT%d\n0;JMP\n(LT%d)\n@SP\nA=M-1\nA=A-1\nM=-1\n@SP\nM=M-1\n@ENDLT%d\n0;JMP\n(ENDLT%d)\n",ltc,ltc,ltc,ltc,ltc);
	sp--;
	ltc++;
	return;
}
void and1()
{
	fputs("@SP\nA=M-1\nD=M\nA=A-1\nM=M&D\n@SP\nM=M-1\n",fout);
	sp--;
	return;
}
void not1()
{
	fputs("@SP\nA=M-1\nM=!M\n",fout);
	return;
}
void push()
{
	int i,space,j;
	for(i=0;i<strlen(buffer);i++)
	{
		if(buffer[i]==' ')
		{
			space=i;
			break;
		}
	}
	for(i=space+1,j=0;i<strlen(buffer);i++,j++)
	{
		if(buffer[i]==' ')
		{
			secondcomponent[j]='\0';
			space=i;
			break;
		}
		secondcomponent[j]=buffer[i];
	}
	secondcomponent[j]='\0';
	for(i=space+1;i<strlen(buffer);i++)
	{
		if(buffer[i]=='\0')
		break;
		index=index*10+(int)buffer[i]-48;
	}
	if(strcmp(secondcomponent,"constant")==0)
	{
		fprintf(fout,"@%d\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",index);
		sp++;
		return;
	}
	if(strcmp(secondcomponent,"local")==0)
	{
		fprintf(fout,"@%d\nD=A\n@LCL\nA=M\nA=A+D\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",index);
		sp++;
		return;
	}
	if(strcmp(secondcomponent,"argument")==0)
	{
		fprintf(fout,"@%d\nD=A\n@ARG\nA=M\nA=A+D\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",index);
		sp++;
		return;
	}
	if(strcmp(secondcomponent,"this")==0)
	{
		fprintf(fout,"@%d\nD=A\n@THIS\nA=M\nA=A+D\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",index);
		sp++;
		return;
	}
	if(strcmp(secondcomponent,"that")==0)
	{
		fprintf(fout,"@%d\nD=A\n@THAT\nA=M\nA=A+D\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",index);
		sp++;
		return;
	}
	if(strcmp(secondcomponent,"temp")==0)
	{
		fprintf(fout,"@%d\nD=A\n@5\nD=D+A\nA=D\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",index);
		sp++;
		return;
	}
	if(strcmp(secondcomponent,"pointer")==0)
	{
		if(index==0)
		{
			fprintf(fout,"@THIS\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");
		}
		if(index==1)
		{
			fprintf(fout,"@THAT\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");
		}
		sp++;
		return;
	}
	if(strcmp(secondcomponent,"static")==0)
	{
		/*fprintf(fout,"@16\nD=A\n@%d\nD=D+A\nA=D\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",index);*/
		fprintf(fout,"@");
		fprintf(fout,filename);
		fprintf(fout,"%d\n",index);
		fprintf(fout,"D=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");
		sp++;
		return;
	}
}
void pop()
{
		int i,space,j;
	for(i=0;i<strlen(buffer);i++)
	{
		if(buffer[i]==' ')
		{
			space=i;
			break;
		}
	}
	for(i=space+1,j=0;i<strlen(buffer);i++,j++)
	{
		if(buffer[i]==' ')
		{
			secondcomponent[j]='\0';
			space=i;
			break;
		}
		secondcomponent[j]=buffer[i];
	}
	secondcomponent[j]='\0';
	for(i=space+1;j<strlen(buffer);i++)
	{
		if(buffer[i]=='\0')
		break;
		index=index*10+(int)buffer[i]-48;
	}
	if(strcmp(secondcomponent,"local")==0)
	{
		fprintf(fout,"@SP\nA=M\nA=A-1\nD=M\n@13\nM=D\n@LCL\nD=M\n@%d\nD=D+A\n@13\nA=M\nA=A+D\nD=A-D\nA=A-D\nM=D\n@SP\nM=M-1\n",index);
		sp--;
		return;
	}
	if(strcmp(secondcomponent,"argument")==0)
	{
		fprintf(fout,"@SP\nA=M\nA=A-1\nD=M\n@13\nM=D\n@ARG\nD=M\n@%d\nD=D+A\n@13\nA=M\nA=A+D\nD=A-D\nA=A-D\nM=D\n@SP\nM=M-1\n",index);
		sp--;
		return;
	}
	if(strcmp(secondcomponent,"this")==0)
	{
		fprintf(fout,"@SP\nA=M\nA=A-1\nD=M\n@13\nM=D\n@THIS\nD=M\n@%d\nD=D+A\n@13\nA=M\nA=A+D\nD=A-D\nA=A-D\nM=D\n@SP\nM=M-1\n",index);
		sp--;
		return;
	}
	if(strcmp(secondcomponent,"that")==0)
	{
		fprintf(fout,"@SP\nA=M\nA=A-1\nD=M\n@13\nM=D\n@THAT\nD=M\n@%d\nD=D+A\n@13\nA=M\nA=A+D\nD=A-D\nA=A-D\nM=D\n@SP\nM=M-1\n",index);
		sp--;
		return;
	}
	if(strcmp(secondcomponent,"temp")==0)
	{
		fprintf(fout,"@SP\nA=M\nA=A-1\nD=M\n@13\nM=D\n@5\nD=A\n@%d\nD=D+A\n@13\nA=M\nA=A+D\nD=A-D\nA=A-D\nM=D\n@SP\nM=M-1\n",index);
		sp--;
		return;
	}
	if(strcmp(secondcomponent,"pointer")==0)
	{
		if(index==0)
		{
			fprintf(fout,"@SP\nA=M\nA=A-1\nD=M\n@THIS\nM=D\n@SP\nM=M-1\n");
		}
		if(index==1)
		{
			fprintf(fout,"@SP\nA=M\nA=A-1\nD=M\n@THAT\nM=D\n@SP\nM=M-1\n");
		}
		sp--;
		return;
	}
	if(strcmp(secondcomponent,"static")==0)
	{
		/*fprintf(fout,"@SP\nA=M\nA=A-1\nD=M\n@13\nM=D\n@16\nD=A\n@%d\nD=D+A\n@13\nA=M\nA=A+D\nD=A-D\nA=A-D\nM=D\n@SP\nM=M-1\n",index);*/
		fprintf(fout,"@SP\nA=M\nA=A-1\nD=M\n@13\nM=D\n@");
		fprintf(fout,filename);
		fprintf(fout,"%d\n",index);
		fprintf(fout,"D=A\n");
		fprintf(fout,"@13\nA=M\nA=A+D\nD=A-D\nA=A-D\nM=D\n@SP\nM=M-1\n",index);
		sp--;
		return;
	}
}
void label()
{
	int i,space,j;
	for(i=0;i<strlen(buffer);i++)
	{
		if(buffer[i]==' ')
		{
			space=i;
			break;
		}
	}
	for(i=space+1,j=0;i<strlen(buffer);i++,j++)
	{
		if(buffer[i]==' ')
		{
			secondcomponent[j]='\0';
			space=i;
			break;
		}
		secondcomponent[j]=buffer[i];
	}
	secondcomponent[j]='\0';
	fprintf(fout,"(");
	fprintf(fout,secondcomponent);
	fprintf(fout,")\n");
	return;
}
void goto1()
{
	int i,space,j;
	for(i=0;i<strlen(buffer);i++)
	{
		if(buffer[i]==' ')
		{
			space=i;
			break;
		}
	}
	for(i=space+1,j=0;i<strlen(buffer);i++,j++)
	{
		if(buffer[i]==' ' || buffer[i]=='\0')
		{
			secondcomponent[j]='\0';
			space=i;
			break;
		}
		secondcomponent[j]=buffer[i];
	}
	secondcomponent[j]='\0';
	fprintf(fout,"@");
	fprintf(fout,secondcomponent);
	fprintf(fout,"\n0;JMP\n");
}
void ifgoto()
{
	int i,space,j;
	for(i=0;i<strlen(buffer);i++)
	{
		if(buffer[i]==' ')
		{
			space=i;
			break;
		}
	}
	for(i=space+1,j=0;i<strlen(buffer);i++,j++)
	{
		if(buffer[i]==' ')
		{
			secondcomponent[j]='\0';
			space=i;
			break;
		}
		secondcomponent[j]=buffer[i];
	}
	secondcomponent[j]='\0';
	fprintf(fout,"@SP\nAM=M-1\nD=M\n@END%d\nD;JEQ\n@",ifgotoc);
	fprintf(fout,secondcomponent);
	fprintf(fout,"\n0;JMP\n(END%d)\n",ifgotoc);
	ifgotoc++;
}
void call()
{
	int i,space,j;
	for(i=0;i<strlen(buffer);i++)
	{
		if(buffer[i]==' ')
		{
			space=i;
			break;
		}
	}
	for(i=space+1,j=0;i<strlen(buffer);i++,j++)
	{
		if(buffer[i]==' ')
		{
			secondcomponent[j]='\0';
			space=i;
			break;
		}
		secondcomponent[j]=buffer[i];
	}
	secondcomponent[j]='\0';
	for(i=space+1;j<strlen(buffer);i++)
	{
		if(buffer[i]=='\0')
		break;
		index=index*10+(int)buffer[i]-48;
	}
	//pushing return address
	fprintf(fout,"@returnaddress%d\n",retc);
	fprintf(fout,"D=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");
	//pushing local
	fprintf(fout,"@LCL\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");
	//pushing argument
	fprintf(fout,"@ARG\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");
	//pushing this
	fprintf(fout,"@THIS\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");
	//pushing that
	fprintf(fout,"@THAT\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");
	//set new argument
	fprintf(fout,"@SP\nD=M\n@5\nD=D-A\n@%d\nD=D-A\n@ARG\nM=D\n",index);
	//set new local
	fprintf(fout,"@SP\nD=M\n@LCL\nM=D\n");
	//goto function
	fprintf(fout,"@");
	fprintf(fout,secondcomponent);
	fprintf(fout,"\n0;JMP\n");
	//return here
	fprintf(fout,"(returnaddress%d)\n",retc); 
	retc++;
	return;
}
void function()
{
	int i,space,j;
	for(i=0;i<strlen(buffer);i++)
	{
		if(buffer[i]==' ')
		{
			space=i;
			break;
		}
	}
	for(i=space+1,j=0;i<strlen(buffer);i++,j++)
	{
		if(buffer[i]==' ')
		{
			secondcomponent[j]='\0';
			space=i;
			break;
		}
		secondcomponent[j]=buffer[i];
	}
	secondcomponent[j]='\0';
	for(i=space+1;j<strlen(buffer);i++)
	{
		if(buffer[i]=='\0')
		break;
		index=index*10+(int)buffer[i]-48;
	}
	fprintf(fout,"(");
	fprintf(fout,secondcomponent);
	fprintf(fout,")\n");
	for(i=0;i<index;i++)
	{
		fprintf(fout,"@SP\nA=M\nM=0\n@SP\nM=M+1\n");
	}
	return;
}
void return1()
{
	//frame=lcl
	fprintf(fout,"@LCL\nD=M\n@R13\nM=D\n");
	//save return address in R14
	fprintf(fout,"@R13\nD=M\n@5\nD=D-A\nA=D\nD=M\n@R14\nM=D\n");
	//save return value at top of the stack
	fprintf(fout,"@SP\nM=M-1\nA=M\nD=M\n@ARG\nA=M\nM=D\n");
	//shift sp to arg+1
	fprintf(fout,"@ARG\nD=M+1\n@SP\nM=D\n");
	//retrive that
	fprintf(fout,"@R13\nM=M-1\nA=M\nD=M\n@THAT\nM=D\n");
	//retrieve this
	fprintf(fout,"@R13\nM=M-1\nA=M\nD=M\n@THIS\nM=D\n");
	//retrieve arg
	fprintf(fout,"@R13\nM=M-1\nA=M\nD=M\n@ARG\nM=D\n");
	//retreive lcl
	fprintf(fout,"@R13\nM=M-1\nA=M\nD=M\n@LCL\nM=D\n");
	//goto return address
	fprintf(fout,"@R14\nA=M\n0;JMP\n");
	return;
}
int main()
{
	int i;
	fin=fopen("vmfilein.vm","r");
    fout=fopen("vmfileout.asm","w");
	firstcomponent[0]='\0';
	secondcomponent[0]='\0';
	index=0;	
	//getting line by line
	while(fgets(buffer,MAXCHARPERLINE,fin)!=NULL)
	{
		//removing comments and newlines characters in the end
		for(i=0;i<strlen(buffer);i++)
		{
			if(buffer[i]=='\n')
			{
			buffer[i]='\0';
			break;
			}
			if(buffer[i]=='/' && buffer[i+1]=='/')
			{
			buffer[i]='\0';
			break;
			}
		}
		//remove white space at the end
		for(i=strlen(buffer)-1;i>=0;i--)
		{
			if(buffer[i]==' ' || buffer[i]=='\t' || buffer[i]=='\n')
			{
				buffer[i]='\0';
			}
			else
			break;
		}
		if(buffer[0]=='\0')
		continue;
		//identifying the type of command
		for(i=0;i<=strlen(buffer);i++)
		{
			if(buffer[i]==' ' || buffer[i]=='\0')
			{
				firstcomponent[i]='\0';
				break;
			}
			firstcomponent[i]=buffer[i];
		}
		if(strcmp(firstcomponent,"add")==0)
		add();
		else if(strcmp(firstcomponent,"sub")==0)
		sub();
		else if(strcmp(firstcomponent,"neg")==0)
		neg();
		else if(strcmp(firstcomponent,"eq")==0)
		eq();
		else if(strcmp(firstcomponent,"gt")==0)
		gt();
		else if(strcmp(firstcomponent,"lt")==0)
		lt();
		else if(strcmp(firstcomponent,"and")==0)
		and1();
		else if(strcmp(firstcomponent,"or")==0)
		or1();
		else if(strcmp(firstcomponent,"not")==0)
		not1();
		else if(strcmp(firstcomponent,"push")==0)
		push();
		else if(strcmp(firstcomponent,"pop")==0)
		pop();
		else if(strcmp(firstcomponent,"label")==0)
		label();
		else if(strcmp(firstcomponent,"goto")==0)
		goto1();
		else if(strcmp(firstcomponent,"if-goto")==0)
		ifgoto();
		else if(strcmp(firstcomponent,"call")==0)
		call();
		else if(strcmp(firstcomponent,"function")==0)
		function();
		else if(strcmp(firstcomponent,"return")==0)
		return1();
		buffer[0]='\0';
		firstcomponent[0]='\0';
		secondcomponent[0]='\0';
		index=0;
		//fprintf(fout,"\n");
	}
	return 0;
}
