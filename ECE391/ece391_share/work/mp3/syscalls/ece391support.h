#if !defined(ECE391SUPPORT_H)
#define ECE391SUPPORT_H

// SCREEN STUFF
#define SCREEN_W 80
#define SCREEN_H 25
#define NULL 0

// Map info
#define MAP_HEIGHT 25 //Must be multiple of 25
#define MAP_WIDTH 160 //Must be multiple of 80

extern uint32_t ece391_strlen(const uint8_t* s);
extern void ece391_strcpy(uint8_t* dst, const uint8_t* src);
extern void ece391_fdputs(int32_t fd, const uint8_t* s);
extern int32_t ece391_strcmp(const uint8_t* s1, const uint8_t* s2);
extern int32_t ece391_strncmp(const uint8_t* s1, const uint8_t* s2, uint32_t n);
extern uint8_t *ece391_itoa(uint32_t value, uint8_t* buf, int32_t radix);
extern uint8_t *ece391_strrev(uint8_t* s);

// video lib function declarations
extern int32_t colored_puts(int8_t* s, int x_coord, int y_coord, uint8_t backg_color, uint8_t text_color);
extern void colored_putc(int8_t c, int x_coord, int y_coord, uint8_t backg_color, uint8_t text_color);
extern int8_t save_c(int x_coord, int y_coord);
extern int32_t ece391_vidcpy(void* dest, const void* src, uint32_t start);
uint8_t* mp1_set_video_mode (void);
void ece391_memset(void* memory, char c, int n);

// Vars
uint8_t *vmem_base_addr;

#endif /* ECE391SUPPORT_H */
