#include "file.h"

/* The design of the op_pointers_t structs for each file that can potentially
    be read calls for static struct entries in each. This struct gets filled out
    in file_open(). Then we can have a getter function that simply returns this
     struct */

int32_t file_load(const uint8_t* filename, dentry_t* file)
{
    if(!directory_valid())
        return -1;

    return read_dentry_by_name(filename, file);
}

int32_t file_open(const uint8_t* filename)
{
    // file_op_pointers.f_open = &file_open;
    // file_op_pointers.f_read = &file_read;
    // file_op_pointers.f_write = &file_write;
    // file_op_pointers.f_close = &file_close;

	return 0;
}

/* Returns the number of bytes read */
int32_t file_read(int32_t fd, void* buf, int32_t nbytes)
{
	file_array_t* file_info = get_cur_file(get_cur_process(), fd);

    int32_t ret = read_data(file_info->inode, file_info->f_pos, (uint8_t*)buf, nbytes);
    if(ret < 0)
        return 0;

    return ret;
}

int32_t file_write(int32_t fd, const void* buf, int32_t nbytes)
{
  return -1;
}

int32_t file_close(int32_t fd)
{
  return 0;
}
