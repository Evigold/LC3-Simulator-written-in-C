/*
 * Scott Robertson, Evi Goldschmidt
 * TCSS 371C Assignment 4
 * Scott: Jetbrains CLion IDE
 * Evi:   Nano editor and gdb
 */

#include <stdio.h>
#include "bstr.h"
#include "comp.h"

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
        sum = BSTR_GetValueTwosComp(comp->reg[ BSTR_GetValue(srBS) ])
                + BSTR_GetValueTwosComp(imm5BS);
        BSTR_SetValueTwosComp(&sumBS, sum, 16);
        comp->reg[BSTR_GetValue(drBS)] = sumBS;

    } else {
        //Register add
        BitString drBS, sr1BS, sr2BS, sumBS;

        BSTR_Substring(&drBS, comp->ir, 4, 3);
        BSTR_Substring(&sr1BS, comp->ir, 7, 3);
        BSTR_Substring(&sr2BS, comp->ir, 13, 3);

        sum = BSTR_GetValueTwosComp(comp->reg[ BSTR_GetValue(sr1BS) ])
                 + BSTR_GetValueTwosComp(comp->reg[ BSTR_GetValue(sr2BS) ]);
        BSTR_SetValueTwosComp(&sumBS, sum, 16);
        comp->reg[ BSTR_GetValue(drBS) ] = sumBS;
    }

    //Update condition code
    COMP_SetConditionCode(&comp->cc, sum);
}


void COMP_ExecuteLD(Computer *comp) {
    BitString drBS, pcO9BS, ldValBit;
    int offset, ldVal;

    //Get register and offset substrings
    BSTR_Substring(&drBS, comp->ir, 4, 3);
    BSTR_Substring(&pcO9BS, comp->ir, 7, 9);

    //Get offset value
    offset = BSTR_GetValueTwosComp(pcO9BS);

    //Set value of destination regiseter to pc + offset with size 16.
    ldVal = BSTR_GetValue(comp->pc) + offset;
    BSTR_Substring(&ldValBit, comp->mem[ldVal], 0 ,16);
    BSTR_SetValue(&comp->reg[ BSTR_GetValue(drBS) ],
                   BSTR_GetValue(ldValBit), 16);

    //Set condition code
    COMP_SetConditionCode(&comp->cc, ldVal);
}

void COMP_ExecuteBR(Computer *comp) {
    BitString nzp;

    BSTR_Substring(&nzp, comp->ir, 4, 3);

	// With bit AND opperataion, if NZP from IR AND from CC != 0, 
	//then they share a 1.
    unsigned int nzpIr, nzpCc, temp;
    nzpIr = BSTR_GetValue(nzp);
    nzpCc = BSTR_GetValue(comp->cc);
    temp = nzpIr & nzpCc;
	
    //Branch if true
    if (temp != 0) {
        BitString pcO9BR;
        int offset, pcAD;

        BSTR_Substring(&pcO9BR, comp->ir, 7, 9);

        offset = BSTR_GetValueTwosComp(pcO9BR);
	pcAD = offset;
	pcAD += BSTR_GetValue(comp->pc);
        //Get value of new memory address and jump pc to that address.
	BSTR_SetValue(&comp->pc, pcAD, 16);
    }
}

void COMP_ExecuteOut(Computer *comp) {
    BitString tempBS, outBS;
    int n;
    //What character to grab? Last 8 or first 8, all 16 and let overflow??
//    tempBS = comp->reg[0];
//    BSTR_Substring(&outBS, tempBS, 8, 8);

    n = BSTR_GetValue(comp->reg[0]);
    printf("%c", n);
}

void COMP_ExecuteAnd(Computer *comp) {
    BitString immBS;
    int and;

    //Check for immediate AND
    BSTR_Substring(&immBS, comp->ir, 10, 1);

    if (BSTR_GetValue(immBS) == 1) {
        //Immediate AND
        BitString drBS, srBS, imm5BS, andBS;

        //Get register substrings
        BSTR_Substring(&drBS, comp->ir, 4, 3);
        BSTR_Substring(&srBS, comp->ir, 7, 3);
        BSTR_Substring(&imm5BS, comp->ir, 11, 5);

        //Calculate AND and set AND
        and = BSTR_GetValueTwosComp(comp->reg[ BSTR_GetValue(srBS) ])
              & BSTR_GetValueTwosComp(imm5BS);
        BSTR_SetValueTwosComp(&andBS, and, 16);
        comp->reg[BSTR_GetValue(drBS)] = andBS;

    } else {
        //Register AND
        BitString drBS, sr1BS, sr2BS, andBS;

        BSTR_Substring(&drBS, comp->ir, 4, 3);
        BSTR_Substring(&sr1BS, comp->ir, 7, 3);
        BSTR_Substring(&sr2BS, comp->ir, 13, 3);

        and = BSTR_GetValue(comp->reg[ BSTR_GetValue(sr1BS) ])
              & BSTR_GetValue(comp->reg[ BSTR_GetValue(sr2BS) ]);
        BSTR_SetValueTwosComp(&andBS, and, 16);
        comp->reg[ BSTR_GetValue(drBS) ] = andBS;
    }

    //Update condition code
    COMP_SetConditionCode(&comp->cc, and);
}

void COMP_ExecuteLDI(Computer *comp) {
    BitString drBS, pcO9BS;
    int offset, address;

    //First address is located from pc, then load from that address

    //Get register and offset substrings
    BSTR_Substring(&drBS, comp->ir, 4, 3);
    BSTR_Substring(&pcO9BS, comp->ir, 7, 9);

    //Get offset value
    offset = BSTR_GetValueTwosComp(pcO9BS);

    //Set value of destination register to pc + offset with size 16.
    address = BSTR_GetValue(comp->pc) + offset;
    //Set value of destination register with data in address.
    BSTR_SetValue(&comp->reg[ BSTR_GetValue(comp->mem[ address ]) ],
                  address, 16);

    //Set condition code
    COMP_SetConditionCode(&comp->cc, address);
}

void COMP_ExecuteSTI(Computer *comp) {
    BitString srcBS, pcO9BS;
    int offset, address;

    BSTR_Substring(&srcBS, comp->ir, 4, 3);
    BSTR_Substring(&pcO9BS, comp->ir, 7, 9);

    offset = BSTR_GetValueTwosComp(pcO9BS);

    address = BSTR_GetValue(comp->pc) + offset;

    comp->mem[address] = comp->reg[BSTR_GetValue(srcBS)];

}

void COMP_ExecuteLDR(Computer *comp) {
    BitString drBS, baseBS, pcO6BS;
    int offset, ldVal;

    //Get register and offset substrings
    BSTR_Substring(&drBS, comp->ir, 4, 3);
    BSTR_Substring(&baseBS, comp->ir, 7, 3);
    BSTR_Substring(&pcO6BS, comp->ir, 10, 6);

    offset = BSTR_GetValueTwosComp(pcO6BS);

    ldVal = BSTR_GetValue(comp->reg[ BSTR_GetValue(baseBS) ]) + offset;

    BSTR_SetValue(&comp->reg[BSTR_GetValue(drBS)], ldVal, 16);

    //Set condition code
    COMP_SetConditionCode(&comp->cc, ldVal);
}

void COMP_ExecuteSTR(Computer *comp) {
    BitString srcBS, baseBS, pcO6BS;
    int offset, address;

    BSTR_Substring(&srcBS, comp->ir, 4, 3);
    BSTR_Substring(&baseBS, comp->ir, 7, 3);
    BSTR_Substring(&pcO6BS, comp->ir, 10, 6);

    offset = BSTR_GetValueTwosComp(pcO6BS);

    //Address = reg[base] + offset
    address = BSTR_GetValue(comp->reg[BSTR_GetValue(baseBS)]) + offset;

    comp->mem[address] = comp->reg[BSTR_GetValue(srcBS)];
}

int COMP_GetTrapVector(BitString trapVect8) {
    int returnValue;
    returnValue = BSTR_GetValue(trapVect8);
    return returnValue;
}

void COMP_Execute(Computer* comp) {
    BitString opCode;

    //Integer i makes sure memory out of scope isn't addressed.
    //Exits loop when exit != 0;
    int opCodeInt, i, exit = 0;

    i = BSTR_GetValue(comp->pc);
    while (i < MAXMEM && exit == 0) {
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
            //x25 = halt = 37
            //x21 = out  = 33

            BitString trapBS;
            BSTR_Substring(&trapBS, comp->ir, 8, 8);
            int n = COMP_GetTrapVector(trapBS);

            if (n == 37) {
                /* Exit execute loop */
                exit = 1;
            } else if (n == 33) {
                COMP_ExecuteOut(comp);
            }

        //Extra credit branching
        } else if (opCodeInt == 5) {
            COMP_ExecuteAnd(comp);
        } else if (opCodeInt == 10) {
            COMP_ExecuteLDI(comp);
        } else if (opCodeInt == 11) {
            COMP_ExecuteSTI(comp);
        } else if (opCodeInt == 6) {
            COMP_ExecuteLDR(comp);
        } else if (opCodeInt == 7) {
            COMP_ExecuteSTR(comp);
        }
        //Assign pc's current value to i.
        i = BSTR_GetValue(comp->pc);
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


