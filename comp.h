/*
 * Scott Robertson, Evi Goldschmidt
 * TCSS 371C Assignment 4
 * Scott: Jetbrains CLion IDE
 * Evi:   Nano editor and gdb
 */



#ifndef COMP
#define COMP

#include "bstr.h"

#define MAXMEM 50

struct compType {
	BitString reg[8];
    BitString mem[MAXMEM];
	BitString pc;
    BitString ir;
	BitString cc;  /* condition code */
};

typedef struct compType Computer;

/* initialized the computer  */
void COMP_Init(Computer*);

/* displays its current contents - register and memory */
void COMP_Display(Computer);

/* put a word (either data or instruction) into the computer's */
/* memory at address addr */
void COMP_LoadWord(Computer* comp, int addr, BitString word);

/* execute the computer's stored program */
/* starting at PC = 0 */
/* and until the program executes a HALT */
void COMP_Execute(Computer* comp);

/* Performs Not operation */
void COMP_ExecuteNot(Computer *comp);

/* Performs Add operation. */
/* Adds either two registers or one register and an immediate value,
/* stores in destination register. */
void COMP_ExecuteAdd(Computer *comp);

/* Performs LD operation. */
/* Loads value in memory into destination register. */
void COMP_ExecuteLD(Computer *comp);

/* Performs branch operation. */
/* Examines nzp bits to determine if branch should be taken. */
void COMP_ExecuteBR(Computer *comp);

/* Performs Out operation. */
/* Prints value of register 0 to console. */
void COMP_ExecuteOut(Computer *comp);

/* Grabs trap vector from a trap instruction. */
int COMP_GetTrapVector(BitString trapVect8);

/* Performs and operation. */
/* Ands either two registers or one register and an immediate value, */
/* stores in destination register. */
void COMP_ExecuteAnd(Computer *comp);

/* Performs indirect load operation. */
/* Loads data from pc's address plus offset. */
void COMP_ExecuteLDI(Computer *comp);

/* Performs indirect store operation.*/
/* Stores data to pc's address plus offset. */
void COMP_ExecuteSTI(Computer *comp);

/* Performs base register load operation. */
/* Loads data to address in source register plus offset. */
void COMP_ExecuteLDR(Computer *comp);


/* Performs base register store operation. */
/* Stores data to address in source register plus offset. */
void COMP_ExecuteSTR(Computer *comp);

/* Updates the nzp condition code. */
/* Sets corresponding nzp bit to 1 depending on value. */
void COMP_SetConditionCode(BitString* nzp, int value);

#endif

