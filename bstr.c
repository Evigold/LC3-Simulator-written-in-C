#include <stdio.h>
#include <string.h>

#include "bstr.h"


void BSTR_SetBits(BitString* bstr, char* bits) {
    int len = strlen(bits);
	int i;
	for (i = 0; i < len; i++) {
		if (bits[i] == '0') {
			bstr->bits[i] = 0;
	    } else {
			bstr->bits[i] = 1;
	    }
    }
	bstr->length = len;
}

void BSTR_Substring(BitString* substr, BitString source,
                    int start, int length)  {
	int i;
	for (i = 0; i < length; i++) {
		substr->bits[i] = source.bits[start+i];
	}
	substr->length = length;
}

void BSTR_Invert(BitString* bstr) {
    int i;
	for (i = 0; i < bstr->length; i++) {
		if (bstr->bits[i] == 0) {
			bstr->bits[i] = 1;
	    } else {
			bstr->bits[i] = 0;
	    }
    }
}

void BSTR_AddOne(BitString* bstr) {
    int i;
	for (i = bstr->length-1; i >= 0; i--) {
		if (bstr->bits[i] == 0) {
			bstr->bits[i] = 1;
			break;
	    } else {
			bstr->bits[i] = 0;  /* and add one to next column */
	    }
    }
}


void BSTR_SetValue(BitString* bstr, int n, int size) {
	int i;
	if (size > MAXBITS) {
		return;
    }
	bstr->length = size;

	for (i = bstr->length-1; i >= 0; i--) {
		bstr->bits[i] = n % 2;
		n = n / 2;
    }
}


void BSTR_SetValueTwosComp(BitString* bstr, int n, int size) {
	int negative = n < 0;
	if (size > MAXBITS) {
		return;
    }
	if (n < 0) {
		n = -1 * n;
    }
	BSTR_SetValue(bstr,n,size);
	if (negative) {
		BSTR_Invert(bstr);
		BSTR_AddOne(bstr);
    }

}

void BSTR_Display(BitString bstr, int fourGrouping) {
    int i;
    for (i = 0; i < bstr.length; i++) {
		if (fourGrouping && (i % 4 == 0) && i != 0)
			printf(" ");
		if (bstr.bits[i] == 0) {
			printf("0");
	    } else {
			printf("1");
	    }

    }
}

void BSTR_Copy(BitString* copy, BitString bstr) {
	int i;
    copy->length = bstr.length;
	for (i = 0; i < bstr.length; i++) {
		copy->bits[i] = bstr.bits[i];
    }
}



int BSTR_GetValue(BitString bstr) {
	int value = 0;
	int i;
    for (i = 0; i < bstr.length; i++) {
        value = value * 2;
		value += bstr.bits[i];
    }
	return value;
}

//1000000..
int BSTR_GetValueTwosComp(BitString bstr) {
	int value = 0;
	int negative = (bstr.bits[0] == 1);
	if (negative) {
		BitString posBstr;
		BSTR_Copy(&posBstr,bstr);
		BSTR_Invert(&posBstr);
		BSTR_AddOne(&posBstr);
		return -1 * BSTR_GetValue(posBstr);
    } else {
		return BSTR_GetValue(bstr);
    }
}

void BSTR_Append(BitString *result, BitString bstr1, BitString bstr2) {
	int d = 0;
	int i;
    for (i = 0; i < bstr1.length; i++) {
		result->bits[d] = bstr1.bits[i];
		d++;
    }
	for (i = 0; i < bstr2.length; i++) {
	    result->bits[d] = bstr2.bits[i];
		d++;
    }
	result->length = bstr1.length + bstr2.length;
}

int BSTR_Length(BitString bstr) {
	return bstr.length;
}

