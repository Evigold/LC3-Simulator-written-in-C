/*
 * Scott Robertson, Evi Goldschmidt
 * TCSS 371C Assignment 4
 * Scott: Jetbrains CLion IDE
 * Evi:   Nano editor and gdb
 * Collaborated on private github repo
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
	//	TRAP : x25
/*
	char* program[] = {"1111000000100101"};
	int programSize = 1;
*/
	// Result : All registered should be loaded with theri equivalent value (R0:0, R5:5)
	//	CC : 000
// PASSED

	// Test for ADD
	//	ADD : R3 R3 #12	(Add #12 and the value stored in R4 and store in R3)
	//	TRAP : x25	(HALT)
/*	char * program[] =
		{
		"0001011011101100",
		"1111000000100101"
		};
	int programSize = 2;
*/

	//Results: R3 should store #15 or ""0000 0000 0000 1111"
	//	CC: 001
// PASSED

	// Test for ADD
	//	ADD : R2 R4 R1	(Add values of R4 and R1 and store in R2)
	//	TRAP : x25	(HALT)
/*	char * program[] =
		{
		"0001010100000001",
		"1111000000100101"
		};
	int programSize = 2;
*/
	// Results: R2 should store #5 or ""0000 0000 0000 0101"
	//	CC: 001
// PASSED

	// Test for ADD with negative outcome
	//	ADD : R0 R5 #-9	(Add #-9 and the value stored in R5 and store in R0)
	//	TRAP : x25	(HALT)
/*	char * program[] =
		{
		"0001000101110111",
		"1111000000100101"
		};
	int programSize = 2;
*/
	// Results: R0 should store #-4 or "1111 1111 1111 1100"
	//	CC: 100
// PASSED

	// Test for ADD with outcome of zero
	//	ADD : R6 R7 #-7	(Add #-7 and the value stored in R7 and store in R6)
	//	TRAP : x25	(HALT)
/*	char * program[] =
		{
		"0001110111111001",
		"1111000000100101"
		};
	int programSize = 2;
*/
	// Results : R6 should store #0 or "0000 0000 0000 0000"
	//	CC : 010
// PASSED

	// Test for LD
	// 	LD : R3 VAL	(Store value in pc + offset of 0 in R3)
	// VAL     :  #33	(Stores #33 in pc)
	//	TRAP : x25	(HALT)
/*	char * program[] =
		{
		"0010011000000000",
		"0000000000100001",
		"1111000000100101"
		};
	int programSize = 3;
*/
	// Result: R3 should store #33 or "0000 0000 0010 0001"
	//	CC: 001
// PASSED

	// Test for TRAP x23 (OUT)
	//	LD : R0 PKEY
	//	TRAP : x21	(OUT)
	//	TRAP : x25	(HALT)
	// PKEY      :  x50	('P' for Pass)
/*	char * program[] =
		{
		"0010000000000010",
		"1111000000100001",
		"1111000000100101",
		"0000000001010000"
		};
	int programSize = 4;
*/
	// Results : Print to console 'P'
	//	CC : 001
// PASSED

	// Test for BRp
	//	LD : R0 NKEY
	//	ADD : R0 R0 #1
	//	ADD : R2 R2 #-1
	//	BRp : #-3
	//	TRAP : x21	(OUT)
	//	TRAP : x25	(HALT)
	// NKEY	     : x4E	('P' for pass)
/*	char * program[] =
		{
		"0010000000000101",
		"0001000000100001",
		"0001010010111111",
		"0000001111111101",
		"1111000000100001",
		"1111000000100101",
		"0000000001001110"
		};
	int programSize = 7;
*/
	// Results : Should print P for pass, if N, then failed.
	//	CC : 010

	// Test for LDI
	//	LDI : R0 ADRS
	//	TRAP : x21	(OUT)
	//	TRAP : x25	(HALT)
	// ADRS     : VAL
	// VAL      : x4E	('P' for pass)
/*	char * program[] =
		{
		"1010000000000010",
		"1111000000000001",
		"1111000000100101",
		"0000000000000100",
		"0000000010110011"
		};
	int programSize = 5;
*/
	// Results : R0 should contain xB3 or "0000 0000 1011 0011"
// PASSED


	// Test for STI
	//
/*	char * program[] =
		{
		
		};
	int programSize ==
*/
	// Results :

    /*
     * Test for LDR/STR/AND
     * .ORIG x00
     *  	ADD R4  R2  R1
     *		STR R4  R4  #10
     *		LDR R7  R4  #9
     *		AND R0  R3  R7
     *		AND R1  R7  #13
     *		HALT
     *
     *	.END
     */

/*	char * program[] =
		{
		"0001100010000001",
		"0111100100001010",
		"0110111110000111",
		"0101000011000111",
		"0101001111101101",
		"1111000000100101"
		};
	int programSize = 6;
*/
/*
	Results:  R4 <- R2 + R1 = 3
		  M[13] <- 3
		  R7 <- M[R6] + 7
		  R0 = 1 (R3 AND R7)
		  R1 = 13 (R7 AND 13)
		  
	PASSED

*/

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


	//Following LD instruction is    LD
	/* execute the program */
	/* During execution, the only output to the screen should be */
	/* the result of executing OUT. */
	COMP_Execute(&comp);

	/* shows final configuration of computer */
	COMP_Display(comp);



}


