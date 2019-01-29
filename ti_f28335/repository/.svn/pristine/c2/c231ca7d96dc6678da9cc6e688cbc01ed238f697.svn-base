#ifndef __IO_H__
#define __IO_H__

/* bit set/clear macros */
#define clrb(ADDR,BITS)		((ADDR) &= ~(BITS))
#define setb(ADDR,BITS)		((ADDR) |= (BITS))

/* memory read/write macros */
#define inp(a)				(*((volatile Uint16 *)(a)))
#define outp(a,b)			(*((volatile Uint16 *)(a)) = (b))

#endif /* __IO_H__ */

