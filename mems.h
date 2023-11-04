/*
All the main functions with respect to the MeMS are inplemented here
read the function discription for more details

NOTE: DO NOT CHANGE THE NAME OR SIGNATURE OF FUNCTIONS ALREADY PROVIDED
you are only allowed to implement the functions 
you can also make additional helper functions a you wish

REFER DOCUMENTATION FOR MORE DETAILS ON FUNSTIONS AND THEIR FUNCTIONALITY
*/
// add other headers as required
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>

/*
Use this macro where ever you need PAGE_SIZE.
As PAGESIZE can differ system to system we should have flexibility to modify this 
macro to make the output of all system same and conduct a fair evaluation. 
*/
#define PAGE_SIZE 4096
int num_nodes=0;

struct sub_node{
    
    struct sub_node *next;
    struct sub_node *prev;
    char *type;
    size_t s_va;
    size_t e_va;
};

struct main_node{
    struct main_node *next;
    struct main_node *prev;
    struct sub_node *sub_head;
    size_t s_va;
    size_t e_va;
    void *s_ptr;
    int num_pages;
};



void *node_size;
void *starting_pa;

struct main_node *fl_head = NULL;
/*
Initializes all the required parameters for the MeMS system. The main parameters to be initialized are:
1. the head of the free list i.e. the pointer that points to the head of the free list
2. the starting MeMS virtual address from which the heap in our MeMS virtual address space will start.
3. any other global variable that you want for the MeMS implementation can be initialized here.
Input Parameter: Nothing
Returns: Nothing
*/
void mems_init(){
    node_size = mmap(NULL,2*PAGE_SIZE,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_PRIVATE|MAP_ANONYMOUS,0,0);
    if (node_size == MAP_FAILED)
    {
        perror("MMAP FAILED\n");
    }
    starting_pa = mmap(NULL,PAGE_SIZE,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_PRIVATE|MAP_ANONYMOUS,0,0);

    if(starting_pa == MAP_FAILED){
        perror("MMAP FAILED\n");
    }


    

    struct main_node* initial_chain =(struct main_node *)(node_size);
    num_nodes++;
    node_size+=sizeof(struct main_node);
    initial_chain->next = NULL;
    initial_chain->prev = (struct main_node *)(node_size);
    node_size+=sizeof(struct main_node);
    initial_chain->prev->next = initial_chain;
    initial_chain->sub_head = (struct sub_node *)(node_size);
    node_size += sizeof(struct sub_node);
    initial_chain->s_va = 1000;
    initial_chain->e_va = 1000 + 4096-1;
    
    initial_chain->sub_head->next = NULL;
    initial_chain->sub_head->s_va = initial_chain->s_va;
    initial_chain->sub_head->e_va = initial_chain->e_va;
    initial_chain->sub_head->type = "HOLE";
    initial_chain->s_ptr = starting_pa;
    initial_chain->num_pages = 1;
    fl_head = initial_chain;



    printf("%lu\n",sizeof(initial_chain->sub_head));
    printf("%lu\n",sizeof(initial_chain));
    

    

}



/*
This function will be called at the end of the MeMS system and its main job is to unmap the 
allocated memory using the munmap system call.
Input Parameter: Nothing
Returns: Nothing
*/


void mems_finish(){
    struct main_node *ptr = fl_head;

    while(ptr!=NULL){
        munmap(ptr->s_ptr,ptr->e_va-ptr->s_va+1);
        printf("memory unmapped.\n");
        ptr = ptr->next;
    }
    
}


/*
Allocates memory of the specified size by reusing a segment from the free list if 
a sufficiently large segment is available. 

Else, uses the mmap system call to allocate more memory on the heap and updates 
the free list accordingly.

Note that while mapping using mmap do not forget to reuse the unused space from mapping
by adding it to the free list.
Parameter: The size of the memory the user program wants
Returns: MeMS Virtual address (that is created by MeMS)
*/ 





void* mems_malloc(size_t size) {
    
    struct main_node *ptr = fl_head;
    struct main_node *ptrr = fl_head ->prev;
    
    while (ptr!= NULL) {
        
            
            
            struct sub_node *ptr2 = ptr->sub_head;
        
            while (ptr2!= NULL) {  
                
                if ((ptr2->e_va - ptr2->s_va +1) >= size && (strcmp(ptr2->type, "HOLE") == 0)) {
                    size_t new_size = (ptr2->e_va - ptr2->s_va+1) - size;

                    if(new_size==0){
                        ptr2->type = "PROCESS";
                        return (void *)ptr2->s_va;
                    }
                    struct sub_node *temp = (struct sub_node*)(node_size);
                    node_size+=sizeof(struct sub_node);
                    temp->next = ptr2->next;
                    temp->prev = ptr2;
                    ptr2->next = temp;
                    ptr2->e_va=ptr2->s_va+size-1;
                    temp->s_va= ptr2->e_va+1;
                    temp->e_va = ptr->e_va;
                    temp->type = "HOLE";
                    
                    
                    ptr2->type = "PROCESS";
                    
                    return  (void *)ptr2->s_va;
                }
                
                ptr2 = ptr2->next;
            }
            
            ptr= ptr->next;
            ptrr = ptrr ->next;
        }

        
    
    void* new_va;
int p_s;
int c;
while(1){
    int i=1;
    if(size<=i*PAGE_SIZE){
        
        new_va = mmap(NULL,i*PAGE_SIZE,PROT_READ |PROT_WRITE|PROT_EXEC,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
        p_s = i*PAGE_SIZE;
        c=i;
        break;
    }
    i++;

}
struct main_node *new_ptr =(struct main_node *)(node_size);
num_nodes++;
node_size+=sizeof(struct main_node);


new_ptr->prev = ptrr;

new_ptr->next= NULL;

new_ptr->num_pages = c;

new_ptr->s_ptr = new_va;

new_ptr->s_va = ptrr->e_va+1;


new_ptr->e_va = new_ptr->s_va + p_s -1;

new_ptr->sub_head = (struct sub_node*)(node_size);

node_size += sizeof(struct sub_node);
ptrr->next = new_ptr;


//printf("Address of next_address:- %p\n Address of new_va:- %p\n",next_address, new_va);
new_ptr->sub_head->next =(struct sub_node*)(node_size);
node_size+=sizeof(struct sub_node);
new_ptr->sub_head->prev = NULL;
new_ptr->sub_head->s_va = new_ptr->s_va;
new_ptr->sub_head->e_va =new_ptr->s_va+size-1;
new_ptr->sub_head->next-> s_va= new_ptr->sub_head->e_va+1;
new_ptr->sub_head->next->e_va = new_ptr->e_va;

new_ptr->sub_head->next->type="HOLE";
new_ptr->sub_head->type = "PROCESS";

return (void *)new_ptr->sub_head->s_va; 

    
}




/*
this function print the stats of the MeMS system like
1. How many pages are utilised by using the mems_malloc
2. how much memory is unused i.e. the memory that is in freelist and is not used.
3. It also prints details about each node in the main chain and each segment (PROCESS or HOLE) in the sub-chain.
Parameter: Nothing
Returns: Nothing but should print the necessary information on STDOUT
*/
void mems_print_stats(){
    size_t unused_mem=0;
    int num_pages=0;
    struct main_node *ptr = fl_head;
    int sub_size[num_nodes];
    int iter=0;
    while(ptr!=NULL){
        num_pages+= ptr->num_pages;
        printf("MAIN[%lu:%lu]->",ptr->s_va,ptr->e_va);
        struct sub_node *ptr2 = ptr->sub_head;
        int c=0;
        while(ptr2!=NULL){
            if(ptr2->type[0] =='H'){
                unused_mem+= (ptr2->e_va - ptr2->s_va+1);
            }
            c++;
            printf("%c[%lu:%lu]<->",ptr2->type[0],ptr2->s_va,ptr2->e_va);
            ptr2 = ptr2->next;
        }
        printf("NULL\n");
        sub_size[iter]=c;
        iter++;
        ptr= ptr->next;
    }
    printf("\n");

    printf("Unused memory is:%lu\n",unused_mem);
    printf("Number of pages is:%d\n",num_pages);

    printf("Main chain length is:%d\n",num_nodes);
    printf("Subchain length array is:");
    printf("[");
    for(int i=0;i<num_nodes;i++){
        printf("%d ,",sub_size[i]);
    }
    printf("]\n");
}



/*
Returns the MeMS physical address mapped to ptr ( ptr is MeMS virtual address).
Parameter: MeMS Virtual address (that is created by MeMS)
Returns: MeMS physical address mapped to the passed ptr (MeMS virtual address).
*/
void *mems_get(void*v_ptr){
    size_t size = (int *)v_ptr;
    struct main_node *ptr = fl_head;
    
    while(ptr!=NULL){
        if(ptr->s_va <= size && ptr->e_va >= size){
            return (void *)(ptr->s_ptr +(size));
        }
        ptr = ptr->next;
    }
    printf("This address does not exist in the system.\n");
}


/*
this function free up the memory pointed by our virtual_address and add it to the free list
Parameter: MeMS Virtual address (that is created by MeMS) 
Returns: nothing
*/
void mems_free(void *v_ptr){
    int address = (int *)(v_ptr);
    
    struct main_node *ptr = fl_head;
    while(ptr!=NULL){
        struct sub_node *ptr2 = ptr->sub_head;
        while(ptr2!=NULL){
            if(ptr2->s_va <= address && ptr2->e_va >= address){
                ptr2->type = "HOLE";
                if(ptr2->prev->type[0]=='H' || ptr2->next->type[0]=='H'){
                    if(ptr2->prev->type[0]=='H' && ptr2->next->type[0]=='H'){
                        ptr2->s_va = ptr2->prev->s_va;
                        ptr2->e_va = ptr2->next->e_va;
                        ptr2->next = ptr2->next->next;
                        ptr2->prev = ptr2->prev->prev;
                        
                    }
                    else if(ptr2->next->type[0]=='H'){
                        ptr2->e_va = ptr2->next->e_va;
                        ptr2->next = ptr2->next->next;
                    }
                    else{
                        ptr2->s_va = ptr2->prev->s_va;
                        ptr2->prev = ptr2->prev->prev;
                    }
        
                }
                return;
            }
            ptr2 = ptr2->next;
        }
        ptr = ptr->next;

    }
    printf("Address is not found!\n");
    return;
}
