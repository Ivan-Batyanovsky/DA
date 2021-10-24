#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    unsigned char key[33];
    int strID;
    
} keys;

typedef struct
{
    char string[65];
} str;

void strCopy(unsigned char dest[], unsigned char source[])
{
    for (int i = 0; i < 33; i++)
        dest[i] = source[i];
}

void radixSort(keys *keyInput, int razmer)
{
    keys *output = (keys*)malloc(sizeof(keys) * razmer);
    
    for (int rad = 32; rad >= 0; rad--)
    {
        
        int input[128] = {0};
    
        for (int i = 0; i < razmer; i++)
        {
            input[keyInput[i].key[rad]]++;
        }
        for (int i = 1; i < 128; i++)
            input[i] = input[i] + input[i - 1];
            
        for (int i = razmer - 1; i >= 0; i--)
        {
            strCopy(output[--input[keyInput[i].key[rad]]].key, keyInput[i].key);
            output[input[keyInput[i].key[rad]]].strID = keyInput[i].strID;
        }
    
        for (int i = 0; i < razmer; i++)
        {
            strCopy(keyInput[i].key, output[i].key);
            keyInput[i].strID = output[i].strID;
        }
    }
    
    free(output);
}


int main()
{
    int ind = 0, size = 64;
    
    keys *keyInput = (keys*)malloc(sizeof(keys) * size);
    str *strInput = (str*)malloc(sizeof(str) * size);
    
    while (scanf("%s %s", keyInput[ind].key, strInput[ind].string) != EOF)
    {
        if (ind + 1 >= size)
        {
            size *= 2;
            keyInput = (keys*)realloc(keyInput, (sizeof(keys) * size));
            strInput = (str*)realloc(strInput, (sizeof(str) * size));
        }
        
        keyInput[ind].strID = ind;
	    ind++;
    }

    radixSort(keyInput, ind);
    
    
    for (int i = 0; i < ind; i++)
    {
        printf("%s\t%s\n", keyInput[i].key, strInput[keyInput[i].strID].string);
    }
    
    free(keyInput);
    free(strInput);
    
    return 0;
}
