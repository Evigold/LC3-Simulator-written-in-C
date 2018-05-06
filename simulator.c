#include <stdio.h>

#include "bstr.h"

#include "comp.h"


void bitStringTest(void) {

	BitString bStr;
	BitString bStr1;
	BitString bStr2;
	BitString bStr3;
	BitString bStr4;

	int testValues[] = { 255, -255, -83, 83 };
	int i;
	for (i = 0; i < 4; i++) {

   	    BSTR_SetValueTwosComp(&bStr, testValues[i], 16);
	    BSTR_Display(bStr, 1);
	    printf("\n");
        printf("Value = %d\n", BSTR_GetValueTwosComp(bStr));
    }


    BSTR_SetValue(&bStr1, 4, 8);
	BSTR_SetValue(&bStr2, 10, 8);
	BSTR_Display(bStr1, 1);
	printf("\n");
	BSTR_Display(bStr2, 1);
	printf("\n");
	printf("Length = %d\n", BSTR_Length(bStr1));
	printf("Length = %d\n", BSTR_Length(bStr2));
	BSTR_Append(&bStr3, bStr1, bStr2);
    printf("Length = %d\n", BSTR_Length(bStr3));
	BSTR_Display(bStr3, 1);
	printf("\n");

	BSTR_Substring(&bStr4,bStr3,4,8);
	BSTR_Display(bStr4, 1);
	printf("\n");
}


int main(int argc, const char * argv[]) {


    Computer comp;

/************************************** */
/** The next two variables - program and programSize - */
/** allow someone using the simulator (such as a grader) */
/** to decide what program will be simulated. */
/** The simulation must load and execute */
/** instructions found in the "program" array. */
/** For grading purposes, it must be possible for me to */
/**    - paste in a different set of binary strings to replace the existing ones */
/**    - adjust the programSize variable */
/**    - recompile your program without further changes */
/**    and see the simulator load and execute the new program. */
/**    Your grade will depend largely on how well that works. */
/************************************** */

    char* program[] =
    {
     "0010000000000111",
     "0010001000000111",
     "0001010000000001",
     "0000010000000011",
     "1111000000100001",
     "0001000000111111",
     "0000111111111011",
     "1111000000100101",
     "0000000000111001",
     "1111111111010000"
	 };

    int programSize = 10;

//	BitString notInstr;

    /*  You may find it useful to play around with the */
	/*  BitString routines.  Some code that does that is provided */
    /*  in the following commented out function. */

    /*  bitStringTest(); */

/*   This is the assembly program that was compiled into the binary
     program shown above.
    .ORIG x3000

            LD   R0  START
            LD   R1  END
     TOP    ADD  R2  R0  R1
            BRZ  DONE
            OUT
            ADD  R0 R0 -1
            BRNZP TOP
     DONE   HALT

     START .FILL x39
     END   .FILL x-30

     .END
*/


//    BitString demoStr, sub;
//
//	//BSTR_SetValue(&demoStr,15,8);
//	BSTR_SetBits(&demoStr,"0000000000001111");
//
//	BSTR_Display(demoStr,1);
//
//	BSTR_Substring(&sub,demoStr,2,4);
//	BSTR_Display(sub,1);

	COMP_Init(&comp);
	COMP_Display(comp);   /* displays computer's initial configuration

	/* TODO: load the instructions in the "program" array */
    
	/* Next 3 lines are a test of NOT */
	/* Once you are confident that single instructions work, you will */
	/* want to replace this with code that loads all the instructions */
	/* from the array shown above. */

	/* following not instruction is   NOT Dest: R4, Source: R5 */
    BitString notInstr;
	BSTR_SetBits(&notInstr,"1001100101111111");
	COMP_LoadWord(&comp,0,notInstr);
    COMP_Display(comp);

	//Following add instruction is    ADD Dest: R3, sr1: R4, sr2: R2
	BitString addInstr;
	BSTR_SetBits(&addInstr, "0001011100000010");
	COMP_LoadWord(&comp, 0, addInstr);
	COMP_Display(comp);

	//Following LD instruction is    LD
	/* execute the program */
	/* During execution, the only output to the screen should be */
	/* the result of executing OUT. */
	COMP_Execute(&comp);

	/* shows final configuration of computer */
	COMP_Display(comp);
    
}


