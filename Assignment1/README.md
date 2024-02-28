## Information
* Name: Noah Fuery
* Student ID: 2377792
* Course: CPSC 380 - Operating Systems
* Date: Spring 2024

## Program instructions
* gcc time.c -o time
* ./time <command>

## Program detail
Takes in a valid linux command on the command line. The command is forked and executed in a child process. The time before the fork and after the child process completes is taken and calculated to give the time to execute the linux command.