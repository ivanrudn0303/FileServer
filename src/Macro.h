#ifndef MACRO_H
#define MACRO_H

#include <stdio.h>

#define CHECK_EQUAL(L, R) \
{\
	int val1 = (L);\
	int val2 = (R);\
	if (val1 != val2)\
		printf("error in line %d: %s != %s: %d != %d\n", __LINE__, #L, #R, val1, val2);\
}

#define CHECK_NOT_EQUAL(L, R) \
{\
	int val1 = (L);\
	int val2 = (R);\
	if (val1 == val2)\
		printf("error in line %d: %s == %s: %d == %d\n", __LINE__, #L, #R, val1, val2);\
}

#endif
