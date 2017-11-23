//elfread.c -- Display contents of ELF files in a Hooman readable format

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ELFMAG_0 0x7f
#define ELFMAG_1 0x45
#define ELFMAG_2 0x4c
#define ELFMAG_3 0x46

#define SYSTEMV 0x00
#define HP_UX 0x01
#define NETBSD 0x02
#define LINUX 0x03
#define HURD 0x04
#define SOLARIS 0x06
#define AIX 0x07
#define IRIX 0x08
#define FREEBSD 0x09
#define TRU64 0x0a
#define NOVELLMODESTO 0x0b
#define OPENBSD 0x0c
#define OPENVMS 0x0d
#define NONSTOPKERNEL 0x0e
#define AROS 0x0f
#define FENIX 0x10
#define CLOUDABI 0x11
#define SORTIX 0x53

#define SPARC 0x02
#define X86 0x03
#define MIPS 0x08
#define POWERPC 0x14
#define S390 0x16
#define ARM 0x28
#define SUPERH 0x2a
#define IA_64 0x32
#define X86_64 0x3e
#define AARCH_64 0xb7
#define RISCV 0xf3

typedef struct elf
{
	bool bitness;
	bool endianess;
	int ABI;
	int objType;
	int ISA;
	int entryPoint;
	int programHeaderOffset;
	int sectionHeaderOffset;
	int nProgramEntries;
	int nSectionEntries;
	int headerSize;
	int programHeaderEntrySize;
	int sectionHeaderEntrySize;
	int sectionHeaderIndex;

}elf;

elf e;

void errorMessage(int errorCode)
{
	if(errorCode == 2)
		printf("\n Usage : ./elf <FILE_NAME>");
	if(errorCode == 3)
		printf("\n Failed to Create File Stream to the file"); 
	if(errorCode == 4)
		printf("\n Magic Numbers do not match. Not an ELF File");
	exit(errorCode);
}


int magicCheck(FILE *f)
{
	char *getMagic;
	getMagic = malloc(5);

	fread(getMagic, 1, 4, f);

	for(int i = 0; *(getMagic + i)!='\0'; i++)
		printf("%x", *(getMagic + i));

	if(*getMagic!= ELFMAG_0 || *(getMagic+1)!=ELFMAG_1 || *(getMagic+2)!=ELFMAG_2 || *(getMagic+3)!=ELFMAG_3)
	{
		free(getMagic);
		return 0;
	}

	free(getMagic);

	return 1;
}

void getProgramHeaders(FILE *f, int size)
{
	char *headers;
	fseek(f, 0, SEEK_SET);

	headers = malloc(size);
}

void getSectionHeaders(FILE *f, int size)
{
	char *headers;
	fseek(f, 0, SEEK_SET);

	headers = malloc(size);

	printf("\n Offset for Section Entries : %d", size);

	free(headers);
}

void getHeaders(FILE *f)
{
	char *headers;
	headers = malloc(65);

	fseek(f, 0, SEEK_SET);
	fread(headers, 64, 1, f);

	printf("\n Headers : ");
	for(int i =0; i < 64; i++)
		printf(" %2.2x ", *(headers+i));

	if(*(headers+4) == 2)
	{		
		e.bitness = 1;
		printf("\n Bitness : 64 bit");
	}
	else
	{	
		e.bitness = 0;
		printf("\n Bitness : 32 bit");
	}
	if(*(headers+5) == 2)
	{		
		e.endianess = 1;
		printf("\n Endianess : Big ");
	}
	else
	{
		e.endianess = 0;
		printf("\n Endianess : Small");
	}

	if(*(headers+6) == 1)
		printf("\n Original ELF Version ");

	e.ABI = *(headers+0x07);
	e.ISA = *(headers+0x12);
	e.entryPoint = *(headers+0x18);

	switch(e.ABI)
	{
		case SYSTEMV:
				printf("\n System-V/UNIX");
				break;
		case HP_UX:
				printf("\n HP-UX");
				break;
		case NETBSD:
				printf("\n NetBSD");
				break;
		case LINUX:
				printf("\n Linux");
				break;
		case HURD:
				printf("\n GNU Hurd");
				break;
		case SOLARIS:
				printf("\n Solaris");
				break;
		case AIX:
				printf("\n AIX");
				break;
		case IRIX:
				printf("\n IRIX");
				break;
		case FREEBSD:
				printf("\n FreeBSD");
				break;
		case TRU64:
				printf("\n Tru64");
				break;
		case NOVELLMODESTO:
				printf("\n Novell Modesto");
				break;
		case OPENBSD:
				printf("\n OpenBSD");
				break;
		case OPENVMS:
				printf("\n OpenVMS");
				break;
		case NONSTOPKERNEL:
				printf("\n NonStop Kernel");
				break;
		case AROS:
				printf("\n AROS");
				break;
		case FENIX:
				printf("\n Fenix OS");
				break;
		case CLOUDABI:
				printf("\n CloudABI");
				break;
		case SORTIX:
				printf("\n Sortix");
				break;
	}

	e.objType = *(headers+ 0x10);

	switch(e.objType)
	{
		case 1:
			printf("\n Relocatable");
			break;
		case 2:
			printf("\n Executable");
			break;
		case 3:
			printf("\n Shared");
			break;
		case 4:
			printf("\n Core");
			break;
	}

	switch(e.ISA)
	{
		case SPARC:
			printf("\n SPARC");
			break;
		case X86:
			printf("\n x86");
			break;
		case MIPS:
			printf("\n MIPS");
			break;
		case POWERPC:
			printf("\n PowerPC");
			break;
		case S390:
			printf("\n S390");
			break;
		case ARM:
			printf("\n ARM");
			break;
		case SUPERH:
			printf("\n SuperH");
			break;
		case IA_64:
			printf("\n IA-64");
			break;
		case X86_64:
			printf("\n x86-64");
			break;
		case AARCH_64:
			printf("\n AArch64");
			break;
		case RISCV:
			printf("\n RISC-V");
			break;
		case 0x00:
			printf("\n Any");
			break;
	}

	printf("\n Entry Point Address : 0x%x", e.entryPoint);

	if(e.bitness == 1)
	{
		e.programHeaderOffset = *(headers+0x20);
		e.sectionHeaderOffset = *(headers+0x28);
		e.headerSize = *(headers+0x34);
		e.programHeaderEntrySize = *(headers+0x36);
		e.nProgramEntries = *(headers + 0x38);
		e.sectionHeaderEntrySize = *(headers+0x3a);
		e.nSectionEntries = *(headers + 0x3c);
		e.sectionHeaderIndex = *(headers + 0x3e);
	}

	else
	{
	}

	printf("\n Program Header Offset : %x", e.programHeaderOffset);
	printf("\n Section Header Offset : %x", e.sectionHeaderOffset);
	printf("\n Header Size : %x", e.headerSize);
	printf("\n Program Header Entry Size : %x", e.programHeaderEntrySize);
	printf("\n No. of Program Header Entries : %d", e.nProgramEntries);
	printf("\n Section Header Entry Size : %x", e.sectionHeaderEntrySize);
	printf("\n No. of Section Header Entries : %d", e.nSectionEntries);
	printf("\n Section Header Index : %x", e.sectionHeaderIndex);
	
	if(e.programHeaderEntrySize)
		getProgramHeaders(f, e.programHeaderEntrySize * e.nProgramEntries);

	if(e.sectionHeaderEntrySize)
		getSectionHeaders(f, e.sectionHeaderEntrySize * e.nSectionEntries);

	free(headers);
}	

int main(int argc, char **argv)
{
	FILE *f;
	if(argc < 2)
		errorMessage(2);

	f = fopen(*(argv+1), "r+");

	if(!f)
		errorMessage(3);

	if(!(magicCheck(f)))
		errorMessage(4);

	getHeaders(f);

	fclose(f);

	return 0;

}
