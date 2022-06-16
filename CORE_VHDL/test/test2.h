#include <elf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Elf32_Obj_{
	// Header :
	Elf32_Ehdr 	Head;
	// Symbol header tab : 			
	Elf32_Shdr 	*Shead_tab;
	// Section Header :
	Elf32_Shdr 	*SecNameHdr;
	char 	   	*SecName;	
	// General Section :
	Elf32_Shdr 	**Section_Hdr;	
	uint32_t   	**Sec;			
	char 	   	**Section_name ;
	// .strtab section :	
	Elf32_Shdr 	*SecSymNameHdr;
	Elf32_Sym   *SecSym;
	//.symtab section :
	Elf32_Shdr 	*SecSymHdr;
	char 		*SymName;
}Elf32_Obj;


typedef struct _seg
{
	uint32_t va;
	void *ra; // mem bloc
	size_t size;
	struct _seg *next;
}Seg;


typedef struct _File_read{
	Elf32_Obj *pObj_struct;
	uint32_t start_adr ;
	uint32_t kernel_start_adr ;
	int size ;

}FILE_READ;

static Seg *TabSeg = NULL;

uint32_t GoodAdr;
uint32_t BadAdr;

uint32_t mem_goodadr()
{
	return GoodAdr;
}
	
uint32_t mem_badadr()
{
	return BadAdr;
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
int mem_add_seg(uint32_t adr, size_t size, void *src)
// take the adress of the segment
// the size of the segement
// a source buffer
{
	Seg *ps = TabSeg; 
	Seg *new_seg = NULL;

	if (ps == NULL)
		{
		if ((TabSeg = malloc(sizeof(Seg))) == NULL) return 0; // malloc return NULL if allocation isn't possible
		if ((TabSeg->ra = malloc(size)) == NULL) return 0; // same
		if (src) memcpy(TabSeg->ra, src, size);// memcpy allow to copy a memory bloc from a source to a destination
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

FILE_READ* Read_Elf32(char *file_name)
{
 	FILE *felf;
	Elf32_Obj *pObj;

	// Structure that will be return 
	FILE_READ* return_structure =(FILE_READ*) malloc(sizeof(FILE_READ));
	int adr_start = 0 ;
	int adr_kernel = 0 ;
	int size = 0 ;
	printf("Receiving file %s, starting parsing and reading\n", file_name) ;
	if( (felf = fopen(file_name, "r")) == NULL)
	{
		fprintf(stderr, "Ouverture du fichier impossible");
		exit(1) ;
	}
	pObj = malloc(sizeof(Elf32_Obj));
	pObj->Shead_tab = NULL;
	
	// Lecture du header
	printf("Starting Header Reading\n") ;
	fread( &(pObj->Head), sizeof(Elf32_Ehdr), 1, felf);
	
	//Fseek allows to place the read cursor of the file at the position of the section table

	if (pObj->Head.e_shnum != 0)
	{
		// Lecture de la table des sections
			printf("Reading section table\n") ;
			pObj->Shead_tab = malloc(pObj->Head.e_shnum * sizeof(Elf32_Shdr));
			fseek(felf, pObj->Head.e_shoff, SEEK_SET);
			fread( pObj->Shead_tab, sizeof(Elf32_Shdr), pObj->Head.e_shnum, felf);


		// Lecture de la table des noms de section
			printf("Reading section name table\n") ;
			pObj->SecNameHdr = pObj->Shead_tab + pObj->Head.e_shstrndx;
			pObj->SecName = malloc(pObj->SecNameHdr->sh_size);
			fseek(felf, pObj->SecNameHdr->sh_offset, SEEK_SET);
			fread(pObj->SecName, sizeof(char), pObj->SecNameHdr->sh_size, felf);

		Elf32_Shdr *psh = pObj->Shead_tab;

		// Initialise to zero to avoid issue

		pObj->Section_name 	= calloc((pObj->Head.e_shnum)*sizeof(char),(pObj->Head.e_shnum)*sizeof(char)) ;
		pObj->Section_Hdr	= calloc((pObj->Head.e_shnum)*sizeof(Elf32_Shdr),(pObj->Head.e_shnum)*sizeof(Elf32_Shdr));
		pObj->Sec			= calloc((pObj->Head.e_shnum)*sizeof(uint32_t),(pObj->Head.e_shnum)*sizeof(uint32_t));

		printf("**********Starting mem initialization**********\n") ;

		for (int i=0; i< pObj->Head.e_shnum; i++, psh++)
		{
			pObj->Section_name[i] = pObj->SecName + psh->sh_name ;
			if(strcmp(pObj->Section_name[i],".symtab") == 0 ){
				pObj-> SecSymHdr = psh;
				printf("Section found : (%s) at adr : 0x%x\n", pObj->Section_name[i], pObj->SecSymHdr->sh_addr);
				pObj-> Section_Hdr[i] = psh; // to avoid segfault
			}
			else if (strcmp(pObj->Section_name[i],".strtab") == 0 ){
				pObj -> SecSymNameHdr = psh ;
				printf("Section found : (%s) at adr : 0x%x\n", pObj->Section_name[i], pObj->SecSymHdr->sh_addr);
				pObj-> Section_Hdr[i] = psh; // to avoid segfault
			}
			else{
				pObj-> Section_Hdr[i] = psh;
				printf("Section found : (%s) at adr : 0x%x\n", pObj->Section_name[i], pObj->Section_Hdr[i]->sh_addr);
			}

		pObj->Sec[i]			=(uint32_t*) malloc(pObj->Section_Hdr[i]->sh_size);
		fseek(felf, pObj->Section_Hdr[i]->sh_offset, SEEK_SET);
		fread(pObj->Sec[i], pObj->Section_Hdr[i]->sh_size, 1,felf);
		}
		printf("**********Ending mem initialization**********\n") ;
		pObj->SymName = malloc(pObj->SecSymNameHdr->sh_size * sizeof(char));
		fseek(felf, pObj->SecSymNameHdr->sh_offset, SEEK_SET);
		fread(pObj->SymName, sizeof(char), pObj->SecSymNameHdr->sh_size, felf);
			

		pObj->SecSym = malloc(pObj->SecSymHdr->sh_size);
		fseek(felf, pObj->SecSymHdr->sh_offset, SEEK_SET);
		fread(pObj->SecSym, pObj->SecSymHdr->sh_size, 1,felf);


		printf("**********Loading Segments into memory **********\n") ;
		// Sections loading
		for (int i=0; i< pObj->Head.e_shnum; i++)
		{
			if(pObj->Section_Hdr[i]->sh_addr != 0)
			{
				if(mem_add_seg(pObj->Section_Hdr[i]->sh_addr, pObj->Section_Hdr[i]->sh_size, pObj->Sec[i]) == 1){
					fprintf(stdout, "Chargement du segment %s adr = 0x%8x\n", pObj->Section_name[i], pObj->Section_Hdr[i]->sh_addr);
					size+= (uint32_t) pObj->Section_Hdr[i]->sh_size;
				}
				else{
					printf("Failed to load segment %s adr = 0x%8x\n", pObj->Section_name[i], pObj->Section_Hdr[i]->sh_addr);
					exit(1) ;
				}
			}
			if(strcmp(pObj->Section_name[i],".text") == 0)
				adr_start = (uint32_t) pObj->Section_Hdr[i]->sh_addr;
			if(strcmp(pObj->Section_name[i],".kernel") == 0)
				adr_kernel = (uint32_t) pObj->Section_Hdr[i]->sh_addr;
		}

		// Search good and bad symbols adr
		if (Elf32_SymAdr(pObj, &GoodAdr, "_good") == 0){
			fprintf(stderr, "Found _good at : 0x%8x\n", GoodAdr);
			}

		if (Elf32_SymAdr(pObj, &BadAdr, "_bad") == 0){
			fprintf(stderr, "Found _bad at : 0x%8x\n", BadAdr);
		}

		return_structure->start_adr 		= adr_start  ;
		return_structure->kernel_start_adr 	= adr_kernel ;
		return_structure->size				= size ;
		return_structure->pObj_struct		= pObj ;

		if(return_structure == NULL){
			printf("Structure return is null\n");
		}
		return return_structure ; 
	
	}
	else{
		printf("Error, the file doesnt contain any sections\n"); 
		return NULL ;
	}
}

// Liberations diverses
void Del_Elf32(Elf32_Obj *pObj)
{
	// free(pObj->Shead_tab);
	// free(pObj->SecName);
	// free(pObj->Section_Hdr);
	// free(pObj->Sec);
	// free(pObj->Section_name);
	// free(pObj->SecSymNameHdr);
	// free(pObj->SecSym);
	// free(pObj->SecSymHdr);
	// free(pObj->SymName);
	// free(pObj);
}