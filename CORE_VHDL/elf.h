#define ELF_OBJ
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

#define STACK_TOP 0x80000000
#define STACK_SIZE 4096


uint32_t GoodAdr;
uint32_t BadAdr;

typedef struct _seg
	{
	uint32_t va;
	void *ra; // mem bloc
	size_t size;
	struct _seg *next;
	}Seg;

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

static Seg *TabSeg = NULL;

uint32_t mem_goodadr()
	{
	return GoodAdr;
	}
	
uint32_t mem_badadr()
	{
	return BadAdr;
	}

void print_header(Elf32_Obj *pObj)
	{
	printf("----------- Header ---------------\n");
	printf("- type = %d\n", pObj->Head.e_type);
	printf("- machine = 0x%x\n", pObj->Head.e_machine);
	printf("- version = %d\n", pObj->Head.e_version);
	printf("- entry adr = 0x%08x\n", pObj->Head.e_entry);
	printf("- program header table offset = 0x%x (%d)\n", pObj->Head.e_phoff, pObj->Head.e_phoff);
	printf("- section header table offset = 0x%x (%d)\n", pObj->Head.e_shoff, pObj->Head.e_shoff);
	printf("- flags = 0x%08x\n", pObj->Head.e_flags);
	printf("- header size = %d\n", pObj->Head.e_ehsize);
	printf("- program header size entry = %d\n", pObj->Head.e_phentsize);
	printf("- program header entry number = %d\n", pObj->Head.e_phnum);
	printf("- section header size entry = %d\n", pObj->Head.e_shentsize);
	printf("- section header entry number = %d\n", pObj->Head.e_shnum);
	printf("- index section entry name = %d\n", pObj->Head.e_shstrndx);
	printf("----------------------------------\n");
	}
	
void print_sheader(Elf32_Obj *pObj)
	{
	int i;
	Elf32_Shdr *psh = pObj->Shead_tab;

	for (i=0; i< pObj->Head.e_shnum; i++)
		{
		printf("----------- Section %d Header ---------------\n", i);
		printf("- section name index = %d (%s)\n", psh->sh_name, pObj->SecName + psh->sh_name);
		printf("- section type = %d\n", psh->sh_type);
		printf("- section flags = 0x%08x\n", psh->sh_flags);
		printf("- section virtual adrr = 0x%x\n", psh->sh_addr);
		printf("- section file offset = 0x%x (%d)\n", psh->sh_offset, psh->sh_offset);
		printf("- section size = %d\n", psh->sh_size);
		printf("- section link = %d\n", psh->sh_link);
		printf("- section info = 0x%x\n", psh->sh_info);
		printf("- section align address = %d\n", psh->sh_addralign);
		printf("- section entry size = %d\n", psh->sh_entsize);
		printf("--------------------------------------------\n");
		psh++;
		}
	}

void print_symbols(Elf32_Obj *pObj)
	{
	int i;
	Elf32_Sym *SecSym = pObj->SecSym;
	size_t SymNub = pObj->SecSymHdr->sh_size / pObj->SecSymHdr->sh_entsize;

	printf("----------- Section symboles ---------------\n");
	for (i=0; i< SymNub; i++)
		{
		printf("-- %d --\n", i);
		printf("- symbol name index = %d", SecSym->st_name);
		printf(" (%s)\n", pObj->SymName + SecSym->st_name);
		printf("- symbol value = 0x%x (%d)\n", SecSym->st_value, SecSym->st_value);
		printf("- symbol size = %d\n", SecSym->st_size);
		printf("- symbol binding = 0x%x\n", ELF32_ST_BIND(SecSym->st_info));
		printf("- symbol type = 0x%x\n", ELF32_ST_TYPE(SecSym->st_info));
		printf("- symbol visibility = %d\n", SecSym->st_other);
		if (SecSym->st_shndx < pObj->Head.e_shnum)
			{
			printf("- symbol section index = %d", SecSym->st_shndx);
			printf(" (%s)\n", pObj->SecName + pObj->Shead_tab[SecSym->st_shndx].sh_name);
			}
		else printf("- symbol section index = %d\n", SecSym->st_shndx);
		printf("------\n");
		SecSym++;
		}
	printf("--------------------------------------------\n");
	}

void print_reloc(Elf32_Obj *pObj)
	{
	int i;
	Elf32_Rel *SecRel = pObj->SecRel;
	size_t RelNub = pObj->SecRelHdr->sh_size / pObj->SecRelHdr->sh_entsize;

	printf("----------- Section Reloc ---------------\n");
	for (i=0; i< RelNub; i++)
		{
		printf("-- %d --\n", i);
		printf("- symbol adress = 0x%08x\n", SecRel->r_offset);
		printf("- symbol index = %d\n", ELF32_R_SYM(SecRel->r_info));
		printf("- symbol type = %d\n", ELF32_R_TYPE(SecRel->r_info));
		printf("------\n");
		SecRel++;
		}
	printf("--------------------------------------------\n");
	}

void print_textdata(uint32_t *tab, uint32_t adr, size_t ssize, char *name)
	{

	printf("----------- Section %s ---------------\n", name);
	while (ssize > 0)
		{
		printf("0x%08x : 0x%08x\n", adr, *tab++);
		adr += 4; ssize -= 4;
		}
	printf("-----------------------------------------\n");
	}

Elf32_Shdr *SecByName(Elf32_Obj *pObj, char *name)
	{
	int i;
	Elf32_Shdr *psh = pObj->Shead_tab;
	for (i=0; i< pObj->Head.e_shnum; i++, psh++)
		if (strcmp(pObj->SecName + psh->sh_name, name) == 0) return psh;
	return NULL;
	}

int Elf32_SymAdr(Elf32_Obj *pObj, uint32_t *adr, char *name)
	{
	int i;
	Elf32_Sym *SecSym = pObj->SecSym;
	size_t SymNub = pObj->SecSymHdr->sh_size / pObj->SecSymHdr->sh_entsize;

	for (i=0; i< SymNub; i++)
		{
		if ( strcmp(pObj->SymName + SecSym->st_name, name) == 0)
			{
			*adr = SecSym->st_value;
			return 0;
			}
		SecSym++;
		}
	return 1;
	}


void print_seg(Elf32_Obj *pObj, char *name)
	{
	Elf32_Shdr *SecHdr;
	if (name == NULL)
		{
		print_sheader(pObj);
		return;
		}
	
	SecHdr = SecByName(pObj, name);
	if (SecHdr == NULL) return;

	if ((SecHdr == pObj->SecTextHdr) || (SecHdr == pObj->SecDataHdr))
		{
		print_textdata(pObj->SecText, SecHdr->sh_addr, SecHdr->sh_size, name);
		return;
		}

	if (SecHdr == pObj->SecSymHdr)
		{
		print_symbols(pObj);
		return;
		}

	if (SecHdr == pObj->SecRelHdr)
		{
		print_reloc(pObj);
		return;
		}

	}

Elf32_Obj *Read_Elf32(char *file_name)
	{
	FILE *felf;
	Elf32_Obj *pObj;

	if( (felf = fopen(file_name, "r")) == NULL)
		{
		fprintf(stderr, "Ouverture du fichier impossible");
		return(NULL);
		}
	
// Allocation de la structure
	pObj = malloc(sizeof(Elf32_Obj));

	pObj->SecRel = NULL;
	pObj->SecSym = NULL;
	pObj->SymName = NULL;
	pObj->SecName = NULL;
	pObj->SecData = NULL;
	pObj->SecBssHdr = NULL;
	pObj->SecText = NULL;
	pObj->Shead_tab = NULL;

// Lecture du Header
	fread( &(pObj->Head), sizeof(Elf32_Ehdr), 1, felf);

if (pObj->Head.e_shnum != 0)
	{
	// Lecture de la table des sections
		pObj->Shead_tab = malloc(pObj->Head.e_shnum * sizeof(Elf32_Shdr));
		fseek(felf, pObj->Head.e_shoff, SEEK_SET);
		fread( pObj->Shead_tab, sizeof(Elf32_Shdr), pObj->Head.e_shnum, felf);
	
	// Lecture de la table des noms de section
		pObj->SecNameHdr = pObj->Shead_tab + pObj->Head.e_shstrndx;
		pObj->SecName = malloc(pObj->SecNameHdr->sh_size);
		fseek(felf, pObj->SecNameHdr->sh_offset, SEEK_SET);
		fread(pObj->SecName, sizeof(char), pObj->SecNameHdr->sh_size, felf);
	
	

	// Lecture de la Section .text
		if((pObj->SecTextHdr = SecByName(pObj, ".text")))
			{
			pObj->SecText = malloc(pObj->SecTextHdr->sh_size);
			fseek(felf, pObj->SecTextHdr->sh_offset, SEEK_SET);
			fread(pObj->SecText, pObj->SecTextHdr->sh_size, 1,felf);
			}
	
	// Lecture de la Section .data
		if((pObj->SecDataHdr = SecByName(pObj, ".data")))
			{
			pObj->SecData = malloc(pObj->SecDataHdr->sh_size);
			fseek(felf, pObj->SecDataHdr->sh_offset, SEEK_SET);
			fread(pObj->SecData, pObj->SecDataHdr->sh_size, 1,felf);
			}

	// Lecture de la Section .bss
		pObj->SecBssHdr = SecByName(pObj, ".bss");
		//if((pObj->SecBssHdr = SecByName(pObj, ".bss")))
		//Pour cette section donnees non initialisee rien a faire
	
	// Lecture de la table des noms de symboles
		if((pObj->SecSymNameHdr = SecByName(pObj, ".strtab")))
			{
			pObj->SymName = malloc(pObj->SecSymNameHdr->sh_size * sizeof(char));
			fseek(felf, pObj->SecSymNameHdr->sh_offset, SEEK_SET);
			fread(pObj->SymName, sizeof(char), pObj->SecSymNameHdr->sh_size, felf);
			}
	
	// Lecture de la table des symboles
		if((pObj->SecSymHdr = SecByName(pObj, ".symtab")))
			{
			pObj->SecSym = malloc(pObj->SecSymHdr->sh_size);
			fseek(felf, pObj->SecSymHdr->sh_offset, SEEK_SET);
			fread(pObj->SecSym, pObj->SecSymHdr->sh_size, 1,felf);
			}
	
	// Lecture de la table de relocation
		if((pObj->SecRelHdr = SecByName(pObj, ".rel.text")))
			{
			pObj->SecRel = malloc(pObj->SecRelHdr->sh_size);
			fseek(felf, pObj->SecRelHdr->sh_offset, SEEK_SET);
			fread(pObj->SecRel, pObj->SecRelHdr->sh_size, 1,felf);
			}
	}

	fclose(felf);
	return pObj;
	}

// Liberations diverses
void Del_Elf32(Elf32_Obj *pObj)
	{
	free(pObj->SecRel);
	free(pObj->SecSym);
	free(pObj->SymName);
	free(pObj->SecName);
	free(pObj->SecData);
	free(pObj->SecText);
	free(pObj->Shead_tab);
	free(pObj);
	}


int mem_add_seg(uint32_t adr, size_t size, void *src)
//src is a mem bloc
	{
	Seg *ps = TabSeg; // on créer un pointeur sur Seg dont next pointe sur TabSeg
	Seg *new_seg = NULL;

	if (ps == NULL)
		{
		if ((TabSeg = malloc(sizeof(Seg))) == NULL) return 0; // malloc return NULL if allocation isn't possible
		if ((TabSeg->ra = malloc(size)) == NULL) return 0; // same
		if (src) memcpy(TabSeg->ra, src, size);// memcpy allow to copy a memory bloc from a source to a destination
		//void * memcpy( void * destination, const void * source, size_t size ); 
		//if src isn't Null, it's copy to TabSeg -> ra
		TabSeg->va = adr;
		TabSeg->size = size;
		TabSeg->next = NULL;
		return 1;
		}
	
	if (adr < ps->va)
		{
		if ((adr + size) > ps->va) return 0; //memory already alloc
		if ((TabSeg = malloc(sizeof(Seg))) == NULL) return 0;
		if ((TabSeg->ra = malloc(size)) == NULL) return 0;
		if (src) memcpy(TabSeg->ra, src, size);
		TabSeg->va = adr;
		TabSeg->size = size;
		TabSeg->next = ps;
		return 1;
		}
	
	while (ps->next)
		{
		if (ps->next->va > adr) break;
		ps = ps->next;
		}

	if (adr < (ps->va + ps->size)) return 0; //memory already alloc
	if (ps->next && ((adr + size) > ps->next->va)) return 0; //memory already alloc
	if ((new_seg = malloc(sizeof(Seg))) == NULL) return 0;
	if ((new_seg->ra = malloc(size)) == NULL) return 0;
	if (src) memcpy(new_seg->ra, src, size);
	new_seg->va = adr;
	new_seg->size = size;
	new_seg->next = ps->next;
	ps->next = new_seg;
	return 1;
	}



uint32_t mem_lw(uint32_t adr)
	{
	Seg *ps = TabSeg;

	if (adr & 3)
		{
		fprintf(stderr, "Mem Error lw adr unalign\n");
		return 0;
		}
	
	while (ps)
		{
		if ((adr >= ps->va) && (adr < ps->va + ps->size))
			{
			return *(uint32_t *) (ps->ra + (adr - ps->va));
			}
		if (adr < ps->va) break;
		ps = ps->next;
		}
	fprintf(stderr, "Mem LW Error adr does not exist: 0x%x\n", adr);
	return 0;
	}

typedef struct _File_read{
	Elf32_Obj *pObj_struct;
	uint32_t start_adr ;
	uint32_t kernel_start_adr ;
	int size ;

}FILE_READ;

FILE_READ init_mem(char *Elf_file)
	{

	// size argument is the number instruction in byte
	// 1 instruction = 4 byte 

	//	We count the number of instruction per section and we increment size which will 
	// indicate the number of instruction
	FILE_READ return_structure;
	Elf32_Obj *pObj = NULL ;
	int adr_start = 0 ;
	int size = 0 ;

	if ((pObj = Read_Elf32(Elf_file)) == NULL)
		{
		fprintf(stderr, "Erreur chargement elf\n");
		exit(1);
		}
	
	// Chargement memoire
	// Section text
	if (pObj->SecText)
		if (mem_add_seg(pObj->SecTextHdr->sh_addr, pObj->SecTextHdr->sh_size, pObj->SecText) == 1){
			fprintf(stdout, "Chargement du segment .text adr = 0x%x\n", pObj->SecTextHdr->sh_addr);
			adr_start = (uint32_t) pObj->SecTextHdr->sh_addr;
			size += (uint32_t) pObj->SecTextHdr->sh_size;
			}

	// Section data
	if (pObj->SecData)
		if (mem_add_seg(pObj->SecDataHdr->sh_addr, pObj->SecDataHdr->sh_size, pObj->SecData) == 1){
			fprintf(stdout, "Chargement du segment .data adr = 0x%x\n", pObj->SecDataHdr->sh_addr);
			size += (uint32_t) pObj->SecDataHdr->sh_size;
			printf(".data size : %d", size) ;
			}
	
	// Section bss
	if (pObj->SecBssHdr)
		if (mem_add_seg(pObj->SecBssHdr->sh_addr, pObj->SecBssHdr->sh_size, NULL) == 1){
			fprintf(stdout, "Chargement du segment .bss adr = 0x%x\n", pObj->SecBssHdr->sh_addr);
			}


	// Search good and bad symbols adr
	if (Elf32_SymAdr(pObj, &GoodAdr, "_good") == 0){
		fprintf(stderr, "Symbol _good found at adr=%x\n", GoodAdr);
		}

	if (Elf32_SymAdr(pObj, &BadAdr, "_bad") == 0){
		fprintf(stderr, "Symbol _bad found at adr=%x\n", BadAdr);
		}
	Del_Elf32(pObj);
	

	return_structure.pObj_struct = pObj ;
	return_structure.start_adr = adr_start ;
	return_structure.size = size ;
	return return_structure ;
	}
