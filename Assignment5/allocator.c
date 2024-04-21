#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
    int used;
    char processID[100];
    int memSize;
    int start;
    int end;
    struct node *next;
    struct node *prev;
} NODE;

NODE *mainMem = NULL;
int totalMem;

void report()
{
    NODE *curr = mainMem;

    while (curr)
    {
        if (curr->used)
        {
            printf("Addresses [%d:%d] Process %s\n", curr->start, curr->end, curr->processID);
        }
        else
        {
            printf("Addresses [%d:%d] Unused\n", curr->start, curr->end);
        }
        curr = curr->next;
    }
    return;
}

void insert_node(NODE *curr, NODE *new, int memSize)
{
    if (curr->memSize == memSize)
    {
        curr->used = new->used;
        strcpy(curr->processID, new->processID);
        curr->memSize = new->memSize;
        curr->start = new->start;
        curr->end = new->end;
        free(new);
    }
    else
    {
        curr->memSize = curr->memSize - new->memSize;
        curr->start = new->end + 1;
        if (!curr->prev)
        {
            curr->prev = new;
            new->next = curr;
            mainMem = new;
        }
        else
        {
            curr->prev->next = new;
            new->prev = curr->prev;
            curr->prev = new;
            new->next = curr;
        }
    }
    return;
}

void fix_index()
{
    NODE *curr = mainMem;
    int idx = 0;

    while (curr)
    {
        curr->start = idx;
        idx = curr->end + 1;
        curr = curr->next;
    }
    return;
}

void merge(NODE *curr)
{
    if (!curr)
    {
        return;
    }

    if (curr->prev && !curr->prev->used)
    {
        curr->prev->memSize += curr->memSize;
        curr->prev->end = curr->end;
        curr->prev->next = curr->next;

        if (curr->next)
        {
            curr->next->prev = curr->prev;
        }

        NODE *temp1 = curr;
        curr = curr->prev;
        free(temp1);
    }

    if (curr->next && !curr->next->used)
    {
        curr->memSize += curr->next->memSize;
        curr->end = curr->next->end;

        NODE *temp2 = curr->next;
        curr->next = curr->next->next;

        if (curr->next)
        {
            curr->next->prev = curr;
        }

        free(temp2);
    }
}

void first_fit(char processID[100], int memSize)
{
    NODE *curr = mainMem;
    while (curr)
    {
        if (curr->used || curr->memSize < memSize)
        {
            curr = curr->next;
        }
        else
        {
            NODE *process = (NODE *)malloc(sizeof(NODE));

            process->used = 1;
            strcpy(process->processID, processID);
            process->memSize = memSize;
            process->start = curr->start;
            process->end = curr->start + memSize - 1;
            insert_node(curr, process, memSize);
            return;
        }
    }
    printf("No contingous memory avalible for Process %s\n", processID);
    return;
}

void best_fit(char processID[100], int memSize)
{
    printf("%d\n", memSize);
    NODE *curr = mainMem;
    NODE *best = NULL;

    while (curr)
    {
        if (curr->memSize >= memSize && !curr->used)
        {
            if (!best || best->memSize > curr->memSize)
            {
                best = curr;
            }
        }
        curr = curr->next;
    }

    if (!best)
    {
        printf("No contingous memory avalible for Process %s\n", processID);
    }
    else
    {
        NODE *process = (NODE *)malloc(sizeof(NODE));

        process->used = 1;
        strcpy(process->processID, processID);
        process->memSize = memSize;
        process->start = best->start;
        process->end = best->start + memSize - 1;
        insert_node(best, process, memSize);
    }

    return;
}

void worst_fit(char processID[100], int memSize)
{
    printf("%d\n", memSize);
    NODE *curr = mainMem;
    NODE *worst = NULL;

    while (curr)
    {
        if (curr->memSize >= memSize && !curr->used)
        {
            if (!worst || worst->memSize < curr->memSize)
            {
                worst = curr;
            }
        }
        curr = curr->next;
    }

    if (!worst)
    {
        printf("No contingous memory avalible for Process %s\n", processID);
    }
    else
    {
        NODE *process = (NODE *)malloc(sizeof(NODE));

        process->used = 1;
        strcpy(process->processID, processID);
        process->memSize = memSize;
        process->start = worst->start;
        process->end = worst->start + memSize - 1;
        insert_node(worst, process, memSize);
    }
    return;
}

void request(char processID[100], int memSize, char strategy)
{
    if (strategy == 'F')
    {
        first_fit(processID, memSize);
    }
    else if (strategy == 'B')
    {
        best_fit(processID, memSize);
    }
    else if (strategy == 'W')
    {
        worst_fit(processID, memSize);
    }
    else
    {
        printf("Invalid Strategy\n");
    }
    return;
}

void release(char processID[100])
{
    NODE *curr = mainMem;
    while (curr)
    {
        if (strcmp(curr->processID, processID) == 0)
        {
            curr->used = 0;
            strcpy(curr->processID, "");
            merge(curr);
            break;
        }
        curr = curr->next;
    }

    if (!curr)
    {
        printf("Process does not exist\n");
        return;
    }

    return;
}

void compact()
{
    if (!mainMem->prev && !mainMem->next)
    {
        return;
    }

    int totalFreeMem = 0;

    NODE *curr = mainMem;
    NODE *lastUsed = NULL;

    while (curr)
    {
        if (!curr->used)
        {
            totalFreeMem += curr->memSize;
        }
        curr = curr->next;
    }

    if (totalFreeMem == 0)
    {
        printf("No free memory to compact.\n");
        return;
    }

    curr = mainMem;
    int currentStart = 0;
    while (curr != NULL)
    {
        NODE *next = curr->next;
        if (curr->used)
        {
            curr->start = currentStart;
            curr->end = currentStart + curr->memSize - 1;
            currentStart = curr->end + 1;
            lastUsed = curr;
        }
        else
        {
            if (curr->prev)
            {
                curr->prev->next = curr->next;
            }
            if (curr->next)
            {
                curr->next->prev = curr->prev;
            }
            if (curr == mainMem)
            {
                mainMem = curr->next;
            }
            free(curr);
        }
        curr = next;
    }

    NODE *newHole = (NODE *)malloc(sizeof(NODE));
    newHole->used = 0;
    newHole->memSize = totalFreeMem;
    newHole->start = lastUsed->end + 1;
    newHole->end = totalFreeMem - 1;
    newHole->next = NULL;
    newHole->prev = lastUsed;

    if (lastUsed)
    {
        lastUsed->next = newHole;
    }
    else
    {
        mainMem = newHole;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Format of command line input must be './allocator <memory>'\n");
        return -1;
    }

    if (!(atoi(argv[1])))
    {
        printf("atoi: %d(%s)\n", errno, strerror(errno));
        printf("Format of command line input must be './prodcon <delay> <#producer threads> <#consumer threads>'\n");
        return -1;
    }

    if (atoi(argv[1]) < 0)
    {
        printf("Number of points must be positive\n");
        return -1;
    }

    totalMem = atoi(argv[1]);

    mainMem = (NODE *)malloc(sizeof(NODE));
    mainMem->used = 0;
    mainMem->memSize = totalMem;
    mainMem->start = 0;
    mainMem->end = totalMem - 1;
    mainMem->next = NULL;
    mainMem->prev = NULL;

    while (1)
    {
        printf("allocator>");
        char input[100];
        scanf("%s", input);

        // Request
        if (strcmp(input, "RQ") == 0)
        {
            while (1)
            {
                char processID[100];
                int memSize;
                char strategy;
                scanf("%s", processID);
                scanf("%d", &memSize);
                scanf(" %c", &strategy);
                if ((processID[0] != '\0') && (memSize > 0) && (strategy != '\0'))
                {
                    request(processID, memSize, strategy);
                    break;
                }
                printf("Invalid request, please format as: RQ processName memorySize strategy");
            }
        }

        // Release
        else if (strcmp(input, "RL") == 0)
        {
            char processID[100];
            scanf("%s", processID);
            release(processID);
        }

        // Compact
        else if (strcmp(input, "C") == 0)
        {
            compact();
        }

        // used report
        else if (strcmp(input, "STAT") == 0)
        {
            report();
        }

        // Exit
        else if (strcmp(input, "X") == 0)
        {
            return 0;
        }

        else
        {
            printf("Invalid command, enter either RQ, RL, C, STAT, or X\n");
        }
    }
}