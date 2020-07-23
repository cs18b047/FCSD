#include<iostream>
#include<cstring>
#define MAXCHARPERLINE 100 
using namespace std;
int main(int argc,char** argv)
{
	FILE *fin,*fout;
	int no_files=2;
	while(no_files!=argc)
	{
	fin=fopen(argv[no_files],"r");
	int it;
	char str[50];
	for(it=0;it<strlen(argv[no_files]);it++)
	{
		if(argv[no_files][it]=='.')
		break;
		str[it]=argv[no_files][it];
	}
	str[it]='.';
	str[it+1]='t';
	str[it+2]='o';
	str[it+3]='k';
	str[it+4]='\0';
	fout=fopen(str,"w");
	no_files++;
	char buffer[MAXCHARPERLINE];
	int flag=0;
	fputs("<tokens>\n",fout);
	string s;
	while(fgets(buffer,MAXCHARPERLINE,fin)!=NULL)
	{
		//decomment
		A:
		for(int i=0;i<strlen(buffer);i++)
		{
			if(buffer[i]=='/' && buffer[i+1]=='*')
			{
				flag=1;
				for(int j=i+2;j<=strlen(buffer);j++)
				{
					if(buffer[j]=='*' && buffer[j+1]=='/')
					{
						flag=0;
						for(int k=j+2;k<=strlen(buffer);k++)
						{
							if(buffer[k]=='\n')
							{			
								buffer[i+k-j-2]='\0';
								break;
							}
							buffer[i+k-j-2]=buffer[k];
						}
						goto A;
					}
				}
				buffer[i]='\0';
			}
			if(int(buffer[i])==47 && int(buffer[i+1])==47)
			{
				buffer[i]='\0';
				break;
			}
			if(buffer[i]=='\n')
			{	
				buffer[i]='\0';		
				break;
			}
			if(buffer[i]=='*' && buffer[i+1]=='/')
			{
				if(flag==0)
				{
					printf("*/ without /*");
					return(1);
				}
				for(int j=i+2;j<=strlen(buffer);j++)
				{
					if(buffer[j]=='\n')
					{
						buffer[j-i-2]='\0';
						break;
					}
					buffer[j-i-2]=buffer[j];
				}
				flag=0;
				goto A;
			}
		}
		if(flag>1)
		continue;
		if(flag>0)
		flag++;
		for(int i=0;i<strlen(buffer);i++)
		{
			if(buffer[i]==' ' || buffer[i]=='\t' || buffer[i]=='\n')
			{
				continue;
			}
			if(buffer[i]=='_' || (buffer[i]>='a' && buffer[i]<='z') || (buffer[i]>='A' && buffer[i]<='Z'))
			{
				//identifier or keyword
				s="";
				s=s+buffer[i];
				for(int j=i;j<strlen(buffer);i++,j++)
				{
					if(buffer[j+1]=='_' || (buffer[j+1]>='a' && buffer[j+1]<='z') || (buffer[j+1]>='A' && buffer[j+1]<='Z') || (buffer[j+1]>='0' && buffer[j+1]<='9'))
					s=s+buffer[j+1];
					else
					break;
				}
				if(s.compare("class")==0 || s.compare("constructor")==0 || s.compare("function")==0 || s.compare("method")==0 || s.compare("field")==0 || s.compare("static")==0 || s.compare("var")==0 || s.compare("int")==0 || s.compare("char")==0 || s.compare("boolean")==0 || s.compare("void")==0 || s.compare("true")==0 || s.compare("false")==0 || s.compare("null")==0 || s.compare("this")==0 || s.compare("let")==0 || s.compare("do")==0 || s.compare("if")==0 || s.compare("else")==0 || s.compare("while")==0 || s.compare("return")==0)
				{
					fputs("<keyword> ",fout);
					for(int j=0;j<s.length();j++)
					{
						fprintf(fout,"%c",s[j]);
					}
					fputs(" </keyword>\n",fout);
					continue;
				}
				fputs("<identifier> ",fout);
				for(int j=0;j<s.length();j++)
				{
					fprintf(fout,"%c",s[j]);
				}
				fputs(" </identifier>\n",fout);
				continue;
			}
			else if(buffer[i]=='(' || buffer[i]==')' || buffer[i]=='{' || buffer[i]=='}' || buffer[i]=='[' || buffer[i]==']' || buffer[i]=='.' || buffer[i]==',' || buffer[i]==';' || buffer[i]=='+' || buffer[i]=='-' || buffer[i]=='*' || buffer[i]=='/' || buffer[i]=='&' || buffer[i]=='|' || buffer[i]=='<' || buffer[i]=='>' || buffer[i]=='=' || buffer[i]=='~')
			{
				//symbol
				fputs("<symbol> ",fout);
				fprintf(fout,"%c ",buffer[i]);
				fputs("</symbol>\n",fout);
				continue;
			}
			else if(buffer[i]>='0' && buffer[i]<='9')
			{
				//integer constant
				s="";
				s=s+buffer[i];
				for(int j=i;j<strlen(buffer);i++,j++)
				{
					if(buffer[j+1]>='0' && buffer[j+1]<='9')
					s=s+buffer[j+1];
					else
					break;
				}
				fputs("<integerConstant> ",fout);
				for(int j=0;j<s.length();j++)
				{
					fprintf(fout,"%c",s[j]);
				}
				fputs(" </integerConstant>\n",fout);
				continue;
			}
			else if(buffer[i]=='"')
			{
				s="";
				i++;
				while(buffer[i]!='"')
				{
					s=s+buffer[i];
					i++;
				}
				fputs("<stringConstant> ",fout);
				for(int j=0;j<s.length();j++)
				{
					fprintf(fout,"%c",s[j]);
				}
				fputs(" </stringConstant>\n",fout);
				continue;
			}
			else
			{
				return 1;
			}
		}
	}
	fputs("</tokens>",fout);
	fclose(fin);
	fclose(fout);
	}
	return 0;
}
