#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int requests[10] = {2069, 1212, 2296, 2800, 544, 1618, 356, 1523, 4965, 3681};

int main(int argc, char *argv[])
{
    if (argc != 2){
        printf("Input correct number of arguments\n");
        return -1;
    }

    int startPos = atoi(argv[1]);
    
    if (!(startPos >= 0 && startPos < 5000)) {
        printf("Starting position must be between 0 and 4999\n");
        return -1;
    }

    int movement = 0;
    int currPos = startPos;

    for (int i = 0; i < 10; i++){
        movement += abs(currPos - requests[i]);
        currPos = requests[i];
    }
    
    printf("Total movement: %d\n", movement);

    return 0;
}