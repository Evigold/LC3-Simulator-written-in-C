#ifndef BSTR
#define BSTR

#define MAXBITS  16


typedef struct bitStringType BitString;	

struct bitStringType {    /* char generally hold the ascii code of some character. */
	char bits[MAXBITS];   /* char can also be thought of as a small int. */
	int length;           /* These bit strings are made of 0's and 1's */
};                        /* - not the ascii codes of 0 and 1 (48 and 49) */
                          /* This bit string is NOT zero-terminated like a */
						  /* c string would be.  Instead the length field */
						  /* keeps track of how long the string is. */
                          /* The maximum size is 16.  */
						  
					  

/* bits is a c-string (array of ascii codes ending with 0). */
/* It should be made of 0's and 1's - "0110" for example. */
/* Those characters are converted to a similar set of 0's and 1's */
/* in the bitstring.  "0" is replace by 0, "1" is replaced by 1. */
void BSTR_SetBits(BitString* bstr, char* bits);

/* flips all bits */
void BSTR_Invert(BitString *bstr);

/* adds one to the binary number treating it as unsigned */
void BSTR_AddOne(BitString *bstr);

/* sets bstr to an unsigned binary number.  The number of bits */
/* is size.  The value is n. */
void BSTR_SetValue(BitString *bstr, int n, int size);

/* sets bstr to a two's complement binary number.  The number of bits */
/* is size.  The value is n. */
void BSTR_SetValueTwosComp(BitString *bstr, int n, int size);

/* prints out the bit string.  If the second parameter is true (1), */
/* then there is a blank after every 4 bits for easy reading. */
void BSTR_Display(BitString bstr, int fourGrouping);

/* bits in src are copied into copy */
void BSTR_Copy(BitString *copy, BitString src);

/* returns the two's complement value of this bitstring */
int BSTR_GetValueTwosComp(BitString bstr);

/* returns the unsigned value of this bitstring */
int BSTR_GetValue(BitString bstr);

/* puts two bit strings bstr1 and bstr2 together to form a new */
/* bigger bitstring which is stored in result.  The original */
/* bit strings are not changed. */
void BSTR_Append(BitString *result, BitString bstr1, BitString bstr2);

/* creates a new bitstring from source, starting at position start */
/* and with the specified length.   The first position is numbered 0 */
/* as with a java (or c) array.  The new bitstring is saved in substr */
/* source is not modified. */
void BSTR_Substring(BitString* substr, BitString source,
                    int start, int length);

/* returns the length of the bistring (i.e. the number of bits) */
int BSTR_Length(BitString bstr);


#endif


