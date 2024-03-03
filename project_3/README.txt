CS3013 Project 3 by Sarayu Vijayanagaram

This virtual memory manager simulates virtual memory management using page tables and physical memory. 
This virtual memory manager contains
    - mmu.c: handles translation, initializes processing instructions, manages swap files
    - pagetable.c: keeps track of the mapping, initalizes, creats, evicts pages
    - memsim.c: simulates physical memory and tracks free pages
    - instruction.c: handles mapping, storing, and loading
    - input.c: handles input

For the mapping instruction, physical pages that are free are identified, or pages are evicted if needed. For the store instruction, values are stored at virtual addresses. For the load instruction, values are loaded from virtual addresses if they are valid.

Run Instructions:
    ./mmu
