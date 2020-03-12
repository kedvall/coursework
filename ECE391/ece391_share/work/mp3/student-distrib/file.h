#ifndef _FILE_H
#define _FILE_H

#include "directory.h"
#include "process_interface.h"

extern int32_t file_load(const uint8_t* filename, dentry_t* file);
extern int32_t file_open(const uint8_t* filename);
extern int32_t file_read(int32_t fd, void* buf, int32_t nbytes);
extern int32_t file_write(int32_t fd, const void* buf, int32_t nbytes);
extern int32_t file_close(int32_t fd);
//extern void reset_file_pos(int32_t pos);


#endif

/* In the very near future, it may be useful to have a FILE struct object that
  keeps track of file position, d_entry info, DOB, SSN, favorite flavor of
  ice cream etc. and then have one allocated every time file_open is called */
