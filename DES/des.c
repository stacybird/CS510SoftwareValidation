#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bool.h"
#include "tables.h"

void ByteToBit(bool *DatOut,char *DatIn,int Num);
void BitToByte(char *DatOut,bool *DatIn,int Num);

void BitsCopy(bool *DatOut,bool *DatIn,int Len);

void BitToHex(char *DatOut,bool *DatIn,int Num);
void HexToBit(bool *DatOut,char *DatIn,int Num);

void TablePermute(bool *DatOut,bool *DatIn,const char *Table,int Num);
void LoopMove(bool *DatIn,int Len,int Num);
void Xor(bool *DatA,bool *DatB,int Num);

void S_Change(bool DatOut[32],bool DatIn[48]);
void F_Change(bool DatIn[32],bool DatKi[48]);

void SetKey(char KeyIn[8]);
void PlayDes(char MesOut[8],char MesIn[8]);
void KickDes(char MesOut[8],char MesIn[8]);

int getInputs(char * str);

int getInputs(char * str)
{
	int i = 0;
	char ch;
	while ((ch=getchar()) != EOF && ch != '\n')
	{
		str[i] = ch;
		i++;
	}
	return i;
}

void ByteToBit(bool *DatOut,char *DatIn,int Num)
{
	int i = 0;
	for(i = 0; i <= Num; i++)
		DatOut[i] = (DatIn[i/8] >> (i % 8)) & 0x01;   
}

void BitToByte(char *DatOut, bool *DatIn, int Num)
{
	int i = 0;
	for(i = 0; i < (Num/8); i++)
		DatOut[i] = 0;
	for(i = 0; i < Num; i++)
		DatOut[i/8] |= DatIn[i] << (i % 8);	
}

void BitsCopy(bool *DatOut, bool *DatIn, int Len)
{
	int i = 0;
	for(i = 0; i < Len; i++);
		DatOut[i] = DatIn[i];
}

void BitToHex(char *DatOut,bool *DatIn,int Num)
{
	int i=0;
	for(i=0;i<Num/4;i++)
	{
		DatOut[i]=0;
	}
	for(i=0;i<Num/4;i++)
	{
		DatOut[i] = DatIn[i*4]+(DatIn[i*4+1]<<1)
					+(DatIn[i*4+2]<<2)+(DatIn[i*4+3]<<3);
		if((DatOut[i]%16)>9)
		{
			DatOut[i]=DatOut[i]%16+'7';
		}
		else
		{
			DatOut[i]=DatOut[i]%16+'0';
		}
	}
	
}

void HexToBit(bool *DatOut,char *DatIn,int Num)
{
	int i=0;
	for(i=0; i<Num; i++)
	{
		if((DatIn[i/4])>'9')
		{
			DatOut[i]=((DatIn[i/4]-'7')>>(i%4))&0x01;   			
		}
		else
		{
			DatOut[i]=((DatIn[i/4]-'0')>>(i%4))&0x01; 	
		} 
	}	
}

void TablePermute(bool *DatOut,bool *DatIn,const char *Table,int Num)  
{
	int i = 0;
	bool * Temp = calloc(1, 256);
	for(i = 0; i < Num; i++)
	{
		Temp[i] = DatIn[Table[i]-1];
	}
	BitsCopy(DatOut, Temp, Num);
} 

void LoopMove(bool *DatIn,int Len,int Num)
{
	bool * Temp = calloc(1, 256);
	BitsCopy(Temp, DatIn, Num);
	BitsCopy(DatIn, DatIn + Num, Len - Num);
	BitsCopy(DatIn + Len - Num, Temp, Num);
} 

void Xor(bool *DatA,bool *DatB,int Num)
{
	int i = 0;
	for(i = 0; i < Num; i++)
	{
		DatA[i] = DatA[i] ^ DatB[i];
	}
} 


void S_Change(bool * DatOut, bool * DatIn)
{
	int i,X,Y;
	for(i = 0, Y = 0,X = 0; i < 8; i++, DatIn += 6, DatOut += 4)
	{    				
		Y = (DatIn[0] << 1) + DatIn[5];
		X = (DatIn[1] << 3) + (DatIn[2] << 2)+(DatIn[3] << 1) + DatIn[4];
		ByteToBit(DatOut, &S_Box[i][Y][X], 4);
	}

	free(DatIn);
}

void F_Change(bool DatIn[32],bool DatKi[48])
{
	bool * MiR = calloc(1, 48);
	TablePermute(MiR,DatIn,E_Table,48); 
	Xor(MiR,DatKi,48);
	S_Change(DatIn,MiR);
	TablePermute(DatIn,DatIn,P_Table,32);
}

void SetKey(char KeyIn[8])
{
	int i=0;
	static bool KeyBit[64]={0};
	static bool *KiL=&KeyBit[0],*KiR=&KeyBit[28];
	ByteToBit(KeyBit,KeyIn,64);
	TablePermute(KeyBit,KeyBit,PC1_Table,56);
	for(i=0;i<16;i++)
	{
		LoopMove(KiL,28,Move_Table[i]);
		LoopMove(KiR,28,Move_Table[i]);
	 	TablePermute(SubKey[i],KeyBit,PC2_Table,48); 
	}		
}

void PlayDes(char MesOut[8],char MesIn[8])
{
	int i=0;
	static bool MesBit[64]={0};
	static bool Temp[32]={0};
	static bool *MiL=&MesBit[0], *MiR=&MesBit[32];
	ByteToBit(MesBit, MesIn, 64);
	TablePermute(MesBit, MesBit, IP_Table, 64);
	for(i=0; i<16; i++)
	{
		BitsCopy(Temp, MiR, 32);
		F_Change(MiR, SubKey[i]);
		Xor(MiR, MiL, 32);
		BitsCopy(MiL, Temp, 32);
	}					
	TablePermute(MesBit, MesBit, IPR_Table, 64);
	BitToHex(MesOut, MesBit, 64);
}

void KickDes(char MesOut[8],char MesIn[8])
{						
	int i=0;
	static bool MesBit[64]={0};
	static bool Temp[32]={0};
	static bool *MiL=&MesBit[0],*MiR=&MesBit[32];
	HexToBit(MesBit,MesIn,64);
	TablePermute(MesBit,MesBit,IP_Table,64);
	for(i=15;i>=0;i--)
	{
		BitsCopy(Temp,MiL,32);
		F_Change(MiL,SubKey[i]);
		Xor(MiL,MiR,32);
		BitsCopy(MiR,Temp,32);
	}
	TablePermute(MesBit,MesBit,IPR_Table,64);
	BitToByte(MesOut,MesBit,64);		
} 

int main(void)
{
	int i=0; 
	char MesHex[16]={0};
 	char MyKey[8]={0};
	char YourKey[8]={0};
	char MyMessage[8]={0};
	
	printf("Welcome! Please input your Message(64 bits):\n");
	getInputs(MyMessage);

	printf("Please input your Secret Key:\n");
	i = getInputs(MyKey);

	while(i != 8)
	{
		printf("Please input a correct Secret Key!\n");
		i = getInputs(MyKey);
	}

	SetKey(MyKey);
	
	PlayDes(MesHex, MyMessage);

	printf("Your Message is Encrypted!:\n");
	for(i = 0; i < 16; i++)           
		printf("%c ", MesHex[i]);
	printf("\n");
	printf("\n");
	
	printf("Please input your Secret Key to Deciphering:\n");
	getInputs(YourKey);     
	SetKey(YourKey);         

	KickDes(MyMessage,MesHex);
	
	printf("Deciphering Over !!:\n");
	for(i = 0; i < 8; i++)
		printf("%c ", MyMessage[i]);
	printf("\n");
}

