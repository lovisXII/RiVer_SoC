#include <stdlib.h>
#include <stdio.h>
#include "ElfObj.h"

int main(int argc, char *argv[])
	{
	Elf32_Obj *pObj = NULL;

	if (argc != 2)
		{
		fprintf(stderr, "Usage : read_elf Fichier_ELF\n");
		return(1);
		}
	
	pObj = Read_Elf32(argv[1]);

	print_header(pObj);
	print_seg(pObj, NULL);
	print_seg(pObj, ".text");
	/*
	print_seg(pObj, ".data");
	print_seg(pObj, ".symtab");
	print_seg(pObj, ".rel.text");
	*/

	Del_Elf32(pObj);
	return 0;
	}
