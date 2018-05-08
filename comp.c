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



void COMP_ExecuteNot(Computer *comp) {
    BitString drBS, srBS;
    BSTR_Substring(&drBS,comp->ir,4,3);
    BSTR_Substring(&srBS,comp->ir,7,3);
    comp->reg[ BSTR_GetValue(drBS) ] = comp->reg[ BSTR_GetValue(srBS) ];
    BSTR_Invert( & comp->reg[ BSTR_GetValue(drBS)  ]  );

    //Get the value to update condition code.
    int val = BSTR_GetValue(comp->reg[ BSTR_GetValue(drBS) ] );
    //Update condition code.
    COMP_SetConditionCode(&comp->cc, val);

}

void COMP_ExecuteAdd(Computer *comp) {
    BitString immBS;
    int sum;

    //Check for immediate add
    BSTR_Substring(&immBS, comp->ir, 10, 1);

    if (BSTR_GetValue(immBS) == 1) {
        //Immediate add
        BitString drBS, srBS, imm5BS, sumBS;

        //Get register substrings
        BSTR_Substring(&drBS, comp->ir, 4, 3);
        BSTR_Substring(&srBS, comp->ir, 7, 3);
        BSTR_Substring(&imm5BS, comp->ir, 11, 5);

        //Calculate sum and set sum
        sum = BSTR_GetValue(comp->reg[ BSTR_GetValue(srBS) ])
                + BSTR_GetValue(imm5BS);
        BSTR_SetValueTwosComp(&sumBS, sum, 16);
        comp->reg[BSTR_GetValue(drBS)] = sumBS;

    } else {
        //Register add
        BitString drBS, sr1BS, sr2BS, sumBS;

        BSTR_Substring(&drBS, comp->ir, 4, 3);
        BSTR_Substring(&sr1BS, comp->ir, 7, 3);
        BSTR_Substring(&sr2BS, comp->ir, 13, 3);

        sum = BSTR_GetValue(comp->reg[ BSTR_GetValue(sr1BS) ])
                 + BSTR_GetValue(comp->reg[ BSTR_GetValue(sr2BS) ]);
        BSTR_SetValueTwosComp(&sumBS, sum, 16);
        comp->reg[ BSTR_GetValue(drBS) ] = sumBS;
    }

    //Update condition code
    COMP_SetConditionCode(&comp->cc, sum);
}


void COMP_ExecuteLD(Computer *comp) {
    BitString drBS, pcO9BS;
    int offset, ldVal;

    //Get register and offset substrings
    BSTR_Substring(&drBS, comp->ir, 4, 3);
    BSTR_Substring(&pcO9BS, comp->ir, 7, 9);

    //Get offset value
    offset = BSTR_GetValueTwosComp(pcO9BS);

    //Set value of destination regiseter to pc + offset with size 16.
    ldVal = BSTR_GetValue(comp->pc) + offset;
    BSTR_SetValue(&comp->reg[ BSTR_GetValue(drBS) ],
                   ldVal, 16);

    //Set condition code
    //TODO depends on value stored in register right (could be 0 or positive)?
    COMP_SetConditionCode(&comp->cc, ldVal);
}

void COMP_ExecuteBR(Computer *comp) {
    BitString nzp;

    BSTR_Substring(&nzp, comp->ir, 4, 3);

    //Branch if true
    if (BSTR_GetValue(nzp) == BSTR_GetValue(comp->cc)) {
        BitString pcO9BR;
        int offset;

	    BSTR_Substring(&pcO9BR, comp->ir, 7, 9);

        offset = BSTR_GetValueTwosComp(pcO9BR);

        //Get value of new memory address and jump pc to that address.
        BSTR_SetValue(&comp->pc, BSTR_GetValue(comp->pc) + offset, 16);

    }
}

//TODO finish...
void COMP_ExecuteOut(Computer *comp) {

}

//TODO finish...
void COMP_ExecuteHalt(Computer *comp) {

}

//TODO finish...
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

void COMP_SetConditionCode(BitString* ccBS, int value) {
    if (value > 0) { //P
        BSTR_SetValue(ccBS, 1, 3);
    } else if (value < 0) { //N
        BSTR_SetValue(ccBS, 4, 3);
    } else {  //Z
        BSTR_SetValue(ccBS, 2, 3);
    }
}


