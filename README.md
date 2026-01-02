# MeMS: Memory Management System [CSE231 OS Assignment 3]

MeMS is a custom Memory Management System designed to simulate heap management using a hierarchical doubly-linked list structure called a Free List. The system utilizes mmap for initial memory allocation and dynamically manages segments as either PROCESS (allocated) or HOLE (free).

## System Architecture: The Free List

The core of MeMS is a two-tier doubly-linked list:
* **Main Chain**: A list of large memory segments allocated via the system.
* **Sub-Chain**: Each main node points to a sub-chain of segments representing the actual memory space. Each sub-node denotes whether a specific block is a PROCESS or a HOLE.

The system maps virtual addresses to physical addresses, efficiently reusing memory segments and minimizing fragmentation through segment merging.



## Key Functions

### void mems_init()
Initializes the MeMS system.
* Allocates memory for internal data structures (main nodes and sub-nodes) using mmap.
* Allocates the initial heap virtual memory space.
* Sets up the initial free list (fl_head) with a starting main node and a sub-node representing a large initial hole.

### void* mems_malloc(size_t size)
Allocates memory of the specified size.
* **Reuse**: Traverses the free list to find a suitable HOLE segment.
* **Allocation**: If a hole is found, it is marked as PROCESS.
* **Expansion**: If no suitable segment exists, it calls mmap to request more memory from the system, updating the main chain accordingly.

### void mems_free(void* v_ptr)
Deallocates the memory segment associated with the given virtual address.
* Locates the segment in the sub-chain and changes its type to HOLE.
* **Coalescing**: To combat fragmentation, it automatically merges adjacent HOLE segments into a single larger block.

### void* mems_get(void* v_ptr)
Translates a MeMS virtual address into its corresponding physical address.
* Traverses the chains to find the segment containing v_ptr.
* Calculates the physical address by adding the segment offset to the base physical address.

### void mems_print_stats()
Provides a detailed snapshot of the memory state:
* Reports total unused memory (total size of all HOLE segments).
* Displays the number of utilized pages.
* Prints the structure of the main chain and the length/type of each sub-chain segment.

### void mems_finish()
Finalizes the system.
* Traverses all segments and calls munmap to release all memory back to the operating system.

## Getting Started

### Prerequisites
* Linux/Unix environment.
* GCC compiler and Make.

### Running the Example
To run the provided example.c and test your implementation:

```bash
# Compile the project using the Makefile
$ make

# Execute the example binary
$ ./example
