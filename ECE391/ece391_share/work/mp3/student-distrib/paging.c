#include "lib.h"
#include "paging.h"

//Global vars
uint32_t first_page_table[1024] __attribute__((aligned(4096)));
uint32_t page_directory[1024] __attribute__((aligned(4096)));

/*
 * int_paging
 *   DESCRIPTION: Set up the page directory structure and initialize page directories
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: A page directory structure is created
 */
void init_paging(){
    int i;

    for(i = 0; i < PD_SIZE; i++){
        // This sets the following flags to the pages:
        //   Supervisor: Only kernel-mode can access them
        //   Write Enabled: It can be both read from and written to
        //   Not Present: The page table is not present
        page_directory[i] = PDPERMS_K_RW_NP; //TODO: update this to allow user code to access memory

        // As the address is page aligned, it will always leave 12 bits zeroed.
        // Those bits are used by the attributes ;)
        if((i == VID_MEM_PAGE) || (i == TERM1_VIDEO_PAGE) || (i == TERM2_VIDEO_PAGE) || (i == TERM3_VIDEO_PAGE)){
            //in vid memory
            //set as
            first_page_table[i] = (i * PT_BASE_ADDR) | PDPERMS_K_RW_P; // attributes: supervisor level, read/write, present.
        }
        else {
            first_page_table[i] = (i * PT_BASE_ADDR) | PDPERMS_K_RW_NP;
        }
    }

    // attributes: supervisor level, read/write, present
    //0x80 (sets to 4kb) | 0x01 (present) | 0x02 (read/write)
    //set cr4 before cr0
    //set video memory
    //kernel begins at 4mb
    page_directory[0] = ((unsigned int)first_page_table) | PDPERMS_K_RW_P;
    uint32_t addr = KERNEL_BASE_ADDR;
    page_directory[1] = addr | PDPERMS_4MB_K_RW_P; //set to 4mb, present, read/write

    //Puts the address of the page directory into CR3
    asm volatile("                  \n\
        movl %0, %%eax          \n\
        movl %%eax, %%cr3       \n\
        "
        :
        :"r"(page_directory)
        :"%eax"
        );

    enablePageExtension();
    enablePaging();
}


/*
 * enablePaging
 *   DESCRIPTION: Enables the x86 page-translation mechanism (paging)
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
void enablePaging()
{
    asm volatile("                  \n\
            movl %%cr0, %%eax          \n\
            orl $0x80000000, %%eax    \n\
            movl %%eax, %%cr0          \n\
            "
            :
            :
            :"%eax"
            );
}


/*
 * enablePageExtension
 *   DESCRIPTION: Enables large page sizes (4-MByte pages)
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
void enablePageExtension()
{
    asm volatile("                  \n\
            movl %%cr4, %%eax          \n\
            orl $0x00000010, %%eax    \n\
            movl %%eax, %%cr4          \n\
            "
            :
            :
            :"%eax"
            );
}
