
-l V:\f28335\common\v110\DSP2833x_headers\cmd\DSP2833x_Headers_nonBIOS.cmd
-l rts2800_fpu32.lib


MEMORY
{
PAGE 0:

	RAMM0		: origin = 0x000000, length = 0x000400
	PRAML0		: origin = 0x3F8002, length = 0x00FFE

	OTP         : origin = 0x3D7800, length = 0x000800     /* on-chip OTP */
	FLASH       : origin = 0x3E8000, length = 0x00FF80     /* on-chip FLASH */
	CSM_RSVD    : origin = 0x3F7F80, length = 0x000076     /* Part of FLASHA.  Program with all 0x0000 when CSM is in use. */
	BEGIN       : origin = 0x3F7FF6, length = 0x000002     /* Part of FLASHA.  Used for "boot to Flash" bootloader mode. */
	CSM_PWL     : origin = 0x3F7FF8, length = 0x000008     /* Part of FLASHA.  CSM password locations in FLASHA */
	
	ROM         : origin = 0x3FF000, length = 0x000FC0     /* Boot ROM available if MP/MCn=0 */
	RESET       : origin = 0x3FFFC0, length = 0x000002     /* part of boot ROM (MP/MCn=0) or XINTF zone 7 (MP/MCn=1) */
	VECTORS     : origin = 0x3FFFC2, length = 0x00003E     /* part of boot ROM (MP/MCn=0) or XINTF zone 7 (MP/MCn=1) */
	
PAGE 1:

	RAMM1		: origin = 0x000400, length = 0x000400
	DRAML1H0		: origin = 0x3f9000, length = 0x003000

}
 
 
SECTIONS
{
	codestart		: > BEGIN,		PAGE = 0
	ramfuncs        : LOAD = FLASH, 
                         RUN = PRAML0, 
                         LOAD_START(_ramfuncs_loadstart),
                         LOAD_END(_ramfuncs_loadend),
                         RUN_START(_ramfuncs_runstart),
                         PAGE = 0
/*	initfuncs		: > FLASH,		PAGE = 0 */
	.text			: > FLASH,		PAGE = 0
	.cinit			: > FLASH,		PAGE = 0
	.pinit			: > FLASH,		PAGE = 0
	.switch			: > FLASH,		PAGE = 0
	.reset			: > RESET,		PAGE = 0, TYPE = DSECT

/*	libfuncs		: {
		rts2800_fpu32.lib(.text)
	} > FLASH, PAGE = 0
*/	 		
	csmpasswds          : > CSM_PWL     PAGE = 0
   	csm_rsvd            : > CSM_RSVD    PAGE = 0
   	
	.stack			: > RAMM1,		PAGE = 1
	.bss			: > DRAML1H0,		PAGE = 1
	.ebss			: > DRAML1H0,		PAGE = 1
	.econst			: > FLASH,		PAGE = 0
	.sysmem			: > DRAML1H0,		PAGE = 1
	.esysmem		: > DRAML1H0,		PAGE = 1

/*	fastdata		: > DRAML1H0,		PAGE = 1*/

}
