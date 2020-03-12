
/*-l %PROJECT_ROOT%\..\..\v110\DSP2833x_headers\cmd\DSP2833x_Headers_BIOS.cmd */
-l ..\..\v110\DSP2833x_headers\cmd\DSP2833x_Headers_BIOS.cmd
/**********************************************************************
* File: f28335_BIOS_flash.cmd -- Linker command file for DSP/BIOS code
* with DSP in Boot to Flash bootmode.
*
* History: 02/11/04 - original (D. Alter)
**********************************************************************/


/**************************************************************/
/* Link all user defined sections                             */
/**************************************************************/
SECTIONS
{

/*** Code Security Password Locations ***/
   csmpasswds         : > PASSWORDS,         PAGE = 0        /* Used by file passwords.asm */
   csm_rsvd          : > CSM_RSVD,          PAGE = 0        /* Used by file passwords.asm */

/*** User Defined Sections ***/
   codestart         : > BEGIN_FLASH,       PAGE = 0        /* Used by file CodeStartBranch.asm */

                         /* Section ramfuncs used by InitFlash() in SysCtrl.c */
   ramfuncs    :   LOAD = FLASH_BCD,  PAGE = 0        /* Can be Flash */ 
                         RUN = L0SARAM,     PAGE = 1        /* Must be CSM secured RAM */
                         LOAD_START(_ramfuncs_loadstart),
                         LOAD_END(_ramfuncs_loadend),
                         RUN_START(_ramfuncs_runstart)

	FPUmathTables : > FPUTABLES, PAGE = 0, TYPE = NOLOAD
}

/******************* end of file ************************/













