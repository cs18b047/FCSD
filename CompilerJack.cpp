#include<iostream>
#include<cstring>
using namespace std;
#define MAXCHAR 200
FILE *fin,*fout,*ferr;
char buffer[MAXCHAR];
char comp1[50];
char comp2[50];
char comp3[50]; 
char kind1[50],type1[50];
char currentClassName[50],currentSubroutineName[50],currentSubroutineType[50];
class SymbolTable
{
	public:
		int index;
		char type[50];
		char kind[50];
		char name[50];
};
SymbolTable classSymbolTable[50];
SymbolTable subroutineSymbolTable[50];
int field_count=0,static_count=0,class_total=0,sub_total=0,arg_count=0,local_count=0,index1,labelNum=0,x=0;
void CompileExpression();
void CompileStatements();
void IdentifierCheck();
int CompileExpressionList();
void msplit()
{
	for(int it=0;it<50;it++)
	{
		comp2[it]='\0';
	}
	int i,j,k;
	for(i=0;i<strlen(buffer);i++)
	{
		if(buffer[i-2]=='>' && buffer[i-1]==' ')
		{
			break;
		}
	}
	for(j=i;j<strlen(buffer);j++)
	{
		if(buffer[j+2]=='<' && buffer[j+1]==' ')
		{
			break;
		}
	}
	for(k=i;k<=j;k++)
	{
		comp2[k-i]=buffer[k];
	}
	comp2[k-i]='\0';
}
void split()
{
	int i;
	for(i=0;i<strlen(buffer);i++)
	{
		if(buffer[i]!=' ')
		break;
	}
	int n=i;
	for(;i<strlen(buffer);i++)
	{
		if(buffer[i]=='\n' || buffer[i]=='\0')
		{
			buffer[i-n]='\0';
			break;
		}
		buffer[i-n]=buffer[i];
	}
	for(int it=0;it<50;it++)
	{
		comp1[it]='\0';
		comp2[it]='\0';
		comp3[it]='\0';
	}
	int j,k;
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
void trim()
{
	int i;
	for(i=0;i<strlen(buffer);i++)
	{
		if(buffer[i]!=' ')
		break;
	}
	int n=i;
	for(;i<strlen(buffer);i++)
	{
		if(buffer[i]=='\n' || buffer[i]=='\0')
		{
			buffer[i-n]='\0';
			break;
		}
		buffer[i-n]=buffer[i];
	}
	return;
}
void CompileClassVarDec()                                       //upto ;
{
	char type1[50],kind1[50],name1[50];
	fgets(buffer,MAXCHAR,fin);                                    //kind
	split();
	strcpy(kind1,strcat(comp2,"\0"));
	fgets(buffer,MAXCHAR,fin);                                    //type
	split();
	strcpy(type1,strcat(comp2,"\0"));
	CLASSVARDEC:
	fgets(buffer,MAXCHAR,fin);                                    //name
	split();
	strcpy(classSymbolTable[class_total].name,strcat(comp2,"\0"));
	strcpy(classSymbolTable[class_total].kind,kind1);
	strcpy(classSymbolTable[class_total].type,type1);
	if(strcmp(kind1,"field")==0)
	{
		classSymbolTable[class_total].index=field_count;
		field_count++;
		class_total++;
	}
	else
	{
		classSymbolTable[class_total].index=static_count;
		static_count++;
		class_total++;
	}
	fgets(buffer,MAXCHAR,fin);                                    
	split();
	if(strcmp(comp2,",")==0)
	{
		goto CLASSVARDEC;
	}
	if(strcmp(comp2,";")!=0)
	{
		fputs("ERROR:; expected",ferr);
	}
	return;
}
void CompileParameterList()										//upto )
{
	PARAMETERLIST:
	fgets(buffer,MAXCHAR,fin);
	split();
	if(strcmp(comp2,")")==0)
	{
		return;
	}
	strcpy(subroutineSymbolTable[sub_total].type,strcat(comp2,"\0"));
	strcpy(subroutineSymbolTable[sub_total].kind,"argument\0");
	subroutineSymbolTable[sub_total].index=arg_count;
	fgets(buffer,MAXCHAR,fin);
	split();
	strcpy(subroutineSymbolTable[sub_total].name,strcat(comp2,"\0"));
	sub_total++;
	arg_count++;
	fgets(buffer,MAXCHAR,fin);
	trim();
	if(strcmp(buffer,"</parameterList>")==0)
	{
		fgets(buffer,MAXCHAR,fin);
		split();
		if(strcmp(comp2,")")==0)
		{
			return;
		}
	}	
	split();
	if(strcmp(comp2,",")==0)
	goto PARAMETERLIST;
}
void CompileVarDec()											//upto ;
{
	char type1[50];
	fgets(buffer,MAXCHAR,fin);
	split();
	if(strcmp(comp2,"var")!=0)
	{
		fputs("ERROR:expected var",ferr);
	}
	fgets(buffer,MAXCHAR,fin);
	split();
	strcpy(type1,strcat(comp2,"\0"));
	VARDEC:
	fgets(buffer,MAXCHAR,fin);
	split();
	strcpy(subroutineSymbolTable[sub_total].name,strcat(comp2,"\0"));
	subroutineSymbolTable[sub_total].index=local_count;
	strcpy(subroutineSymbolTable[sub_total].kind,"local\0");
	strcpy(subroutineSymbolTable[sub_total].type,type1);
	sub_total++;
	local_count++;
	fgets(buffer,MAXCHAR,fin);
	split();
	if(strcmp(comp2,",")==0)
	goto VARDEC;
	if(strcmp(comp2,";")!=0)
	fputs("ERROR:; expected",ferr);
	return;
}
void IdentifierCheck()											//operates on comp2,output on index1 and kind1
{												
	//if x==1 -> in subroutine,if x==2 -> class symbol table else x=0;
	x=0;
	split();
	for(int i=0;i<sub_total;i++)
	{
		if(strcmp(comp2,subroutineSymbolTable[i].name)==0)
		{
			x=1;
			index1=subroutineSymbolTable[i].index;
			strcpy(kind1,subroutineSymbolTable[i].kind);
			strcpy(type1,subroutineSymbolTable[i].type);
		}
	}
	if(x==0)
	{
		for(int i=0;i<class_total;i++)
		{
			if(strcmp(comp2,classSymbolTable[i].name)==0)
			{
				x=2;
				index1=classSymbolTable[i].index;
				strcpy(kind1,classSymbolTable[i].kind);
				strcpy(type1,classSymbolTable[i].type);
			}
		}
	}
	return;
}
void CompileLetStatement()										//upto ;
{					
	char kind11[50],type11[50],name11[50];
	int index11;				
	fgets(buffer,MAXCHAR,fin);
	split();
	if(strcmp(comp2,"let")!=0)
	{
		fputs("ERROR:let expected",ferr);
	}
	fgets(buffer,MAXCHAR,fin);
	split();
	if(strcmp(comp1,"<identifier>")!=0)
	{
		fputs("ERROR:identifier expected",ferr);
	}
	IdentifierCheck();
	strcpy(kind11,kind1);
	strcpy(type11,type1);
	strcpy(name11,strcat(comp2,"\0"));
	index11=index1;
	if(x==0)
	{
		fputs(name11,ferr);
		fputs(" not recognised\n",ferr);
	}
	//cout<<comp2<<index11<<endl;
	//for(int i=0;i<sub_total;i++)
	//cout<<subroutineSymbolTable[i].name<<subroutineSymbolTable[i].type<<subroutineSymbolTable[i].kind<<subroutineSymbolTable[i].index<<endl;
	fgets(buffer,MAXCHAR,fin);
	split();
	if(strcmp(comp2,"=")==0)
	{
		fgets(buffer,MAXCHAR,fin);
		trim();
		if(strcmp(buffer,"<expression>")!=0)
		fputs("ERROR:expression expected",ferr);
		CompileExpression();
		fputs("pop ",fout);
		if(strcmp(kind11,"field")==0)
		fputs("this",fout);
		else
		fputs(kind11,fout);
		fprintf(fout," %d\n",index11);
		fgets(buffer,MAXCHAR,fin);
		split();
		if(strcmp(comp2,";")!=0)
		fputs("ERROR:; expected",ferr);
		return;
	}
	if(strcmp(comp2,"[")!=0)
	fputs("ERROR:[ missing",ferr);
	fgets(buffer,MAXCHAR,fin);
	trim();
	CompileExpression();
	fgets(buffer,MAXCHAR,fin);
	split();
	if(strcmp(comp2,"]")!=0)
	fputs("ERROR:] expected",ferr);
	fputs("push ",fout);
	if(strcmp(kind11,"field")==0)
	fprintf(fout," %d\n",index11);
	fputs("add\n",fout);
	fgets(buffer,MAXCHAR,fin);
	split();
	fgets(buffer,MAXCHAR,fin);
	trim();
	if(strcmp(buffer,"<expression>")!=0)
	fputs("ERROR:expression expected",ferr);
	CompileExpression();
	fputs("pop temp 0\n",fout);
	fputs("pop pointer 1\n",fout);
	fputs("push temp 0\n",fout);
	fputs("pop that 0\n",fout);
	fgets(buffer,MAXCHAR,fin);
	split();
	if(strcmp(comp2,";")!=0)
	fputs("ERROR:; expected",ferr);
	return;
}
void CompileReturnStatement()									//upto ;
{
	fgets(buffer,MAXCHAR,fin);
	split();
	if(strcmp(comp2,"return")!=0)
	fputs("ERROR:return expected",ferr);
	fgets(buffer,MAXCHAR,fin);
	trim();
	if(strcmp(buffer,"<expression>")!=0)
	{
		fputs("push constant 0\n",fout);
		fputs("return\n",fout);
		split();
		if(strcmp(comp2,";")!=0)
		fputs("ERROR:; expected",ferr);
		return;
	}
	CompileExpression();
	fputs("return\n",fout);
	fgets(buffer,MAXCHAR,fin);
	split();
	if(strcmp(comp2,";")!=0)
	fputs("ERROR:; expected",ferr);
	return;
}
void CompileIfStatement()										//upto /ifStatement
{
	int tLabelNum=labelNum;
	labelNum+=2;
	fgets(buffer,MAXCHAR,fin);
	split();
	if(strcmp(comp2,"if")!=0)
	fputs("ERROR:if expected",ferr);
	fgets(buffer,MAXCHAR,fin);
	split();
	if(strcmp(comp2,"(")!=0)
	fputs("ERROR:( expected",ferr);
	fgets(buffer,MAXCHAR,fin);
	trim();
	if(strcmp(buffer,"<expression>")!=0)
	fputs("ERROR:expression expected",ferr);
	CompileExpression();
	fgets(buffer,MAXCHAR,fin);
	split();
	if(strcmp(comp2,")")!=0)
	fputs("ERROR:) expected",ferr);
	fgets(buffer,MAXCHAR,fin);
	split();
	if(strcmp(comp2,"{")!=0)
	fputs("ERROR:{ expected",ferr);
	fputs("not\n",fout);
	fputs("if-goto ",fout);
	fputs(currentClassName,fout);
	fprintf(fout,".%d\n",tLabelNum);
	fgets(buffer,MAXCHAR,fin);
	trim();
	if(strcmp(buffer,"<statements>")!=0)
	fputs("ERROR:statements expected",ferr);
	CompileStatements();
	fgets(buffer,MAXCHAR,fin);
	split();
	if(strcmp(comp2,"}")!=0)
	fputs("ERROR:} expected",ferr);
	fputs("goto ",fout);
	fputs(currentClassName,fout);
	fprintf(fout,".%d\n",tLabelNum+1);
	fputs("label ",fout);
	fputs(currentClassName,fout);
	fprintf(fout,".%d\n",tLabelNum);
	fgets(buffer,MAXCHAR,fin);
	split();
	if(strcmp(comp2,"else")==0)
	{
		fgets(buffer,MAXCHAR,fin);
		split();
		if(strcmp(comp2,"{")!=0)
		fputs("ERROR:{ expected",ferr);
		fgets(buffer,MAXCHAR,fin);
		trim();
		if(strcmp(buffer,"<statements>")!=0)
		fputs("ERROR:statements expected",ferr);
		CompileStatements();
		fgets(buffer,MAXCHAR,fin);
		split();
		if(strcmp(comp2,"}")!=0)
		fputs("ERROR:} expected",ferr);
		fgets(buffer,MAXCHAR,fin);
	}
	fputs("label ",fout);
	fputs(currentClassName,fout);
	fprintf(fout,".%d\n",tLabelNum+1);
	trim();
	if(strcmp(buffer,"</ifStatement>")!=0)
	cout<<"ERROR:expected /ifStatement";
	return;
}
void CompileWhileStatement()									//upto } (while close)
{
	fgets(buffer,MAXCHAR,fin);
	split();
	if(strcmp(comp2,"while")!=0)
	fputs("ERROR:while expected",ferr);
	fgets(buffer,MAXCHAR,fin);
	split();
	if(strcmp(comp2,"(")!=0)
	fputs("ERROR:( expected",ferr);
	fgets(buffer,MAXCHAR,fin);
	trim();
	if(strcmp(buffer,"<expression>")!=0)
	fputs("ERROR:expression expected",ferr);
	int tLabelNum=labelNum;
	labelNum+=2;
	fputs("label ",fout);
	fputs(currentClassName,fout);
	fprintf(fout,".%d\n",tLabelNum);
	CompileExpression();
	fgets(buffer,MAXCHAR,fin);
	split();
	if(strcmp(comp2,")")!=0)
	fputs("ERROR:) expected",ferr);
	fputs("not\n",fout);
	fputs("if-goto ",fout);
	fputs(currentClassName,fout);
	fprintf(fout,".%d\n",tLabelNum+1);
	fgets(buffer,MAXCHAR,fin);
	split();
	if(strcmp(comp2,"{")!=0)
	fputs("ERROR:{ expected",ferr);
	CompileStatements();
	fgets(buffer,MAXCHAR,fin);
	split();
	if(strcmp(comp2,"}")!=0)
	fputs("ERROR:} expected",ferr);
	fputs("goto ",fout);
	fputs(currentClassName,fout);
	fprintf(fout,".%d\n",tLabelNum);
	fputs("label ",fout);
	fputs(currentClassName,fout);
	fprintf(fout,".%d\n",tLabelNum+1);
	return;
}
void CompileTerm()		
{
	fgets(buffer,MAXCHAR,fin);
	split();
	if(strcmp(comp2,"~")==0 || strcmp(comp2,"-")==0)
	{
		char x;
		x=comp2[0];
		fgets(buffer,MAXCHAR,fin);
		trim();
		if(strcmp(buffer,"<term>")!=0)
		fputs("ERROR:Term expected",ferr);
		CompileTerm();
		if(x=='~')
		fputs("not\n",fout);
		if(x=='-')
		fputs("neg\n",fout);
		fgets(buffer,MAXCHAR,fin);
		trim();
		if(strcmp(buffer,"</term>")!=0)
		fputs("ERROR:No /term",ferr);
		return;
	}	
	if(strcmp(comp2,"(")==0)
	{
		fgets(buffer,MAXCHAR,fin);
		trim();
		if(strcmp(buffer,"<expression>")!=0)
		fputs("ERROR:<expression> expected",ferr);
		CompileExpression();
		fgets(buffer,MAXCHAR,fin);
		split();
		if(strcmp(comp2,")")!=0)
		fputs("ERROR:) expected",ferr);
		fgets(buffer,MAXCHAR,fin);
		trim();
		if(strcmp(buffer,"</term>")!=0)
		fputs("ERROR:No /term",ferr);
		return;
	}
	if(strcmp(comp1,"<integerConstant>")==0)
	{
		fputs("push constant ",fout);
		fputs(comp2,fout);
		fputs("\n",fout);
		fgets(buffer,MAXCHAR,fin);
		trim();
		if(strcmp(buffer,"</term>")!=0)
		fputs("ERROR:No /term",ferr);
		return;
	}
	if(strcmp(comp2,"true")==0 || strcmp(comp2,"false")==0 || strcmp(comp2,"null")==0 || strcmp(comp2,"this")==0)
	{
		if(strcmp(comp2,"true")==0)
		{
			fputs("push constant 0\n",fout);
			fputs("not\n",fout);
		}
		if(strcmp(comp2,"false")==0 || strcmp(comp2,"null")==0)
		{
			fputs("push constant 0\n",fout);
		}
		if(strcmp(comp2,"this")==0)
		{
			fputs("push pointer 0\n",fout);
		}
		fgets(buffer,MAXCHAR,fin);
		trim();
		if(strcmp(buffer,"</term>")!=0)
		fputs("ERROR:No /term",ferr);
		return;
	}
	if(strcmp(comp1,"<identifier>")==0)
	{
		char kind11[50],type11[50],name11[50];
		int index11,x11;	
		char id1[50];
		int np;
		IdentifierCheck();
		strcpy(kind11,kind1);
		strcpy(type11,type1);
		strcpy(name11,strcat(comp2,"\0"));
		index11=index1;
		x11=x;
		strcpy(id1,strcat(comp2,"\0"));
		fgets(buffer,MAXCHAR,fin);
		trim();
		if(strcmp(buffer,"</term>")==0)
		{
			if(x11==0)
			{
			fputs(name11,ferr);
			fputs(" not recognised\n",ferr);
			}
			fputs("push ",fout);
			if(strcmp(kind11,"field")==0) { fputs("this",fout); } else { fputs(kind11,fout); }
			fprintf(fout," %d\n",index11);
			return;
		}
		split();
		if(strcmp(comp2,"[")==0)
		{
			fgets(buffer,MAXCHAR,fin);
			CompileExpression();
			fgets(buffer,MAXCHAR,fin);
			split();
			if(strcmp(comp2,"]")!=0)
			fputs("ERROR:] expected",ferr);
			fputs("push ",fout);
			if(strcmp(kind11,"field")==0) { fputs("this",fout); } else { fputs(kind11,fout); }
			fprintf(fout," %d\n",index11);
			fputs("add\n",fout);
			fputs("pop pointer 1\n",fout);
			fputs("push that 0\n",fout);
			fgets(buffer,MAXCHAR,fin);
			trim();
			if(strcmp(buffer,"</term>")!=0)
			fputs("ERROR:No /term",ferr);
			return;
		}
		if(strcmp(comp2,".")!=0)
		{
			fputs("push pointer 0\n",fout);
			fgets(buffer,MAXCHAR,fin);
			split();
			if(strcmp(comp2,"(")!=0)
			fputs("ERROR:( expected",ferr);
			fgets(buffer,MAXCHAR,fin);
			np=CompileExpressionList();
			fgets(buffer,MAXCHAR,fin);
			split();
			if(strcmp(comp2,")")!=0)
			fputs("ERROR:) expected",ferr);
			fgets(buffer,MAXCHAR,fin);
			split();
			if(strcmp(comp2,";")!=0)
			fputs("ERROR:; expected",ferr);
			fputs("call ",fout);
			fputs(currentClassName,fout);
			fputs(".",fout);
			fputs(id1,fout);
			fprintf(fout," %d\n",np+1);
			fgets(buffer,MAXCHAR,fin);
			trim();
			if(strcmp(buffer,"</term>")!=0)
			fputs("ERROR:No /term",ferr);
			return;
		}
		fgets(buffer,MAXCHAR,fin);
		split();
		char id2[50];
		strcpy(id2,strcat(comp2,"\0"));
		if(x11==1 || x11==2)
		{
			fputs("push ",fout);
			if(strcmp(kind11,"field")==0) { fputs("this",fout); } else { fputs(kind11,fout); }
			fputs(" ",fout);
			fprintf(fout,"%d",index11);
			fputs("\n",fout);
		}
		fgets(buffer,MAXCHAR,fin);
		split();
		if(strcmp(comp2,"(")!=0)
		fputs("ERROR:( expected",ferr);
		fgets(buffer,MAXCHAR,fin);
		np=CompileExpressionList();
		fgets(buffer,MAXCHAR,fin);
		split();
		if(strcmp(comp2,")")!=0)
		fputs("ERROR:) expected",ferr);
		strcpy(comp2,id1);
		IdentifierCheck();		
		if(x11==1 || x11==2)
		{
			fputs("call ",fout);
			fputs(type11,fout);
			fputs(".",fout);
			fputs(id2,fout);
			fprintf(fout," %d\n",np+1);
		}
		else
		{
			fputs("call ",fout);
			fputs(id1,fout);
			fputs(".",fout);
			fputs(id2,fout);
			fprintf(fout," %d\n",np);
		}
		fgets(buffer,MAXCHAR,fin);
		trim();
		if(strcmp(buffer,"</term>")!=0)
		fputs("ERROR:No /term",ferr);
		return;
	}
	if(strcmp(comp1,"<stringConstant>")==0)
	{
		msplit();
		int length=strlen(comp2);
		fputs("push constant ",fout);
		fprintf(fout,"%d\n",length);
		fputs("call String.new 1\n",fout);
		for(int i=0;i<length;i++)
		{
			fputs("push constant ",fout);
			fprintf(fout,"%d\n",comp2[i]);
			fputs("call String.appendChar 2\n",fout);
		}
		fgets(buffer,MAXCHAR,fin);
		trim();
		if(strcmp(buffer,"</term>")!=0)
		fputs("ERROR:No /term",ferr);
		return;
	}
}
void CompileExpression()
{
	fgets(buffer,MAXCHAR,fin);
	trim();
	if(strcmp(buffer,"<term>")!=0)
	{
		fputs("ERROR:No term in expression\n",ferr);
	}
	CompileTerm();
	fgets(buffer,MAXCHAR,fin);
	split();
	char op[5];
	while(strcmp(comp1,"<symbol>")==0)
	{
		strcpy(op,strcat(comp2,"\0"));
		fgets(buffer,MAXCHAR,fin);
		trim();
		if(strcmp(buffer,"<term>")!=0)
		{
			fputs("ERROR:No term in expression\n",ferr);
		}
		CompileTerm();
		if(strcmp(op,"+")==0)
		fputs("add\n",fout);
		if(strcmp(op,"-")==0)
		fputs("sub\n",fout);
		if(strcmp(op,"&")==0)
		fputs("and\n",fout);
		if(strcmp(op,"|")==0)
		fputs("or\n",fout);
		if(strcmp(op,">")==0)
		fputs("gt\n",fout);
		if(strcmp(op,"<")==0)
		fputs("lt\n",fout);
		if(strcmp(op,"=")==0)
		fputs("eq\n",fout);
		if(strcmp(op,"*")==0)
		fputs("call Math.multiply 2\n",fout);
		if(strcmp(op,"/")==0)
		fputs("call Math.divide 2\n",fout);
		fgets(buffer,MAXCHAR,fin);
		split();
	}
	trim();
	if(strcmp(buffer,"</expression>")!=0)
	{
		fputs("ERROR:expected /expression",ferr);
	}
	return;
}
int CompileExpressionList()										//upto </expressionList>
{
	int np=0;
	for(int i=0;i<strlen(buffer);i++)
	{
		if(buffer[i]=='<' && buffer[i+1]=='/')
		return 0;
	}
	EXPRESSIONLIST:
	fgets(buffer,MAXCHAR,fin);
	trim();
	if(strcmp(buffer,"<expression>")!=0)
	{
		fputs("ERROR:expression expected",ferr);
	}
	np++;
	CompileExpression();
	fgets(buffer,MAXCHAR,fin);
	split();
	if(strcmp(comp2,",")==0)
	{
		goto EXPRESSIONLIST;
	}
	trim();
	if(strcmp(buffer,"</expressionList>")!=0)
	fputs("ERROR:/expressionlist expected",ferr);
	return np;
}
void CompileDoStatement()										//upto ;
{
	int np;
	char kind11[50],type11[50],name11[50];
	int index11,x11;
	fgets(buffer,MAXCHAR,fin);
	split();
	if(strcmp(comp2,"do")!=0)
	fputs("ERROR:do expected",ferr);
	fgets(buffer,MAXCHAR,fin);
	split();
	IdentifierCheck();
	strcpy(kind11,kind1);
	strcpy(type11,type1);
	strcpy(name11,strcat(comp2,"\0"));
	index11=index1;
	x11=x;
	char id1[50];
	strcpy(id1,strcat(comp2,"\0"));
	fgets(buffer,MAXCHAR,fin);
	split();
	if(strcmp(comp2,".")==0)
	{
		char id2[50];
		fgets(buffer,MAXCHAR,fin);
		split();
		strcpy(id2,strcat(comp2,"\0"));
		if(x11==1 || x11==2)
		{
			fputs("push ",fout);
			if(strcmp(kind11,"field")==0)
			fputs("this",fout);
			else
			fputs(kind11,fout);
			fputs(" ",fout);
			fprintf(fout,"%d",index11);
			fputs("\n",fout);
		}
		fgets(buffer,MAXCHAR,fin);
		split();
		if(strcmp(comp2,"(")!=0)
		{
			fputs("ERROR:( expected",ferr);
		}
		fgets(buffer,MAXCHAR,fin);
		trim();
		np=CompileExpressionList();
		fgets(buffer,MAXCHAR,fin);
		split();
		if(strcmp(comp2,")")!=0)
		fputs("ERROR:) expected",ferr);
		fgets(buffer,MAXCHAR,fin);
		split();
		if(strcmp(comp2,";")!=0)
		fputs("ERROR:; expected",ferr);
		if(x11==1 || x11==2)
		{
			fputs("call ",fout);
			fputs(type11,fout);
			fputs(".",fout);
			fputs(id2,fout);
			fprintf(fout," %d\n",np+1);
			fputs("pop temp 0\n",fout);
		}
		else
		{
			fputs("call ",fout);
			fputs(id1,fout);
			fputs(".",fout);
			fputs(id2,fout);
			fprintf(fout," %d\n",np);
			fputs("pop temp 0\n",fout);
		}
		return;
	}
	fputs("push pointer 0\n",fout);
	if(strcmp(comp2,"(")!=0)
	{
		fputs("ERROR:( expected",ferr);
	}
	fgets(buffer,MAXCHAR,fin);
	trim();
	np=CompileExpressionList();
	fgets(buffer,MAXCHAR,fin);
	split();
	if(strcmp(comp2,")")!=0)
	fputs("ERROR:) expected",ferr);
	fgets(buffer,MAXCHAR,fin);
	split();
	if(strcmp(comp2,";")!=0)
	fputs("ERROR:; expected",ferr);
	fputs("call ",fout);
	fputs(currentClassName,fout);
	fputs(".",fout);
	fputs(id1,fout);
	fprintf(fout," %d\n",np+1);
	fputs("pop temp 0\n",fout);
	return;
}
void CompileStatements()										//upto </statements> 
{
	fgets(buffer,MAXCHAR,fin);
	trim();
	STATEMENTS:
	if(strcmp(buffer,"<letStatement>")==0)
	{
		CompileLetStatement();
		fgets(buffer,MAXCHAR,fin);
		trim();
		if(strcmp(buffer,"</letStatement>")!=0)
		fputs("ERROR:/letStatement expected",ferr);
	}
	if(strcmp(buffer,"<returnStatement>")==0)
	{
		CompileReturnStatement();
		fgets(buffer,MAXCHAR,fin);
		trim();
		if(strcmp(buffer,"</returnStatement>")!=0)
		fputs("ERROR:/returnStatement expected",ferr);
	}
	if(strcmp(buffer,"<ifStatement>")==0)
	{
		CompileIfStatement();
	}
	if(strcmp(buffer,"<whileStatement>")==0)
	{
		CompileWhileStatement();
		fgets(buffer,MAXCHAR,fin);
		trim();
		if(strcmp(buffer,"</whileStatement>")!=0)
		fputs("ERROR:/whileStatement expected",ferr);
	}
	if(strcmp(buffer,"<doStatement>")==0)
	{
		CompileDoStatement();
		fgets(buffer,MAXCHAR,fin);
		trim();
		if(strcmp(buffer,"</doStatement>")!=0)
		fputs("ERROR:/doStatement expected",ferr);
	}
	fgets(buffer,MAXCHAR,fin);
	trim();
	if(strcmp(buffer,"<letStatement>")==0 || strcmp(buffer,"<returnStatement>")==0 || strcmp(buffer,"<ifStatement>")==0 || strcmp(buffer,"<whileStatement>")==0 || strcmp(buffer,"<doStatement>")==0)
	goto STATEMENTS;
	if(strcmp(buffer,"</statements>")!=0)
	fputs("ERROR:/statements expected",ferr);
	return;
}
void CompileSubroutineBody()									//upto }
{
	fgets(buffer,MAXCHAR,fin);
	split();
	if(strcmp(comp2,"{")!=0)
	fputs("ERROR:{ expected",ferr);
	fgets(buffer,MAXCHAR,fin);
	trim();
	if(strcmp(buffer,"<varDec>")==0)
	{
		SUBROUTINEDEC1:
		CompileVarDec();
		fgets(buffer,MAXCHAR,fin);
		trim();
		if(strcmp(buffer,"</varDec>")!=0)
		fputs("ERROR:</varDec expected",ferr);
		fgets(buffer,MAXCHAR,fin);
		trim();
		if(strcmp(buffer,"<varDec>")==0)
		goto SUBROUTINEDEC1;
	}
	fputs("function ",fout);
	fputs(currentClassName,fout);
	fputs(".",fout);
	fputs(currentSubroutineName,fout);
	fputs(" ",fout);
	fprintf(fout,"%d\n",local_count);
	if(strcmp(currentSubroutineType,"constructor")==0)
	{
		fputs("push constant ",fout);
		fprintf(fout,"%d\n",field_count);
		fputs("call Memory.alloc 1\n",fout);
		fputs("pop pointer 0\n",fout);
	}
	if(strcmp(currentSubroutineType,"method")==0)
	{
		fputs("push argument 0\n",fout);
		fputs("pop pointer 0\n",fout);
	}
	if(strcmp(buffer,"<statements>")==0)
	{
		CompileStatements();
	}
	fgets(buffer,MAXCHAR,fin);
	split();
	if(strcmp(comp2,"}")!=0)
	fputs("ERROR:} expected",ferr);
	return;
}
void CompileSubroutineDec()										
{
	fgets(buffer,MAXCHAR,fin);
	split();
	strcpy(currentSubroutineType,strcat(comp2,"\0"));
	fgets(buffer,MAXCHAR,fin);
	split();
	fgets(buffer,MAXCHAR,fin);
	split();
	strcpy(currentSubroutineName,strcat(comp2,"\0"));
	sub_total=0;
	arg_count=0;
	local_count=0;
	if(strcmp(currentSubroutineType,"method")==0)
	{
		strcpy(subroutineSymbolTable[sub_total].name,"this\0");
		strcpy(subroutineSymbolTable[sub_total].kind,"argument\0");
		strcpy(subroutineSymbolTable[sub_total].type,currentClassName);
		subroutineSymbolTable[sub_total].index=arg_count;
		arg_count++;
		sub_total++;
	}
	fgets(buffer,MAXCHAR,fin);
	split();
	if(strcmp(comp2,"(")!=0)
	{
		fputs("ERROR:( expected",ferr);
	}
	fgets(buffer,MAXCHAR,fin);
	split();
	CompileParameterList();
	fgets(buffer,MAXCHAR,fin);
	trim();
	if(strcmp(buffer,"<subroutineBody>")!=0)
	fputs("ERROR:<subroutineBody> expected",ferr);
	CompileSubroutineBody();
	fgets(buffer,MAXCHAR,fin);
	trim();
	if(strcmp(buffer,"</subroutineBody>")!=0)
	fputs("ERROR:</subroutineBody> expected",ferr);
	return;
}
void CompileClass()
{
	fgets(buffer,MAXCHAR,fin);
	split();
	if(strcmp(comp2,"class")!=0)
	{
		fputs("ERROR:expected class",ferr);
	}
	fgets(buffer,MAXCHAR,fin);
	split();
	if(strcmp(comp1,"<identifier>")!=0)
	{
		fputs("ERROR:expected identifier",ferr);
	}
	strcpy(currentClassName,strcat(comp2,"\0"));
	fgets(buffer,MAXCHAR,fin);
	split();
	if(strcmp(comp2,"{")!=0)
	{
		fputs("ERROR:expected {",ferr);
	}
	fgets(buffer,MAXCHAR,fin);
	trim();
	if(strcmp(buffer,"<classVarDec>")==0)
	{
		CLASS1:
		CompileClassVarDec();
		fgets(buffer,MAXCHAR,fin);
		trim();
		if(strcmp(buffer,"</classVarDec>")!=0)
		{
			fputs("ERROR:</classVarDec> expected",ferr);
		}
		fgets(buffer,MAXCHAR,fin);
		trim();
		if(strcmp(buffer,"<classVarDec>")==0)
		goto CLASS1;
	}
	if(strcmp(buffer,"<subroutineDec>")==0)
	{
		CLASS2:
		CompileSubroutineDec();
		fgets(buffer,MAXCHAR,fin);
		trim();
		if(strcmp(buffer,"</subroutineDec>")!=0)
		{
			fputs("ERROR:</subroutineDec> expected",ferr);
		}
		fgets(buffer,MAXCHAR,fin);
		trim();
		if(strcmp(buffer,"<subroutineDec>")==0)
		goto CLASS2;
	}
	split();
	if(strcmp(comp2,"}")!=0)
	fputs("ERROR:} expected",ferr);
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
	str[it+1]='v';
	str[it+2]='m';
	str[it+3]='\0';
	fout=fopen(str,"w");
	str[it+1]='a';
	str[it+2]='n';
	str[it+3]='z';
	str[it+4]='\0';
	fin=fopen(str,"r");
	str[it+1]='e';
	str[it+2]='r';
	str[it+3]='r';
	str[it+4]='\0';
	ferr=fopen(str,"a");
	no_files++;
	fgets(buffer,MAXCHAR,fin);
	trim();
	if(strcmp(buffer,"<class>")!=0)
	{
		fputs("ERROR:expected class",ferr);
	}
	CompileClass();
	fgets(buffer,MAXCHAR,fin);
	trim();
	if(strcmp(buffer,"</class>")!=0)
	fputs("ERROR:</class> expected",ferr);
	}
	return 0;
}
