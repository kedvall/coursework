/* To run program from flash */

/*-l %PROJECT_ROOT%\..\..\F28335_FLASH_BIOS.cmd */
-l ..\..\F28335_FLASH_BIOS.cmd

MEMORY
{
PAGE 0 :
   ADC_CAL    : origin = 0x380080, length = 0x000009
}
 
 
SECTIONS
{
   
   /* Allocate ADC_cal function (pre-programmed by factory into TI reserved memory) */
   .adc_cal     : load = ADC_CAL,   PAGE = 0, TYPE = NOLOAD
   .dmaram			: > MEM_forDMA,		PAGE = 1
   .my_vars: {} > L1SARAM
   .my_arrs: {} > L1SARAM
     
}
