*This project has been created as part of the 42 curriculum by dde-paul*

""" Description: 
In the Codexion project we have to solve a problem similar to the Philosophers dining problem. We have a certain amount of coders in a table who have the goal of take two dongles in order to compile and consequently debug and refactor. At the left and at the right of each coder you have a dongle and in the middle of the table you have a Quantum compiler, to solve this you must synchronize each thread which is represented by coder and synchronize all of them to avoid problems as Deadlock or racing conditions, through Mutex, Routines (Monitor, Coder), schedules (fifo/edf) and variable conditions.

""" Instuctions :

Installation:
make
make re

Usage:
./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_refactor> <time_to_debug> <cooldown> <compiles_needed> <schedule (fifo/edf)>

*All the arguments are given in miliseconds in exception of <number_of_coders> <compiles_needed> and <schedule (fifo/edf)>*

Cleanup:
make clean or clean fclean

""" Blocking cases handled:

Concurrency issues: 

*Racing condition: Threads can very often  experience concurrency issues 
for example the racing condition, 
where two threads access the same critical section simultaneously 
and as threads have shared memory they will for example in a incrementation do it twice but as they did it simultaneously it's going to count as one increment.


*Deadlock prevention: Deadlock occurs when two threads hold one resource each and 
wait for each other's resource (circular wait). By aquiring dongles in a fixed order 
(by memory address), I broke the circular wait condition.

*Visual Aid*
*Coder 1* wants a dongle A and B: always acquires A first (the one lower address)
*Coder 2* wants the same dongles: also acquires A first (same order)
-> No circular wait -> No DEADLOCK

*Starvation Prevention*

