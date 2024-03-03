#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>

#include "mmu.h"
#include "pagetable.h"
#include "memsim.h"

int pageToEvict = 1;

typedef struct
{
    int ptStartPA;
    int present;
} ptRegister;

// Page table root pointer register values
// One stored for each process, swapped in with process)
ptRegister ptRegVals[NUM_PROCESSES];

/*
 * Sets the page table entry for a given process using teh pid, VPN, PFN, and other flags.
 */
void PT_SetPTE(int pid, int VPN, int PFN, int valid, int protection, int present, int referenced)
{
    // todo - completed

    int pt_start = ptRegVals[pid].ptStartPA;
    int index = pt_start + VPN;          // calculate index where PTE should be stored in physical memory array
    char *physmem = Memsim_GetPhysMem(); // pointer to physical memory array
    int *pt_entry = (int *)&physmem[index];
    *pt_entry = (PFN << 8) | (valid << 7) | (protection << 6) | (present << 5) | (referenced << 4); // update PTE at index by setting bits according to given information
}

/*
 * Set all PTE valid bits to zero (invalid)
 * Returns a new page for the map instruction
 */
int PT_PageTableInit(int pid, int pa)
{
    // todo - completed

    char *physmem = Memsim_GetPhysMem(); // pointer to physical memory array
    int pt_start = pa;                   // page table start in physical memory

    // set PTE valid bits as invalid
    for (int i = 0; i < PAGE_SIZE; i++)
    {
        int index = pt_start + i;
        int *pt_entry = (int *)&physmem[index];
        *pt_entry = 0;
    }

    return pt_start; // was originally return 0
}

void PT_PageTableCreate(int pid, int pa)
{
    // todo - completed

    char *physmem = Memsim_GetPhysMem();      // pointer to physical memory array
    int pt_start = PT_PageTableInit(pid, pa); // initialize page table for the given pid at the given physical address
    ptRegVals[pid].ptStartPA = pt_start;      // update page table register for pid with start address of page table
}

int PT_PageTableExists(int pid)
{
    // todo - completed

    // if the start address is not zero, the page table exists for the process
    if (ptRegVals[pid].ptStartPA != 0)
    {
        return 1; // page table exists
    }
    else
    {
        return 0; // page table doesn't exist
    }
}

/* Gets the location of the start of the page table. If it is on disk, brings it into memory. */
int PT_GetRootPtrRegVal(int pid)
{
    // todo - completed

    if (PT_PageTableExists(pid))
    {
        // if page table exists, return its start address
        return ptRegVals[pid].ptStartPA;
    }
    else
    {
        // if page table doesn't exist, bring it into memory
        int new_pa = Memsim_FirstFreePFN();
        if (new_pa != -1)
        {
            // initialize page table and return start address
            return PT_PageTableInit(pid, new_pa);
        }
        else
        {
            // return error
            return -1;
        }
    }
}

/*
 * Evicts the next page.
 * Updates the corresponding information in the page table, returns the location that was evicted.
 *
 * The supplied input and output used in autotest.sh *RR tests, uses the round-robin algorithm.
 * You may also implement the simple and powerful Least Recently Used (LRU) policy,
 * or another fair algorithm.
 */
int PT_Evict()
{
    char *physmem = Memsim_GetPhysMem();
    FILE *swapFile = MMU_GetSwapFileHandle();
    // todo - completed

    // write page to swap file
    int pageToEvict2 = pageToEvict * PAGE_SIZE;
    fseek(swapFile, pageToEvict2, SEEK_SET);
    fwrite(&physmem[pageToEvict2], sizeof(char), PAGE_SIZE, swapFile);

    // update page table entry
    int pid;
    for (pid = 0; pid < NUM_PROCESSES; pid++)
    {
        if (PT_VPNtoPA(pid, pageToEvict) != -1)
        {
            // found process
            PT_SetPTE(pid, pageToEvict, 0, 0, 0, 0, 0); // page is on disk
            break;
        }
    }

    // move on to next page
    pageToEvict = (pageToEvict + 1) % NUM_PAGES;

    return 0;
}

/*
 * Searches through the process's page table. If an entry is found containing the specified VPN,
 * return the address of the start of the corresponding physical page frame in physical memory.
 *
 * If the physical page is not present, first swaps in the phyical page from the physical disk,
 * and returns the physical address.
 *
 * Otherwise, returns -1.
 */
int PT_VPNtoPA(int pid, int VPN)
{
    char *physmem = Memsim_GetPhysMem();
    // todo - pretty much done

    int pt_start_pa = ptRegVals[pid].ptStartPA;
    int index = pt_start_pa + VPN; // calculate index of page tabe entry that corresponds to the VPN

    // check if page table entry is valid
    int *pt_entry = (int *)&physmem[index];

    // check if valid bit is set
    if (*pt_entry & (1 << 7))
    {
        // get PFN from page table entry
        int PFN = *pt_entry & ~(1 << 7); // clear valid bit to get PFN

        int pa = PFN * PAGE_SIZE;

        return pa;
    }
    else
    {
        // HANDLE SWAPPING in the MMU_TranslateAddress function
        return -1;
    }
}

/*
 * Finds the page table entry corresponding to the VPN, and checks
 * to see if the protection bit is set to 1 (readable and writable).
 * If it is 1, it returns TRUE, and FALSE if it is not found or is 0.
 */
int PT_PIDHasWritePerm(int pid, int VPN)
{
    char *physmem = Memsim_GetPhysMem();
    // todo - done

    int pt_start_pa = ptRegVals[pid].ptStartPA;

    int index = pt_start_pa + VPN;

    // check if page table entry is valid and protection bit is set to 0
    int *pt_entry = (int *)&physmem[index];
    if (*pt_entry & (1 << 7) && (*pt_entry & (1 << 6))) // valid and protection bits
    {
        return 1;
    }
    else
    {
        // page isn't present or doesn't have right permissions
        return 0;
    }
}

/* Initialize the register values for each page table location (per process). */
void PT_Init()
{
    // todo - completed

    for (int i = 0; i < NUM_PROCESSES; i++)
    {
        ptRegVals[i].ptStartPA = -1; // set page table start address
        ptRegVals[i].present = 0;    // set as not present
    }
}
