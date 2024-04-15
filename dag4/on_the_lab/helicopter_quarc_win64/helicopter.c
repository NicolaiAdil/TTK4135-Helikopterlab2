/*
 * helicopter.c
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "helicopter".
 *
 * Model version              : 11.0
 * Simulink Coder version : 9.4 (R2020b) 29-Jul-2020
 * C source code generated on : Mon Apr 15 12:19:58 2024
 *
 * Target selection: quarc_win64.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "helicopter.h"
#include "helicopter_private.h"
#include "helicopter_dt.h"

/* Block signals (default storage) */
B_helicopter_T helicopter_B;

/* Continuous states */
X_helicopter_T helicopter_X;

/* Block states (default storage) */
DW_helicopter_T helicopter_DW;

/* Real-time model */
static RT_MODEL_helicopter_T helicopter_M_;
RT_MODEL_helicopter_T *const helicopter_M = &helicopter_M_;

/*
 * This function updates continuous states using the ODE1 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE1_IntgData *id = (ODE1_IntgData *)rtsiGetSolverData(si);
  real_T *f0 = id->f[0];
  int_T i;
  int_T nXc = 4;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);
  rtsiSetdX(si, f0);
  helicopter_derivatives();
  rtsiSetT(si, tnew);
  for (i = 0; i < nXc; ++i) {
    x[i] += h * f0[i];
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model output function */
void helicopter_output(void)
{
  /* local block i/o variables */
  real_T rtb_HILReadEncoderTimebase_o1;
  real_T rtb_HILReadEncoderTimebase_o2;
  real_T rtb_HILReadEncoderTimebase_o3;
  real_T rtb_Gain1[6];
  real_T rtb_Gain1_0[6];
  real_T rtb_Sum_p[2];
  real_T Clock_tmp;
  real_T rtb_Frontgain;
  real_T *lastU;
  int32_T i;
  int32_T i_0;
  int8_T rtAction;
  if (rtmIsMajorTimeStep(helicopter_M)) {
    /* set solver stop time */
    if (!(helicopter_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&helicopter_M->solverInfo,
                            ((helicopter_M->Timing.clockTickH0 + 1) *
        helicopter_M->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&helicopter_M->solverInfo,
                            ((helicopter_M->Timing.clockTick0 + 1) *
        helicopter_M->Timing.stepSize0 + helicopter_M->Timing.clockTickH0 *
        helicopter_M->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(helicopter_M)) {
    helicopter_M->Timing.t[0] = rtsiGetT(&helicopter_M->solverInfo);
  }

  /* Reset subsysRan breadcrumbs */
  srClearBC(helicopter_DW.IfActionSubsystem_SubsysRanBC);
  if (rtmIsMajorTimeStep(helicopter_M)) {
    /* S-Function (hil_read_encoder_timebase_block): '<S4>/HIL Read Encoder Timebase' */

    /* S-Function Block: helicopter/Helicopter_interface/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
    {
      t_error result;
      result = hil_task_read_encoder(helicopter_DW.HILReadEncoderTimebase_Task,
        1, &helicopter_DW.HILReadEncoderTimebase_Buffer[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
      } else {
        rtb_HILReadEncoderTimebase_o1 =
          helicopter_DW.HILReadEncoderTimebase_Buffer[0];
        rtb_HILReadEncoderTimebase_o2 =
          helicopter_DW.HILReadEncoderTimebase_Buffer[1];
        rtb_HILReadEncoderTimebase_o3 =
          helicopter_DW.HILReadEncoderTimebase_Buffer[2];
      }
    }

    /* Gain: '<S4>/Elevation: Count to rad' incorporates:
     *  Gain: '<S4>/Elevation_gain'
     */
    helicopter_B.ElevationCounttorad = helicopter_P.elevation_gain *
      rtb_HILReadEncoderTimebase_o3 * helicopter_P.ElevationCounttorad_Gain;

    /* Gain: '<S9>/Gain' */
    helicopter_B.Gain = helicopter_P.Gain_Gain *
      helicopter_B.ElevationCounttorad;

    /* Sum: '<Root>/Sum' incorporates:
     *  Constant: '<Root>/elavation_offset [deg]'
     */
    helicopter_B.Sum = helicopter_B.Gain +
      helicopter_P.elavation_offsetdeg_Value;
  }

  /* Gain: '<S10>/Gain' incorporates:
   *  TransferFcn: '<S4>/Elevation: Transfer Fcn'
   */
  helicopter_B.Gain_d = (helicopter_P.ElevationTransferFcn_C *
    helicopter_X.ElevationTransferFcn_CSTATE +
    helicopter_P.ElevationTransferFcn_D * helicopter_B.ElevationCounttorad) *
    helicopter_P.Gain_Gain_n;
  if (rtmIsMajorTimeStep(helicopter_M)) {
  }

  /* FromWorkspace: '<Root>/pitch_ref' */
  {
    real_T *pDataValues = (real_T *) helicopter_DW.pitch_ref_PWORK.DataPtr;
    real_T *pTimeValues = (real_T *) helicopter_DW.pitch_ref_PWORK.TimePtr;
    int_T currTimeIndex = helicopter_DW.pitch_ref_IWORK.PrevIndex;
    real_T t = helicopter_M->Timing.t[0];

    /* Get index */
    if (t <= pTimeValues[0]) {
      currTimeIndex = 0;
    } else if (t >= pTimeValues[95]) {
      currTimeIndex = 94;
    } else {
      if (t < pTimeValues[currTimeIndex]) {
        while (t < pTimeValues[currTimeIndex]) {
          currTimeIndex--;
        }
      } else {
        while (t >= pTimeValues[currTimeIndex + 1]) {
          currTimeIndex++;
        }
      }
    }

    helicopter_DW.pitch_ref_IWORK.PrevIndex = currTimeIndex;

    /* Post output */
    {
      real_T t1 = pTimeValues[currTimeIndex];
      real_T t2 = pTimeValues[currTimeIndex + 1];
      if (t1 == t2) {
        if (t < t1) {
          {
            int_T elIdx;
            for (elIdx = 0; elIdx < 2; ++elIdx) {
              (&helicopter_B.pitch_ref[0])[elIdx] = pDataValues[currTimeIndex];
              pDataValues += 96;
            }
          }
        } else {
          {
            int_T elIdx;
            for (elIdx = 0; elIdx < 2; ++elIdx) {
              (&helicopter_B.pitch_ref[0])[elIdx] = pDataValues[currTimeIndex +
                1];
              pDataValues += 96;
            }
          }
        }
      } else {
        real_T f1 = (t2 - t) / (t2 - t1);
        real_T f2 = 1.0 - f1;
        real_T d1;
        real_T d2;
        int_T TimeIndex= currTimeIndex;

        {
          int_T elIdx;
          for (elIdx = 0; elIdx < 2; ++elIdx) {
            d1 = pDataValues[TimeIndex];
            d2 = pDataValues[TimeIndex + 1];
            (&helicopter_B.pitch_ref[0])[elIdx] = (real_T) rtInterpolate(d1, d2,
              f1, f2);
            pDataValues += 96;
          }
        }
      }
    }
  }

  if (rtmIsMajorTimeStep(helicopter_M)) {
    /* Gain: '<S4>/Travel: Count to rad' incorporates:
     *  Gain: '<S4>/Travel_gain'
     */
    helicopter_B.TravelCounttorad = helicopter_P.travel_gain *
      rtb_HILReadEncoderTimebase_o1 * helicopter_P.TravelCounttorad_Gain;

    /* Gain: '<S14>/Gain' */
    helicopter_B.Gain_p = helicopter_P.Gain_Gain_a *
      helicopter_B.TravelCounttorad;

    /* Sum: '<Root>/Sum3' incorporates:
     *  Constant: '<Root>/travel_offset'
     */
    helicopter_B.Sum3 = helicopter_P.travel_offset_Value + helicopter_B.Gain_p;
  }

  /* Gain: '<S15>/Gain' incorporates:
   *  TransferFcn: '<S4>/Travel: Transfer Fcn'
   */
  helicopter_B.Gain_dc = (helicopter_P.TravelTransferFcn_C *
    helicopter_X.TravelTransferFcn_CSTATE + helicopter_P.TravelTransferFcn_D *
    helicopter_B.TravelCounttorad) * helicopter_P.Gain_Gain_l;
  if (rtmIsMajorTimeStep(helicopter_M)) {
    /* Gain: '<S4>/Pitch: Count to rad' */
    helicopter_B.PitchCounttorad = helicopter_P.PitchCounttorad_Gain *
      rtb_HILReadEncoderTimebase_o2;

    /* Gain: '<S11>/Gain' */
    helicopter_B.Gain_i = helicopter_P.Gain_Gain_ar *
      helicopter_B.PitchCounttorad;

    /* Sum: '<Root>/Sum4' incorporates:
     *  Constant: '<Root>/pitch_offset'
     */
    helicopter_B.Sum4 = helicopter_P.pitch_offset_Value + helicopter_B.Gain_i;
  }

  /* Gain: '<S12>/Gain' incorporates:
   *  TransferFcn: '<S4>/Pitch: Transfer Fcn'
   */
  helicopter_B.Gain_b = (helicopter_P.PitchTransferFcn_C *
    helicopter_X.PitchTransferFcn_CSTATE + helicopter_P.PitchTransferFcn_D *
    helicopter_B.PitchCounttorad) * helicopter_P.Gain_Gain_ae;
  if (rtmIsMajorTimeStep(helicopter_M)) {
  }

  /* FromWorkspace: '<Root>/x +' */
  {
    real_T *pDataValues = (real_T *) helicopter_DW.x_PWORK.DataPtr;
    real_T *pTimeValues = (real_T *) helicopter_DW.x_PWORK.TimePtr;
    int_T currTimeIndex = helicopter_DW.x_IWORK.PrevIndex;
    real_T t = helicopter_M->Timing.t[0];

    /* Get index */
    if (t <= pTimeValues[0]) {
      currTimeIndex = 0;
    } else if (t >= pTimeValues[95]) {
      currTimeIndex = 94;
    } else {
      if (t < pTimeValues[currTimeIndex]) {
        while (t < pTimeValues[currTimeIndex]) {
          currTimeIndex--;
        }
      } else {
        while (t >= pTimeValues[currTimeIndex + 1]) {
          currTimeIndex++;
        }
      }
    }

    helicopter_DW.x_IWORK.PrevIndex = currTimeIndex;

    /* Post output */
    {
      real_T t1 = pTimeValues[currTimeIndex];
      real_T t2 = pTimeValues[currTimeIndex + 1];
      if (t1 == t2) {
        if (t < t1) {
          {
            int_T elIdx;
            for (elIdx = 0; elIdx < 6; ++elIdx) {
              (&helicopter_B.x[0])[elIdx] = pDataValues[currTimeIndex];
              pDataValues += 96;
            }
          }
        } else {
          {
            int_T elIdx;
            for (elIdx = 0; elIdx < 6; ++elIdx) {
              (&helicopter_B.x[0])[elIdx] = pDataValues[currTimeIndex + 1];
              pDataValues += 96;
            }
          }
        }
      } else {
        real_T f1 = (t2 - t) / (t2 - t1);
        real_T f2 = 1.0 - f1;
        real_T d1;
        real_T d2;
        int_T TimeIndex= currTimeIndex;

        {
          int_T elIdx;
          for (elIdx = 0; elIdx < 6; ++elIdx) {
            d1 = pDataValues[TimeIndex];
            d2 = pDataValues[TimeIndex + 1];
            (&helicopter_B.x[0])[elIdx] = (real_T) rtInterpolate(d1, d2, f1, f2);
            pDataValues += 96;
          }
        }
      }
    }
  }

  /* Gain: '<S2>/Gain1' */
  rtb_Gain1[0] = helicopter_P.Gain1_Gain * helicopter_B.Sum3;
  rtb_Gain1[1] = helicopter_P.Gain1_Gain * helicopter_B.Gain_dc;
  rtb_Gain1[2] = helicopter_P.Gain1_Gain * helicopter_B.Sum4;
  rtb_Gain1[3] = helicopter_P.Gain1_Gain * helicopter_B.Gain_b;
  rtb_Gain1[4] = helicopter_P.Gain1_Gain * helicopter_B.Sum;
  rtb_Gain1[5] = helicopter_P.Gain1_Gain * helicopter_B.Gain_d;

  /* Sum: '<S5>/Sum1' incorporates:
   *  FromWorkspace: '<Root>/x +'
   */
  for (i = 0; i < 6; i++) {
    rtb_Gain1_0[i] = rtb_Gain1[i] - helicopter_B.x[i];
  }

  /* End of Sum: '<S5>/Sum1' */

  /* Sum: '<S5>/Sum' incorporates:
   *  FromWorkspace: '<Root>/pitch_ref'
   *  Gain: '<S5>/Gain'
   */
  for (i = 0; i < 2; i++) {
    rtb_Frontgain = 0.0;
    for (i_0 = 0; i_0 < 6; i_0++) {
      rtb_Frontgain += helicopter_P.K[(i_0 << 1) + i] * rtb_Gain1_0[i_0];
    }

    rtb_Sum_p[i] = helicopter_B.pitch_ref[i] - rtb_Frontgain;
  }

  /* End of Sum: '<S5>/Sum' */

  /* Sum: '<Root>/Sum1' incorporates:
   *  Constant: '<Root>/Vd_bias'
   *  Gain: '<S7>/K_pd'
   *  Gain: '<S7>/K_pp'
   *  Sum: '<S7>/Sum2'
   *  Sum: '<S7>/Sum3'
   */
  helicopter_B.Sum1 = ((rtb_Sum_p[0] - rtb_Gain1[2]) * helicopter_P.K_pp -
                       helicopter_P.K_pd * rtb_Gain1[3]) + helicopter_P.Vd_ff;
  if (rtmIsMajorTimeStep(helicopter_M)) {
  }

  /* Integrator: '<S3>/Integrator' */
  /* Limited  Integrator  */
  if (helicopter_X.Integrator_CSTATE >= helicopter_P.Integrator_UpperSat) {
    helicopter_X.Integrator_CSTATE = helicopter_P.Integrator_UpperSat;
  } else {
    if (helicopter_X.Integrator_CSTATE <= helicopter_P.Integrator_LowerSat) {
      helicopter_X.Integrator_CSTATE = helicopter_P.Integrator_LowerSat;
    }
  }

  /* Sum: '<S3>/Sum' */
  rtb_Frontgain = rtb_Sum_p[1] - rtb_Gain1[4];

  /* Sum: '<Root>/Sum2' incorporates:
   *  Constant: '<Root>/Vs_bias'
   *  Gain: '<S3>/K_ed'
   *  Gain: '<S3>/K_ep'
   *  Integrator: '<S3>/Integrator'
   *  Sum: '<S3>/Sum1'
   */
  helicopter_B.Sum2 = ((helicopter_P.K_ep * rtb_Frontgain +
                        helicopter_X.Integrator_CSTATE) - helicopter_P.K_ed *
                       rtb_Gain1[5]) + helicopter_P.Vs_ff;
  if (rtmIsMajorTimeStep(helicopter_M)) {
  }

  /* Clock: '<S6>/Clock' incorporates:
   *  Clock: '<S3>/Clock'
   *  Derivative: '<S4>/Derivative'
   */
  Clock_tmp = helicopter_M->Timing.t[0];

  /* Clock: '<S6>/Clock' */
  helicopter_B.Clock = Clock_tmp;
  if (rtmIsMajorTimeStep(helicopter_M)) {
  }

  /* If: '<S3>/If' incorporates:
   *  Gain: '<S3>/K_ei'
   *  Inport: '<S8>/In1'
   */
  if (rtmIsMajorTimeStep(helicopter_M)) {
    rtAction = (int8_T)!(Clock_tmp >= 2.0);
    helicopter_DW.If_ActiveSubsystem = rtAction;
  } else {
    rtAction = helicopter_DW.If_ActiveSubsystem;
  }

  if (rtAction == 0) {
    /* Outputs for IfAction SubSystem: '<S3>/If Action Subsystem' incorporates:
     *  ActionPort: '<S8>/Action Port'
     */
    helicopter_B.In1 = helicopter_P.K_ei * rtb_Frontgain;
    if (rtmIsMajorTimeStep(helicopter_M)) {
      srUpdateBC(helicopter_DW.IfActionSubsystem_SubsysRanBC);
    }

    /* End of Outputs for SubSystem: '<S3>/If Action Subsystem' */
  }

  /* End of If: '<S3>/If' */
  if (rtmIsMajorTimeStep(helicopter_M)) {
  }

  /* Derivative: '<S4>/Derivative' */
  if ((helicopter_DW.TimeStampA >= Clock_tmp) && (helicopter_DW.TimeStampB >=
       Clock_tmp)) {
    rtb_Frontgain = 0.0;
  } else {
    rtb_Frontgain = helicopter_DW.TimeStampA;
    lastU = &helicopter_DW.LastUAtTimeA;
    if (helicopter_DW.TimeStampA < helicopter_DW.TimeStampB) {
      if (helicopter_DW.TimeStampB < Clock_tmp) {
        rtb_Frontgain = helicopter_DW.TimeStampB;
        lastU = &helicopter_DW.LastUAtTimeB;
      }
    } else {
      if (helicopter_DW.TimeStampA >= Clock_tmp) {
        rtb_Frontgain = helicopter_DW.TimeStampB;
        lastU = &helicopter_DW.LastUAtTimeB;
      }
    }

    rtb_Frontgain = (helicopter_B.PitchCounttorad - *lastU) / (Clock_tmp -
      rtb_Frontgain);
  }

  /* Gain: '<S13>/Gain' */
  helicopter_B.Gain_l = helicopter_P.Gain_Gain_a1 * rtb_Frontgain;
  if (rtmIsMajorTimeStep(helicopter_M)) {
  }

  /* Gain: '<S1>/Back gain' incorporates:
   *  Sum: '<S1>/Subtract'
   */
  rtb_Frontgain = (helicopter_B.Sum2 - helicopter_B.Sum1) *
    helicopter_P.Backgain_Gain;

  /* Saturate: '<S4>/Back motor: Saturation' */
  if (rtb_Frontgain > helicopter_P.BackmotorSaturation_UpperSat) {
    /* Saturate: '<S4>/Back motor: Saturation' */
    helicopter_B.BackmotorSaturation = helicopter_P.BackmotorSaturation_UpperSat;
  } else if (rtb_Frontgain < helicopter_P.BackmotorSaturation_LowerSat) {
    /* Saturate: '<S4>/Back motor: Saturation' */
    helicopter_B.BackmotorSaturation = helicopter_P.BackmotorSaturation_LowerSat;
  } else {
    /* Saturate: '<S4>/Back motor: Saturation' */
    helicopter_B.BackmotorSaturation = rtb_Frontgain;
  }

  /* End of Saturate: '<S4>/Back motor: Saturation' */
  if (rtmIsMajorTimeStep(helicopter_M)) {
  }

  /* Gain: '<S1>/Front gain' incorporates:
   *  Sum: '<S1>/Add'
   */
  rtb_Frontgain = (helicopter_B.Sum1 + helicopter_B.Sum2) *
    helicopter_P.Frontgain_Gain;

  /* Saturate: '<S4>/Front motor: Saturation' */
  if (rtb_Frontgain > helicopter_P.FrontmotorSaturation_UpperSat) {
    /* Saturate: '<S4>/Front motor: Saturation' */
    helicopter_B.FrontmotorSaturation =
      helicopter_P.FrontmotorSaturation_UpperSat;
  } else if (rtb_Frontgain < helicopter_P.FrontmotorSaturation_LowerSat) {
    /* Saturate: '<S4>/Front motor: Saturation' */
    helicopter_B.FrontmotorSaturation =
      helicopter_P.FrontmotorSaturation_LowerSat;
  } else {
    /* Saturate: '<S4>/Front motor: Saturation' */
    helicopter_B.FrontmotorSaturation = rtb_Frontgain;
  }

  /* End of Saturate: '<S4>/Front motor: Saturation' */
  if (rtmIsMajorTimeStep(helicopter_M)) {
    /* S-Function (hil_write_analog_block): '<S4>/HIL Write Analog' */

    /* S-Function Block: helicopter/Helicopter_interface/HIL Write Analog (hil_write_analog_block) */
    {
      t_error result;
      helicopter_DW.HILWriteAnalog_Buffer[0] = helicopter_B.FrontmotorSaturation;
      helicopter_DW.HILWriteAnalog_Buffer[1] = helicopter_B.BackmotorSaturation;
      result = hil_write_analog(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILWriteAnalog_channels, 2,
        &helicopter_DW.HILWriteAnalog_Buffer[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
      }
    }
  }
}

/* Model update function */
void helicopter_update(void)
{
  real_T *lastU;

  /* Update for Derivative: '<S4>/Derivative' */
  if (helicopter_DW.TimeStampA == (rtInf)) {
    helicopter_DW.TimeStampA = helicopter_M->Timing.t[0];
    lastU = &helicopter_DW.LastUAtTimeA;
  } else if (helicopter_DW.TimeStampB == (rtInf)) {
    helicopter_DW.TimeStampB = helicopter_M->Timing.t[0];
    lastU = &helicopter_DW.LastUAtTimeB;
  } else if (helicopter_DW.TimeStampA < helicopter_DW.TimeStampB) {
    helicopter_DW.TimeStampA = helicopter_M->Timing.t[0];
    lastU = &helicopter_DW.LastUAtTimeA;
  } else {
    helicopter_DW.TimeStampB = helicopter_M->Timing.t[0];
    lastU = &helicopter_DW.LastUAtTimeB;
  }

  *lastU = helicopter_B.PitchCounttorad;

  /* End of Update for Derivative: '<S4>/Derivative' */
  if (rtmIsMajorTimeStep(helicopter_M)) {
    rt_ertODEUpdateContinuousStates(&helicopter_M->solverInfo);
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
  if (!(++helicopter_M->Timing.clockTick0)) {
    ++helicopter_M->Timing.clockTickH0;
  }

  helicopter_M->Timing.t[0] = rtsiGetSolverStopTime(&helicopter_M->solverInfo);

  {
    /* Update absolute timer for sample time: [0.002s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick1"
     * and "Timing.stepSize1". Size of "clockTick1" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick1 and the high bits
     * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++helicopter_M->Timing.clockTick1)) {
      ++helicopter_M->Timing.clockTickH1;
    }

    helicopter_M->Timing.t[1] = helicopter_M->Timing.clockTick1 *
      helicopter_M->Timing.stepSize1 + helicopter_M->Timing.clockTickH1 *
      helicopter_M->Timing.stepSize1 * 4294967296.0;
  }
}

/* Derivatives for root system: '<Root>' */
void helicopter_derivatives(void)
{
  XDot_helicopter_T *_rtXdot;
  boolean_T lsat;
  boolean_T usat;
  _rtXdot = ((XDot_helicopter_T *) helicopter_M->derivs);

  /* Derivatives for TransferFcn: '<S4>/Elevation: Transfer Fcn' */
  _rtXdot->ElevationTransferFcn_CSTATE = 0.0;
  _rtXdot->ElevationTransferFcn_CSTATE += helicopter_P.ElevationTransferFcn_A *
    helicopter_X.ElevationTransferFcn_CSTATE;
  _rtXdot->ElevationTransferFcn_CSTATE += helicopter_B.ElevationCounttorad;

  /* Derivatives for TransferFcn: '<S4>/Travel: Transfer Fcn' */
  _rtXdot->TravelTransferFcn_CSTATE = 0.0;
  _rtXdot->TravelTransferFcn_CSTATE += helicopter_P.TravelTransferFcn_A *
    helicopter_X.TravelTransferFcn_CSTATE;
  _rtXdot->TravelTransferFcn_CSTATE += helicopter_B.TravelCounttorad;

  /* Derivatives for TransferFcn: '<S4>/Pitch: Transfer Fcn' */
  _rtXdot->PitchTransferFcn_CSTATE = 0.0;
  _rtXdot->PitchTransferFcn_CSTATE += helicopter_P.PitchTransferFcn_A *
    helicopter_X.PitchTransferFcn_CSTATE;
  _rtXdot->PitchTransferFcn_CSTATE += helicopter_B.PitchCounttorad;

  /* Derivatives for Integrator: '<S3>/Integrator' */
  lsat = (helicopter_X.Integrator_CSTATE <= helicopter_P.Integrator_LowerSat);
  usat = (helicopter_X.Integrator_CSTATE >= helicopter_P.Integrator_UpperSat);
  if (((!lsat) && (!usat)) || (lsat && (helicopter_B.In1 > 0.0)) || (usat &&
       (helicopter_B.In1 < 0.0))) {
    _rtXdot->Integrator_CSTATE = helicopter_B.In1;
  } else {
    /* in saturation */
    _rtXdot->Integrator_CSTATE = 0.0;
  }

  /* End of Derivatives for Integrator: '<S3>/Integrator' */
}

/* Model initialize function */
void helicopter_initialize(void)
{
  /* Start for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: helicopter/HIL Initialize (hil_initialize_block) */
  {
    t_int result;
    t_boolean is_switching;
    result = hil_open("q8_usb", "0", &helicopter_DW.HILInitialize_Card);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_M, _rt_error_message);
      return;
    }

    is_switching = false;
    result = hil_set_card_specific_options(helicopter_DW.HILInitialize_Card,
      "update_rate=normal;decimation=1", 32);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_M, _rt_error_message);
      return;
    }

    result = hil_watchdog_clear(helicopter_DW.HILInitialize_Card);
    if (result < 0 && result != -QERR_HIL_WATCHDOG_CLEAR) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_M, _rt_error_message);
      return;
    }

    if ((helicopter_P.HILInitialize_AIPStart && !is_switching) ||
        (helicopter_P.HILInitialize_AIPEnter && is_switching)) {
      {
        int_T i1;
        real_T *dw_AIMinimums = &helicopter_DW.HILInitialize_AIMinimums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AIMinimums[i1] = (helicopter_P.HILInitialize_AILow);
        }
      }

      {
        int_T i1;
        real_T *dw_AIMaximums = &helicopter_DW.HILInitialize_AIMaximums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AIMaximums[i1] = helicopter_P.HILInitialize_AIHigh;
        }
      }

      result = hil_set_analog_input_ranges(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_AIChannels, 8U,
        &helicopter_DW.HILInitialize_AIMinimums[0],
        &helicopter_DW.HILInitialize_AIMaximums[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_P.HILInitialize_AOPStart && !is_switching) ||
        (helicopter_P.HILInitialize_AOPEnter && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOMinimums = &helicopter_DW.HILInitialize_AOMinimums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOMinimums[i1] = (helicopter_P.HILInitialize_AOLow);
        }
      }

      {
        int_T i1;
        real_T *dw_AOMaximums = &helicopter_DW.HILInitialize_AOMaximums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOMaximums[i1] = helicopter_P.HILInitialize_AOHigh;
        }
      }

      result = hil_set_analog_output_ranges(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_AOChannels, 8U,
        &helicopter_DW.HILInitialize_AOMinimums[0],
        &helicopter_DW.HILInitialize_AOMaximums[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_P.HILInitialize_AOStart && !is_switching) ||
        (helicopter_P.HILInitialize_AOEnter && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &helicopter_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = helicopter_P.HILInitialize_AOInitial;
        }
      }

      result = hil_write_analog(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_AOChannels, 8U,
        &helicopter_DW.HILInitialize_AOVoltages[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if (helicopter_P.HILInitialize_AOReset) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &helicopter_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = helicopter_P.HILInitialize_AOWatchdog;
        }
      }

      result = hil_watchdog_set_analog_expiration_state
        (helicopter_DW.HILInitialize_Card, helicopter_P.HILInitialize_AOChannels,
         8U, &helicopter_DW.HILInitialize_AOVoltages[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_P.HILInitialize_EIPStart && !is_switching) ||
        (helicopter_P.HILInitialize_EIPEnter && is_switching)) {
      {
        int_T i1;
        int32_T *dw_QuadratureModes =
          &helicopter_DW.HILInitialize_QuadratureModes[0];
        for (i1=0; i1 < 8; i1++) {
          dw_QuadratureModes[i1] = helicopter_P.HILInitialize_EIQuadrature;
        }
      }

      result = hil_set_encoder_quadrature_mode(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_EIChannels, 8U, (t_encoder_quadrature_mode *)
        &helicopter_DW.HILInitialize_QuadratureModes[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_P.HILInitialize_EIStart && !is_switching) ||
        (helicopter_P.HILInitialize_EIEnter && is_switching)) {
      {
        int_T i1;
        int32_T *dw_InitialEICounts =
          &helicopter_DW.HILInitialize_InitialEICounts[0];
        for (i1=0; i1 < 8; i1++) {
          dw_InitialEICounts[i1] = helicopter_P.HILInitialize_EIInitial;
        }
      }

      result = hil_set_encoder_counts(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_EIChannels, 8U,
        &helicopter_DW.HILInitialize_InitialEICounts[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_P.HILInitialize_POPStart && !is_switching) ||
        (helicopter_P.HILInitialize_POPEnter && is_switching)) {
      uint32_T num_duty_cycle_modes = 0;
      uint32_T num_frequency_modes = 0;

      {
        int_T i1;
        int32_T *dw_POModeValues = &helicopter_DW.HILInitialize_POModeValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POModeValues[i1] = helicopter_P.HILInitialize_POModes;
        }
      }

      result = hil_set_pwm_mode(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_POChannels, 8U, (t_pwm_mode *)
        &helicopter_DW.HILInitialize_POModeValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }

      {
        int_T i1;
        const uint32_T *p_HILInitialize_POChannels =
          helicopter_P.HILInitialize_POChannels;
        int32_T *dw_POModeValues = &helicopter_DW.HILInitialize_POModeValues[0];
        for (i1=0; i1 < 8; i1++) {
          if (dw_POModeValues[i1] == PWM_DUTY_CYCLE_MODE || dw_POModeValues[i1] ==
              PWM_ONE_SHOT_MODE || dw_POModeValues[i1] == PWM_TIME_MODE ||
              dw_POModeValues[i1] == PWM_RAW_MODE) {
            helicopter_DW.HILInitialize_POSortedChans[num_duty_cycle_modes] =
              (p_HILInitialize_POChannels[i1]);
            helicopter_DW.HILInitialize_POSortedFreqs[num_duty_cycle_modes] =
              helicopter_P.HILInitialize_POFrequency;
            num_duty_cycle_modes++;
          } else {
            helicopter_DW.HILInitialize_POSortedChans[7U - num_frequency_modes] =
              (p_HILInitialize_POChannels[i1]);
            helicopter_DW.HILInitialize_POSortedFreqs[7U - num_frequency_modes] =
              helicopter_P.HILInitialize_POFrequency;
            num_frequency_modes++;
          }
        }
      }

      if (num_duty_cycle_modes > 0) {
        result = hil_set_pwm_frequency(helicopter_DW.HILInitialize_Card,
          &helicopter_DW.HILInitialize_POSortedChans[0], num_duty_cycle_modes,
          &helicopter_DW.HILInitialize_POSortedFreqs[0]);
        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(helicopter_M, _rt_error_message);
          return;
        }
      }

      if (num_frequency_modes > 0) {
        result = hil_set_pwm_duty_cycle(helicopter_DW.HILInitialize_Card,
          &helicopter_DW.HILInitialize_POSortedChans[num_duty_cycle_modes],
          num_frequency_modes,
          &helicopter_DW.HILInitialize_POSortedFreqs[num_duty_cycle_modes]);
        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(helicopter_M, _rt_error_message);
          return;
        }
      }

      {
        int_T i1;
        int32_T *dw_POModeValues = &helicopter_DW.HILInitialize_POModeValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POModeValues[i1] = helicopter_P.HILInitialize_POConfiguration;
        }
      }

      {
        int_T i1;
        int32_T *dw_POAlignValues = &helicopter_DW.HILInitialize_POAlignValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POAlignValues[i1] = helicopter_P.HILInitialize_POAlignment;
        }
      }

      {
        int_T i1;
        int32_T *dw_POPolarityVals =
          &helicopter_DW.HILInitialize_POPolarityVals[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POPolarityVals[i1] = helicopter_P.HILInitialize_POPolarity;
        }
      }

      result = hil_set_pwm_configuration(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_POChannels, 8U,
        (t_pwm_configuration *) &helicopter_DW.HILInitialize_POModeValues[0],
        (t_pwm_alignment *) &helicopter_DW.HILInitialize_POAlignValues[0],
        (t_pwm_polarity *) &helicopter_DW.HILInitialize_POPolarityVals[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }

      {
        int_T i1;
        real_T *dw_POSortedFreqs = &helicopter_DW.HILInitialize_POSortedFreqs[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POSortedFreqs[i1] = helicopter_P.HILInitialize_POLeading;
        }
      }

      {
        int_T i1;
        real_T *dw_POValues = &helicopter_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_P.HILInitialize_POTrailing;
        }
      }

      result = hil_set_pwm_deadband(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_POChannels, 8U,
        &helicopter_DW.HILInitialize_POSortedFreqs[0],
        &helicopter_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_P.HILInitialize_POStart && !is_switching) ||
        (helicopter_P.HILInitialize_POEnter && is_switching)) {
      {
        int_T i1;
        real_T *dw_POValues = &helicopter_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_P.HILInitialize_POInitial;
        }
      }

      result = hil_write_pwm(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_POChannels, 8U,
        &helicopter_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if (helicopter_P.HILInitialize_POReset) {
      {
        int_T i1;
        real_T *dw_POValues = &helicopter_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_P.HILInitialize_POWatchdog;
        }
      }

      result = hil_watchdog_set_pwm_expiration_state
        (helicopter_DW.HILInitialize_Card, helicopter_P.HILInitialize_POChannels,
         8U, &helicopter_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }
  }

  /* Start for S-Function (hil_read_encoder_timebase_block): '<S4>/HIL Read Encoder Timebase' */

  /* S-Function Block: helicopter/Helicopter_interface/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
  {
    t_error result;
    result = hil_task_create_encoder_reader(helicopter_DW.HILInitialize_Card,
      helicopter_P.HILReadEncoderTimebase_SamplesI,
      helicopter_P.HILReadEncoderTimebase_Channels, 3,
      &helicopter_DW.HILReadEncoderTimebase_Task);
    if (result >= 0) {
      result = hil_task_set_buffer_overflow_mode
        (helicopter_DW.HILReadEncoderTimebase_Task, (t_buffer_overflow_mode)
         (helicopter_P.HILReadEncoderTimebase_Overflow - 1));
    }

    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_M, _rt_error_message);
    }
  }

  /* Start for FromWorkspace: '<Root>/pitch_ref' */
  {
    static real_T pTimeValues0[] = { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75,
      2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75, 4.0, 4.25, 4.5, 4.75, 5.0,
      5.25, 5.5, 5.75, 6.0, 6.25, 6.5, 6.75, 7.0, 7.25, 7.5, 7.75, 8.0, 8.25,
      8.5, 8.75, 9.0, 9.25, 9.5, 9.75, 10.0, 10.25, 10.5, 10.75, 11.0, 11.25,
      11.5, 11.75, 12.0, 12.25, 12.5, 12.75, 13.0, 13.25, 13.5, 13.75, 14.0,
      14.25, 14.5, 14.75, 15.0, 15.25, 15.5, 15.75, 16.0, 16.25, 16.5, 16.75,
      17.0, 17.25, 17.5, 17.75, 18.0, 18.25, 18.5, 18.75, 19.0, 19.25, 19.5,
      19.75, 20.0, 20.25, 20.5, 20.75, 21.0, 21.25, 21.5, 21.75, 22.0, 22.25,
      22.5, 22.75, 23.0, 23.25, 23.5, 23.75 } ;

    static real_T pDataValues0[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.52359877559829882,
      0.52359877559829882, 0.52359877559829882, 0.52359877559829882,
      0.52359877559829882, 0.52359877559829882, 0.52359877559829882,
      0.52359877559829882, 0.52359877559829882, 0.52359877559829882,
      0.52359877559829882, 0.38714565612375768, 0.12648330821860249,
      -0.099711567222785763, -0.27326552966085405, -0.40100428857664067,
      -0.48955624805414805, -0.52359877559829882, -0.52359877559829882,
      -0.52359877559829882, -0.52359877559829882, -0.52359877559829882,
      -0.51801303013118827, -0.4812086227596, -0.44117824730285965,
      -0.39991730998959063, -0.358927191900965, -0.31928149953132373, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.285643748605681, 0.30313409346497655,
      0.31978815101488922, 0.33501593045945127, 0.34806874827140621,
      0.35801388561164066, 0.36369586812756638, 0.36369568928241808,
      0.35627381815466136, 0.33930847357267435, 0.31022518676565236,
      0.26590650744417343, 0.20259659451391862, 0.1157697296131867,
      -4.6524159199405855E-7, 3.4123155775447112E-7, -1.3131673518545879E-6,
      -2.6901559753529817E-7, -9.6564917377866774E-7, -1.105145631279774E-7,
      3.3095258299058876E-7, 8.4148902273888643E-7, -4.4655620896351253E-8,
      1.019429601838447E-6, 5.1502286027423757E-7, 2.56734362157644E-7,
      2.2946752899479502E-7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 } ;

    helicopter_DW.pitch_ref_PWORK.TimePtr = (void *) pTimeValues0;
    helicopter_DW.pitch_ref_PWORK.DataPtr = (void *) pDataValues0;
    helicopter_DW.pitch_ref_IWORK.PrevIndex = 0;
  }

  /* Start for FromWorkspace: '<Root>/x +' */
  {
    static real_T pTimeValues0[] = { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75,
      2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75, 4.0, 4.25, 4.5, 4.75, 5.0,
      5.25, 5.5, 5.75, 6.0, 6.25, 6.5, 6.75, 7.0, 7.25, 7.5, 7.75, 8.0, 8.25,
      8.5, 8.75, 9.0, 9.25, 9.5, 9.75, 10.0, 10.25, 10.5, 10.75, 11.0, 11.25,
      11.5, 11.75, 12.0, 12.25, 12.5, 12.75, 13.0, 13.25, 13.5, 13.75, 14.0,
      14.25, 14.5, 14.75, 15.0, 15.25, 15.5, 15.75, 16.0, 16.25, 16.5, 16.75,
      17.0, 17.25, 17.5, 17.75, 18.0, 18.25, 18.5, 18.75, 19.0, 19.25, 19.5,
      19.75, 20.0, 20.25, 20.5, 20.75, 21.0, 21.25, 21.5, 21.75, 22.0, 22.25,
      22.5, 22.75, 23.0, 23.25, 23.5, 23.75 } ;

    static real_T pDataValues0[] = { 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1378421413625261, 3.1262155534579983,
      3.1033093000299643, 3.0666274151911783, 3.0144539223941584,
      2.9456562771175667, 2.8595077632935446, 2.7555515879651526,
      2.633505110490284, 2.4931956060320961, 2.334518576064299,
      2.1583887284416625, 1.9677109641615416, 1.7670241979411219,
      1.5616856618980135, 1.357130276094934, 1.1583501832072942,
      0.96943652180629669, 0.79337942116095184, 0.63223045404938316,
      0.48734183642365025, 0.35958040506563654, 0.24945062469039389,
      0.15698156020163567, 0.081765846810397544, 0.023051041054761762,
      -0.020160927220642492, -0.049024301180242806, -0.066793829730228835,
      -0.076700578110169809, -0.081314156539469579, -0.082483739024502636,
      -0.081466109880911283, -0.079084356718973736, -0.075863698654514311,
      -0.072132892712830479, -0.068094692839212712, -0.063872679373327981,
      -0.059541457620359624, -0.055145710363260006, -0.050712020558458817,
      -0.046256112916185609, -0.041787243273372895, -0.03731083632580201,
      -0.032830059348160343, -0.028346755372130161, -0.023861993631969024,
      -0.019376392768276365, -0.01489030984234864, -0.010403950482830632,
      -0.0059174328701894182, -0.0014308248002737392, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.015002048909068423,
      -0.046506351618112111, -0.091625013712135384, -0.14672753935514368,
      -0.2086939711880792, -0.27519058110636668, -0.34459405529608833,
      -0.41582470131356863, -0.48818590989947463, -0.56123801783275173,
      -0.63470811987118847, -0.70451939049054557, -0.7627110571204847,
      -0.80274706488167891, -0.82135414417243313, -0.8182215432123181,
      -0.79512037155055915, -0.75565464560399065, -0.70422840258137909,
      -0.64459586844627481, -0.57955447050293174, -0.511045725432055,
      -0.44051912150097061, -0.36987625795503276, -0.30086285356495257,
      -0.23485922302254311, -0.17284787310161703, -0.11545349583840127,
      -0.071078114199944087, -0.039626993519763946, -0.018454313717199092,
      -0.00467832994013224, 0.0040705165743654273, 0.0095270126477501379,
      0.012882632257837775, 0.014923223766735301, 0.016152799494471067,
      0.016888053863538911, 0.017324887011873468, 0.017582989028398461,
      0.017734759219204747, 0.017823630569092853, 0.017875478571250866,
      0.01790562779028353, 0.017923107910566661, 0.017933215904120725,
      0.017939046960644547, 0.017942403454770647, 0.017944331703710903,
      0.017945437438072035, 0.017946070450564854, 0.017946432279662715,
      0.017946638805391282, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.10602875205865551, 0.22266037932317656, 0.31888147181640641,
      0.38944360631144165, 0.43795507377677839, 0.46997264230390062,
      0.49051724877547076, 0.50343100141474351, 0.5114213858602934,
      0.51630439857701838, 0.51925862127063693, 0.49339939819321355,
      0.41127647499243419, 0.28295921217561643, 0.13150772995164375,
      -0.022140027173085563, -0.16327025843173135, -0.27892867811370159,
      -0.36346104480812857, -0.42145997621820225, -0.45968775984421906,
      -0.48419364508129026, -0.49845510083810868, -0.49927677939364823,
      -0.48776038440831426, -0.46648845235545916, -0.43827253766743546,
      -0.40564153825174565, -0.31362824943252104, -0.22228450905462008,
      -0.14964041450674448, -0.097363391968396357, -0.061833505776945782,
      -0.038564430284200465, -0.023716237815094413, -0.014422121435633211,
      -0.0086901716401304729, -0.0051964970698644732, -0.0030873698557114526,
      -0.0018241664776485946, -0.0010726537440607397, -0.00062810875897811381,
      -0.00036644187729755135, -0.00021308316543642707, -0.0001235428140975605,
      -7.143943796279734E-5, -4.1211680494565031E-5, -2.3722418560275334E-5,
      -1.3628127066696948E-5, -7.8149081588833525E-6, -4.4738905370958625E-6,
      -2.5572698727432338E-6, -1.4596449725460589E-6, -8.3203533329583652E-7,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.42411500823462206,
      0.46652650905808424, 0.38488436997291947, 0.28224853798014093,
      0.19404586986134695, 0.12807027410848904, 0.082178425886280423,
      0.051655010557090604, 0.031961537782199796, 0.019532050866899783,
      0.011816890774474327, -0.10343689230969372, -0.32849169280311724,
      -0.51326905126727118, -0.60580592889589058, -0.61459102849891734,
      -0.56452092503458318, -0.46263367872788108, -0.33812946677770778,
      -0.23199572564029453, -0.15291113450406732, -0.098023540948284985,
      -0.057045823027273596, -0.0032867142221582831, 0.046065579941335894,
      0.085087728211420272, 0.11286365875209491, 0.13052399766275918,
      0.36805315527689864, 0.36537496151160387, 0.29057637819150245,
      0.20910809015339249, 0.1421195447658023, 0.093076301970981282,
      0.059392769876424216, 0.0371764655178448, 0.022927799182010957,
      0.013974698281063997, 0.0084365088566120822, 0.005052813512251432,
      0.0030060509343514193, 0.0017781799403305035, 0.0010466675267222496,
      0.00061343484744449711, 0.00035816140535546633, 0.00020841350453905258,
      0.00012091102987292927, 6.9957047737158771E-5, 4.0377165974313554E-5,
      2.3252875631254378E-5, 1.3364070487149965E-5, 7.6664826574105131E-6,
      4.3904996007886994E-6, 2.5104385570008895E-6, 1.4333562834622907E-6, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0044631835719637653, 0.01254704146132685, 0.023536887494644873,
      0.03681784841022908, 0.051849379421554133, 0.068141715761319985,
      0.085233569402175816, 0.10267049046908483, 0.1199831851560242,
      0.1366651746572223, 0.15214919805827132, 0.1657816114338542,
      0.17679416203516085, 0.1842721393326929, 0.18711820625464268,
      0.18637350960077476, 0.18289124461940515, 0.17736745559249714,
      0.17036692303686959, 0.1623451553997261, 0.15366685167055152,
      0.14462149396881582, 0.13543643043741763, 0.12628793787419376,
      0.11731038227342339, 0.10860397055503576, 0.10024119062865292,
      0.092272168643943372, 0.08472913355183849, 0.07763010459769823,
      0.07098194017034555, 0.064782846465492011, 0.0590244333716902,
      0.053693391575315538, 0.048772853456601879, 0.044243490624202332,
      0.040084392664643267, 0.0362737646539708, 0.032789475010581409,
      0.029609480205321066, 0.026712148554335475, 0.024076501687888139,
      0.021682389216891147, 0.019510609524770146, 0.017542987424165477,
      0.015762417574887437, 0.014152881009426325, 0.012699440810722874,
      0.011388221895923, 0.010206378947155549, 0.0091420557684561629,
      0.00818433871338232, 0.0073232063006948055, 0.0065494766987825685, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.017852734287855061,
      0.032335431557452332, 0.043959384133272092, 0.053123843662336849,
      0.060126124045300218, 0.0651693453590634, 0.068367414563423309,
      0.0697476842676361, 0.069250778747757424, 0.06672795800479242,
      0.061936093604196073, 0.0545296535023315, 0.044050202405226585,
      0.029911909190128219, 0.011384267687799109, -0.0029787866154716144,
      -0.01392905992547837, -0.022095156107632047, -0.028002130222510219,
      -0.032087070548573932, -0.034713214916698362, -0.036181430806942735,
      -0.036740254125592828, -0.0365939702528955, -0.035910222403081463,
      -0.034825646873550567, -0.033451119705531329, -0.031876087938838227,
      -0.030172140368419476, -0.028396115816561068, -0.026592657709410706,
      -0.024796374819414168, -0.023033652375207225, -0.021324167185498667,
      -0.019682152474854636, -0.0181174513295982, -0.016636391838236265,
      -0.015242512042689846, -0.013937158573557588, -0.012719979221041367,
      -0.011589326603942362, -0.010542587465789338, -0.00957644988398797,
      -0.0086871187684839871, -0.0078704884024186853, -0.0071222793971121483,
      -0.0064381462618444537, -0.0058137607948138053, -0.0052448756591994992,
      -0.0047273717950698036, -0.00425729271479754, -0.0038308682202953773,
      -0.0034445296507500528, -0.0030949184076489475, -0.0027788891995301486,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0 } ;

    helicopter_DW.x_PWORK.TimePtr = (void *) pTimeValues0;
    helicopter_DW.x_PWORK.DataPtr = (void *) pDataValues0;
    helicopter_DW.x_IWORK.PrevIndex = 0;
  }

  /* Start for If: '<S3>/If' */
  helicopter_DW.If_ActiveSubsystem = -1;

  /* InitializeConditions for TransferFcn: '<S4>/Elevation: Transfer Fcn' */
  helicopter_X.ElevationTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S4>/Travel: Transfer Fcn' */
  helicopter_X.TravelTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S4>/Pitch: Transfer Fcn' */
  helicopter_X.PitchTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for Integrator: '<S3>/Integrator' */
  helicopter_X.Integrator_CSTATE = helicopter_P.Integrator_IC;

  /* InitializeConditions for Derivative: '<S4>/Derivative' */
  helicopter_DW.TimeStampA = (rtInf);
  helicopter_DW.TimeStampB = (rtInf);
}

/* Model terminate function */
void helicopter_terminate(void)
{
  /* Terminate for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: helicopter/HIL Initialize (hil_initialize_block) */
  {
    t_boolean is_switching;
    t_int result;
    t_uint32 num_final_analog_outputs = 0;
    t_uint32 num_final_pwm_outputs = 0;
    hil_task_stop_all(helicopter_DW.HILInitialize_Card);
    hil_monitor_stop_all(helicopter_DW.HILInitialize_Card);
    is_switching = false;
    if ((helicopter_P.HILInitialize_AOTerminate && !is_switching) ||
        (helicopter_P.HILInitialize_AOExit && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &helicopter_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = helicopter_P.HILInitialize_AOFinal;
        }
      }

      num_final_analog_outputs = 8U;
    } else {
      num_final_analog_outputs = 0;
    }

    if ((helicopter_P.HILInitialize_POTerminate && !is_switching) ||
        (helicopter_P.HILInitialize_POExit && is_switching)) {
      {
        int_T i1;
        real_T *dw_POValues = &helicopter_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_P.HILInitialize_POFinal;
        }
      }

      num_final_pwm_outputs = 8U;
    } else {
      num_final_pwm_outputs = 0;
    }

    if (0
        || num_final_analog_outputs > 0
        || num_final_pwm_outputs > 0
        ) {
      /* Attempt to write the final outputs atomically (due to firmware issue in old Q2-USB). Otherwise write channels individually */
      result = hil_write(helicopter_DW.HILInitialize_Card
                         , helicopter_P.HILInitialize_AOChannels,
                         num_final_analog_outputs
                         , helicopter_P.HILInitialize_POChannels,
                         num_final_pwm_outputs
                         , NULL, 0
                         , NULL, 0
                         , &helicopter_DW.HILInitialize_AOVoltages[0]
                         , &helicopter_DW.HILInitialize_POValues[0]
                         , (t_boolean *) NULL
                         , NULL
                         );
      if (result == -QERR_HIL_WRITE_NOT_SUPPORTED) {
        t_error local_result;
        result = 0;

        /* The hil_write operation is not supported by this card. Write final outputs for each channel type */
        if (num_final_analog_outputs > 0) {
          local_result = hil_write_analog(helicopter_DW.HILInitialize_Card,
            helicopter_P.HILInitialize_AOChannels, num_final_analog_outputs,
            &helicopter_DW.HILInitialize_AOVoltages[0]);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (num_final_pwm_outputs > 0) {
          local_result = hil_write_pwm(helicopter_DW.HILInitialize_Card,
            helicopter_P.HILInitialize_POChannels, num_final_pwm_outputs,
            &helicopter_DW.HILInitialize_POValues[0]);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(helicopter_M, _rt_error_message);
        }
      }
    }

    hil_task_delete_all(helicopter_DW.HILInitialize_Card);
    hil_monitor_delete_all(helicopter_DW.HILInitialize_Card);
    hil_close(helicopter_DW.HILInitialize_Card);
    helicopter_DW.HILInitialize_Card = NULL;
  }
}

/*========================================================================*
 * Start of Classic call interface                                        *
 *========================================================================*/

/* Solver interface called by GRT_Main */
#ifndef USE_GENERATED_SOLVER

void rt_ODECreateIntegrationData(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

void rt_ODEDestroyIntegrationData(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

void rt_ODEUpdateContinuousStates(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

#endif

void MdlOutputs(int_T tid)
{
  helicopter_output();
  UNUSED_PARAMETER(tid);
}

void MdlUpdate(int_T tid)
{
  helicopter_update();
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
  helicopter_initialize();
}

void MdlTerminate(void)
{
  helicopter_terminate();
}

/* Registration function */
RT_MODEL_helicopter_T *helicopter(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* non-finite (run-time) assignments */
  helicopter_P.Integrator_UpperSat = rtInf;
  helicopter_P.Integrator_LowerSat = rtMinusInf;

  /* initialize real-time model */
  (void) memset((void *)helicopter_M, 0,
                sizeof(RT_MODEL_helicopter_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&helicopter_M->solverInfo,
                          &helicopter_M->Timing.simTimeStep);
    rtsiSetTPtr(&helicopter_M->solverInfo, &rtmGetTPtr(helicopter_M));
    rtsiSetStepSizePtr(&helicopter_M->solverInfo,
                       &helicopter_M->Timing.stepSize0);
    rtsiSetdXPtr(&helicopter_M->solverInfo, &helicopter_M->derivs);
    rtsiSetContStatesPtr(&helicopter_M->solverInfo, (real_T **)
                         &helicopter_M->contStates);
    rtsiSetNumContStatesPtr(&helicopter_M->solverInfo,
      &helicopter_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&helicopter_M->solverInfo,
      &helicopter_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&helicopter_M->solverInfo,
      &helicopter_M->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&helicopter_M->solverInfo,
      &helicopter_M->periodicContStateRanges);
    rtsiSetErrorStatusPtr(&helicopter_M->solverInfo, (&rtmGetErrorStatus
      (helicopter_M)));
    rtsiSetRTModelPtr(&helicopter_M->solverInfo, helicopter_M);
  }

  rtsiSetSimTimeStep(&helicopter_M->solverInfo, MAJOR_TIME_STEP);
  helicopter_M->intgData.f[0] = helicopter_M->odeF[0];
  helicopter_M->contStates = ((real_T *) &helicopter_X);
  rtsiSetSolverData(&helicopter_M->solverInfo, (void *)&helicopter_M->intgData);
  rtsiSetSolverName(&helicopter_M->solverInfo,"ode1");

  /* Initialize timing info */
  {
    int_T *mdlTsMap = helicopter_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    mdlTsMap[1] = 1;
    helicopter_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    helicopter_M->Timing.sampleTimes = (&helicopter_M->Timing.sampleTimesArray[0]);
    helicopter_M->Timing.offsetTimes = (&helicopter_M->Timing.offsetTimesArray[0]);

    /* task periods */
    helicopter_M->Timing.sampleTimes[0] = (0.0);
    helicopter_M->Timing.sampleTimes[1] = (0.002);

    /* task offsets */
    helicopter_M->Timing.offsetTimes[0] = (0.0);
    helicopter_M->Timing.offsetTimes[1] = (0.0);
  }

  rtmSetTPtr(helicopter_M, &helicopter_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = helicopter_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    mdlSampleHits[1] = 1;
    helicopter_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(helicopter_M, -1);
  helicopter_M->Timing.stepSize0 = 0.002;
  helicopter_M->Timing.stepSize1 = 0.002;

  /* External mode info */
  helicopter_M->Sizes.checksums[0] = (1131567537U);
  helicopter_M->Sizes.checksums[1] = (107643233U);
  helicopter_M->Sizes.checksums[2] = (617425500U);
  helicopter_M->Sizes.checksums[3] = (1436847312U);

  {
    static const sysRanDType rtAlwaysEnabled = SUBSYS_RAN_BC_ENABLE;
    static RTWExtModeInfo rt_ExtModeInfo;
    static const sysRanDType *systemRan[2];
    helicopter_M->extModeInfo = (&rt_ExtModeInfo);
    rteiSetSubSystemActiveVectorAddresses(&rt_ExtModeInfo, systemRan);
    systemRan[0] = &rtAlwaysEnabled;
    systemRan[1] = (sysRanDType *)&helicopter_DW.IfActionSubsystem_SubsysRanBC;
    rteiSetModelMappingInfoPtr(helicopter_M->extModeInfo,
      &helicopter_M->SpecialInfo.mappingInfo);
    rteiSetChecksumsPtr(helicopter_M->extModeInfo, helicopter_M->Sizes.checksums);
    rteiSetTPtr(helicopter_M->extModeInfo, rtmGetTPtr(helicopter_M));
  }

  helicopter_M->solverInfoPtr = (&helicopter_M->solverInfo);
  helicopter_M->Timing.stepSize = (0.002);
  rtsiSetFixedStepSize(&helicopter_M->solverInfo, 0.002);
  rtsiSetSolverMode(&helicopter_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* block I/O */
  helicopter_M->blockIO = ((void *) &helicopter_B);

  {
    int32_T i;
    for (i = 0; i < 6; i++) {
      helicopter_B.x[i] = 0.0;
    }

    helicopter_B.ElevationCounttorad = 0.0;
    helicopter_B.Gain = 0.0;
    helicopter_B.Sum = 0.0;
    helicopter_B.Gain_d = 0.0;
    helicopter_B.pitch_ref[0] = 0.0;
    helicopter_B.pitch_ref[1] = 0.0;
    helicopter_B.TravelCounttorad = 0.0;
    helicopter_B.Gain_p = 0.0;
    helicopter_B.Sum3 = 0.0;
    helicopter_B.Gain_dc = 0.0;
    helicopter_B.PitchCounttorad = 0.0;
    helicopter_B.Gain_i = 0.0;
    helicopter_B.Sum4 = 0.0;
    helicopter_B.Gain_b = 0.0;
    helicopter_B.Sum1 = 0.0;
    helicopter_B.Sum2 = 0.0;
    helicopter_B.Clock = 0.0;
    helicopter_B.Gain_l = 0.0;
    helicopter_B.BackmotorSaturation = 0.0;
    helicopter_B.FrontmotorSaturation = 0.0;
    helicopter_B.In1 = 0.0;
  }

  /* parameters */
  helicopter_M->defaultParam = ((real_T *)&helicopter_P);

  /* states (continuous) */
  {
    real_T *x = (real_T *) &helicopter_X;
    helicopter_M->contStates = (x);
    (void) memset((void *)&helicopter_X, 0,
                  sizeof(X_helicopter_T));
  }

  /* states (dwork) */
  helicopter_M->dwork = ((void *) &helicopter_DW);
  (void) memset((void *)&helicopter_DW, 0,
                sizeof(DW_helicopter_T));

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_AIMinimums[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_AIMaximums[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_AOMinimums[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_AOMaximums[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_AOVoltages[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_FilterFrequency[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_POSortedFreqs[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_POValues[i] = 0.0;
    }
  }

  helicopter_DW.TimeStampA = 0.0;
  helicopter_DW.LastUAtTimeA = 0.0;
  helicopter_DW.TimeStampB = 0.0;
  helicopter_DW.LastUAtTimeB = 0.0;
  helicopter_DW.HILWriteAnalog_Buffer[0] = 0.0;
  helicopter_DW.HILWriteAnalog_Buffer[1] = 0.0;

  /* data type transition information */
  {
    static DataTypeTransInfo dtInfo;
    (void) memset((char_T *) &dtInfo, 0,
                  sizeof(dtInfo));
    helicopter_M->SpecialInfo.mappingInfo = (&dtInfo);
    dtInfo.numDataTypes = 16;
    dtInfo.dataTypeSizes = &rtDataTypeSizes[0];
    dtInfo.dataTypeNames = &rtDataTypeNames[0];

    /* Block I/O transition table */
    dtInfo.BTransTable = &rtBTransTable;

    /* Parameters transition table */
    dtInfo.PTransTable = &rtPTransTable;
  }

  /* Initialize Sizes */
  helicopter_M->Sizes.numContStates = (4);/* Number of continuous states */
  helicopter_M->Sizes.numPeriodicContStates = (0);
                                      /* Number of periodic continuous states */
  helicopter_M->Sizes.numY = (0);      /* Number of model outputs */
  helicopter_M->Sizes.numU = (0);      /* Number of model inputs */
  helicopter_M->Sizes.sysDirFeedThru = (0);/* The model is not direct feedthrough */
  helicopter_M->Sizes.numSampTimes = (2);/* Number of sample times */
  helicopter_M->Sizes.numBlocks = (88);/* Number of blocks */
  helicopter_M->Sizes.numBlockIO = (21);/* Number of block outputs */
  helicopter_M->Sizes.numBlockPrms = (157);/* Sum of parameter "widths" */
  return helicopter_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/
