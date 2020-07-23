#include<iostream>
#include<cstring>
using namespace std;
#define MAXCHARPERLINE 100
FILE *fin,*fout,*ferr;
char buffer[MAXCHARPERLINE];
char comp1[50];
char comp2[50];
char comp3[50];
int gpu=0,ggpu=0;
void compileExpression(int);
void compileStatements(int);
void split()
{
	for(int it=0;it<50;it++)
	{
		comp1[it]='\0';
		comp2[it]='\0';
		comp3[it]='\0';
	}
	int i,j,k;
	for(i=0;i<strlen(buffer);i++)
	{
		if(buffer[i]==' ')
		break;
		comp1[i]=buffer[i];
	}
	comp1[i]='\0';
	i++;
	j=i;
	for(;i<strlen(buffer);i++)
	{
		if(buffer[i]==' ')
		break;
		comp2[i-j]=buffer[i];
	}
	comp2[i]='\0';
	i++;
	k=i;
	for(;i<strlen(buffer);i++)
	{
		if(buffer[i]==' ' || buffer[i]=='\n')
		break;
		comp3[i-k]=buffer[i];
	}
	comp3[i]='\0';
	return;
}
void printSpace(int space)
{
	for(int i=0;i<space;i++)
	{
		fprintf(fout," ");
	}
	return;
}
void checkIdentifier(int space)
{
	fgets(buffer,MAXCHARPERLINE,fin);
	split();
	if(strcmp(comp1,"<identifier>")!=0)
	{
		fputs("Error,expected identifier\n",ferr);
	}
	printSpace(space+2);
	fputs(buffer,fout);
	return;
}
void checkopencurlybrace(int space)
{
	fgets(buffer,MAXCHARPERLINE,fin);
	split();
	if(strcmp(comp2,"{")!=0)
	{
		fputs("Error,expected {\n",ferr);
	}
	printSpace(space+2);
	fputs(buffer,fout);
	return;
}
void checkopencircularbrace(int space)
{
	if(ggpu==0)
	{
	fgets(buffer,MAXCHARPERLINE,fin);
	}
	ggpu=0;
	split();
	if(strcmp(comp2,"(")!=0)
	{
		cout<<comp2;
		fputs("Error,expected (\n",ferr);
	}
	printSpace(space+2);
	fputs(buffer,fout);
	return;
}
void checkType(int space)
{
	fgets(buffer,MAXCHARPERLINE,fin);
	split();
	if(strcmp(comp1,"<identifier>")==0 || strcmp(comp2,"int")==0 || strcmp(comp2,"char")==0 || strcmp(comp2,"boolean")==0 || strcmp("void",comp2)==0)
	{
		printSpace(space+2);
		fputs(buffer,fout);
		return;
	}
	fputs("error,invald type\n",ferr);
}
void compileClassVarDec(int space)
{
	printSpace(space);
	fputs("<classVarDec>\n",fout);
	printSpace(space+2);
	fputs(buffer,fout);
	checkType(space);
	B:
	checkIdentifier(space);
	fgets(buffer,MAXCHARPERLINE,fin);
	split();
	if(strcmp(comp2,",")==0)
	{
		printSpace(space+2);
		fputs(buffer,fout);
		goto B;
	}
	if(strcmp(comp2,";")==0)
	{
		printSpace(space+2);
		fputs(buffer,fout);
		printSpace(space);
		fputs("</classVarDec>\n",fout);
		return;
	}
	fputs("error, ; expected\n",ferr);
}
void compileVarDec(int space)
{
	printSpace(space);
	fputs("<varDec>\n",fout);
	printSpace(space+2);
	fputs(buffer,fout);
	checkType(space);
	F:
	checkIdentifier(space);
	fgets(buffer,MAXCHARPERLINE,fin);
	split();
	if(strcmp(comp2,",")==0)
	{
		printSpace(space+2);
		fputs(buffer,fout);
		goto F;
	}
	if(strcmp(comp2,";")==0)
	{
		printSpace(space+2);
		fputs(buffer,fout);
		printSpace(space);
		fputs("</varDec>\n",fout);
		return;
	}
	fputs("error, ; expected\n",ferr);
}
void compileExpressionList(int space)
{
	fgets(buffer,MAXCHARPERLINE,fin);
	split();
	int i=0;
	while(true)
	{
		if(strcmp(comp2,")")==0)
		{
			if(i==0){
			printSpace(space);
			fputs("<expressionList>",fout);
			}
			printSpace(space);
			fputs("</expressionList>\n",fout);
			return;
		}
		if(i==0){
		printSpace(space);
		fputs("<expressionList>\n",fout);
		}
		gpu=1;
		compileExpression(space+2);
		split();
		if(strcmp(comp2,",")==0)
		{
			printSpace(space+2);
			fputs(buffer,fout);
			fgets(buffer,MAXCHARPERLINE,fin);
			split();
		}
		i++;
	}
	return;
}
void compileTerm(int space)
{
	printSpace(space);
	fputs("<term>\n",fout);
	if(gpu==0)
	fgets(buffer,MAXCHARPERLINE,fin);
	gpu=0;
	split();
	if(strcmp(comp1,"<integerConstant>")==0 || strcmp(comp1,"<stringConstant>")==0 || strcmp(comp2,"true")==0 || strcmp(comp2,"false")==0 || strcmp(comp2,"null")==0)
	{
		printSpace(space+2);
		fputs(buffer,fout);
		printSpace(space);
		fputs("</term>\n",fout);
		fgets(buffer,MAXCHARPERLINE,fin);
		return;
	}
	else if(strcmp(comp2,"-")==0 || strcmp(comp2,"~")==0)
	{
		printSpace(space+2);
		fputs(buffer,fout);
		compileTerm(space+2);
		printSpace(space);
		fputs("</term>\n",fout);
		return;
	}
	else if(strcmp(comp2,"(")==0)
	{
		printSpace(space+2);
		fputs(buffer,fout);
		compileExpression(space+2);
		printSpace(space+2);
		fputs(buffer,fout);
		printSpace(space);
		fputs("</term>\n",fout);
		fgets(buffer,MAXCHARPERLINE,fin);
		return;
	}
	else if(strcmp(comp1,"<identifier>")==0 || strcmp(comp2,"this")==0)
	{
		printSpace(space+2);
		fputs(buffer,fout);
		fgets(buffer,MAXCHARPERLINE,fin);
		split();
		if(strcmp(comp2,".")==0)
		{
			printSpace(space+2);
			fputs(buffer,fout);
			checkIdentifier(space);
			checkopencircularbrace(space);
			compileExpressionList(space+2);
			printSpace(space+2);
			fputs(buffer,fout);
			fgets(buffer,MAXCHARPERLINE,fin);
		}
		if(strcmp(comp2,"[")==0)
		{
			printSpace(space+2);
			fputs(buffer,fout);
			compileExpression(space+2);
			printSpace(space+2);
			fputs(buffer,fout);
			fgets(buffer,MAXCHARPERLINE,fin);
		}
		printSpace(space);
		fputs("</term>\n",fout);
	}
	return;
}
void compileExpression(int space)
{
	if(strcmp(comp2,")")==0)
	return;
	printSpace(space);
	fputs("<expression>\n",fout);
	compileTerm(space+2);
	split();
	if(strcmp(comp2,"+")==0 || strcmp(comp2,"-")==0 || strcmp(comp2,"*")==0 || strcmp(comp2,"/")==0 || strcmp(comp2,"&")==0 || strcmp(comp2,"|")==0 || strcmp(comp2,"<")==0 || strcmp(comp2,">")==0 || strcmp(comp2,"=")==0)
	{
		K:
		printSpace(space+2);
		fputs(buffer,fout);
		compileTerm(space+2);
		split();
		if(strcmp(comp2,"+")==0 || strcmp(comp2,"-")==0 || strcmp(comp2,"*")==0 || strcmp(comp2,"/")==0 || strcmp(comp2,"&")==0 || strcmp(comp2,"|")==0 || strcmp(comp2,"<")==0 || strcmp(comp2,">")==0 || strcmp(comp2,"=")==0)
		{
			goto K;
		}
	}
	printSpace(space);
	fputs("</expression>\n",fout);
	return;
}
void compileLetStatement(int space)
{
	printSpace(space);
	fputs("<letStatement>\n",fout);
	printSpace(space+2);
	fputs(buffer,fout);
	checkIdentifier(space);
	fgets(buffer,MAXCHARPERLINE,fin);
	split();
	if(strcmp(comp2,"[")==0)
	{
		printSpace(space+2);
		fputs(buffer,fout);
		compileExpression(space+2);
		printSpace(space+2);
		fputs(buffer,fout);
		fgets(buffer,MAXCHARPERLINE,fin);
		split();
	}
	if(strcmp(comp2,"=")!=0)
	{
		cout<<"error = expected";
	}
	printSpace(space+2);
	fputs(buffer,fout);
	compileExpression(space+2);
	printSpace(space+2);
	fputs(buffer,fout);
	printSpace(space);
	fputs("</letStatement>\n",fout);
	return;
}
void compileDoStatement(int space)
{
	printSpace(space);
	fputs("<doStatement>\n",fout);
	printSpace(space+2);
	fputs(buffer,fout);
	checkIdentifier(space);
	fgets(buffer,MAXCHARPERLINE,fin);
	split();
	if(strcmp(comp2,".")==0)
	{
		printSpace(space+2);
		fputs(buffer,fout);
		checkIdentifier(space);
		fgets(buffer,MAXCHARPERLINE,fin);
		split();
	}
	ggpu=1;
	checkopencircularbrace(space);
	compileExpressionList(space+2);
	printSpace(space+2);
	fputs(buffer,fout);
	fgets(buffer,MAXCHARPERLINE,fin);
	printSpace(space+2);
	fputs(buffer,fout);
	printSpace(space);
	fputs("</doStatement>\n",fout);
	return;
}
void compileWhileStatement(int space)
{
	printSpace(space);
	fputs("<whileStatement>\n",fout);
	printSpace(space+2);
	fputs(buffer,fout);
	checkopencircularbrace(space);
	compileExpression(space+2);
	printSpace(space+2);
	fputs(buffer,fout);
	checkopencurlybrace(space);
	fgets(buffer,MAXCHARPERLINE,fin);
	split();
	compileStatements(space+2);
	printSpace(space);
	fputs("</whileStatement>\n",fout);
	return;
}
void compileReturnStatement(int space)
{
	printSpace(space);
	fputs("<returnStatement>\n",fout);
	printSpace(space+2);
	fputs(buffer,fout);
	fgets(buffer,MAXCHARPERLINE,fin);
	split();
	if(strcmp(comp2,";")!=0)
	{
		gpu=1;
		compileExpression(space+2);
	}
	printSpace(space+2);
	fputs(buffer,fout);
	printSpace(space);
	fputs("</returnStatement>\n",fout);
	return;
}
void compileIfStatement(int space)
{
	printSpace(space);
	fputs("<ifStatement>\n",fout);
	printSpace(space+2);
	fputs(buffer,fout);
	checkopencircularbrace(space);
	compileExpression(space+2);
	printSpace(space+2);
	fputs(buffer,fout);
	checkopencurlybrace(space);
	fgets(buffer,MAXCHARPERLINE,fin);
	split();
	compileStatements(space+2);
	fgets(buffer,MAXCHARPERLINE,fin);
	split();
	if(strcmp(comp2,"else")!=0)
	{
		printSpace(space);
		fputs("</ifStatement>\n",fout);
		return;
	}
	printSpace(space+2);
	fputs(buffer,fout);
	checkopencurlybrace(space);
	fgets(buffer,MAXCHARPERLINE,fin);
	split();
	compileStatements(space+2);
	fgets(buffer,MAXCHARPERLINE,fin);
	printSpace(space);
	fputs("</ifStatement>\n",fout);
	return;
}
void compileStatements(int space)
{
	if(strcmp(comp2,"let")==0 || strcmp(comp2,"do")==0 || strcmp(comp2,"while")==0 || strcmp(comp2,"if")==0 || strcmp(comp2,"return")==0)
	{
		printSpace(space);
		fputs("<statements>\n",fout);
		G:
		if(strcmp(comp2,"let")==0)
		{
		compileLetStatement(space+2);
		fgets(buffer,MAXCHARPERLINE,fin);
		}
		else if(strcmp(comp2,"do")==0)
		{
		compileDoStatement(space+2);
		fgets(buffer,MAXCHARPERLINE,fin);
		}	
		else if(strcmp(comp2,"while")==0)
		{
		compileWhileStatement(space+2);
		fgets(buffer,MAXCHARPERLINE,fin);
		}
		else if(strcmp(comp2,"if")==0)
		compileIfStatement(space);
		else if(strcmp(comp2,"return")==0)
		{
		compileReturnStatement(space+2);
		fgets(buffer,MAXCHARPERLINE,fin);
		}
		split();
		if(strcmp(comp2,"let")==0 || strcmp(comp2,"do")==0 || strcmp(comp2,"while")==0 || strcmp(comp2,"if")==0 || strcmp(comp2,"return")==0)
		goto G;
		printSpace(space);
		fputs("</statements>\n",fout);
	}
	if(strcmp(comp2,"}")!=0)
	{
		fputs("error, } expected\n",ferr);
	}
	printSpace(space);
	fputs(buffer,fout);
	return;
}
void compileSubroutineBody(int space)
{
	printSpace(space);
	fputs("<subroutineBody>\n",fout);
	checkopencurlybrace(space);
	fgets(buffer,MAXCHARPERLINE,fin);
	split();
	if(strcmp(comp2,"var")==0)
	{
		E:
		compileVarDec(space+2);
		fgets(buffer,MAXCHARPERLINE,fin);
		split();
		if(strcmp(comp2,"var")==0)
		goto E;
	}
	compileStatements(space+2);
	printSpace(space);
	fputs("</subroutineBody>\n",fout);
	return;
}
void compileSubroutineDec(int space)
{
	int qpu=0;
	printSpace(space);
	fputs("<subroutineDec>\n",fout);
	printSpace(space+2);
	fputs(buffer,fout);
	checkType(space);
	checkIdentifier(space);
	checkopencircularbrace(space);
	fgets(buffer,MAXCHARPERLINE,fin);
	split();
	if(strcmp(comp1,"<identifier>")==0 || strcmp(comp2,"int")==0 || strcmp(comp2,"char")==0 || strcmp(comp2,"boolean")==0)
	{
		qpu=1;
		printSpace(space+2);
		fputs("<parameterList>\n",fout);
		D:
		printSpace(space+4);
		fputs(buffer,fout);
		checkIdentifier(space+2);
		fgets(buffer,MAXCHARPERLINE,fin);
		split();
		if(strcmp(comp2,",")==0)
		{
			printSpace(space+4);
			fputs(buffer,fout);
			fgets(buffer,MAXCHARPERLINE,fin);
			goto D;
		}
		if(strcmp(comp2,")")!=0)
		{
			cout<<"error,) expected\n";
		}
		printSpace(space+2);
		fputs("</parameterList>\n",fout);
		printSpace(space+2);
		fputs(buffer,fout);
	}
	else
	{
		if(qpu==0)
		{
			printSpace(space+2);
			fputs("<parameterList>  </parameterList>\n",fout);
		}
		printSpace(space+2);
		fputs(buffer,fout);
	}
	compileSubroutineBody(space+2);
	printSpace(space);
	fputs("</subroutineDec>\n",fout);
	return;
}
void compileClass(int space)
{
	printSpace(space+2);
	fputs(buffer,fout);
	checkIdentifier(space);
	checkopencurlybrace(space);
	fgets(buffer,MAXCHARPERLINE,fin);
	split();
	if(strcmp(comp2,"field")==0 || strcmp(comp2,"static")==0)
	{
		C:
		compileClassVarDec(space+2);
		fgets(buffer,MAXCHARPERLINE,fin);
		split();
		if(strcmp(comp2,"field")==0 || strcmp(comp2,"static")==0)
		goto C;
	}
	if(strcmp(comp2,"function")==0 || strcmp(comp2,"constructor")==0 || strcmp(comp2,"method")==0)
	{
		H:
		compileSubroutineDec(space+2);
		fgets(buffer,MAXCHARPERLINE,fin);
		split();
		if(strcmp(comp2,"function")==0 || strcmp(comp2,"constructor")==0 || strcmp(comp2,"method")==0)
		{
		goto H;
		}
	}
	if(strcmp(comp2,"}")!=0)
	{
		fputs("error,expected }\n",ferr);
	}
	printSpace(space+2);
	fputs(buffer,fout);
	return;
}
int main(int argc,char** argv)
{
	int no_files=2;
	while(no_files!=argc)
	{
	int it;
	char str[50];
	for(it=0;it<strlen(argv[no_files]);it++)
	{
		if(argv[no_files][it]=='.')
		break;
		str[it]=argv[no_files][it];
	}
	str[it]='.';
	str[it+1]='a';
	str[it+2]='n';
	str[it+3]='z';
	str[it+4]='\0';
	fout=fopen(str,"w");
	str[it+1]='t';
	str[it+2]='o';
	str[it+3]='k';
	str[it+4]='\0';
	fin=fopen(str,"r");
	str[it+1]='e';
	str[it+2]='r';
	str[it+3]='r';
	str[it+4]='\0';
	ferr=fopen(str,"w");
	no_files++;
	fgets(buffer,MAXCHARPERLINE,fin);
	if(strcmp(buffer,"<tokens>\n")!=0)
	{
		fputs("Error,wrong file inserted\n",ferr);
		return 1;
	}
	fgets(buffer,MAXCHARPERLINE,fin);
	split();
	if(strcmp("class",comp2)!=0)
	{
		fputs("Error,class keyword expected\n",ferr);
		return 1;
	}
	fputs("<class>\n",fout);
	compileClass(0);
	fputs("</class>\n",fout);
	}
}
