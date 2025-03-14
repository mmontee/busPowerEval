#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h> 

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

#define ARRAYSIZE 60

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
    
    printf("\n\n\t\t %d random 64-bit numbers.\n", ARRAYSIZE);
    printf("*-----------------------------------------------------------------------------------------------*\n");
    printf("*\t\t\t\t");
    printf("Bits");
    printf("\t\t\t\t\t\t*");
    printf("  Transitions  ");
    printf("*\n");
    printf("*-----------------------------------------------------------------------------------------------*\n");
    for (int i = 0; i < ARRAYSIZE; i++) 
    {
        printf("*\t");
        print_bits(randArray[i]);
        printf("\t*\t");
        printf("%d", randArrayTransitions[i]);
        printf("\t*\n");
    }
    printf("*-----------------------------------------------------------------------------------------------*\n");
    printf("\ntotal transition for randoms = %ld\n", randSum);
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

    printf("\n\n\t\t %d random 64-bit numbers    --->    Grey code.\n", ARRAYSIZE);
    printf("*-----------------------------------------------------------------------------------------------*\n");
    printf("*\t\t\t\t");
    printf("Bits");
    printf("\t\t\t\t\t\t*");
    printf("  Transitions  ");
    printf("*\n");
    printf("*-----------------------------------------------------------------------------------------------*\n");
    for (int i = 0; i < ARRAYSIZE; i++) 
    {
        printf("*\t");
        print_bits(greyArray[i]);
        printf("\t*\t");
        printf("%d", greyArrayTransitions[i]);
        printf("\t*\n");
        
    }
    printf("*-----------------------------------------------------------------------------------------------*\n");
    printf("\ntotal transition for random --> grey = %ld\n", greySum);
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

    printf("\n\n\t\t %d random 64-bit numbers    --->    bus encode.\n", ARRAYSIZE);
    printf("*-----------------------------------------------------------------------------------------------*\n");
    printf("*\t\t\t\t");
    printf("Bits");
    printf("\t\t\t\t\t\t*");
    printf("  Transitions  ");
    printf("*\n");
    printf("*-----------------------------------------------------------------------------------------------*\n");
    for (int i = 0; i < ARRAYSIZE; i++) 
    {
        printf("*\t");
        print_bits(busEncodedArray[i]);
        printf("\t*\t");
        printf("%d", busEncodedArrayTransitions[i]);
        printf("\t*\n");
    }
    printf("*-----------------------------------------------------------------------------------------------*\n");
    printf("\ntotal transition for random --> bus encode = %ld\n", busSum);

    


   //----------------------------------------------------------------------------------------------------------------------------------------------------------Random --> T0
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

   printf("\n\n\t\t %d random 64-bit numbers    --->    bus encode.\n", ARRAYSIZE);
   printf("*-----------------------------------------------------------------------------------------------*\n");
   printf("*\t\t\t\t");
   printf("Bits");
   printf("\t\t\t\t\t\t*");
   printf("  Transitions  ");
   printf("*\n");
   printf("*-----------------------------------------------------------------------------------------------*\n");
   for (int i = 0; i < ARRAYSIZE; i++) 
   {
       printf("*\t");
       print_bits(busEncodedArray[i]);
       printf("\t*\t");
       printf("%d", busEncodedArrayTransitions[i]);
       printf("\t*\n");
   }
   printf("*-----------------------------------------------------------------------------------------------*\n");
   printf("\ntotal transition for random --> bus encode = %ld\n", busSum);

   

    
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
    printf("\t total diffs for; (random transitions) - (bus encode transitions) = %ld\n", randSum - busSum);
    return 0;
}

