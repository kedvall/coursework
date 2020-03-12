/*
 * simulink5ms_plotAndGains.c
 *
 * Classroom License -- for classroom instructional use only.  Not for
 * government, commercial, academic research, or other organizational use.
 *
 * Code generation for model "simulink5ms_plotAndGains".
 *
 * Model version              : 1.116
 * Simulink Coder version : 8.12 (R2017a) 16-Feb-2017
 * C source code generated on : Mon Apr 09 16:53:07 2018
 *
 * Target selection: sldrt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "simulink5ms_plotAndGains.h"
#include "simulink5ms_plotAndGains_private.h"
#include "simulink5ms_plotAndGains_dt.h"

/* options for Simulink Desktop Real-Time board 0 */
static double SLDRTBoardOptions0[] = {
  115200.0,
  8.0,
  0.0,
  0.0,
  0.0,
  0.0,
  0.0,
  1.0,
};

/* list of Simulink Desktop Real-Time timers */
const int SLDRTTimerCount = 1;
const double SLDRTTimers[2] = {
  0.005, 0.0,
};

/* list of Simulink Desktop Real-Time boards */
const int SLDRTBoardCount = 1;
SLDRTBOARD SLDRTBoards[1] = {
  { "Standard_Devices/Serial_Port", 1U, 8, SLDRTBoardOptions0 },
};

/* Block signals (auto storage) */
B_simulink5ms_plotAndGains_T simulink5ms_plotAndGains_B;

/* Block states (auto storage) */
DW_simulink5ms_plotAndGains_T simulink5ms_plotAndGains_DW;

/* Real-time model */
RT_MODEL_simulink5ms_plotAndGains_T simulink5ms_plotAndGains_M_;
RT_MODEL_simulink5ms_plotAndGains_T *const simulink5ms_plotAndGains_M =
  &simulink5ms_plotAndGains_M_;

/* Model output function */
void simulink5ms_plotAndGains_output(void)
{
  real_T tmp;

  /* S-Function (sldrtpi): '<S1>/Packet Input1' */
  /* S-Function Block: <S1>/Packet Input1 */
  {
    uint8_T indata[16U];
    int status = RTBIO_DriverIO(0, STREAMINPUT, IOREAD, 16U,
      &simulink5ms_plotAndGains_P.PacketInput1_PacketID, (double*) indata, NULL);
    simulink5ms_plotAndGains_B.PacketInput1_o2 = 0;/* Missed Ticks value is always zero */
    if (status & 0x1) {
      RTWin_ANYTYPEPTR indp;
      indp.p_uint8_T = indata;
      simulink5ms_plotAndGains_B.PacketInput1_o1[0] = *indp.p_int32_T++;
      simulink5ms_plotAndGains_B.PacketInput1_o1[1] = *indp.p_int32_T++;
      simulink5ms_plotAndGains_B.PacketInput1_o1[2] = *indp.p_int32_T++;
      simulink5ms_plotAndGains_B.PacketInput1_o1[3] = *indp.p_int32_T++;
    }
  }

  /* Gain: '<Root>/Gain1' incorporates:
   *  DataTypeConversion: '<S1>/Data Type Conversion'
   */
  simulink5ms_plotAndGains_B.Gain1 = simulink5ms_plotAndGains_P.Gain1_Gain *
    (real_T)simulink5ms_plotAndGains_B.PacketInput1_o1[0];

  /* Gain: '<Root>/Gain2' incorporates:
   *  DataTypeConversion: '<S1>/Data Type Conversion2'
   */
  simulink5ms_plotAndGains_B.Gain2 = simulink5ms_plotAndGains_P.Gain2_Gain *
    (real_T)simulink5ms_plotAndGains_B.PacketInput1_o1[1];

  /* Gain: '<Root>/Gain3' incorporates:
   *  DataTypeConversion: '<S1>/Data Type Conversion6'
   */
  simulink5ms_plotAndGains_B.Gain3 = simulink5ms_plotAndGains_P.Gain3_Gain *
    (real_T)simulink5ms_plotAndGains_B.PacketInput1_o1[2];

  /* Gain: '<Root>/Gain4' incorporates:
   *  DataTypeConversion: '<S1>/Data Type Conversion5'
   */
  simulink5ms_plotAndGains_B.Gain4 = simulink5ms_plotAndGains_P.Gain4_Gain *
    (real_T)simulink5ms_plotAndGains_B.PacketInput1_o1[3];

  /* S-Function (sldrtpo): '<S2>/Packet Output' incorporates:
   *  Constant: '<S2>/Constant Must be This Value 0x7fff'
   */
  /* S-Function Block: <S2>/Packet Output */

  /* no code required */

  /* Gain: '<S2>/Gain1' incorporates:
   *  Constant: '<Root>/Value_16bit2'
   *  Gain: '<Root>/Gain6'
   */
  tmp = floor(simulink5ms_plotAndGains_P.Gain6_Gain *
              simulink5ms_plotAndGains_P.Value_16bit2_Value *
              simulink5ms_plotAndGains_P.Gain1_Gain_g);
  if (rtIsNaN(tmp) || rtIsInf(tmp)) {
    tmp = 0.0;
  } else {
    tmp = fmod(tmp, 65536.0);
  }

  simulink5ms_plotAndGains_B.Gain1_i = (int16_T)(tmp < 0.0 ? (int32_T)(int16_T)
    -(int16_T)(uint16_T)-tmp : (int32_T)(int16_T)(uint16_T)tmp);

  /* End of Gain: '<S2>/Gain1' */

  /* Gain: '<S2>/Gain2' incorporates:
   *  Constant: '<Root>/Value_16bit3'
   *  Gain: '<Root>/Gain7'
   */
  tmp = floor(simulink5ms_plotAndGains_P.Gain7_Gain *
              simulink5ms_plotAndGains_P.Value_16bit3_Value *
              simulink5ms_plotAndGains_P.Gain2_Gain_h);
  if (rtIsNaN(tmp) || rtIsInf(tmp)) {
    tmp = 0.0;
  } else {
    tmp = fmod(tmp, 65536.0);
  }

  simulink5ms_plotAndGains_B.Gain2_d = (int16_T)(tmp < 0.0 ? (int32_T)(int16_T)
    -(int16_T)(uint16_T)-tmp : (int32_T)(int16_T)(uint16_T)tmp);

  /* End of Gain: '<S2>/Gain2' */

  /* Gain: '<S2>/Gain3' incorporates:
   *  Constant: '<Root>/Value_16bit1'
   *  Gain: '<Root>/Gain5'
   */
  tmp = floor(simulink5ms_plotAndGains_P.Gain5_Gain *
              simulink5ms_plotAndGains_P.Value_16bit1_Value *
              simulink5ms_plotAndGains_P.Gain3_Gain_d);
  if (rtIsNaN(tmp) || rtIsInf(tmp)) {
    tmp = 0.0;
  } else {
    tmp = fmod(tmp, 65536.0);
  }

  simulink5ms_plotAndGains_B.Gain3_j = (int16_T)(tmp < 0.0 ? (int32_T)(int16_T)
    -(int16_T)(uint16_T)-tmp : (int32_T)(int16_T)(uint16_T)tmp);

  /* End of Gain: '<S2>/Gain3' */

  /* Gain: '<S2>/Gain4' incorporates:
   *  Constant: '<Root>/Value_16bit4'
   *  Gain: '<Root>/Gain8'
   */
  tmp = floor(simulink5ms_plotAndGains_P.Gain8_Gain *
              simulink5ms_plotAndGains_P.Value_16bit4_Value *
              simulink5ms_plotAndGains_P.Gain4_Gain_h);
  if (rtIsNaN(tmp) || rtIsInf(tmp)) {
    tmp = 0.0;
  } else {
    tmp = fmod(tmp, 65536.0);
  }

  simulink5ms_plotAndGains_B.Gain4_h = (int16_T)(tmp < 0.0 ? (int32_T)(int16_T)
    -(int16_T)(uint16_T)-tmp : (int32_T)(int16_T)(uint16_T)tmp);

  /* End of Gain: '<S2>/Gain4' */

  /* Gain: '<S2>/Gain5' incorporates:
   *  Constant: '<Root>/Value_16bit5 '
   *  Gain: '<Root>/Gain9'
   */
  tmp = floor(simulink5ms_plotAndGains_P.Gain9_Gain *
              simulink5ms_plotAndGains_P.Value_16bit5_Value *
              simulink5ms_plotAndGains_P.Gain5_Gain_d);
  if (rtIsNaN(tmp) || rtIsInf(tmp)) {
    tmp = 0.0;
  } else {
    tmp = fmod(tmp, 65536.0);
  }

  simulink5ms_plotAndGains_B.Gain5 = (int16_T)(tmp < 0.0 ? (int32_T)(int16_T)
    -(int16_T)(uint16_T)-tmp : (int32_T)(int16_T)(uint16_T)tmp);

  /* End of Gain: '<S2>/Gain5' */

  /* Gain: '<S2>/Gain6' incorporates:
   *  Constant: '<Root>/Value_16bit6'
   *  Gain: '<Root>/Gain10'
   */
  tmp = floor(simulink5ms_plotAndGains_P.Gain10_Gain *
              simulink5ms_plotAndGains_P.Value_16bit6_Value *
              simulink5ms_plotAndGains_P.Gain6_Gain_j);
  if (rtIsNaN(tmp) || rtIsInf(tmp)) {
    tmp = 0.0;
  } else {
    tmp = fmod(tmp, 65536.0);
  }

  simulink5ms_plotAndGains_B.Gain6 = (int16_T)(tmp < 0.0 ? (int32_T)(int16_T)
    -(int16_T)(uint16_T)-tmp : (int32_T)(int16_T)(uint16_T)tmp);

  /* End of Gain: '<S2>/Gain6' */

  /* Gain: '<S2>/Gain7' incorporates:
   *  Constant: '<Root>/Value_16bit7'
   *  Gain: '<Root>/Gain11'
   */
  tmp = floor(simulink5ms_plotAndGains_P.Gain11_Gain *
              simulink5ms_plotAndGains_P.Value_16bit7_Value *
              simulink5ms_plotAndGains_P.Gain7_Gain_e);
  if (rtIsNaN(tmp) || rtIsInf(tmp)) {
    tmp = 0.0;
  } else {
    tmp = fmod(tmp, 65536.0);
  }

  simulink5ms_plotAndGains_B.Gain7 = (int16_T)(tmp < 0.0 ? (int32_T)(int16_T)
    -(int16_T)(uint16_T)-tmp : (int32_T)(int16_T)(uint16_T)tmp);

  /* End of Gain: '<S2>/Gain7' */
}

/* Model update function */
void simulink5ms_plotAndGains_update(void)
{
  /* Update for S-Function (sldrtpo): '<S2>/Packet Output' incorporates:
   *  Constant: '<S2>/Constant Must be This Value 0x7fff'
   */

  /* S-Function Block: <S2>/Packet Output */
  {
    uint8_T outdata[16U];
    RTWin_ANYTYPEPTR outdp;
    outdp.p_uint8_T = outdata;

    {
      int16_T pktout =
        simulink5ms_plotAndGains_P.ConstantMustbeThisValue0x7fff_Value;
      *outdp.p_int16_T++ = pktout;
    }

    {
      int16_T pktout = simulink5ms_plotAndGains_B.Gain3_j;
      *outdp.p_int16_T++ = pktout;
    }

    {
      int16_T pktout = simulink5ms_plotAndGains_B.Gain1_i;
      *outdp.p_int16_T++ = pktout;
    }

    {
      int16_T pktout = simulink5ms_plotAndGains_B.Gain2_d;
      *outdp.p_int16_T++ = pktout;
    }

    {
      int16_T pktout = simulink5ms_plotAndGains_B.Gain4_h;
      *outdp.p_int16_T++ = pktout;
    }

    {
      int16_T pktout = simulink5ms_plotAndGains_B.Gain5;
      *outdp.p_int16_T++ = pktout;
    }

    {
      int16_T pktout = simulink5ms_plotAndGains_B.Gain6;
      *outdp.p_int16_T++ = pktout;
    }

    {
      int16_T pktout = simulink5ms_plotAndGains_B.Gain7;
      *outdp.p_int16_T++ = pktout;
    }

    RTBIO_DriverIO(0, STREAMOUTPUT, IOWRITE, 16U,
                   &simulink5ms_plotAndGains_P.PacketOutput_PacketID, (double*)
                   outdata, NULL);
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++simulink5ms_plotAndGains_M->Timing.clockTick0)) {
    ++simulink5ms_plotAndGains_M->Timing.clockTickH0;
  }

  simulink5ms_plotAndGains_M->Timing.t[0] =
    simulink5ms_plotAndGains_M->Timing.clockTick0 *
    simulink5ms_plotAndGains_M->Timing.stepSize0 +
    simulink5ms_plotAndGains_M->Timing.clockTickH0 *
    simulink5ms_plotAndGains_M->Timing.stepSize0 * 4294967296.0;
}

/* Model initialize function */
void simulink5ms_plotAndGains_initialize(void)
{
  /* Start for S-Function (sldrtpo): '<S2>/Packet Output' incorporates:
   *  Constant: '<S2>/Constant Must be This Value 0x7fff'
   */

  /* S-Function Block: <S2>/Packet Output */
  /* no initial value should be set */
}

/* Model terminate function */
void simulink5ms_plotAndGains_terminate(void)
{
  /* Terminate for S-Function (sldrtpo): '<S2>/Packet Output' incorporates:
   *  Constant: '<S2>/Constant Must be This Value 0x7fff'
   */

  /* S-Function Block: <S2>/Packet Output */
  /* no initial value should be set */
}

/*========================================================================*
 * Start of Classic call interface                                        *
 *========================================================================*/
void MdlOutputs(int_T tid)
{
  simulink5ms_plotAndGains_output();
  UNUSED_PARAMETER(tid);
}

void MdlUpdate(int_T tid)
{
  simulink5ms_plotAndGains_update();
  UNUSED_PARAMETER(tid);
}

void MdlInitializeSizes(void)
{
}

void MdlInitializeSampleTimes(void)
{
}

void MdlInitialize(void)
{
}

void MdlStart(void)
{
  simulink5ms_plotAndGains_initialize();
}

void MdlTerminate(void)
{
  simulink5ms_plotAndGains_terminate();
}

/* Registration function */
RT_MODEL_simulink5ms_plotAndGains_T *simulink5ms_plotAndGains(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize real-time model */
  (void) memset((void *)simulink5ms_plotAndGains_M, 0,
                sizeof(RT_MODEL_simulink5ms_plotAndGains_T));

  /* Initialize timing info */
  {
    int_T *mdlTsMap = simulink5ms_plotAndGains_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    simulink5ms_plotAndGains_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    simulink5ms_plotAndGains_M->Timing.sampleTimes =
      (&simulink5ms_plotAndGains_M->Timing.sampleTimesArray[0]);
    simulink5ms_plotAndGains_M->Timing.offsetTimes =
      (&simulink5ms_plotAndGains_M->Timing.offsetTimesArray[0]);

    /* task periods */
    simulink5ms_plotAndGains_M->Timing.sampleTimes[0] = (0.005);

    /* task offsets */
    simulink5ms_plotAndGains_M->Timing.offsetTimes[0] = (0.0);
  }

  rtmSetTPtr(simulink5ms_plotAndGains_M,
             &simulink5ms_plotAndGains_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = simulink5ms_plotAndGains_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    simulink5ms_plotAndGains_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(simulink5ms_plotAndGains_M, 1000.0);
  simulink5ms_plotAndGains_M->Timing.stepSize0 = 0.005;

  /* External mode info */
  simulink5ms_plotAndGains_M->Sizes.checksums[0] = (2584985140U);
  simulink5ms_plotAndGains_M->Sizes.checksums[1] = (2628880283U);
  simulink5ms_plotAndGains_M->Sizes.checksums[2] = (596269646U);
  simulink5ms_plotAndGains_M->Sizes.checksums[3] = (429537900U);

  {
    static const sysRanDType rtAlwaysEnabled = SUBSYS_RAN_BC_ENABLE;
    static RTWExtModeInfo rt_ExtModeInfo;
    static const sysRanDType *systemRan[1];
    simulink5ms_plotAndGains_M->extModeInfo = (&rt_ExtModeInfo);
    rteiSetSubSystemActiveVectorAddresses(&rt_ExtModeInfo, systemRan);
    systemRan[0] = &rtAlwaysEnabled;
    rteiSetModelMappingInfoPtr(simulink5ms_plotAndGains_M->extModeInfo,
      &simulink5ms_plotAndGains_M->SpecialInfo.mappingInfo);
    rteiSetChecksumsPtr(simulink5ms_plotAndGains_M->extModeInfo,
                        simulink5ms_plotAndGains_M->Sizes.checksums);
    rteiSetTPtr(simulink5ms_plotAndGains_M->extModeInfo, rtmGetTPtr
                (simulink5ms_plotAndGains_M));
  }

  simulink5ms_plotAndGains_M->solverInfoPtr =
    (&simulink5ms_plotAndGains_M->solverInfo);
  simulink5ms_plotAndGains_M->Timing.stepSize = (0.005);
  rtsiSetFixedStepSize(&simulink5ms_plotAndGains_M->solverInfo, 0.005);
  rtsiSetSolverMode(&simulink5ms_plotAndGains_M->solverInfo,
                    SOLVER_MODE_SINGLETASKING);

  /* block I/O */
  simulink5ms_plotAndGains_M->blockIO = ((void *) &simulink5ms_plotAndGains_B);
  (void) memset(((void *) &simulink5ms_plotAndGains_B), 0,
                sizeof(B_simulink5ms_plotAndGains_T));

  /* parameters */
  simulink5ms_plotAndGains_M->defaultParam = ((real_T *)
    &simulink5ms_plotAndGains_P);

  /* states (dwork) */
  simulink5ms_plotAndGains_M->dwork = ((void *) &simulink5ms_plotAndGains_DW);
  (void) memset((void *)&simulink5ms_plotAndGains_DW, 0,
                sizeof(DW_simulink5ms_plotAndGains_T));

  /* data type transition information */
  {
    static DataTypeTransInfo dtInfo;
    (void) memset((char_T *) &dtInfo, 0,
                  sizeof(dtInfo));
    simulink5ms_plotAndGains_M->SpecialInfo.mappingInfo = (&dtInfo);
    dtInfo.numDataTypes = 14;
    dtInfo.dataTypeSizes = &rtDataTypeSizes[0];
    dtInfo.dataTypeNames = &rtDataTypeNames[0];

    /* Block I/O transition table */
    dtInfo.BTransTable = &rtBTransTable;

    /* Parameters transition table */
    dtInfo.PTransTable = &rtPTransTable;
  }

  /* Initialize Sizes */
  simulink5ms_plotAndGains_M->Sizes.numContStates = (0);/* Number of continuous states */
  simulink5ms_plotAndGains_M->Sizes.numY = (0);/* Number of model outputs */
  simulink5ms_plotAndGains_M->Sizes.numU = (0);/* Number of model inputs */
  simulink5ms_plotAndGains_M->Sizes.sysDirFeedThru = (0);/* The model is not direct feedthrough */
  simulink5ms_plotAndGains_M->Sizes.numSampTimes = (1);/* Number of sample times */
  simulink5ms_plotAndGains_M->Sizes.numBlocks = (37);/* Number of blocks */
  simulink5ms_plotAndGains_M->Sizes.numBlockIO = (13);/* Number of block outputs */
  simulink5ms_plotAndGains_M->Sizes.numBlockPrms = (32);/* Sum of parameter "widths" */
  return simulink5ms_plotAndGains_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/
