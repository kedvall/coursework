
-l V:\f28335\common\v110\DSP2833x_headers\cmd\DSP2833x_Headers_nonBIOS.cmd
-l rts2800_fpu32.lib

_ramfuncs_loadstart = 0;
_ramfuncs_loadend = 0;
_ramfuncs_runstart = 0;

_extfuncs_loadstart = 0;
_extfuncs_loadend = 0;
_extfuncs_runstart = 0;

MEMORY
{
PAGE 0:

	BEGIN      : origin = 0x000000, length = 0x000002             
    RAMM0      : origin = 0x000002, length = 0x0003FE
    PRAMH0     : origin = 0x3FA000, length = 0x002000 
    RESET      : origin = 0x3FFFC0, length = 0x000002
    BOOTROM    : origin = 0x3FF000, length = 0x000FC0

PAGE 1:

	RAMM1		: origin = 0x000400, length = 0x000400
	DRAML0		: origin = 0x3F8000, length = 0x002000

}
 
 
SECTIONS
{
	codestart		: > BEGIN,		PAGE = 0
	ramfuncs		: > PRAMH0,		PAGE = 0
/*	initfuncs		: > PRAMH0,		PAGE = 0 */
	.text			: > PRAMH0,		PAGE = 0
	.cinit			: > PRAMH0,		PAGE = 0
	.pinit			: > PRAMH0,		PAGE = 0
	.switch			: > RAMM0,		PAGE = 0
	.reset			: > RESET,		PAGE = 0, TYPE = DSECT

/*	libfuncs		: {
		rts2800_fpu32.lib(.text)
	} > PRAMH0, PAGE = 0
*/
	.stack			: > RAMM1,		PAGE = 1
	.bss			: > DRAML0,		PAGE = 1
	.ebss			: > DRAML0,		PAGE = 1
	.econst			: > DRAML0,		PAGE = 1
	.sysmem			: > DRAML0,		PAGE = 1
	.esysmem		: > DRAML0,		PAGE = 1

/*	fastdata		: > DRAML0,		PAGE = 1 */

}
