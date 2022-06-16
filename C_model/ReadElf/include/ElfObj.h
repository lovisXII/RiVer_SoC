#ifndef ELF_OBJ
#define ELF_OBJ

#include <elf.h>

typedef struct Elf32_Obj_
	{
	Elf32_Ehdr Head;
	Elf32_Shdr *Shead_tab;
	Elf32_Shdr *SecNameHdr;
	char *SecName;
	Elf32_Shdr *SecTextHdr;
	uint32_t *SecText;
	Elf32_Shdr *SecDataHdr;
	uint32_t *SecData;
	Elf32_Shdr *SecBssHdr;
	Elf32_Shdr *SecSymNameHdr;
	char *SymName;
	Elf32_Shdr *SecSymHdr;
	Elf32_Sym *SecSym;
	Elf32_Shdr *SecRelHdr;
	Elf32_Rel *SecRel;
	} Elf32_Obj;

Elf32_Obj *Read_Elf32(char *file_name);

void print_header(Elf32_Obj *pObj);
void print_seg(Elf32_Obj *pObj, char *name);

int Elf32_SymAdr(Elf32_Obj *pObj, uint32_t *adr, char *name);

void Del_Elf32(Elf32_Obj *pObj);

#endif
