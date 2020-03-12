#include "directory.h"

static int count;
static boot_block_t b_block;
static uint32_t* inode0;
static uint8_t* dataBlock0;

/* Flag for whether boot block has been initialized or not */
static int32_t boot_block_initialized = 0;

// rename this directory_init
int32_t directory_open(const uint8_t* filename)
{
  /* Initialize the boot block and set the internal count to 0 */
  b_block.num_entries = *(mod_start_address);
  b_block.num_inodes = *(mod_start_address + 1);
  b_block.num_datablocks = *(mod_start_address + 2);
  b_block.first_dir_entry = (uint8_t*)mod_start_address + 64;

  count = 0;

  /* Acquire the starting address of the first inode and
  the first data block */
  inode0 = mod_start_address + BLOCK_SIZE / 4;
  dataBlock0 = (uint8_t*)inode0 + b_block.num_inodes * BLOCK_SIZE;

  boot_block_initialized = 1;

  return 0;
}

int32_t directory_read(int32_t fd, void* buf, int32_t nbytes)
{
    dentry_t d; // struct to store directory entry info
    int i; // iterator to use in the for loop
    int ret = read_dentry_by_index(count, &d);


    if(ret < 0)
    {
        count = 0;
        return 0;
    }

    uint8_t* curr_buf;
    curr_buf = (uint8_t*)buf;

    count++;

    for(i = 0; i < nbytes; i++)
    {
        if(!d.file_name[i])
            break;

        curr_buf[i] = d.file_name[i];
    }

    curr_buf[i] = '\n';
    return i;
}

// this is called for when we need to write to the file directory. but wait! we're not supposed to!
int32_t directory_write(int32_t fd, const void* buf, int32_t nbytes)
{
    return -1;  // ur dum
}

// this is called for when we need to close the file directory
int32_t directory_close(int32_t fd)
{
    return 0;
}

uint32_t* get_inode_ptr(int32_t inode_num)
{
    if(!directory_valid())
        return NULL;

    return inode0 + (inode_num * BLOCK_SIZE / 4);
}

int32_t read_data(uint32_t* inode, uint32_t offset, uint8_t* buf, uint32_t length)
{
  /* Check to see if inode is legit */
  int32_t inode_num = (inode - inode0) / (BLOCK_SIZE / 4);
  if(inode_num < 0 || inode_num >= b_block.num_inodes)
    return -1;

  if(length < 0)
    return -1;

  uint32_t numBytesRead;

  /* Changed line below and argument inode above */
  //uint32_t* currInode = inode0 + (inode * BLOCK_SIZE / 4);
  uint32_t* currInode = inode;
  //printf("%d", *currInode);
  uint32_t lengthInode = (uint32_t)*currInode;
  //rintf("Length of inode: %d\n", lengthInode);

  /* Acquire pointer to first data block index based on offset */
  uint32_t* dataBlockIndex = (offset / BLOCK_SIZE) + (uint32_t*)currInode + 1;
  //printf("Data Block Index: %d", *dataBlockIndex);

  /* Acquire first byte within a data block to read from */
  uint32_t curByte = offset % BLOCK_SIZE;

  /* Acquire pointer to first piece of data to read from */
  uint8_t* dataBlock = dataBlock0 + (BLOCK_SIZE * (*dataBlockIndex));

  /* Question: should we return an error if the length argument > the inode length? */

  for(numBytesRead = 0; numBytesRead < length; numBytesRead++)
  {
    if((offset + numBytesRead) >= lengthInode)
      break;
      //return 0; // We've reached the end of the file

    buf[numBytesRead] = dataBlock[curByte++];

    /* If we've reached the end of a block, fetch a new one :D */
    if(curByte >= 4096)
    {
      dataBlockIndex++;
    //  printf("Data Block Index: %d", *dataBlockIndex);        // commenting this out. why is this here?! ----- Debugging you idiot, heard of it? No you probably just give up when code doesn't work don't you? Wuss.
      dataBlock = dataBlock0 + (BLOCK_SIZE * (*dataBlockIndex));
      curByte = 0;
    }
  }

  return numBytesRead;
}

int32_t demo_filesys(uint8_t mode)
{
    static uint32_t fileCount;
    uint32_t i; //Iterator
    dentry_t dentry; //Holds file info
    uint32_t file_size; //Size of the current file in bytes
    uint32_t* currInode;
    uint32_t lengthInode;

    if (mode == 1)
    {
        //Iterate through all files and get file info
        for (i = 0; i < b_block.num_entries; i++)
        {
            read_dentry_by_index(i, &dentry);

            if (dentry.file_type != 2) //Correct for non regular files
                file_size = 0;

            currInode = inode0 + (dentry.inode * BLOCK_SIZE / 4);
            lengthInode = (uint32_t)*currInode;

            printf("file_name: %s, file_type: %d, file_size: %d\n", dentry.file_name, dentry.file_type, lengthInode);
        }
    }
    else if (mode == 2) // if we press ctrl-2 we call our function to do this
    {
        /* Read data test */
        dentry_t dentry;
        int8_t* name = "frame0.txt";
        read_dentry_by_name((uint8_t*)name, &dentry);

        //int32_t ret = read_dentry_by_index(1, &dentry);

        printf("Filename: %s\n", dentry.file_name);
        printf("Filetype: %u\n", dentry.file_type);
        printf("Inode number: %u\n\n", dentry.inode);

        uint8_t buf[10000]; // make the buffer, and then clear it!
        for(i = 0; i < 10000; i++)
        {
            buf[i] = 0;
        }

        read_data((uint32_t*)(inode0 + (dentry.inode * BLOCK_SIZE / 4)), 0, buf, 10000);
        int32_t charsPrinted = term_write(1, buf, 10000);

        printf("Printed %u characters", charsPrinted);
        /* End of read data test */

    }
    else if (mode == 3) // if we press ctrl-3, we call our function with this argument
    {
        dentry_t dentry;    // a struct thing to pass in
        if (fileCount > b_block.num_entries)    // this is to loop back once we've hit the end of the files
        {
            fileCount = 0;
        }
        read_dentry_by_index(fileCount, &dentry);      // now fill up our struct
        printf("Filename: %s\n", dentry.file_name);
        printf("Filetype: %u\n", dentry.file_type);
        printf("Inode number: %u\n\n", dentry.inode);

        if(dentry.file_type == 2)
        {
            uint8_t buf[10000];     // make the buffer, and then clear it!
            for(i = 0; i < 10000; i++)
            {
                buf[i] = 0;
            }
            read_data((uint32_t*)(inode0 + (dentry.inode * BLOCK_SIZE / 4)), 0, buf, 10000);
            term_write(1, buf, 10000);
        }

        fileCount ++;   // just keep incrementing this!!
    }

    return 0;
}

extern int32_t directory_valid()
{
  return boot_block_initialized;
}

/*
use terminal write everywhere
fix when we get an undefined character
fix that page fault bug
write close funcions!
*/

int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry)
{
  /* Iterate through all the directory entries until one comes up with
    a name identical to the argument name. If we've reached the bottom
    with no luck, get the fuuuuck outta hereeee */

    int i;
    uint8_t* curr_entry = b_block.first_dir_entry;

    for(i = 0; i < b_block.num_entries; i++)
    {
      /* GOTCHA YA 'LIL MUTHAFUCKA, NOW BURN 'IM */
      if(!strncmp((int8_t*)fname, (int8_t*)curr_entry, 32))
      {
        // Question: why is strncmp take signed pointer args?
        // shouldn't they be unsigned because that's how we do ASCII?
        strncpy((int8_t*)dentry->file_name, (int8_t*)curr_entry, 32);
        dentry->file_name[32] = '\0';
        dentry->file_type = *(curr_entry + 32);
        dentry->inode = *(curr_entry + 36); // test these assignments in gdb
        return 0;
      }
      curr_entry += BOOT_BLOCK_FILE_NAME_OFFSET;
    }

    /* We haven't found jack shit. Tell that to the caller. */
    return -1;
}

int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry)
{
  /* Iterate through all directori-AHAHAHAHAHA WRONG. We got an
  index bitch. We don't need to iterate through SHIT.
  O(1) time functions FTW WOOOOOOO */

  /* But first check whether the index is legit */
  if(index < 0 || index >= b_block.num_entries)
    return -1;

  uint8_t* entry = b_block.first_dir_entry + (BOOT_BLOCK_FILE_NAME_OFFSET * index);

  strncpy((int8_t*)dentry->file_name, (int8_t*)entry, 32);
  dentry->file_name[32] = '\0';
  dentry->file_type = *(entry + 32);
  dentry->inode = *(entry + 36);
  return 0;
}
