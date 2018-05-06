#include <stdio.h>
#include "bstr.h"
#include "comp.h"
#include <string.h>

void COMP_Init(Computer *cmp) {
    int r, m;
    BSTR_SetValue(&(cmp->pc),0,16);
    BSTR_SetValue(&(cmp->ir),0,16);
    BSTR_SetValue(&(cmp->cc),0,3);
    for (r = 0; r < 8; r++) {
        BSTR_SetValue(&(cmp->reg[r]),r,16);  /* put some interesting data in registers */
    }
    for (m = 0; m < MAXMEM; m++) {
        BSTR_SetValue(&(cmp->mem[m]),0,16);
    }
}



void COMP_LoadWord(Computer* comp, int addr, BitString word) {
    comp->mem[addr] = word;
}


// TODO - Missing Piece: The Condition Code is not set.
void COMP_ExecuteNot(Computer *comp) {
    BitString drBS, srBS;
    BSTR_Substring(&drBS,comp->ir,4,3);
    BSTR_Substring(&srBS,comp->ir,7,3);
    comp->reg[ BSTR_GetValue(drBS) ] = comp->reg[ BSTR_GetValue(srBS) ];
    BSTR_Invert( & comp->reg[ BSTR_GetValue(drBS)  ]  );
    int val = BSTR_GetValue(comp->reg[ BSTR_GetValue(drBS) ] );
    char bits[3];
    if (val > 0) {
	strcpy(bits, "001");
    } else if (val < 0) {
	strcpy(bits, "100");
    } else {
	strcpy(bits, "010");
    }
    BSTR_SetBits(&comp.cc, bits);

}

void COMP_ExecuteAdd(Computer *comp) {
    BitString immBS;
    BSTR_Substring(&immBS, comp->ir, 5, 1);

    if (BSTR_GetValue(immBS) == 1) {
        //Do an immediate add
        BitString drBS, srBS, immBS, sumBS;
        int sum;

        BSTR_Substring(&drBS, comp->ir, 9, 3);
        BSTR_Substring(&srBS, comp->ir, 6, 3);
        BSTR_Substring(&immBS, comp->ir, 0, 5);

        sum = BSTR_GetValue(comp->reg[ BSTR_GetValue(srBS) ])
                + BSTR_GetValue(immBS);
        BSTR_SetValueTwosComp(&sumBS, sum, 16);
        comp->reg[BSTR_GetValue(drBS)] = sumBS;
    } else {
        //Register add
        BitString drBS, sr1BS, sr2BS, sumBS;
        int sum;

        BSTR_Substring(&drBS, comp->ir, 9, 3);
        BSTR_Substring(&sr1BS, comp->ir, 6, 3);
        BSTR_Substring(&sr2BS, comp->ir, 0, 3);
        sum = BSTR_GetValue(comp->reg[ BSTR_GetValue(sr1BS) ])
                 + BSTR_GetValue(comp->reg[ BSTR_GetValue(sr2BS) ]);
        BSTR_SetValueTwosComp(&sumBS, sum, 16);
        comp->reg[ BSTR_GetValue(drBS) ] = sumBS;
    }


}

void COMP_ExecuteLD(Computer *comp) {

}

void COMP_ExecuteBR(Computer *comp) {

}

void COMP_ExecuteOut(Computer *comp) {

}

void COMP_ExecuteHalt(Computer *comp) {

}

void COMP_Execute(Computer* comp) {
    BitString opCode;
    int opCodeInt;
    
    /* use the PC to load current instruction from memory into IR */
    comp->ir = comp->mem[BSTR_GetValue(comp->pc)];
    
    BSTR_AddOne(&comp->pc);
    
    BSTR_Substring(&opCode,comp->ir,0,4);  /* isolate op code */
    opCodeInt = BSTR_GetValue(opCode); /* get its value */

    /*what kind of instruction is this? */
    if (opCodeInt == 9) {   // NOT
        COMP_ExecuteNot(comp);
    } else if (opCodeInt == 1) {
        COMP_ExecuteAdd(comp);
    } else if (opCodeInt == 2) {
        COMP_ExecuteLD(comp);
    } else if (opCodeInt == 0) {
        COMP_ExecuteBR(comp);
    } else if (opCodeInt == 15) {
        //TODO PLACEHOLDER CONDIION?? CALL out and halt here
        COMP_ExecuteHalt(comp);
        COMP_ExecuteOut(comp);
    }
}


void COMP_Display(Computer cmp) {
    int r, m;
    printf("\n");
    
    printf("PC ");
    BSTR_Display(cmp.pc,1);
    printf("   ");
    
    
    printf("IR ");
    BSTR_Display(cmp.ir,1);
    printf("   ");
    
    
    printf("CC ");
    BSTR_Display(cmp.cc,1);
    printf("\n");
    
    
    for (r = 0; r < 8; r++) {
        printf("R%d ",r);
        BSTR_Display(cmp.reg[r], 1);
        if (r % 3 == 2)
            printf("\n");
        else
            printf("   ");
    }
    printf("\n");
    for (m = 0; m < MAXMEM; m++) {
        printf("%3d ",m);
        BSTR_Display(cmp.mem[m], 1);
        
        if (m % 3 == 2)
            printf("\n");
        else
            printf("    ");
    }
    printf("\n");
}

