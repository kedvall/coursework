#include "lib.h"

// Constant definitions
#define PD_SIZE 1024
#define PDPERMS_K_RW_NP 0x00000002 //Attributes = kernel access only, read / write enabled, not present
#define PDPERMS_K_RW_P 0x00000003 //Attributes = kernel access only, read / write enabled, present
#define PDPERMS_4MB_K_RW_P 0x83 //Attributes = Page size is 4MB, read / write, present
#define PT_BASE_ADDR 0x1000
#define KERNEL_BASE_ADDR 0x400000
#define VID_MEM_PAGE 184
#define TERM1_VIDEO_PAGE 185
#define TERM2_VIDEO_PAGE 186
#define TERM3_VIDEO_PAGE 187
#define USER_LEVEL_PAGE_FLAGS 0x7


//Global vars
uint32_t first_page_table[1024] __attribute__((aligned(4096)));
uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t second_page_table[1024] __attribute__((aligned(4096)));

//External functions
extern void init_paging();
extern void enablePaging();
extern void enablePageExtension();
