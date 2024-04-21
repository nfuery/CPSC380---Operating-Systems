# Continguous Memory Allocation

This project will involve managing a contiguous region of memory of size `MAX` where addresses may range `0 ... MAX-1`. The program must respond to four different requests:

1. Request for a contiguous block of memory.
2. Release of a continuous block of memory.
3. Compact usused holes of memroy into one single block.
4. Report the regions of free and allocated memory.

## Contributors

Ponthea Zahraii </br>
2366637

Noah Fuery </br>
2377792

## Included Files

`allocator.c`

## Execution Intructions

`gcc allocator.c -o allocator` </br>
`./allocator <memory>`

## Usage

### Request

Command `RQ <processID> <memory> <strategy>`

Strategies:

- First Fit: `F`
- Best Fit: `B`
- Worst Fit: `W`

### Release

Command: `RL <processID>`

### Compact

Command: `C`

### Status Report

Command: `STAT`

### Exit

Command: `X`

## Sample Output

```
$.\allocator 1048576
allocator>PR
Invalid command, enter either RQ, RL, C, STAT, or X
allocator>STAT
Addresses [0:1048575] Unused
allocator>RQ P0 40000 F
allocator>RQ P1 40000 F
allocator>RQ P2 40000 F
allocator>STAT
Addresses [0:39999] Process P0
Addresses [40000:79999] Process P1
Addresses [80000:119999] Process P2
Addresses [120000:1048575] Unused
allocator>RL P1
allocator>STAT
Addresses [0:39999] Process P0
Addresses [40000:79999] Unused
Addresses [80000:119999] Process P2
Addresses [120000:1048575] Unused
allocator>RQ P3 40000 W
allocator>STAT
Addresses [0:39999] Process P0
Addresses [40000:79999] Unused
Addresses [80000:119999] Process P2
Addresses [120000:159999] Process P3
Addresses [160000:1048575] Unused
allocator>RQ P4 30000 B
allocator>STAT
Addresses [0:39999] Process P0
Addresses [40000:69999] Process P4
Addresses [70000:79999] Unused
Addresses [80000:119999] Process P2
Addresses [120000:159999] Process P3
Addresses [160000:1048575] Unused
allocator>C
allocator>STAT
Addresses [0:39999] Process P0
Addresses [40000:69999] Process P4
Addresses [70000:109999] Process P2
Addresses [110000:149999] Process P3
Addresses [150000:1048575] Unused
allocator>RQ P5 600000000 B
No contingous memory avalible for Process P5
allocator>RL P6
Process does not exist
allocator>RL P0
allocator>RL P2
allocator>RL P3
allocator>RL P4
allocator>STAT
Addresses [0:1048575] Unused
allocator>X
```

## References

[Man Pages](https://man7.org/linux/man-pages/) </br>
Course Material
