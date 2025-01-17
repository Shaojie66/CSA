#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "SimpleTest.h"
#include "SABuildFunc.h"
#include "FileOperation.h"
#include "BasicStep.h"
#include "MergeStep.h"

/*
 * Global variables.
 */
char* FILEPATH = "NC_008253.fna";   // file path
int ARRAYLENGTH = 0; // length of T ~ n
int PARTLENGTH = 0; // part length of T ~ l
int PARTNUM = 0; // number of parts ~ ceil(n/l)

char* T = NULL; // DNA sequence of (A,C,G,T) plus a '$'
int* SA = NULL; // SA of T
int* SA_inverse = NULL; // inverse of SA
int* Psi = NULL; // Psi of T - the compressed suffix array

char* BWT = NULL; // BWT of T - Burrows-Wheeler Transform

char* BWTFILEPATH = "NC_008253.bwt";
char* BWTFILEHEADER = ">gi|110640213|ref|NC_008253.1| Escherichia coli 536, complete genome";
int LINELENGTH = 70;

/*
 * Functions.
 */
void directlyConstruction();
void testSet();
void performanceProblem();

int main() {
    int i = 0;
    long startTime = 0;
    long endTime = 0;

//    testSet();
//    return 0;

    ARRAYLENGTH = fnaDataSize(FILEPATH);    // get length of DNA sequence in the fnaFile
    ARRAYLENGTH = ARRAYLENGTH + 1; // get ready to add character '$' to the end of the DNA sequence
    printf("DNA (plus a \'$\') sequence length: %d\n", ARRAYLENGTH);
    PARTLENGTH = ARRAYLENGTH / log2(ARRAYLENGTH);   // length of a part (an increment)
    PARTNUM = (int)ceil((double)ARRAYLENGTH / PARTLENGTH);
    printf("PartLength: %d, PartNum: %d\n", PARTLENGTH, PARTNUM);

    // cannot apply for memory in a function's stack, because memory applied there will be recycled.
    T = (char*)malloc(sizeof(char) * ARRAYLENGTH);
    SA = (int*)malloc(sizeof(int) * ARRAYLENGTH);
    SA_inverse = (int*)malloc(sizeof(int) * ARRAYLENGTH);
    Psi = (int*)malloc(sizeof(int) * ARRAYLENGTH);

    BWT = (char*)malloc(sizeof(char) * ARRAYLENGTH);

    if(T == NULL || SA == NULL || SA_inverse == NULL || Psi == NULL) {
        printf("System memory not enough. \n");
        exit(-1);
    }

    i = PARTNUM;
    baseStep(FILEPATH, T, SA, SA_inverse, Psi, ARRAYLENGTH, PARTLENGTH, PARTNUM);

    printf("\n");
    for(i = PARTNUM - 1; i > 0; i--) {
        printf("increment part (%d)\n", i);
        int partIndex = i; // T_i and T_apostrophe is stored using partIndex
        int* order = (int*)malloc(sizeof(int) * PARTLENGTH);
        // sorted suffixes are stored in SA[startIndex_i]...[startIndex_apostrophe]

        startTime = clock();
        mergeStepA(T, SA, SA_inverse, ARRAYLENGTH, PARTLENGTH, partIndex);
        endTime = clock();
        printf("merge step (a) takes time: %ld\n", endTime - startTime);

        startTime = clock();
        mergeStepB(T, SA, Psi, ARRAYLENGTH, PARTLENGTH, partIndex, order);
        endTime = clock();
        printf("merge step (b) takes time: %ld\n", endTime - startTime);

        startTime = clock();
        mergeStepC(T, SA, SA_inverse, Psi, ARRAYLENGTH, PARTLENGTH, partIndex, order);
        endTime = clock();
        printf("merge step (c) takes time: %ld\n", endTime - startTime);

        printf("\n");
        free(order);
    }

    printf("Converting Psi[] to BWT[]\n");
    convertPsiToBWT(T, Psi, BWT);

    printf("i\tT[]\t");
    printf("SA[]\tT_SA[]\t");
    printf("Psi[]\t");
    printf("BWT[]\t");
    printf("\n");
    for(i = 0; i < ARRAYLENGTH; i++) {
        if(i % PARTLENGTH != 0) {
            continue;
        }
        printf("%d\t%c\t", i, T[i]);
        printf("%d\t%c\t", SA[i], T[SA[i]]);
        printf("%d\t", Psi[i]);
        printf("%c\t", BWT[i]);
        printf("\n");
    }
    printf("Total length: %d\n", ARRAYLENGTH);

    // write BWT data into an output file
    writeBWTData(BWT, ARRAYLENGTH, BWTFILEHEADER, LINELENGTH, BWTFILEPATH);

    /*printf("%x\n", T);
    free(T);
    printf("%x\n", SA);
    free(SA);
    printf("%x\n", SA_inverse);
    free(SA_inverse);
    printf("%x\n", Psi);
    free(Psi);
    printf("%x\n", BWT);
    free(BWT);
    printf("%x\n", FILEPATH);
    free(FILEPATH);*/

    return 0;
}



////////////////////////////////// Actually Working Funcs //////////////////////////////////




/**
 * This is simply a debug function, mainly used when developing this program.
 * Or if you just want to see the effect or progress of some function, you can
 * disable the comment mark, and execute this function.
 */
void testSet() {

    int i = 0;

//    _CLanguageReview();
//    _timeOperationTest();
//    _mathematicalFuncsTest();
//    _mathematicalFuncsTest();
//    _mathematicalFuncsTest();
//    _quickSortTest();
//    _myStrLengthTest();
//    _suffixArrayQuickSortTest();
//    _compareSuffixTest();
//    _inverseSAWholeTest();
//    _psiArrayBuildWholeTest();
//    _lowerCaseTest();
//    _binarySearchBoundTest();
//    _CSABinaryBoundSearchTest();
//    _CSABinarySearchOrderValueTest();
//    _fgpsiFuncTest();
//    _convertPsiToBWTTest();

//    readAndPrint();
    directlyConstruction();
//    performanceProblem();

    for(i = 0; i < 10; i++) {
        printf("\n");
    }
}

/////////////////////////////////// Test Functions Below ///////////////////////////////////

/**
 * Test the performance that a program can do best.
 */
void performanceProblem() {
    printf("\n ******* performanceProblem *********\n");
    int arrayLength = 100;
    int integerValue = 0;

    int* intArray = NULL;

    // maximum int[] length: 489000001(windows 10), 1744000001(deepin)

    while(1) {
        intArray = (int*)malloc(sizeof(int) * arrayLength);
        if(intArray == NULL) {
            printf("Memory not enough. \n");
            break;
        } else {
            printf(" - got array - length: %d\n", arrayLength);
        }
        free(intArray);
        arrayLength = arrayLength + 1E7;
    }

    integerValue = 0;
    arrayLength = integerValue;
    while(1) {
        printf("%d\n", integerValue += 1000000);
        if(integerValue < arrayLength) {
            break;
        }
        arrayLength = integerValue;
    }

}

/**
 * Test steps of construction of CSA - directly build.
 *
 * <note> bug detected - array directly defined cannot be too big.
 * To solve this problem, use (int*)malloc(sizeof(int)*ARRAYLENGTH).
 */
void directlyConstruction() {
    printf("\n ******* directlyConstruction *********\n");
    long startTime = 0;
    long endTime = 0;
    int i = 0;

    startTime = clock();
    ARRAYLENGTH = fnaDataSize(FILEPATH);
    printf("data length: %d\n", ARRAYLENGTH);

    // build T[] - DNA sequence array
    ARRAYLENGTH++; // get ready to add character '$' to the end of the DNA sequence
    T = (char*)malloc(sizeof(char) * ARRAYLENGTH);
    loadFnaData(FILEPATH, ARRAYLENGTH, T);

    printf("DNA sequence - T[]: \n");
//    for(i = 0; i < ARRAYLENGTH; i++) {
//        printf("%c", T[i]);
//    }
//    printf("\n");

    // build SA[] - suffix array
    SA = (int*)malloc(sizeof(int) * ARRAYLENGTH);
    for(i = 0; i < ARRAYLENGTH; i++) {
        SA[i] = i;
    }
    suffixArrayQuickSort(SA, T, 0, ARRAYLENGTH - 1);
    printf("Suffix array - SA[]: \n");
//    for(i = 0; i < ARRAYLENGTH; i++) {
//        printf("%d\t%d\t%c\n", i, SA[i], T[SA[i]]);
//    }
//    printf("\n");

    // build Psi[] - ... Psi array (I don't know how to describe it)
    SA_inverse = (int*)malloc(sizeof(int) * ARRAYLENGTH);
    Psi = (int*)malloc(sizeof(int) * ARRAYLENGTH);
    printf("Inverse suffix array - SA_inverse[]\n");
    inverseSAWhole(SA, SA_inverse, ARRAYLENGTH);
    printf("Psi array - Psi[]: \n");
    psiArrayBuildWhole(SA, SA_inverse, Psi, ARRAYLENGTH);
    endTime = clock();

    printf("Direct construction takes time: %ld\n", endTime - startTime);


    printf("i\tT[]\tPsi[]\tT[SA[]]\n");
    for(i = 0; i < ARRAYLENGTH; i++) {
        printf("%d\t%c\t%d\t%c", i, T[i], Psi[i], T[SA[i]]);
        printf("\n");
        i = i + ARRAYLENGTH / 10;
    }
    printf("\n");

    free(T);
    free(SA);
    free(SA_inverse);
    free(Psi);
    printf("direct construction ended. \n");
}



