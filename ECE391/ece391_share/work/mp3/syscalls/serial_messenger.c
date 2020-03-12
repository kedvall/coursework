
int8_t read_buffer[256];
int8_t write_buffer[256];

write_buffer[0] = 254;
ece391_write(7, write_buffer, 1);
