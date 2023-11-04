# MeMS: Memory Management System [CSE231 OS Assignment 3]
[Documentation](https://docs.google.com/document/d/1Gs9kC3187lLrinvK1SueTc8dHCJ0QP43eRlrCRlXiCY/edit?usp=sharing)

In this assignment, we were tasked with creating a Memory managment system that uses a data structure that we created called 'Free List' which consists of a doubly-linked list which contains main nodes ,each of which points to another doubly-linked list called sub_chain which denotes our memory space and mentions which type(PROCESS or HOLE) certain memory is.

We mmap to allocate memory for the nodes for the Free List and use mmap everytime we allocate more memory from the mems_malloc calls.

void mems_init()

This function initializes the MeMS system. It performs the following tasks:

    Calls mmap to allocate memory for data structures required for MeMS. It allocates memory for the main node and sub-node structures, which will be used to manage memory.

    Initializes the node_size variable to point to the allocated memory, which will be used to manage memory nodes.

    Calls mmap again to allocate the initial virtual memory space (starting_pa) for the MeMS heap. This is the space from which memory allocations will occur.

    Sets up the initial memory chain. It creates a main node and a sub-node to represent the initial state of the memory.

    Updates the free list (fl_head) to point to the main node, which is the head of the free list.

void mems_finish()

This function is used to finalize the MeMS system. It unmaps (deallocates) all memory used by the MeMS system by calling munmap. It traverses the memory chain and deallocates each segment of memory.
void* mems_malloc(size_t size)

The mems_malloc function allocates memory of the specified size. It attempts to reuse segments from the free list if available. If a sufficiently large segment is found in the free list, it is marked as used. If not, it uses the mmap system call to allocate more memory from the system heap.

Key steps:

    It traverses the free list (main chain) and its sub-chains (segments) to find a suitable segment for allocation.
    If a suitable segment is found in the free list, it updates the segment's status to "PROCESS" and returns the virtual address within that segment.
    If a suitable segment is not found in the free list, it uses mmap to allocate a new segment from the system heap, updates the memory chain, and returns the virtual address within the new segment.

void mems_print_stats()

This function prints various statistics about the MeMS system:

    It calculates the total unused memory in the system (memory in the "HOLE" segments) and the total number of pages utilized.

    It prints details about each main node in the memory chain, including the start and end virtual addresses and the type of each segment.

    It also prints the length of each sub-chain, which indicates the number of segments (PROCESS or HOLE) in each main node's sub-chain.

void* mems_get(void* v_ptr)

The mems_get function returns the MeMS physical address mapped to a given MeMS virtual address.

Key steps:

    It takes a virtual address (v_ptr) as input and traverses the main chain and sub-chains to find the corresponding physical address.

    If the provided virtual address is found within a segment, it calculates the physical address by adding the offset within the segment to the starting physical address of the segment.

void mems_free(void* v_ptr)

The mems_free function frees the memory pointed to by a MeMS virtual address. It adds the freed memory to the free list.

Key steps:

    It takes a virtual address (v_ptr) as input and searches the main chain and sub-chains to locate the corresponding segment.

    If the segment is found, it updates its type to "HOLE."

    If the adjacent segments are also "HOLE," it merges them into a larger "HOLE" segment to reduce fragmentation.

These functions collectively manage memory allocation, deallocation, and provide statistics about the MeMS memory management system. The system efficiently reuses memory and minimizes fragmentation.
---

### How to run the example.c
After implementing functions in mems.h follow the below steps to run example.c file
```
$ make
$ ./example
``` 

