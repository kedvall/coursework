/*
 * simulink5ms_plotAndGains_dt.h
 *
 * Classroom License -- for classroom instructional use only.  Not for
 * government, commercial, academic research, or other organizational use.
 *
 * Code generation for model "simulink5ms_plotAndGains".
 *
 * Model version              : 1.109
 * Simulink Coder version : 8.12 (R2017a) 16-Feb-2017
 * C source code generated on : Mon Feb 26 15:33:20 2018
 *
 * Target selection: sldrt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "ext_types.h"

/* data type size table */
static uint_T rtDataTypeSizes[] = {
  sizeof(real_T),
  sizeof(real32_T),
  sizeof(int8_T),
  sizeof(uint8_T),
  sizeof(int16_T),
  sizeof(uint16_T),
  sizeof(int32_T),
  sizeof(uint32_T),
  sizeof(boolean_T),
  sizeof(fcn_call_T),
  sizeof(int_T),
  sizeof(pointer_T),
  sizeof(action_T),
  2*sizeof(uint32_T)
};

/* data type name table */
static const char_T * rtDataTypeNames[] = {
  "real_T",
  "real32_T",
  "int8_T",
  "uint8_T",
  "int16_T",
  "uint16_T",
  "int32_T",
  "uint32_T",
  "boolean_T",
  "fcn_call_T",
  "int_T",
  "pointer_T",
  "action_T",
  "timer_uint32_pair_T"
};

/* data type transitions for block I/O structure */
static DataTypeTransition rtBTransitions[] = {
  { (char_T *)(&simulink5ms_plotAndGains_B.Gain1), 0, 0, 4 },

  { (char_T *)(&simulink5ms_plotAndGains_B.PacketInput1_o1[0]), 6, 0, 5 },

  { (char_T *)(&simulink5ms_plotAndGains_B.Gain1_i), 4, 0, 7 }
  ,

  { (char_T *)(&simulink5ms_plotAndGains_DW.PacketInput1_PWORK), 11, 0, 8 }
};

/* data type transition table for block I/O structure */
static DataTypeTransitionTable rtBTransTable = {
  4U,
  rtBTransitions
};

/* data type transitions for Parameters structure */
static DataTypeTransition rtPTransitions[] = {
  { (char_T *)(&simulink5ms_plotAndGains_P.PacketInput1_MaxMissedTicks), 0, 0, 4
  },

  { (char_T *)(&simulink5ms_plotAndGains_P.PacketInput1_PacketID), 6, 0, 2 },

  { (char_T *)(&simulink5ms_plotAndGains_P.Gain1_Gain), 0, 0, 25 },

  { (char_T *)(&simulink5ms_plotAndGains_P.ConstantMustbeThisValue0x7fff_Value),
    4, 0, 1 }
};

/* data type transition table for Parameters structure */
static DataTypeTransitionTable rtPTransTable = {
  4U,
  rtPTransitions
};

/* [EOF] simulink5ms_plotAndGains_dt.h */
