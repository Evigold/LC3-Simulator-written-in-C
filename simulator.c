/*
 * Scott Robertson, Evi Goldschmidt
 * TCSS 371C Assignment 4
 * Scott: Jetbrains CLion IDE
 * Evi:   Nano editor and gdb
 */

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


	// N - 4E
	// Z - 5A
	// P - 50
	// F - 46


	//  TESTS


	// Test for TRAP HALT
	//	TRAP: x25

/*	char* program[] = {"1111000000100101"};
	int programSize = 1;
*/
	// Result:

	// Test for ADD
	//	ADD: R3 R3 #24	(Add #24 adn the value stored in R4 and store in R3)
/*	char * program[] = 
		{
		"0001011011111000",
		"1111000000100101"
		};
*/
	//Results: R3 should store #27 or ""0000 0000 0001 1011"
	//	CC: 001


	// Test for ADD
	//	ADD: R2 R4 R1	(Add values of R4 and R1 and store in R2)
/* 	char * program[] = 
		{
		"0001010100000001",
		"1111000000100101"
		};
*/
	// Results: R2 should store #5 or ""0000 0000 0000 0101"
	//	CC: 001


	// Test for ADD with negative outcome
	//	ADD: R0 R5 #-9	(Add #-9 and the value stored in R5 and store in R0)
/*	char * program[] = 
		{
		 "000100010110111",
		"1111000000100101"
		};
*/
	// Results: R0 should store #-4 or "1111 1111 1111 1100"
	//	CC: 100

	// Test for ADD with outcome of zero
	//	ADD: R6 R7 #-7	(Add #-7 and the value stored in R7 and store in R6)
//	BSTR_SetBits(&test, "000111011111001");
	// Results: R6 should store #0 or "0000 0000 0000 0000"
	//	CC: 010

	// Test for LD
	// 	LD: R3 0	(Store value in pc + offset of 0 in R3)
	//	#33		(Stores #33 in pc)
//	BSTR_SetBits(&test, "0010011000000000");
//	BSTR_SetBits(&testb, "0000000000100001");
	// Result: R3 should store #33 or "0000 0000 0010 0001"
	//	CC: 001



	// Test for BRp
	//	BR: p #1
	//	LD: R0 #1
	//	LD: R0 #1
	//	x46 ('F' for fail)
	//	x4E ('P' for positive)

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
	COMP_Display(comp);   /* displays computer's initial configuration */


    int i;
    //Loads program array into memory
    for (i = 0; i < programSize; i++) {
        BitString word;
        BSTR_SetBits(&word, program[i]);
        COMP_LoadWord(&comp, i, word);
    }


<<<<<<< HEAD
	/* Next 3 lines are a test of NOT */
	/* Once you are confident that single instructions work, you will */
	/* want to replace this with code that loads all the instructions */
	/* from the array shown above. */

	/* following not instruction is   NOT Dest: R4, Source: R5 
    BitString notInstr;
	BSTR_SetBits(&notInstr,"1001100101111111");
	COMP_LoadWord(&comp,0,notInstr);
    COMP_Display(comp);

	//Following add instruction is    ADD Dest: R3, sr1: R4, sr2: R2
	BitString addInstr;
	BSTR_SetBits(&addInstr, "0001011100000010");
	COMP_LoadWord(&comp, 0, addInstr);
	COMP_Display(comp);
=======
//	/* Next 3 lines are a test of NOT */
//	/* Once you are confident that single instructions work, you will */
//	/* want to replace this with code that loads all the instructions */
//	/* from the array shown above. */
//
//	/* following not instruction is   NOT Dest: R4, Source: R5 */
//    BitString notInstr;
//	BSTR_SetBits(&notInstr,"1001100101111111");
//	COMP_LoadWord(&comp,0,notInstr);
//    COMP_Display(comp);
//
//	//Following add instruction is    ADD Dest: R3, sr1: R4, sr2: R2
//	BitString addInstr;
//	BSTR_SetBits(&addInstr, "0001011100000010");
//	COMP_LoadWord(&comp, 0, addInstr);
//	COMP_Display(comp);
>>>>>>> Scott

	//Following LD instruction is    LD
	/* execute the program */
	/* During execution, the only output to the screen should be */
	/* the result of executing OUT. */
	COMP_Execute(&comp);

	/* shows final configuration of computer */
	COMP_Display(comp);



}


