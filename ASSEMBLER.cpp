#include <stdio.h>
#include <string.h>
#define MAXCAPACITYOFLINE 85
#define MAXLENGTH 50
#define MAXENTRIES 3000
char alu[8];
char dest[4];
struct symboltableentry
{
	char symbol[MAXLENGTH];
	int address;
 } ;
 struct symboltableentry symboltable[MAXENTRIES];
 int computealu(char *check)
 {
 	int flag=0,posequal=0,k;
 	char use[80];
 	for(int i=0;i<strlen(check);i++)
 	{
 		if(check[i]=='=')
 		{
 			flag=1;
 			posequal=i;
 			for(int j=0;j<i;j++)
 			{
 				if(check[j]=='M')
 				dest[2]='1';
 				else if(check[j]=='A')
 				dest[0]='1';
 				else if(check[j]=='D')
 				dest[1]='1';
 				else
 				{
 					printf("ERROR \n");
 					printf(check);
 					printf(":Invalid destination\n");
 					return(-1);
				 }
 				
			}
			for(k=i+1;k<strlen(check);k++)
			{
				if(check[k]=='\n')
				break;
				use[k-i-1]=check[k];
			}
			use[k-i-1]='\0';
		}
	 }
	 if(flag==0)
	 {
	 	for(k=0;k<strlen(check);k++)
	 	{
	 		if(check[k]=='\n')
	 		break;
	 		use[k]=check[k];
		 }
	 }
	 use[k]='\0';
	 if(strcmp(use,"0")==0) strcpy(alu,"0101010");
	 else if(strcmp(use,"1")==0) strcpy(alu,"0111111"); 
	 else if(strcmp(use,"-1")==0) strcpy(alu,"0111010");
	 else if(strcmp(use,"D")==0) strcpy(alu,"0001100"); 
	 else if(strcmp(use,"A")==0) strcpy(alu,"0110000");
	 else if(strcmp(use,"M")==0) strcpy(alu,"1110000"); 
	 else if(strcmp(use,"!D")==0) strcpy(alu,"0001101");
	 else if(strcmp(use,"!A")==0) strcpy(alu,"0110001");
	 else if(strcmp(use,"!M")==0) strcpy(alu,"1110001");
	 else if(strcmp(use,"-D")==0) strcpy(alu,"000111"); 
	 else if(strcmp(use,"-A")==0) strcpy(alu,"0110011");
	 else if(strcmp(use,"-M")==0) strcpy(alu,"1110011");
	 else if(strcmp(use,"D+1")==0) strcpy(alu,"0011111");
	 else if(strcmp(use,"1+D")==0) strcpy(alu,"0011111"); 
	 else if(strcmp(use,"A+1")==0) strcpy(alu,"0110111");
	 else if(strcmp(use,"1+A")==0) strcpy(alu,"0110111");   
	 else if(strcmp(use,"M+1")==0) strcpy(alu,"1110111");
	 else if(strcmp(use,"1+M")==0) strcpy(alu,"1110111"); 
	 else if(strcmp(use,"D-1")==0) strcpy(alu,"0001110");
	 else if(strcmp(use,"-1+D")==0) strcpy(alu,"0001110"); 
	 else if(strcmp(use,"A-1")==0) strcpy(alu,"0110010");
	 else if(strcmp(use,"-1+A")==0) strcpy(alu,"0110010"); 
	 else if(strcmp(use,"M-1")==0) strcpy(alu,"1110010");
	 else if(strcmp(use,"-1+M")==0) strcpy(alu,"1110010"); 
	 else if(strcmp(use,"D+A")==0) strcpy(alu,"0000010");
	 else if(strcmp(use,"A+D")==0) strcpy(alu,"0000010"); 
	 else if(strcmp(use,"M+D")==0) strcpy(alu,"1000010");
	 else if(strcmp(use,"D+M")==0) strcpy(alu,"1000010"); 
	 else if(strcmp(use,"D-A")==0) strcpy(alu,"0010011");
	 else if(strcmp(use,"-A+D")==0) strcpy(alu,"0010011"); 
	 else if(strcmp(use,"D-M")==0) strcpy(alu,"1010011");
	 else if(strcmp(use,"-M+D")==0) strcpy(alu,"1010011"); 
	 else if(strcmp(use,"A-D")==0) strcpy(alu,"0000111");
	 else if(strcmp(use,"-D+A")==0) strcpy(alu,"0000111"); 
	 else if(strcmp(use,"M-D")==0) strcpy(alu,"1000111");
	 else if(strcmp(use,"-D+M")==0) strcpy(alu,"1000111");
	 else if(strcmp(use,"D&A")==0) strcpy(alu,"0000000");
	 else if(strcmp(use,"A&D")==0) strcpy(alu,"0000000"); 
	 else if(strcmp(use,"M&D")==0) strcpy(alu,"1000000");
	 else if(strcmp(use,"D&M")==0) strcpy(alu,"1000000");  
	 else if(strcmp(use,"D|A")==0) strcpy(alu,"0010101");
	 else if(strcmp(use,"A|D")==0) strcpy(alu,"0010101"); 
	 else if(strcmp(use,"M|D")==0) strcpy(alu,"1010101");
	 else if(strcmp(use,"D|M")==0) strcpy(alu,"1010101"); 
	 else {printf("ERROR  \n");printf(use);printf(":Invalid ALU input\n");return(-1);}
	return (0);
 }
 int checkjumptype(char *check)
 {
 	int jump=0;
 	for(int i=0;i<=strlen(check);i++)
 	{
 		if(check[i]=='\n')
 		check[i]='\0';
	 }
	for(int i=0;i<strlen(check);i++)
	{
		if(check[i]==';')
		{
			if(check[i+1]!='J')
			{
				printf("ERROR \n");
				printf(check);
				printf(":Invalid jump statement\n");
				return(9);
			}
			if(check[i+4]!='\0')
			{
				printf("ERROR \n");
				printf(check);
				printf(":Invalid jump statement\n");
				return(9);
			}
			if(check[i+2]=='G' && check[i+3]=='T')
			jump=1;
			else if(check[i+2]=='E' && check[i+3]=='Q')
			jump=2;
			else if(check[i+2]=='G' && check[i+3]=='E')
			jump=3;
			else if(check[i+2]=='L' && check[i+3]=='T')
			jump=4;
			else if(check[i+2]=='N' && check[i+3]=='E')
			jump=5;
			else if(check[i+2]=='L' && check[i+3]=='E')
			jump=6;
			else if(check[i+2]=='M' && check[i+3]=='P')
			jump=7;
			else
			{
				printf("ERROR \n");
				printf(check);
				printf(":Invalid jump statement\n");
				return(9);
			}
			break;
		}
	}
	return jump;
}
//function to add entry
 void addEntry(struct symboltableentry item)
 {
 	int i=0;
 	while(strcmp(symboltable[i].symbol,item.symbol)!=0 && strcmp(symboltable[i].symbol,"\0")!=0 && i<MAXENTRIES)
 	{
 		i++;	
	 }
	if(strcmp(symboltable[i].symbol,item.symbol)==0)
	{
		symboltable[i].address=item.address;
	}
	if(strcmp(symboltable[i].symbol,"\0")==0)
	{
		strcpy(symboltable[i].symbol,item.symbol);
		symboltable[i].address=item.address;
	}
	if(i==MAXENTRIES)
	{
		printf("ERROR:MAXIMUM ENTRIES OF SYMBOL TABLE REACHED\n");
	}
 }
 //function to get address of a symbol
int getAddress(char *symbol)
{
	int i=0;
	for(i=0;i<MAXENTRIES;i++)
	{
		if(strcmp(symbol,symboltable[i].symbol)==0)
		{
			return symboltable[i].address;
		}
		if(strcmp("\0",symboltable[i].symbol)==0)
		{
			return -1;
		}
		if(i==MAXENTRIES-1)
		{
			return -1;
		}
	}
}
int main()
{
	int i;//for loops
	//For decommenting and removing white spaces and empty lines
	int flag=0;//for detecting multiple line commments
	FILE *fpin,*fpout;
	fpin=fopen("mult.asm","r");
	fpout=fopen("pass1.inter","w");
	char buffer[MAXCAPACITYOFLINE];
	while(fgets(buffer,MAXCAPACITYOFLINE,fpin)!=NULL)
	{
		//decomment
		A:
		for(i=0;i<strlen(buffer);i++)
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
		//remove white space
		for(i=0;i<strlen(buffer);i++)
		{
			if(buffer[i]==' ')
			{
				for(int j=i;j<strlen(buffer);j++)
				{
					buffer[j]=buffer[j+1];
				}
				i--;
			}
		}
		fputs(buffer,fpout);
		if(buffer[0]!='\0')
			fputs("\n",fpout);
	}
	fclose(fpin);
	fclose(fpout);
	//for construction of symbol table
	FILE *fin,*fout;
	fin=fopen("pass1.inter","r");
	fout=fopen("mult.hack","w");
	//intialise the inbuilt symbols
	strcpy(symboltable[0].symbol,"SP");
	strcpy(symboltable[1].symbol,"LCL");
	strcpy(symboltable[2].symbol,"ARG");
	strcpy(symboltable[3].symbol,"THIS");
	strcpy(symboltable[4].symbol,"THAT");
	strcpy(symboltable[5].symbol,"R0");
	strcpy(symboltable[6].symbol,"R1");
	strcpy(symboltable[7].symbol,"R2");
	strcpy(symboltable[8].symbol,"R3");
	strcpy(symboltable[9].symbol,"R4");
	strcpy(symboltable[10].symbol,"R5");
	strcpy(symboltable[11].symbol,"R6");
	strcpy(symboltable[12].symbol,"R7");
	strcpy(symboltable[13].symbol,"R8");
	strcpy(symboltable[14].symbol,"R9");
	strcpy(symboltable[15].symbol,"R10");
	strcpy(symboltable[16].symbol,"R11");
	strcpy(symboltable[17].symbol,"R12");
	strcpy(symboltable[18].symbol,"R13");
	strcpy(symboltable[19].symbol,"R14");
	strcpy(symboltable[20].symbol,"R15");
	strcpy(symboltable[21].symbol,"SCREEN");
	strcpy(symboltable[22].symbol,"KBD");
	//initialise inbuilt address
	symboltable[0].address=0;
	symboltable[1].address=1;
	symboltable[2].address=2;
	symboltable[3].address=3;
	symboltable[4].address=4;
	symboltable[5].address=0;
	symboltable[6].address=1;
	symboltable[7].address=2;
	symboltable[8].address=3;
	symboltable[9].address=4;
	symboltable[10].address=5;
	symboltable[11].address=6;
	symboltable[12].address=7;
	symboltable[13].address=8;
	symboltable[14].address=9;
	symboltable[15].address=10;
	symboltable[16].address=11;
	symboltable[17].address=12;
	symboltable[18].address=13;
	symboltable[19].address=14;
	symboltable[20].address=15;
	symboltable[21].address=16384;
	symboltable[22].address=24576;
	//initialse the others to null 
	for(i=23;i<MAXENTRIES;i++)
	{
		strcpy(symboltable[i].symbol,"\0");
		symboltable[i].address=-1;
	}
	int counter=0;	//counter to count the number of instructions 
	struct symboltableentry newitem;
	int pos;
	//generating the symbol table
	while(fgets(buffer,MAXCAPACITYOFLINE,fin)!=NULL)
	{
		//if it is a A instruction
		if(buffer[0]=='@')
		{
			if(buffer[1]<48 || buffer[1]>57)
			{				
				for(i=1;i<strlen(buffer);i++)
				{
					newitem.symbol[i-1]=buffer[i];
				}
				newitem.symbol[i-2]='\0';
				newitem.address=getAddress(newitem.symbol);
				addEntry(newitem);
			}
			counter+=1;
		}
		//if it is a label
		else if(buffer[0]=='(')
		{
			for(i=2;i<strlen(buffer);i++)
			{
				if(buffer[i]==')')
				{
					pos=i;
					break;
				}
			}
			for(i=1;i<pos;i++)
			{
				newitem.symbol[i-1]=buffer[i];
			}
			newitem.symbol[i-1]='\0';
			newitem.address=counter;
			addEntry(newitem);
			//if a Atype is followed
			if(buffer[pos+1]=='@')
			{
				if(buffer[pos+2]<48 || buffer[pos+2]>57)
				{				
					for(i=pos+2;i<strlen(buffer);i++)
					{
						newitem.symbol[i-pos-2]=buffer[i];
					}
					newitem.symbol[i-pos-3]='\0';
					newitem.address=getAddress(newitem.symbol);
					addEntry(newitem);
				}
				counter+=1;
			}
			//if it is followed by a c type instruction
			if(buffer[pos+1]!='\n' && buffer[pos+1]!='\0')
			{
				counter+=1;
			}
		}
		//if it is a c type instruction
		else
		{
			counter+=1;
		}
	}
	//populating symbol table addresses
	int val=16;
	for(i=23;i<MAXENTRIES;i++)
	{
		if(strcmp(symboltable[i].symbol,"\0")==0)
		break;
		if(symboltable[i].address==-1)
		{
			symboltable[i].address=val;
			val++;
		}
	}
	/*for(i=0;i<MAXENTRIES;i++)
	{
		if(strcmp(symboltable[i].symbol,"\0")==0)
		break;
		printf(symboltable[i].symbol);
		printf("%d\n",symboltable[i].address);
	}
	printf("%d",getAddress("e"));*/
	//symbol table generation completed 
	//running pass2
	fclose(fin);
	fin=fopen("pass1.inter","r");
	int value=0;
	char use[80];
	char binary[17];
	int jump;
	char jumptype[4];
	int length;
	while(fgets(buffer,MAXCAPACITYOFLINE,fin)!=NULL)
	{
		for(i=0;i<7;i++)
		{
			alu[i]='0';
		}
		alu[7]='\0';
		jump=0;
		for(i=0;i<3;i++)
		{
			dest[i]='0';
			jumptype[i]='0';
		}
		dest[3]='\0';
		jumptype[3]='\0';
		value=0;
		for(i=0;i<17;i++)
		{
			binary[i]='0';
		}
		binary[16]='\0';
		//type A instruction
		if(buffer[0]=='@')
		{
			//if it is a constant
			if((int)buffer[1]>=48 && (int)buffer[1]<=57)
			{
				for(i=1;i<strlen(buffer);i++)
				{
					if(buffer[i]=='\0' || buffer[i]=='\n')
					break;
					use[i-1]=buffer[i];
				}
				use[i-1]='\0';
				for(i=0;i<strlen(use);i++)
				{
					value=value*10+((int)use[i]-48);
				}
				i=15;
				while(value>0)
				{
					if(value%2==0)
					{
						binary[i]='0';
					}
					else
					{
						binary[i]='1';
					}
					value=value/2;
					i--;
				}
				fputs(binary,fout);
				fputs("\n",fout);
			}
			//if it is a variable
			else
			{
				for(i=1;i<strlen(buffer);i++)
				{
					if(buffer[i]=='\0' || buffer[i]=='\n')
					break;
					use[i-1]=buffer[i];
				}
				use[i-1]='\0';
				value=getAddress(use);
				i=15;
				while(value>0)
				{
					if(value%2==0)
					{
						binary[i]='0';
					}
					else
					{
						binary[i]='1';
					}
					value=value/2;
					i--;
				}
				fputs(binary,fout);
				fputs("\n",fout);
			}
		}
		//if line has a label
		else if(buffer[0]=='(')
		{
			//finding the other end
			for(i=1;i<strlen(buffer);i++)
			{
				if(buffer[i]==')')
				{
					pos=i;
					break;
				}
			}
			//if it has a a instruction attached
			if(buffer[pos+1]=='@')
			{
				//if constant
				if((int)buffer[pos+2]>=48 && (int)buffer[pos+2]<=57)
				{
					for(i=pos+2;i<strlen(buffer);i++)
					{		
						if(buffer[i]=='\0' || buffer[i]=='\n')
						break;
						use[i-1]=buffer[i];
					}	
					use[i-1]='\0';
					for(i=0;i<strlen(use);i++)
					{
						value=value*10+((int)use[i]-48);
					}
					i=15;
					while(value>0)
					{
						if(value%2==0)
						{
							binary[i]='0';
						}
						else
						{
							binary[i]='1';
						}
						value=value/2;
						i--;
					}
					fputs(binary,fout);
					fputs("\n",fout);
				}
				//if it is a variable
				
				else
				{
					for(i=pos+2;i<strlen(buffer);i++)
					{
						if(buffer[i]=='\0' || buffer[i]=='\n')
						break;
						use[i-pos-2]=buffer[i];
					}
					use[i-pos-2]='\0';
					value=getAddress(use);
					i=15;
					while(value>0)
					{
						if(value%2==0)
						{
							binary[i]='0';
						}
						else
						{
							binary[i]='1';
						}
						value=value/2;
						i--;
					}
					fputs(binary,fout);
					fputs("\n",fout);
				}	
			}
			//if followed by c type instruction
			else if(buffer[pos+1]!='\0' && buffer[pos+1]!='\n')
			{
				binary[0]='1';
				binary[1]='1';
				binary[2]='1';
				length=strlen(buffer);
				for(i=pos+1;i<strlen(buffer);i++)
				{
					if(buffer[i]=='\n' || buffer[i]=='\0')
					{
						buffer[i-pos-1]='\0';
						break;
					}
					buffer[i-pos-1]=buffer[i];
				}
				buffer[length-pos-1]='\0';
				jump=checkjumptype(buffer);
				if(jump==9)//invalid jump
				{
					return(1);
				}
				i=2;
				while(jump>0)
				{
					if(jump%2==0)
					jumptype[i]='0';
					else
					jumptype[i]='1';
					jump=jump/2;
					i--;
				}
				if(jumptype[0]=='1' || jumptype[1]=='1' || jumptype[2]=='1')
				{
					for(i=0;i<strlen(buffer);i++)
					{
						if(buffer[i]==';')
						{
							buffer[i]='\0';
							break;
						}
					}
				}
				for(i=0;i<strlen(buffer);i++)
				{		
					if(buffer[i]=='\n'){
					buffer[i]='\0';break;}
				}
				if(computealu(buffer)==-1)//bad destination or alu input
				{
					return 1;
				}
				for(i=0;i<3;i++)
				{
					binary[i+13]=jumptype[i];
					binary[i+10]=dest[i];
				}
				binary[3]=alu[0];
				for(i=0;i<7;i++)
				{
					binary[i+3]=alu[i];
				}
				fputs(binary,fout);
				fputs("\n",fout);
			}
		}
		//c type
		else
		{
			binary[0]='1';
			binary[1]='1';
			binary[2]='1';
			jump=checkjumptype(buffer);
			if(jump==9)//if error is present in jump statement
			{
				return(1);
			}
			i=2;
			while(jump>0)
			{
				if(jump%2==0)
				jumptype[i]='0';
				else
				jumptype[i]='1';
				jump=jump/2;
				i--;
			}
			if(jumptype[0]=='1' || jumptype[1]=='1' || jumptype[2]=='1')
			{
				for(i=0;i<strlen(buffer);i++)
				{
					if(buffer[i]==';')
					{
						buffer[i]='\0';
						break;
					}
				}
			}
			for(i=0;i<strlen(buffer);i++)
			{
				if(buffer[i]=='\n'){
				buffer[i]='\0';break;}
			}
			if(computealu(buffer)==-1)//if some error is present in destination or alu input
			{
				return(1);
			}
			for(i=0;i<3;i++)
			{
				binary[i+13]=jumptype[i];
				binary[i+10]=dest[i];
			}
			binary[3]=alu[0];
			for(i=0;i<7;i++)
			{
				binary[i+3]=alu[i];
			}
			fputs(binary,fout);
			fputs("\n",fout);
		}	
	}
	return 0;
}
