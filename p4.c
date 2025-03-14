#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h> 

#define ARRAYSIZE 60

void print_bits(uint64_t reg) 
{
    for (int i = 0; i < 64; i++) 
    {
        int temp = (reg & (1ULL << (63 - i))) ? 1 : 0;
        printf("%d", temp);
    }
}

uint8_t countDifferentBits(uint64_t num1, uint64_t num2) 
{
    uint8_t count = 0;
    for (int j = 0; j < 64; j++)
     {
        uint64_t x = num1 & ((uint64_t)0x1 << j);
        uint64_t y = num2 & ((uint64_t)0x1 << j);
        if (x ^ y) 
        {
            count++;
        }
    }
    return count;
}

void print(uint64_t Array[], uint8_t Transitions[], uint64_t Sum)
{
    printf("*---------------------------------------------------------------------------------------------------------------*\n");
    printf("*     ");
    printf("Index");
    printf("     *\t\t\t\t\t");
    printf("Bits");
    printf("\t\t\t\t\t*");
    printf("  Transitions  ");
    printf("*\n");
    printf("*---------------------------------------------------------------------------------------------------------------*\n");
    for (int i = 0; i < ARRAYSIZE; i++) 
    {
            printf("*\t%d\t", i);
            printf("*\t");
            print_bits(Array[i]);
            printf("\t*\t");
            printf("%d", Transitions[i]);
            printf("\t*\n");
    }
    printf("*---------------------------------------------------------------------------------------------------------------*\n");
    printf("\ntotal transition for random --> hybrid encode = %ld\n", Sum);
}



int main(int argc, char *argv[]) 
{
    srand(time(NULL));// seed the RNG


    //----------------------------------------------------------------------------------------------------------------------------------------------------------Random

    uint64_t randArray[ARRAYSIZE] = {0};
    uint8_t randArrayTransitions[ARRAYSIZE] = {0};
    
    // default to random
    if(argc <= 1)
    {
        for (int i = 0; i < ARRAYSIZE; i++) 
        {
            
            uint64_t upper = (uint64_t)rand() << 32;
            uint64_t lower = (uint64_t)rand();
            randArray[i] = upper | lower;
        }
    }
    else // sequential for me
    {
         
        uint64_t upper = (uint64_t)rand() << 32;
        uint64_t lower = (uint64_t)rand();
        uint64_t offset = (upper | lower) - ARRAYSIZE;
        for (int i = 0; i < ARRAYSIZE; i++) 
        {
            randArray[i] = offset + i;
        }
    }

    // count the transitions
    uint64_t randSum = 0;
    for (int i = 1; i < ARRAYSIZE; i++) 
    {
        randArrayTransitions[i] = countDifferentBits(randArray[i - 1] , randArray[i]);
        randSum += randArrayTransitions[i];
    }
    printf("\n\n\t\t %d random 64-bit numbers\n", ARRAYSIZE);
    print(randArray, randArrayTransitions, randSum);
    //----------------------------------------------------------------------------------------------------------------------------------------------------------Random --> Grey
    // Create the grey code array
    uint64_t greyArray[ARRAYSIZE] = {0};
    uint8_t greyArrayTransitions[ARRAYSIZE] = {0};
    for (int i = 0; i < ARRAYSIZE; i++)
    {
        uint64_t temp = randArray[i]; // get the value
        uint64_t gray = (temp >> 1) ^ temp; // xor each bit the previous. Here MSB is also in xor but it works out

        greyArray[i] = gray; // Store the Gray Code value
    }

    // count the transitions
    uint64_t greySum = 0;
    for (int i = 1; i < ARRAYSIZE; i++) 
    {
        greyArrayTransitions[i] = countDifferentBits(greyArray[i - 1] , greyArray[i]);
        greySum += greyArrayTransitions[i];
    }
    printf("\n\n\t\t %d random 64-bit numbers    --->    grey encode.\n", ARRAYSIZE);
    print(greyArray, greyArrayTransitions, greySum);
    //----------------------------------------------------------------------------------------------------------------------------------------------------------Random --> Bus Invert
    uint64_t busEncodedArray[ARRAYSIZE] = {0};
    uint8_t busEncodedArrayTransitions[ARRAYSIZE] = {0};
    uint8_t invertSignalArray[ARRAYSIZE] = {0};

    busEncodedArray[0] = randArray[0]; // init 0 so i can start at 1

    for (int i = 1; i < ARRAYSIZE; i++) 
    {
        uint64_t current_data = randArray[i];
        uint8_t invert_signal = 0;

        uint8_t transitionsTemp = countDifferentBits(busEncodedArray[i - 1] , randArray[i]);// blue arrow logic

        if (transitionsTemp > 32) 
        { // if trasitions > n/2 invert
            current_data = ~current_data;
            invert_signal = 1;
        }

        busEncodedArray[i] = current_data;
        invertSignalArray[i] = invert_signal;
    }


     // count the transitions
     uint64_t busSum = 0;
    for (int i = 1; i < ARRAYSIZE; i++) 
    {
        busEncodedArrayTransitions[i] = countDifferentBits(busEncodedArray[i - 1] , busEncodedArray[i]);
        busSum += busEncodedArrayTransitions[i];

        uint8_t invert_transition = invertSignalArray[i - 1] ^ invertSignalArray[i];
        busSum += invert_transition; 
    }
    printf("\n\n\t\t %d random 64-bit numbers    --->    bus invert encode.\n", ARRAYSIZE);
    print(busEncodedArray, busEncodedArrayTransitions, busSum);

    


   //----------------------------------------------------------------------------------------------------------------------------------------------------------Random --> T0
   uint64_t T0EncodedArray[ARRAYSIZE] = {0};
   uint8_t T0EncodedArrayTransitions[ARRAYSIZE] = {0};
   uint8_t T0SignalArray[ARRAYSIZE] = {0};

   T0EncodedArray[0] = randArray[0]; // init 0 so i can start at 1
    uint64_t count = 0;
   for (int i = 1; i < ARRAYSIZE; i++) 
   {
        if(randArray[i] == (T0EncodedArray[i - 1] + 1+ count))
        {
            T0SignalArray[i] = 1;
            T0EncodedArray[i] = T0EncodedArray[i - 1];
            count++;
        }
        else
        {
            T0EncodedArray[i] = randArray[i];
            count = 0;
        }
   }


    // count the transitions
    uint64_t T0Sum = 0;
   for (int i = 1; i < ARRAYSIZE; i++) 
   {
        T0EncodedArrayTransitions[i] = countDifferentBits(T0EncodedArray[i - 1] , T0EncodedArray[i]);
        T0Sum += T0EncodedArrayTransitions[i];
        uint8_t T0_transition = T0SignalArray[i - 1] ^ T0SignalArray[i];
        T0Sum += T0_transition; 
   }
   printf("\n\n\t\t %d random 64-bit numbers    --->    T0 encode.\n", ARRAYSIZE);
   print(T0EncodedArray, T0EncodedArrayTransitions, T0Sum);

   


   //----------------------------------------------------------------------------------------------------------------------------------------------------------Random --> T0 + Bus invert
   uint64_t hybridEncodedArray[ARRAYSIZE] = {0};
   uint8_t hybridEncodedArrayTransitions[ARRAYSIZE] = {0};
   uint8_t hybridHoldSignalArray[ARRAYSIZE] = {0};
   uint8_t hybridinvertSignalArray[ARRAYSIZE] = {0};

   hybridEncodedArray[0] = randArray[0]; // init 0 so i can start at 1
   count = 0;
   for (int i = 1; i < ARRAYSIZE; i++) 
   {
        if(randArray[i] == (hybridEncodedArray[i - 1] + 1+ count))
        {
            hybridHoldSignalArray[i] = 1;
            hybridEncodedArray[i] = hybridEncodedArray[i - 1];
            count++;
        }
        else
        {
            uint64_t current_data = randArray[i];
            uint8_t invert_signal = 0;
    
            uint8_t transitionsTemp = countDifferentBits(hybridEncodedArray[i - 1] , randArray[i]);// blue arrow logic
    
            if (transitionsTemp > 32) 
            { // if trasitions > n/2 invert
                current_data = ~current_data;
                invert_signal = 1;
            }
    
            hybridEncodedArray[i] = current_data;
            hybridinvertSignalArray[i] = invert_signal;
            count = 0;
        }
   }


    // count the transitions
    uint64_t hybridSum = 0;
   for (int i = 1; i < ARRAYSIZE; i++) 
   {
        hybridEncodedArrayTransitions[i] = countDifferentBits(hybridEncodedArray[i - 1] , hybridEncodedArray[i]);
        hybridSum += hybridEncodedArrayTransitions[i];
        uint8_t hybrid_hold_transition = hybridHoldSignalArray[i - 1] ^ hybridHoldSignalArray[i];
        hybridSum += hybrid_hold_transition; 
        uint8_t hybrid_inv_transition = hybridinvertSignalArray[i - 1] ^ hybridinvertSignalArray[i];
        hybridSum += hybrid_inv_transition; 
   }
   printf("\n\n\t\t %d random 64-bit numbers    --->    hybrid encode.\n", ARRAYSIZE);
   print(hybridEncodedArray, hybridEncodedArrayTransitions, hybridSum);


    
    //----------------------------------------------------------------------------------------------------------------------------------------------------------Results
    printf("\n\n\n\t\t\t\t ***Results*** \n\n");
    if(argc <= 1)
    {
        printf("\t Given a random access pattern\n");
    }
    else
    {
        printf("\t Given a sequential access pattern\n");
    }
    printf("\t total diffs for; (random transitions) - (greyCode transitions) = %ld\n", randSum - greySum);
    printf("\t reduction in switching activity for greyCode transitions = %f\n\n", ((float)(randSum - greySum) / (float)randSum) * 100);

    printf("\t total diffs for; (random transitions) - (bus encode transitions) = %ld\n", randSum - busSum);
    printf("\t reduction in switching activity for bus encoded transitions = %f\n\n", ((float)(randSum - busSum) / (float)randSum) * 100);

    printf("\t total diffs for; (random transitions) - (T0 transitions) = %ld\n", randSum - T0Sum);
    printf("\t reduction in switching activity for T0 encoded transitions = %f\n\n", ((float)(randSum - T0Sum) / (float)randSum) * 100);

    printf("\t total diffs for; (random transitions) - (hybrid transitions) = %ld\n", randSum - hybridSum);
    printf("\t reduction in switching activity for hybrid encoded transitions = %f\n", ((float)(randSum - hybridSum) / (float)randSum) * 100);
    return 0;
}

