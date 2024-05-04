# Virtual Address Manager

The objective of this assignment consists of writing a C/C++ program that translates logical to physical addresses for a virtual address space of size 216 = 65,536 bytes. The program will read from a file containing logical addresses and, using a TLB as well as a page table, will translate each logical address to its corresponding physical address and output the value of the byte stored at the translated physical address. The goal behind this project is to simulate the steps involved in translating logical to physical addresses.

## Contributors

Ponthea Zahraii </br>
2366637

Noah Fuery </br>
2377792

## Included Files

`vmmgr.c`
`addresses.txt`
`BACKING_STORE.bin`

## Execution Intructions

`gcc vmmgr.c -o vmmgr` </br>
`./vmmgr addresses.txt` </br>
Must have `BACKING_STORE.bin` in working directory

## Sample Output

`output.txt`

## References

[Man Pages](https://man7.org/linux/man-pages/) </br>
Course Material