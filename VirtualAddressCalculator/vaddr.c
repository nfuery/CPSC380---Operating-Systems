// Assume that a system has a 32-bit virtual address with a 4-KB page size.
// Write a C program that is passed a virtual address (in decimal) on the
// command line and have it output the page number and offset for the
// given address. As an example, your program would run as follows:
//   ./vaddr 19986 

// Your program would output:
//    The address 19986 contains:
//    page number = 4
//    offset = 3602

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 

typedef struct address {
    uint32_t offset : 12;
    uint32_t page_num : 20;
} ADDRESS;

typedef union address_union {
    struct address bitfield;
    uint32_t ul;
} ADD_UNION;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Run with this format: ./vaddr address\n");
        return 0;
    }

    ADD_UNION u_address;
    u_address.ul = atoi(argv[1]);
     
    printf("Page: %d\n", u_address.bitfield.page_num);
    printf("Offset: %d\n", u_address.bitfield.offset);

    return 0;
}