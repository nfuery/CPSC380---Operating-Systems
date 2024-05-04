#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_TABLE_SIZE 256
#define PHYSICAL_MEM_SIZE 65536
#define TLB_SIZE 16
#define FRAME_SIZE 256

typedef struct tlb
{
    int page_number;
    int frame_number;
} TLB;

typedef struct logical_address
{
    uint32_t page_offset : 8;
    uint32_t page_number : 8;
    uint32_t unused : 16;
} LOGICAL_ADDRESS;

typedef union logical_address_union
{
    LOGICAL_ADDRESS bit_field;
    uint32_t ul;
} LOGICAL_ADDRESS_UNION;

TLB tlb[TLB_SIZE];
int page_table[PAGE_TABLE_SIZE];
uint8_t physical_mem[PHYSICAL_MEM_SIZE];

int check_tlb(int page_number)
{
    for (int i = 0; i < TLB_SIZE; i++)
    {
        if (tlb[i].page_number == page_number)
        {
            return i;
        }
    }
    return -1;
}

int check_page_table(int page_number)
{
    for (int i = 0; i < PAGE_TABLE_SIZE; i++)
    {
        if (page_table[i] == page_number)
        {
            return i;
        }
    }
    return -1;
}

void update_tlb(int page_number, int frame_number)
{
    static int tlb_index = 0;
    tlb[tlb_index].page_number = page_number;
    tlb[tlb_index].frame_number = frame_number;
    tlb_index = (tlb_index + 1) % TLB_SIZE;
}

void update_page_table(int page_number, int free_frame)
{
    page_table[page_number] = free_frame;
}

int main(int argc, char *argv[])
{
    int total_hits = 0;
    int tlb_hits = 0;
    int page_faults = 0;
    if (argc != 2)
    {
        printf("Format of command line input must be './vmmgr addresses.txt'\n");
        return -1;
    }
    LOGICAL_ADDRESS_UNION log_add_union;
    char str[10];
    FILE *file = fopen(argv[1], "r");
    FILE *backing_store = fopen("BACKING_STORE.bin", "rb");

    for (int i = 0; i < TLB_SIZE; ++i)
    {
        tlb[i].page_number = -1;
        tlb[i].frame_number = -1;
    }

    for (int i = 0; i < PAGE_TABLE_SIZE; ++i)
    {
        page_table[i] = -1;
    }

    int value;
    int free_frame = 0;
    while (fgets(str, 10, file) != NULL)
    {
        total_hits++;
        int logical_address = atoi(str);
        log_add_union.ul = logical_address;
        int tlb_idx = check_tlb(log_add_union.bit_field.page_number);

        if (tlb_idx != -1)
        {
            tlb_hits++;
            printf("Frame num: %d\n", tlb[tlb_idx].frame_number);
            value = physical_mem[(tlb[tlb_idx].frame_number * FRAME_SIZE) + log_add_union.bit_field.page_offset];
            printf("Logical Address: %d\n", logical_address);
            printf("Physical Address: %d\n", ((tlb[tlb_idx].frame_number * FRAME_SIZE) + log_add_union.bit_field.page_offset));
            printf("Byte Value: %d\n", (int8_t)value);
            continue;
        }

        int frame_number = check_page_table(log_add_union.bit_field.page_number);
        if (frame_number != -1)
        {
            update_tlb(log_add_union.bit_field.page_number, frame_number);
            value = physical_mem[(frame_number * FRAME_SIZE) + log_add_union.bit_field.page_offset];
            printf("Logical Address: %d\n", logical_address);
            printf("Physical Address: %d\n", ((frame_number * FRAME_SIZE) + log_add_union.bit_field.page_offset));
            printf("Byte Value: %d\n", (int8_t)value);
            continue;
        }
        else
        {
            page_faults++;
        }

        if (free_frame < PHYSICAL_MEM_SIZE / FRAME_SIZE)
        {
            fseek(backing_store, free_frame * FRAME_SIZE, SEEK_SET);
            fread(&physical_mem[free_frame * FRAME_SIZE], FRAME_SIZE, 1, backing_store);

            update_page_table(log_add_union.bit_field.page_number, free_frame);
            update_tlb(log_add_union.bit_field.page_number, free_frame);
            ++free_frame;
        }
    }
    float page_fault_rate = (float)page_faults / total_hits;
    float tlb_hit_rate = (float)tlb_hits / total_hits;
    printf("Page fault rate: %.2f\n", page_fault_rate * 100);
    printf("TLB hit rate: %.2f\n", tlb_hit_rate * 100);

    return 0;
}