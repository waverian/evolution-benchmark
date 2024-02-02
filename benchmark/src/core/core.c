/************************************************************************
 * cc  lloops.c cpuidc.c -lm -O3 -o lloopsIL
 * ~/toolchain/raspbian-toolchain-gcc-4.7.2-linux32/bin/arm-linux-gnueabihf-gcc
 lloops.c cpuidc.c -lm -O3 -march=armv6 -mfloat-abi=hard -mfpu=vfp -o
 liverloopsPiA6
 *
 * L. L. N. L.  " C "  K E R N E L S:  M F L O P S  P C  V E R S I O N  *
 *
 *  #define Version not used
 *
 *  Different compilers can produce different floating point numeric
 *  results, probably due to compiling instructions in a different
 *  sequence. As the program checks these, they may need to be changed.
 *  The log file indicates non-standard results and these values can
 *  be copied and pasted into this program. See // COMPILER
 *  #define codes and function checkOut(). Some values are for
 *  optimised compiling and non-optimised results might be different.
 *
 *  Change #define options for print of optimisation level and

 * gcc  lloops2.c cpuidc.c -lm -lrt -O3 -mcpu=cortex-a7 -mfloat-abi=hard
 -mfpu=neon-vfpv4 -o liverloopsPiA7
 */

#define concat2(X, Y) X##_##Y
#define concat(X, Y) concat2(X, Y)
#define BENCHMARK_INCLUDE_OPTIMIZATION concat(benchmark_core, OPTIMIZATION)

#include "core.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "platform.h"

typedef int Boolean;

/* COMPILER for numeric results see function checkOut() */

/* #define WATCOM */
/* #define VISUALC */
/* #define GCCINTEL64 */
/* #define GCCINTEL32 */
/* #define CCINTEL32 */
/* #define GCCARMDP */
#define GCCARMPI

struct Arrays {
  double U[1001];
  double V[1001];
  double W[1001];
  double X[1001];
  double Y[1001];
  double Z[1024];
  double G[1001];
  double Du1[101];
  double Du2[101];
  double Du3[101];
  double Grd[1001];
  double Dex[1001];
  double Xi[1001];
  double Ex[1001];
  double Ex1[1001];
  double Dex1[1001];
  double Vx[1001];
  double Xx[1001];
  double Rx[1001];
  double Rh[2048];
  double Vsp[101];
  double Vstp[101];
  double Vxne[101];
  double Vxnd[101];
  double Ve3[101];
  double Vlr[101];
  double Vlin[101];
  double B5[101];
  double Plan[300];
  double D[300];
  double Sa[101];
  double Sb[101];
  double P[512][4];
  double Px[101][25];
  double Cx[101][25];
  double Vy[25][101];
  double Vh[7][101];
  double Vf[7][101];
  double Vg[7][101];
  double Vs[7][101];
  double Za[7][101];
  double Zp[7][101];
  double Zq[7][101];
  double Zr[7][101];
  double Zm[7][101];
  double Zb[7][101];
  double Zu[7][101];
  double Zv[7][101];
  double Zz[7][101];
  double B[64][64];
  double C[64][64];
  double H[64][64];
  double U1[2][101][5];
  double U2[2][101][5];
  double U3[2][101][5];
  double Xtra[40];
  long E[96];
  long F[96];
  long Ix[1001];
  long Ir[1001];
  long Zone[301];
  double X0[1001];
  double W0[1001];
  double Px0[101][25];
  double P0[512][4];
  double H0[64][64];
  double Rh0[2048];
  double Vxne0[101];
  double Zr0[7][101];
  double Zu0[7][101];
  double Zv0[7][101];
  double Zz0[7][101];
  double Za0[101][101]; /* was 101 25 */
  double Stb50;
  double Xx0;
};

#define u as1->U
#define v as1->V
#define w as1->W
#define x as1->X
#define y as1->Y
#define z as1->Z
#define g as1->G
#define du1 as1->Du1
#define du2 as1->Du2
#define du3 as1->Du3
#define grd as1->Grd
#define dex as1->Dex
#define xi as1->Xi
#define ex as1->Ex
#define ex1 as1->Ex1
#define dex1 as1->Dex1
#define vx as1->Vx
#define xx as1->Xx
#define rx as1->Rx
#define rh as1->Rh
#define vsp as1->Vsp
#define vstp as1->Vstp
#define vxne as1->Vxne
#define vxnd as1->Vxnd
#define ve3 as1->Ve3
#define vlr as1->Vlr
#define vlin as1->Vlin
#define b5 as1->B5
#define plan as1->Plan
#define d as1->D
#define sa as1->Sa
#define sb as1->Sb
#define p as1->P
#define px as1->Px
#define cx as1->Cx
#define vy as1->Vy
#define vh as1->Vh
#define vf as1->Vf
#define vg as1->Vg
#define vs as1->Vs
#define za as1->Za
#define zb as1->Zb
#define zp as1->Zp
#define zq as1->Zq
#define zr as1->Zr
#define zm as1->Zm
#define zz as1->Zz
#define zu as1->Zu
#define zv as1->Zv
#define b as1->B
#define c as1->C
#define h as1->H
#define u1 as1->U1
#define u2 as1->U2
#define u3 as1->U3
#define xtra as1->Xtra
#define a11 as1->Xtra[1]
#define a12 as1->Xtra[2]
#define a13 as1->Xtra[3]
#define a21 as1->Xtra[4]
#define a22 as1->Xtra[5]
#define a23 as1->Xtra[6]
#define a31 as1->Xtra[7]
#define a32 as1->Xtra[8]
#define a33 as1->Xtra[9]
#define c0 as1->Xtra[12]
#define dk as1->Xtra[15]
#define dm22 as1->Xtra[16]
#define dm23 as1->Xtra[17]
#define dm24 as1->Xtra[18]
#define dm25 as1->Xtra[19]
#define dm26 as1->Xtra[20]
#define dm27 as1->Xtra[21]
#define dm28 as1->Xtra[22]
#define expmax as1->Xtra[26]
#define flx as1->Xtra[27]
#define q as1->Xtra[28]
#define r as1->Xtra[30]
#define s as1->Xtra[32]
#define sig as1->Xtra[34]
#define stb5 as1->Xtra[35]
#define t as1->Xtra[36]
#define xnm as1->Xtra[39]
#define e as1->E
#define f as1->F
#define ix as1->Ix
#define ir as1->Ir
#define zone as1->Zone
#define x0 as1->X0
#define w0 as1->W0
#define px0 as1->Px0
#define p0 as1->P0
#define h0 as1->H0
#define rh0 as1->Rh0
#define vxne0 as1->Vxne0
#define zr0 as1->Zr0
#define zu0 as1->Zu0
#define zv0 as1->Zv0
#define zz0 as1->Zz0
#define za0 as1->Za0
#define stb50 as1->Stb50
#define xx0 as1->Xx0

struct Parameters {
  long Inner_loops;
  long Outer_loops;
  long Loop_mult;
  double Flops_per_loop;
  double Sumcheck[3][25];
  long Accuracy[3][25];
  double LoopTime[3][25];
  double LoopSpeed[3][25];
  double LoopFlos[3][25];
  long Xflops[25];
  long Xloops[3][25];
  long Nspan[3][25];
  double TimeStart;
  double TimeEnd;
  double Loopohead;
  long Count;
  long Count2;
  long Pass;
  long Extra_loops[3][25];
  long K2;
  long K3;
  long M16;
  long J5;
  long Section;
  long N16;
  double Mastersum;
  long M24;
  double Secs;
};

#define n as2->Inner_loops
#define loop as2->Outer_loops
#define mult as2->Loop_mult
#define nflops as2->Flops_per_loop
#define Checksum as2->Sumcheck
#define accuracy as2->Accuracy
#define RunTime as2->LoopTime
#define Mflops as2->LoopSpeed
#define FPops as2->LoopFlos
#define nspan as2->Nspan
#define xflops as2->Xflops
#define xloops as2->Xloops
#define StartTime as2->TimeStart
#define EndTime as2->TimeEnd
#define overhead_l as2->Loopohead
#define count as2->Count
#define count2 as2->Count2
#define pass as2->Pass
#define extra_loops as2->Extra_loops
#define k2 as2->K2
#define k3 as2->K3
#define m16 as2->M16
#define j5 as2->J5
#define section as2->Section
#define n16 as2->N16
#define MasterSum as2->Mastersum
#define m24 as2->M24
#define secs as2->Secs

struct RunParameters {
  double RunSecs;
  Boolean Reliability;
  Boolean RunRel;
  Boolean NsRes;
  double Sumscomp[3][25];
  int CompareFail;
  int FailCount;
};

#define runSecs as3->RunSecs
#define reliability as3->Reliability
#define runRel as3->RunRel
#define nsRes as3->NsRes
#define sumscomp as3->Sumscomp
#define compareFail as3->CompareFail
#define failCount as3->FailCount

#define TRUE 1
#define FALSE 0

static void init(struct Arrays *as1, struct Parameters *as2, long which);

/* Initialises arrays and variables  */

static long endloop(struct Arrays *as1, struct Parameters *as2, struct RunParameters *as3, long which);

/* Controls outer loops and stores results */

static long parameters(struct Arrays *as1, struct Parameters *as2, long which,
                       benchmark_core_progress_callback_handler_t *callback);

/* Gets loop parameters and variables, starts timer */

static void kernels(struct Arrays *as1, struct Parameters *as2, struct RunParameters *as3,
                    benchmark_core_progress_callback_handler_t *callback);

/* The 24 kernels */

static void check(struct Parameters *as2, long which);

/* Calculates checksum accuracy */

static void iqranf(struct Arrays *as1, struct Parameters *as2);

/* Random number generator for Kernel 14 */

static void checkOut(struct Parameters *as2, struct RunParameters *as3, int which);

static void set_progress(benchmark_core_progress_callback_handler_t *callback, int current_kernel, int current_run) {
  if (callback && callback->callback) {
    callback->callback(callback->data, current_kernel, current_run);
  }
}

void *BENCHMARK_INCLUDE_OPTIMIZATION(void *benchmark_args_as_void) {
  benchmark_core_args_t *benchmark_args = (benchmark_core_args_t *)benchmark_args_as_void;
  benchmark_core_progress_callback_handler_t *callback_handler = &benchmark_args->callback_handler;
  double pass_time, least, lmult; /*wt;*/
  long i, k, loop_passes;
  long mul[3] = {1, 2, 8};
  /* double weight[3] = {1.0, 2.0, 1.0};*/
  long which;
  /*  double maximum[4];
    double minimum[4];
    double average[4];
    double harmonic[4];
    double geometric[4];
    long xspan[4];*/

  struct Arrays *as1 = (struct Arrays *)calloc(1, sizeof(struct Arrays));

  struct Parameters *as2 = (struct Parameters *)calloc(1, sizeof(struct Parameters));

  struct RunParameters *as3 = (struct RunParameters *)calloc(1, sizeof(struct RunParameters));

  runSecs = benchmark_args->execution_time;
  reliability = FALSE;
  nsRes = FALSE;
  compareFail = 0;

  /************************************************************************
   *                  Open results file LLloops.txt                       *
   ************************************************************************/
  /*
      outfile = fopen("LLloops.txt", "a+");
      if (outfile == NULL) {
          exit(0);
      }
  */

  /************************************************************************
   *       Calculate overhead of executing endloop procedure              *
   ************************************************************************/

  pass = -20;
  extra_loops[0][0] = 1;
  loop = 1000;
  which = 0;
  section = 0;
  runRel = FALSE;
  do {
    secs = thread_clock();
    count = 0;
    count2 = 0;
    pass = pass + 1;
    do {
      endloop(as1, as2, as3, 0);
    } while (count < loop);

    overhead_l = thread_clock() - secs;
    if (overhead_l > 0.2) {
      pass = 0;
    }
    if (pass < 0) {
      if (overhead_l < (double)runSecs / 50) {
        loop = loop * 10;
      } else {
        loop = loop * 2;
      }
    }
  } while (pass < 0);

  overhead_l = overhead_l / (double)(loop);

  /************************************************************************
   *      Execute the kernels three times at different Do Spans           *
   ************************************************************************/
  for (section = 0; section < 3; section++) {
    loop_passes = 200 * mul[section];
    pass = -20;
    mult = 2 * mul[section];
    runRel = FALSE;

    for (i = 1; i < 25; i++) {
      extra_loops[section][i] = 1;
    }

    /************************************************************************
     *   Calculate extra loops for running time of runSecs seconds per kernel
     **
     ************************************************************************/

    do
    /* Run a number of times with increased number of loops
 or until the time for each loop is at least 0.001 seconds */

    {
      pass = pass + 1;
      mult = mult * 2;

      count2 = 0;
      for (i = 1; i < 25; i++) {
        RunTime[section][i] = 0.0;
      }
      secs = thread_clock();

      kernels(as1, as2, as3, NULL);

      pass_time = thread_clock() - secs;
      least = 1.0;
      for (i = 1; i < 25; i++) {
        if (RunTime[section][i] < 0.001) {
          least = 0.0;
          RunTime[section][i] = 0.0008;
          extra_loops[section][i] = extra_loops[section][i] * 2;
        }
      }

      if (least > 0.0) {
        pass = 0;
      } else {
        if (pass_time < (double)runSecs / 5) {
          mult = mult * 2;
        }
      }
    } while (pass < 0);

    lmult = (double)(mult) / (double)(loop_passes);

    for (i = 1; i < 25; i++) {
      /* Calculate extra loops to produce a run time of about runSecs
     seconds for each kernel. For each of the extra loops the parameters
     are re-initialised. The time for initialising parameters is
     not included in the loop time. */

      extra_loops[section][i] = (long)(runSecs / RunTime[section][i] * (double)extra_loops[section][i] * lmult) + 1;
      RunTime[section][i] = 0.0;
    }

    mult = loop_passes;

    pass = 1;
    count2 = 0;
    if (reliability) runRel = TRUE;

    /************************************************************************
     *                      Execute the kernels                             *
     ************************************************************************/

    kernels(as1, as2, as3, callback_handler);
    /*
    maximum[section] = 0.0;
    minimum[section] = Mflops[section][1];
    average[section] = 0.0;
    harmonic[section] = 0.0;
    geometric[section] = 0.0;
    xspan[section] = 0;
    */
    /************************************************************************
     *                        Calculate averages etc.                       *
     ************************************************************************/
    /*
    for (k = 1; k <= 24; k++) {
      average[section] = average[section] + Mflops[section][k];
      harmonic[section] = harmonic[section] + 1 / Mflops[section][k];
      geometric[section] = geometric[section] + log(Mflops[section][k]);
      xspan[section] = xspan[section] + nspan[section][k];
      if (Mflops[section][k] < minimum[section]) {
        minimum[section] = Mflops[section][k];
      }
      if (Mflops[section][k] > maximum[section]) {
        maximum[section] = Mflops[section][k];
      }
    }
    average[section] = average[section] / 24.0;
    harmonic[section] = 24.0 / harmonic[section];
    geometric[section] = exp(geometric[section] / 24.0);
    xspan[section] = xspan[section] / 24;
     */
  }

  /************************************************************************
   *   Waverian: Calculate MFlops average per kernel                      *
   ************************************************************************/

  for (k = 1; k <= 24; k++) {
    Mflops[0][k] = (Mflops[0][k] + Mflops[1][k] + Mflops[2][k]) / 3;
  }

  /************************************************************************
   *    End of executing the kernels three times at different Do Spans    *
   ************************************************************************/

  /* maximum[0] = 0.0;
   minimum[0] = Mflops[0][1];
   average[0] = 0.0;
   harmonic[0] = 0.0;
   geometric[0] = 0.0;
   xspan[3] = 0;
   wt = 0.0;*/

  /************************************************************************
   *   Waverian: Calculate weighted averages for all Do Spans and display *
   ************************************************************************/

  /************************************************************************
   *     Calculate weighted averages for all Do Spans and display         *
   ************************************************************************/
  /*
  for (section = 0; section < 3; section++) {
    for (k = 1; k <= 24; k++) {
      average[3] = average[3] + weight[section] * Mflops[section][k];
      harmonic[3] = harmonic[3] + weight[section] / Mflops[section][k];
      geometric[3] = geometric[3] + weight[section] * log(Mflops[section][k]);
      xspan[3] = xspan[3] + (long)weight[section] * nspan[section][k];
      if (Mflops[section][k] < minimum[3]) {
        minimum[3] = Mflops[section][k];
      }
      if (Mflops[section][k] > maximum[3]) {
        maximum[3] = Mflops[section][k];
      }
    }
    wt = wt + weight[section];
  }
  average[3] = average[3] / (24.0 * wt);
  harmonic[3] = 24.0 * wt / harmonic[3];
  geometric[3] = exp(geometric[3] / (24.0 * wt));
  xspan[3] = (long)((double)xspan[3] / (24.0 * wt));


      if (reliability) {
          if (!compareFail) {
              if (nsRes) {
                  fprintf(outfile, "\n Numeric results were consistent with
     first\n\n"); } else { fprintf(outfile, " Numeric results were as
     expected\n\n");
              }
          } else {
              printf("\n ERRORS have occurred - see log file\n");
              fprintf(outfile, "\n");
          }
      }
      */
  /************************************************************************
   *              Add results to output file LLloops.txt                  *
   ************************************************************************/

  for (which = 1; which < 25; which++) {
    benchmark_args->results.kernels[which - 1] = Mflops[0][which];
  }

  if (!reliability) {
    checkOut(as2, as3, which);
  }
  /*
      fflush(outfile);

      fclose(outfile);
  */
  free(as1);
  free(as2);
  free(as3);

  return NULL;
}

/************************************************************************
 *                          The Kernels                                 *
 ************************************************************************/

static void kernels(struct Arrays *as1, struct Parameters *as2, struct RunParameters *as3,
                    benchmark_core_progress_callback_handler_t *callback) {
  long lw;
  long ipnt, ipntp, ii;
  double temp;
  long nl1, nl2;
  long kx, ky;
  double ar, br, cr;
  long i, j, k, m;
  long ip, i1, i2, j1, j2, j4, lb;
  long ng, nz;
  double tmp;
  double scale, xnei, xnc, e3, e6;
  long ink, jn, kn, kb5i;
  double di, dn;
  double qa;

  for (k = 0; k < 25; k++) {
    Checksum[section][k] = 0.0;
  }

  /*
   *******************************************************************
   *   Kernel 1 -- hydro fragment
   *******************************************************************
   */

  parameters(as1, as2, 1, callback);

  do {
    for (k = 0; k < n; k++) {
      x[k] = q + y[k] * (r * z[k + 10] + t * z[k + 11]);
    }

    endloop(as1, as2, as3, 1);
  } while (count < loop);

  /*
   *******************************************************************
   *   Kernel 2 -- ICCG excerpt (Incomplete Cholesky Conjugate Gradient)
   *******************************************************************
   */

  parameters(as1, as2, 2, callback);

  do {
    ii = n;
    ipntp = 0;
    do {
      ipnt = ipntp;
      ipntp += ii;
      ii /= 2;
      i = ipntp;
      for (k = ipnt + 1; k < ipntp; k = k + 2) {
        i++;
        x[i] = x[k] - v[k] * x[k - 1] - v[k + 1] * x[k + 1];
      }
    } while (ii > 0);

    endloop(as1, as2, as3, 2);
  } while (count < loop);

  /*
   *******************************************************************
   *   Kernel 3 -- inner product
   *******************************************************************
   */

  parameters(as1, as2, 3, callback);

  do {
    q = 0.0;
    for (k = 0; k < n; k++) {
      q += z[k] * x[k];
    }

    endloop(as1, as2, as3, 3);
  } while (count < loop);

  /*
   *******************************************************************
   *   Kernel 4 -- banded linear equations

   *******************************************************************
    */

  parameters(as1, as2, 4, callback);

  m = (1001 - 201) / 2;
  do {
    for (k = 6; k < 1001; k = k + m) {
      lw = k - 6;
      temp = x[k - 1];

      for (j = 4; j < n; j = j + 5) {
        // Index 1001 out of bounds for type 'double[1001]'
        temp -= x[lw] * y[j];
        lw++;
      }
      x[k - 1] = y[4] * temp;
    }

    endloop(as1, as2, as3, 4);
  } while (count < loop);

  /*
   *******************************************************************
   *   Kernel 5 -- tri-diagonal elimination, below diagonal
   *******************************************************************
   */

  parameters(as1, as2, 5, callback);

  do {
    for (i = 1; i < n; i++) {
      x[i] = z[i] * (y[i] - x[i - 1]);
    }

    endloop(as1, as2, as3, 5);
  } while (count < loop);

  /*
   *******************************************************************
   *   Kernel 6 -- general linear recurrence equations
   *******************************************************************
   */

  parameters(as1, as2, 6, callback);

  do {
    for (i = 1; i < n; i++) {
      w[i] = 0.01;
      for (k = 0; k < i; k++) {
        w[i] += b[k][i] * w[(i - k) - 1];
      }
    }

    endloop(as1, as2, as3, 6);
  } while (count < loop);

  /*
   *******************************************************************
   *   Kernel 7 -- equation of state fragment
   *******************************************************************
   */

  parameters(as1, as2, 7, callback);

  do {
    for (k = 0; k < n; k++) {
      x[k] = u[k] + r * (z[k] + r * y[k]) +
             t * (u[k + 3] + r * (u[k + 2] + r * u[k + 1]) + t * (u[k + 6] + q * (u[k + 5] + q * u[k + 4])));
    }

    endloop(as1, as2, as3, 7);
  } while (count < loop);

  /*
   *******************************************************************
   *   Kernel 8 -- ADI integration
   *******************************************************************
   */

  nl1 = 0;
  nl2 = 1;

  parameters(as1, as2, 8, callback);

  do {
    for (kx = 1; kx < 3; kx++) {
      for (ky = 1; ky < n; ky++) {
        du1[ky] = u1[nl1][ky + 1][kx] - u1[nl1][ky - 1][kx];
        du2[ky] = u2[nl1][ky + 1][kx] - u2[nl1][ky - 1][kx];
        du3[ky] = u3[nl1][ky + 1][kx] - u3[nl1][ky - 1][kx];
        u1[nl2][ky][kx] = u1[nl1][ky][kx] + a11 * du1[ky] + a12 * du2[ky] + a13 * du3[ky] +
                          sig * (u1[nl1][ky][kx + 1] - 2.0 * u1[nl1][ky][kx] + u1[nl1][ky][kx - 1]);
        u2[nl2][ky][kx] = u2[nl1][ky][kx] + a21 * du1[ky] + a22 * du2[ky] + a23 * du3[ky] +
                          sig * (u2[nl1][ky][kx + 1] - 2.0 * u2[nl1][ky][kx] + u2[nl1][ky][kx - 1]);
        u3[nl2][ky][kx] = u3[nl1][ky][kx] + a31 * du1[ky] + a32 * du2[ky] + a33 * du3[ky] +
                          sig * (u3[nl1][ky][kx + 1] - 2.0 * u3[nl1][ky][kx] + u3[nl1][ky][kx - 1]);
      }
    }

    endloop(as1, as2, as3, 8);
  } while (count < loop);

  /*
   *******************************************************************
   *   Kernel 9 -- integrate predictors
   *******************************************************************
   */

  parameters(as1, as2, 9, callback);

  do {
    for (i = 0; i < n; i++) {
      px[i][0] = dm28 * px[i][12] + dm27 * px[i][11] + dm26 * px[i][10] + dm25 * px[i][9] + dm24 * px[i][8] +
                 dm23 * px[i][7] + dm22 * px[i][6] + c0 * (px[i][4] + px[i][5]) + px[i][2];
    }

    endloop(as1, as2, as3, 9);
  } while (count < loop);

  /*
   *******************************************************************
   *   Kernel 10 -- difference predictors
   *******************************************************************
   */

  parameters(as1, as2, 10, callback);

  do {
    for (i = 0; i < n; i++) {
      ar = cx[i][4];
      br = ar - px[i][4];
      px[i][4] = ar;
      cr = br - px[i][5];
      px[i][5] = br;
      ar = cr - px[i][6];
      px[i][6] = cr;
      br = ar - px[i][7];
      px[i][7] = ar;
      cr = br - px[i][8];
      px[i][8] = br;
      ar = cr - px[i][9];
      px[i][9] = cr;
      br = ar - px[i][10];
      px[i][10] = ar;
      cr = br - px[i][11];
      px[i][11] = br;
      px[i][13] = cr - px[i][12];
      px[i][12] = cr;
    }

    endloop(as1, as2, as3, 10);
  } while (count < loop);

  /*
   *******************************************************************
   *   Kernel 11 -- first sum
   *******************************************************************
   */

  parameters(as1, as2, 11, callback);

  do {
    x[0] = y[0];
    for (k = 1; k < n; k++) {
      x[k] = x[k - 1] + y[k];
    }

    endloop(as1, as2, as3, 11);
  } while (count < loop);

  /*
   *******************************************************************
   *   Kernel 12 -- first difference
   *******************************************************************
   */

  parameters(as1, as2, 12, callback);

  do {
    for (k = 0; k < n; k++) {
      x[k] = y[k + 1] - y[k];
    }

    endloop(as1, as2, as3, 12);
  } while (count < loop);

  /*
   *******************************************************************
   *   Kernel 13 -- 2-D PIC (Particle In Cell)
   *******************************************************************
   */

  parameters(as1, as2, 13, callback);

  do {
    for (ip = 0; ip < n; ip++) {
      i1 = (long)p[ip][0];
      j1 = (long)p[ip][1];
      i1 &= 64 - 1;
      j1 &= 64 - 1;
      p[ip][2] += b[j1][i1];
      p[ip][3] += c[j1][i1];
      p[ip][0] += p[ip][2];
      p[ip][1] += p[ip][3];
      i2 = (long)p[ip][0];
      j2 = (long)p[ip][1];
      i2 = (i2 & (64 - 1)) - 1;
      j2 = (j2 & (64 - 1)) - 1;
      p[ip][0] += y[i2 + 32];
      p[ip][1] += z[j2 + 32];
      i2 += e[i2 + 32];
      j2 += f[j2 + 32];
      // Index -1 out of bounds for type 'double[64]'
      h[j2][i2] += 1.0;
    }
    endloop(as1, as2, as3, 13);
  } while (count < loop);

  /*
   *******************************************************************
   *   Kernel 14 -- 1-D PIC (Particle In Cell)
   *******************************************************************
   */

  parameters(as1, as2, 14, callback);

  do {
    for (k = 0; k < n; k++) {
      vx[k] = 0.0;
      xx[k] = 0.0;
      ix[k] = (long)grd[k];
      xi[k] = (double)ix[k];
      ex1[k] = ex[ix[k] - 1];
      dex1[k] = dex[ix[k] - 1];
    }
    for (k = 0; k < n; k++) {
      vx[k] = vx[k] + ex1[k] + (xx[k] - xi[k]) * dex1[k];
      xx[k] = xx[k] + vx[k] + flx;
      ir[k] = (long)xx[k];
      rx[k] = xx[k] - ir[k];
      ir[k] = (ir[k] & (2048 - 1)) + 1;
      xx[k] = rx[k] + ir[k];
    }
    for (k = 0; k < n; k++) {
      //      Index -1 out of bounds for type 'double[2048]'
      rh[ir[k] - 1] += 1.0 - rx[k];
      //      Index 2048 out of bounds for type 'double[2048]'
      rh[ir[k]] += rx[k];
    }
    endloop(as1, as2, as3, 14);
  } while (count < loop);

  /*
   *******************************************************************
   *   Kernel 15 -- Casual Fortran.  Development version
   *******************************************************************
   */

  parameters(as1, as2, 15, callback);

  do {
    ng = 7;
    nz = n;
    ar = 0.053;
    br = 0.073;
    for (j = 1; j < ng; j++) {
      for (k = 1; k < nz; k++) {
        if ((j + 1) >= ng) {
          vy[j][k] = 0.0;
          continue;
        }
        if (vh[j + 1][k] > vh[j][k]) {
          t = ar;
        } else {
          t = br;
        }
        if (vf[j][k] < vf[j][k - 1]) {
          if (vh[j][k - 1] > vh[j + 1][k - 1])
            r = vh[j][k - 1];
          else
            r = vh[j + 1][k - 1];
          s = vf[j][k - 1];
        } else {
          if (vh[j][k] > vh[j + 1][k])
            r = vh[j][k];
          else
            r = vh[j + 1][k];
          s = vf[j][k];
        }
        vy[j][k] = sqrt(vg[j][k] * vg[j][k] + r * r) * t / s;
        if ((k + 1) >= nz) {
          vs[j][k] = 0.0;
          continue;
        }
        if (vf[j][k] < vf[j - 1][k]) {
          if (vg[j - 1][k] > vg[j - 1][k + 1])
            r = vg[j - 1][k];
          else
            r = vg[j - 1][k + 1];
          s = vf[j - 1][k];
          t = br;
        } else {
          if (vg[j][k] > vg[j][k + 1])
            r = vg[j][k];
          else
            r = vg[j][k + 1];
          s = vf[j][k];
          t = ar;
        }
        vs[j][k] = sqrt(vh[j][k] * vh[j][k] + r * r) * t / s;
      }
    }
    endloop(as1, as2, as3, 15);
  } while (count < loop);

  /*
   *******************************************************************
   *   Kernel 16 -- Monte Carlo search loop
   *******************************************************************
   */

  parameters(as1, as2, 16, callback);

  ii = n / 3;
  lb = ii + ii;
  k3 = k2 = 0;
  do {
    i1 = m16 = 1;
  label410:
    j2 = (n + n) * (m16 - 1) + 1;
    for (k = 1; k <= n; k++) {
      k2++;
      j4 = j2 + k + k;
      j5 = zone[j4 - 1];
      if (j5 < n) {
        if (j5 + lb < n) {        /* 420 */
          tmp = plan[j5 - 1] - t; /* 435 */
        } else {
          if (j5 + ii < n) {        /* 415 */
            tmp = plan[j5 - 1] - s; /* 430 */
          } else {
            tmp = plan[j5 - 1] - r; /* 425 */
          }
        }
      } else if (j5 == n) {
        break; /* 475 */
      } else {
        k3++; /* 450 */
        tmp = (d[j5 - 1] - (d[j5 - 2] * (t - d[j5 - 3]) * (t - d[j5 - 3]) + (s - d[j5 - 4]) * (s - d[j5 - 4]) +
                            (r - d[j5 - 5]) * (r - d[j5 - 5])));
      }
      if (tmp < 0.0) {
        if (zone[j4 - 2] < 0) /* 445 */
          continue;           /* 470 */
        else if (!zone[j4 - 2])
          break; /* 480 */
      } else if (tmp) {
        if (zone[j4 - 2] > 0) /* 440 */
          continue;           /* 470 */
        else if (!zone[j4 - 2])
          break; /* 480 */
      } else
        break;                    /* 485 */
      m16++;                      /* 455 */
      if (m16 > zone[0]) m16 = 1; /* 460 */
      if (i1 - m16)               /* 465 */
        goto label410;
      else
        break;
    }
    endloop(as1, as2, as3, 16);
  } while (count < loop);

  /*
   *******************************************************************
   *   Kernel 17 -- implicit, conditional computation
   *******************************************************************
   */

  parameters(as1, as2, 17, callback);

  do {
    i = n - 1;
    j = 0;
    ink = -1;
    scale = 5.0 / 3.0;
    xnm = 1.0 / 3.0;
    e6 = 1.03 / 3.07;
    goto l61;
  l60:
    e6 = xnm * vsp[i] + vstp[i];
    vxne[i] = e6;
    xnm = e6;
    ve3[i] = e6;
    i += ink;
    if (i == j) goto l62;
  l61:
    e3 = xnm * vlr[i] + vlin[i];
    xnei = vxne[i];
    vxnd[i] = e6;
    xnc = scale * e3;
    if (xnm > xnc) goto l60;
    if (xnei > xnc) goto l60;
    ve3[i] = e3;
    e6 = e3 + e3 - xnm;
    vxne[i] = e3 + e3 - xnei;
    xnm = e6;
    i += ink;
    if (i != j) goto l61;
  l62:;
    endloop(as1, as2, as3, 17);
  } while (count < loop);

  /*
   *******************************************************************
   *   Kernel 18 - 2-D explicit hydrodynamics fragment
   *******************************************************************
   */

  parameters(as1, as2, 18, callback);

  do {
    t = 0.0037;
    s = 0.0041;
    kn = 6;
    jn = n;
    for (k = 1; k < kn; k++) {
      for (j = 1; j < jn; j++) {
        za[k][j] = (zp[k + 1][j - 1] + zq[k + 1][j - 1] - zp[k][j - 1] - zq[k][j - 1]) * (zr[k][j] + zr[k][j - 1]) /
                   (zm[k][j - 1] + zm[k + 1][j - 1]);
        zb[k][j] =
            (zp[k][j - 1] + zq[k][j - 1] - zp[k][j] - zq[k][j]) * (zr[k][j] + zr[k - 1][j]) / (zm[k][j] + zm[k][j - 1]);
      }
    }
    for (k = 1; k < kn; k++) {
      for (j = 1; j < jn; j++) {
        zu[k][j] += s * (za[k][j] * (zz[k][j] - zz[k][j + 1]) - za[k][j - 1] * (zz[k][j] - zz[k][j - 1]) -
                         zb[k][j] * (zz[k][j] - zz[k - 1][j]) + zb[k + 1][j] * (zz[k][j] - zz[k + 1][j]));
        zv[k][j] += s * (za[k][j] * (zr[k][j] - zr[k][j + 1]) - za[k][j - 1] * (zr[k][j] - zr[k][j - 1]) -
                         zb[k][j] * (zr[k][j] - zr[k - 1][j]) + zb[k + 1][j] * (zr[k][j] - zr[k + 1][j]));
      }
    }
    for (k = 1; k < kn; k++) {
      for (j = 1; j < jn; j++) {
        zr[k][j] = zr[k][j] + t * zu[k][j];
        zz[k][j] = zz[k][j] + t * zv[k][j];
      }
    }
    endloop(as1, as2, as3, 18);
  } while (count < loop);

  /*
   *******************************************************************
   *   Kernel 19 -- general linear recurrence equations
   *******************************************************************
   */

  parameters(as1, as2, 19, callback);

  kb5i = 0;

  do {
    for (k = 0; k < n; k++) {
      b5[k + kb5i] = sa[k] + stb5 * sb[k];
      stb5 = b5[k + kb5i] - stb5;
    }
    for (i = 1; i <= n; i++) {
      k = n - i;
      b5[k + kb5i] = sa[k] + stb5 * sb[k];
      stb5 = b5[k + kb5i] - stb5;
    }
    endloop(as1, as2, as3, 19);
  } while (count < loop);

  /*
   *******************************************************************
   * Kernel 20 - Discrete ordinates transport, conditional recurrence on xx
   *******************************************************************
   */

  parameters(as1, as2, 20, callback);

  do {
    for (k = 0; k < n; k++) {
      di = y[k] - g[k] / (xx[k] + dk);
      dn = 0.2;

      if (di) {
        dn = z[k] / di;
        if (t < dn) dn = t;
        if (s > dn) dn = s;
      }
      x[k] = ((w[k] + v[k] * dn) * xx[k] + u[k]) / (vx[k] + v[k] * dn);
      xx[k + 1] = (x[k] - xx[k]) * dn + xx[k];
    }
    endloop(as1, as2, as3, 20);
  } while (count < loop);

  /*
   *******************************************************************
   *   Kernel 21 -- matrix*matrix product
   *******************************************************************
   */

  parameters(as1, as2, 21, callback);

  do {
    for (k = 0; k < 25; k++) {
      for (i = 0; i < 25; i++) {
        for (j = 0; j < n; j++) {
          px[j][i] += vy[k][i] * cx[j][k];
        }
      }
    }
    endloop(as1, as2, as3, 21);
  } while (count < loop);

  /*
   *******************************************************************
   *   Kernel 22 -- Planckian distribution
   *******************************************************************
   */

  parameters(as1, as2, 22, callback);

  expmax = 20.0;
  u[n - 1] = 0.99 * expmax * v[n - 1];
  do {
    for (k = 0; k < n; k++) {
      y[k] = u[k] / v[k];
      w[k] = x[k] / (exp(y[k]) - 1.0);
    }
    endloop(as1, as2, as3, 22);
  } while (count < loop);

  /*
   *******************************************************************
   *   Kernel 23 -- 2-D implicit hydrodynamics fragment
   *******************************************************************
   */

  parameters(as1, as2, 23, callback);

  do {
    for (j = 1; j < 6; j++) {
      for (k = 1; k < n; k++) {
        qa = za[j + 1][k] * zr[j][k] + za[j - 1][k] * zb[j][k] + za[j][k + 1] * zu[j][k] + za[j][k - 1] * zv[j][k] +
             zz[j][k];
        za[j][k] += 0.175 * (qa - za[j][k]);
      }
    }
    endloop(as1, as2, as3, 23);
  } while (count < loop);

  /*
   *******************************************************************
   *   Kernel 24 -- find location of first minimum in array
   *******************************************************************
   */

  parameters(as1, as2, 24, callback);

  x[n / 2] = -1.0e+10;
  do {
    m24 = 0;
    for (k = 1; k < n; k++) {
      if (x[k] < x[m24]) m24 = k;
    }
    endloop(as1, as2, as3, 24);
  } while (count < loop);

  return;
} /* kernels */

/************************************************************************
 *        endloop procedure - calculate checksums and MFLOPS            *
 ************************************************************************/

static long endloop(struct Arrays *as1, struct Parameters *as2, struct RunParameters *as3, long which) {
  double useflops = 0;
  long i, j, k, m;
  double Scale = 1000000.0;
  Boolean reinit = TRUE;
  Boolean getend = FALSE;

  count = count + 1;
  if (count >= loop) /* else return */
  {
    /************************************************************************
     *               End of standard set of loops for one kernel            *
     ************************************************************************/

    count2 = count2 + 1;
    if (count2 == extra_loops[section][which]) getend = TRUE;
    if (count2 == extra_loops[section][which] || runRel)
    /* else re-initialise parameters if required */
    {
      reinit = FALSE;

      /************************************************************************
       *           End of extra loops for runSecs seconds execution time *
       ************************************************************************/

      Checksum[section][which] = 0;
      if (which == 1) {
        for (k = 0; k < n; k++) {
          Checksum[section][1] = Checksum[section][1] + x[k] * (double)(k + 1);
        }
        useflops = nflops * (double)(n * loop);
      }
      if (which == 2) {
        for (k = 0; k < n * 2; k++) {
          Checksum[section][2] = Checksum[section][2] + x[k] * (double)(k + 1);
        }
        useflops = nflops * (double)((n - 4) * loop);
      }
      if (which == 3) {
        Checksum[section][3] = q;
        useflops = nflops * (double)(n * loop);
      }
      if (which == 4) {
        for (k = 0; k < 3; k++) {
          Checksum[section][4] = Checksum[section][4] + v[k] * (double)(k + 1);
        }
        useflops = nflops * (double)((((n - 5) / 5) + 1) * 3 * loop);
      }
      if (which == 5) {
        for (k = 1; k < n; k++) {
          Checksum[section][5] = Checksum[section][5] + x[k] * (double)(k);
        }
        useflops = nflops * (double)((n - 1) * loop);
      }
      if (which == 6) {
        for (k = 0; k < n; k++) {
          Checksum[section][6] = Checksum[section][6] + w[k] * (double)(k + 1);
        }
        useflops = nflops * (double)(n * ((n - 1) / 2) * loop);
      }
      if (which == 7) {
        for (k = 0; k < n; k++) {
          Checksum[section][7] = Checksum[section][7] + x[k] * (double)(k + 1);
        }
        useflops = nflops * (double)(n * loop);
      }
      if (which == 8) {
        for (i = 0; i < 2; i++) {
          for (j = 0; j < 101; j++) {
            for (k = 0; k < 5; k++) {
              m = 101 * 5 * i + 5 * j + k + 1;
              if (m < 10 * n + 1) {
                Checksum[section][8] = Checksum[section][8] + u1[i][j][k] * m + u2[i][j][k] * m + u3[i][j][k] * m;
              }
            }
          }
        }
        useflops = nflops * (double)(2 * (n - 1) * loop);
      }
      if (which == 9) {
        for (i = 0; i < n; i++) {
          for (j = 0; j < 25; j++) {
            m = 25 * i + j + 1;
            if (m < 15 * n + 1) {
              Checksum[section][9] = Checksum[section][9] + px[i][j] * (double)(m);
            }
          }
        }
        useflops = nflops * (double)(n * loop);
      }
      if (which == 10) {
        for (i = 0; i < n; i++) {
          for (j = 0; j < 25; j++) {
            m = 25 * i + j + 1;
            if (m < 15 * n + 1) {
              Checksum[section][10] = Checksum[section][10] + px[i][j] * (double)(m);
            }
          }
        }
        useflops = nflops * (double)(n * loop);
      }
      if (which == 11) {
        for (k = 1; k < n; k++) {
          Checksum[section][11] = Checksum[section][11] + x[k] * (double)(k);
        }
        useflops = nflops * (double)((n - 1) * loop);
      }
      if (which == 12) {
        for (k = 0; k < n - 1; k++) {
          Checksum[section][12] = Checksum[section][12] + x[k] * (double)(k + 1);
        }
        useflops = nflops * (double)(n * loop);
      }
      if (which == 13) {
        for (k = 0; k < 2 * n; k++) {
          for (j = 0; j < 4; j++) {
            m = 4 * k + j + 1;
            Checksum[section][13] = Checksum[section][13] + p[k][j] * (double)(m);
          }
        }
        for (i = 0; i < 8 * n / 64; i++) {
          for (j = 0; j < 64; j++) {
            m = 64 * i + j + 1;
            if (m < 8 * n + 1) {
              Checksum[section][13] = Checksum[section][13] + h[i][j] * (double)(m);
            }
          }
        }
        useflops = nflops * (double)(n * loop);
      }
      if (which == 14) {
        for (k = 0; k < n; k++) {
          Checksum[section][14] = Checksum[section][14] + (xx[k] + vx[k]) * (double)(k + 1);
        }
        for (k = 0; k < 67; k++) {
          Checksum[section][14] = Checksum[section][14] + rh[k] * (double)(k + 1);
        }
        useflops = nflops * (double)(n * loop);
      }
      if (which == 15) {
        for (j = 0; j < 7; j++) {
          for (k = 0; k < 101; k++) {
            m = 101 * j + k + 1;
            if (m < n * 7 + 1) {
              Checksum[section][15] = Checksum[section][15] + (vs[j][k] + vy[j][k]) * (double)(m);
            }
          }
        }
        useflops = nflops * (double)((n - 1) * 5 * loop);
      }
      if (which == 16) {
        Checksum[section][16] = (double)(k3 + k2 + j5 + m16);
        useflops = (k2 + k2 + 10 * k3);
      }
      if (which == 17) {
        Checksum[section][17] = xnm;
        for (k = 0; k < n; k++) {
          Checksum[section][17] = Checksum[section][17] + (vxne[k] + vxnd[k]) * (double)(k + 1);
        }
        useflops = nflops * (double)(n * loop);
      }
      if (which == 18) {
        for (k = 0; k < 7; k++) {
          for (j = 0; j < 101; j++) {
            m = 101 * k + j + 1;
            if (m < 7 * n + 1) {
              Checksum[section][18] = Checksum[section][18] + (zz[k][j] + zr[k][j]) * (double)(m);
            }
          }
        }
        useflops = nflops * (double)((n - 1) * 5 * loop);
      }
      if (which == 19) {
        Checksum[section][19] = stb5;
        for (k = 0; k < n; k++) {
          Checksum[section][19] = Checksum[section][19] + b5[k] * (double)(k + 1);
        }
        useflops = nflops * (double)(n * loop);
      }
      if (which == 20) {
        for (k = 1; k < n + 1; k++) {
          Checksum[section][20] = Checksum[section][20] + xx[k] * (double)(k);
        }
        useflops = nflops * (double)(n * loop);
      }
      if (which == 21) {
        for (k = 0; k < n; k++) {
          for (i = 0; i < 25; i++) {
            m = 25 * k + i + 1;
            Checksum[section][21] = Checksum[section][21] + px[k][i] * (double)(m);
          }
        }
        useflops = nflops * (double)(n * 625 * loop);
      }
      if (which == 22) {
        for (k = 0; k < n; k++) {
          Checksum[section][22] = Checksum[section][22] + w[k] * (double)(k + 1);
        }
        useflops = nflops * (double)(n * loop);
      }
      if (which == 23) {
        for (j = 0; j < 7; j++) {
          for (k = 0; k < 101; k++) {
            m = 101 * j + k + 1;
            if (m < 7 * n + 1) {
              Checksum[section][23] = Checksum[section][23] + za[j][k] * (double)(m);
            }
          }
        }
        useflops = nflops * (double)((n - 1) * 5 * loop);
      }
      if (which == 24) {
        Checksum[section][24] = (double)(m24);
        useflops = nflops * (double)((n - 1) * loop);
      }
      if (runRel) checkOut(as2, as3, which);
      if (getend) {
        /************************************************************************
         *                           End of timing *
         ************************************************************************/
        count2 = 0;

        RunTime[section][which] = thread_clock() - secs;

        /************************************************************************
         *     Deduct overheads from time, calculate MFLOPS, display
         *results *
         ************************************************************************/

        RunTime[section][which] = RunTime[section][which] - (loop * extra_loops[section][which]) * overhead_l;
        FPops[section][which] = useflops * extra_loops[section][which];
        Mflops[section][which] = FPops[section][which] / Scale / RunTime[section][which];
        if (pass > 0) {
          /************************************************************************
           *      Compare sumcheck with standard result, calculate
           *accuracy *
           ************************************************************************/

          check(as2, which);

          if (reliability) {
            if (compareFail) {
              printf(" ERRORS have occurred - see log file\n");
            }
          }
        }
      }
    }
    if (reinit || (runRel && !getend)) {
      /************************************************************************
       *                     Re-initialise data if reqired *
       ************************************************************************/

      count = 0;
      if (which == 2) {
        for (k = 0; k < n; k++) {
          x[k] = x0[k];
        }
      }
      if (which == 4) {
        m = (1001 - 7) / 2;
        for (k = 6; k < 1001; k = k + m) {
          x[k] = x0[k];
        }
      }
      if (which == 5) {
        for (k = 0; k < n; k++) {
          x[k] = x0[k];
        }
      }
      if (which == 6) {
        for (k = 0; k < n; k++) {
          w[k] = w0[k];
        }
      }
      if (which == 10) {
        for (i = 0; i < n; i++) {
          for (j = 4; j < 13; j++) {
            px[i][j] = px0[i][j];
          }
        }
      }
      if (which == 13) {
        for (i = 0; i < n; i++) {
          for (j = 0; j < 4; j++) {
            p[i][j] = p0[i][j];
          }
        }
        for (i = 0; i < 64; i++) {
          for (j = 0; j < 64; j++) {
            h[i][j] = h0[i][j];
          }
        }
      }
      if (which == 14) {
        for (i = 0; i < n; i++) {
          // Index -1 out of bounds for type 'double[2048]'
          rh[ir[i] - 1] = rh0[ir[i] - 1];
          // Index 2048 out of bounds for type 'double[2048]'
          rh[ir[i]] = rh0[ir[i]];
        }
      }
      if (which == 17) {
        //        Index 2048 out of bounds for type 'double[2048]'
        for (i = 0; i < n; i++) {
          vxne[i] = vxne0[i];
        }
      }
      if (which == 18) {
        for (i = 1; i < 6; i++) {
          for (j = 1; j < n; j++) {
            zr[i][j] = zr0[i][j];
            zu[i][j] = zu0[i][j];
            zv[i][j] = zv0[i][j];
            zz[i][j] = zz0[i][j];
          }
        }
      }
      if (which == 21) {
        for (i = 0; i < n; i++) {
          for (j = 0; j < 25; j++) {
            px[i][j] = px0[i][j];
          }
        }
      }
      if (which == 23) {
        for (i = 1; i < 6; i++) {
          for (j = 1; j < n; j++) {
            za[i][j] = za0[i][j];
          }
        }
      }
      k3 = k2 = 0;
      stb5 = stb50;
      xx[0] = xx0;
    }
  }
  return 0;
} /* endloop */

/************************************************************************
 *          init procedure - initialises data for all loops             *
 ************************************************************************/

static void init(struct Arrays *as1, struct Parameters *as2, long which) {
  long i, j, k, l, m, nn;
  double ds, dw, rr, ss;
  double fuzz, fizz, buzz, scaled, one;

  scaled = (double)(10.0);
  scaled = (double)(1.0) / scaled;
  fuzz = (double)(0.0012345);
  buzz = (double)(1.0) + fuzz;
  fizz = (double)(1.1) * fuzz;
  one = (double)(1.0);

  for (k = 0; k < 2048; k++) {
    if (k < 101) {
      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      du1[k] = (buzz - fizz) * scaled;

      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      du2[k] = (buzz - fizz) * scaled;

      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      du3[k] = (buzz - fizz) * scaled;

      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      vsp[k] = (buzz - fizz) * scaled;

      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      vstp[k] = (buzz - fizz) * scaled;

      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      vxne[k] = (buzz - fizz) * scaled;

      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      vxnd[k] = (buzz - fizz) * scaled;

      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      ve3[k] = (buzz - fizz) * scaled;

      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      vlr[k] = (buzz - fizz) * scaled;

      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      vlin[k] = (buzz - fizz) * scaled;

      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      b5[k] = (buzz - fizz) * scaled;

      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      sa[k] = (buzz - fizz) * scaled;

      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      sb[k] = (buzz - fizz) * scaled;
    }
    if (k < 300) {
      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      plan[k] = (buzz - fizz) * scaled;

      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      d[k] = (buzz - fizz) * scaled;
    }
    if (k < 1001) {
      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      u[k] = (buzz - fizz) * scaled;

      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      v[k] = (buzz - fizz) * scaled;

      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      w[k] = (buzz - fizz) * scaled;

      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      x[k] = (buzz - fizz) * scaled;

      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      y[k] = (buzz - fizz) * scaled;

      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      g[k] = (buzz - fizz) * scaled;

      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      grd[k] = (buzz - fizz) * scaled;

      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      dex[k] = (buzz - fizz) * scaled;

      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      xi[k] = (buzz - fizz) * scaled;

      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      ex[k] = (buzz - fizz) * scaled;

      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      ex1[k] = (buzz - fizz) * scaled;

      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      dex1[k] = (buzz - fizz) * scaled;

      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      vx[k] = (buzz - fizz) * scaled;

      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      xx[k] = (buzz - fizz) * scaled;

      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      rx[k] = (buzz - fizz) * scaled;

      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      z[k] = (buzz - fizz) * scaled;
    }

    buzz = (one - fuzz) * buzz + fuzz;
    fuzz = -fuzz;
    rh[k] = (buzz - fizz) * scaled;
  }

  fuzz = (double)(0.0012345);
  buzz = (double)(1.0) + fuzz;
  fizz = (double)(1.1) * fuzz;

  for (i = 0; i < 512; ++i) {
    for (j = 0; j < 4; ++j) {
      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      p[i][j] = (buzz - fizz) * scaled;
    }
  }
  for (i = 0; i < 101; ++i) {
    for (j = 0; j < 25; ++j) {
      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      px[i][j] = (buzz - fizz) * scaled;

      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      cx[i][j] = (buzz - fizz) * scaled;

      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      vy[j][i] = (buzz - fizz) * scaled;
    }
    for (j = 0; j < 7; ++j) {
      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      vh[j][i] = (buzz - fizz) * scaled;
      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      vf[j][i] = (buzz - fizz) * scaled;
      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      vg[j][i] = (buzz - fizz) * scaled;
      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      vs[j][i] = (buzz - fizz) * scaled;
      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      za[j][i] = (buzz - fizz) * scaled;
      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      zp[j][i] = (buzz - fizz) * scaled;
      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      zq[j][i] = (buzz - fizz) * scaled;
      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      zr[j][i] = (buzz - fizz) * scaled;
      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      zm[j][i] = (buzz - fizz) * scaled;
      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      zb[j][i] = (buzz - fizz) * scaled;
      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      zu[j][i] = (buzz - fizz) * scaled;
      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      zv[j][i] = (buzz - fizz) * scaled;
      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      zz[j][i] = (buzz - fizz) * scaled;
    }
    for (j = 0; j < 2; ++j) {
      for (k = 0; k < 5; ++k) {
        buzz = (one - fuzz) * buzz + fuzz;
        fuzz = -fuzz;
        u1[j][i][k] = (buzz - fizz) * scaled;
        buzz = (one - fuzz) * buzz + fuzz;
        fuzz = -fuzz;
        u2[j][i][k] = (buzz - fizz) * scaled;
        buzz = (one - fuzz) * buzz + fuzz;
        fuzz = -fuzz;
        u3[j][i][k] = (buzz - fizz) * scaled;
      }
    }
  }
  for (i = 0; i < 64; ++i) {
    for (j = 0; j < 64; ++j) {
      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      b[j][i] = (buzz - fizz) * scaled;
      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      c[j][i] = (buzz - fizz) * scaled;
      buzz = (one - fuzz) * buzz + fuzz;
      fuzz = -fuzz;
      h[j][i] = (buzz - fizz) * scaled;
    }
  }

  fuzz = (double)(0.0012345);
  buzz = (double)(1.0) + fuzz;
  fizz = (double)(1.1) * fuzz;

  for (k = 1; k < 40; k++) {
    buzz = (one - fuzz) * buzz + fuzz;
    fuzz = -fuzz;
    xtra[k] = (buzz - fizz) * scaled;
  }

  ds = 1.0;
  dw = 0.5;
  for (l = 0; l < 4; l++) {
    for (i = 0; i < 512; i++) {
      p[i][l] = ds;
      ds = ds + dw;
    }
  }
  for (i = 0; i < 96; i++) {
    e[i] = 1;
    f[i] = 1;
  }

  iqranf(as1, as2);
  dw = -100.0;
  for (i = 0; i < 1001; i++) {
    dex[i] = dw * dex[i];
    grd[i] = ix[i];
  }
  flx = 0.001;

  d[0] = 1.01980486428764;
  nn = n16;

  for (l = 1; l < 300; l++) {
    d[l] = d[l - 1] + 1.000e-4 / d[l - 1];
  }
  rr = d[nn - 1];
  for (l = 1; l <= 2; l++) {
    m = (nn + nn) * (l - 1);
    for (j = 1; j <= 2; j++) {
      for (k = 1; k <= nn; k++) {
        m = m + 1;
        ss = (double)(k);
        plan[m - 1] = rr * ((ss + 1.0) / ss);
        zone[m - 1] = k + k;
      }
    }
  }
  k = nn + nn + 1;
  zone[k - 1] = nn;

  if (which == 16) {
    r = d[n - 1];
    s = d[n - 2];
    t = d[n - 3];
    k3 = k2 = 0;
  }
  expmax = 20.0;
  if (which == 22) {
    u[n - 1] = 0.99 * expmax * v[n - 1];
  }
  if (which == 24) {
    x[n / 2] = -1.0e+10;
  }

  /************************************************************************
   *                 Make copies of data for extra loops                  *
   ************************************************************************/

  for (i = 0; i < 1001; i++) {
    x0[i] = x[i];
    w0[i] = w[i];
  }
  for (i = 0; i < 101; i++) {
    for (j = 0; j < 25; j++) {
      px0[i][j] = px[i][j];
    }
  }
  for (i = 0; i < 512; i++) {
    for (j = 0; j < 4; j++) {
      p0[i][j] = p[i][j];
    }
  }
  for (i = 0; i < 64; i++) {
    for (j = 0; j < 64; j++) {
      h0[i][j] = h[i][j];
    }
  }
  for (i = 0; i < 2048; i++) {
    rh0[i] = rh[i];
  }
  for (i = 0; i < 101; i++) {
    vxne0[i] = vxne[i];
  }
  for (i = 0; i < 7; i++) {
    for (j = 0; j < 101; j++) {
      zr0[i][j] = zr[i][j];
      zu0[i][j] = zu[i][j];
      zv0[i][j] = zv[i][j];
      zz0[i][j] = zz[i][j];
      za0[i][j] = za[i][j];
    }
  }
  stb50 = stb5;
  xx0 = xx[0];

  return;
}

/************************************************************************
 *   parameters procedure for loop counts, Do spans, sumchecks, FLOPS   *
 ************************************************************************/

static long parameters(struct Arrays *as1, struct Parameters *as2, long which,
                       benchmark_core_progress_callback_handler_t *callback) {
  long nloops[3][25] = {
      {0,  1001, 101, 1001, 1001, 1001, 64,  995,  100, 101, 101, 1001, 1000,
       64, 1001, 101, 75,   101,  100,  101, 1000, 101, 101, 100, 1001},
      {0,  101, 101, 101, 101, 101, 32,  101, 100, 101, 101, 101, 100,
       32, 101, 101, 40,  101, 100, 101, 100, 50,  101, 100, 101},
      {0, 27, 15, 27, 27, 27, 8, 21, 14, 15, 15, 27, 26, 8, 27, 15, 15, 15, 14, 15, 26, 20, 15, 14, 27}};

  long lpass[3][25] = {{0, 7, 67, 9, 14, 10, 3, 4, 10, 36, 34, 11, 12, 36, 2, 1, 25, 35, 2, 39, 1, 1, 11, 8, 5},
                       {0, 40, 40, 53, 70, 55, 7, 22, 6, 21, 19, 64, 68, 41, 10, 1, 27, 20, 1, 23, 8, 1, 7, 5, 31},
                       {0, 28, 46, 37, 38, 40, 21, 20, 9, 26, 25, 46, 48, 31, 8, 1, 14, 26, 2, 28, 7, 1, 8, 7, 23}};

  double sums[3][25] = {{0.0,
                         5.114652693224671e+04,
                         1.539721811668385e+03,
                         1.000742883066363e+01,
                         5.999250595473891e-01,
                         4.548871642387267e+03,
                         4.375116344729986e+03,
                         6.104251075174761e+04,
                         1.501268005625798e+05,
                         1.189443609974981e+05,
                         7.310369784325296e+04,
                         3.342910972650109e+07,
                         2.907141294167248e-05,
                         1.202533961842803e+11,
                         3.165553044000334e+09,
                         3.943816690352042e+04,
                         5.650760000000000e+05,
                         1.114641772902486e+03,
                         1.015727037502300e+05,
                         5.421816960147207e+02,
                         3.040644339351239e+07,
                         1.597308280710199e+08,
                         2.938604376566697e+02,
                         3.549900501563623e+04,
                         5.000000000000000e+02},

                        {0.0,
                         5.253344778937972e+02,
                         1.539721811668385e+03,
                         1.009741436578952e+00,
                         5.999250595473891e-01,
                         4.589031939600982e+01,
                         8.631675645333210e+01,
                         6.345586315784055e+02,
                         1.501268005625798e+05,
                         1.189443609974981e+05,
                         7.310369784325296e+04,
                         3.433560407475758e+04,
                         7.127569130821465e-06,
                         9.816387810944345e+10,
                         3.039983465145393e+07,
                         3.943816690352042e+04,
                         6.480410000000000e+05,
                         1.114641772902486e+03,
                         1.015727037502300e+05,
                         5.421816960147207e+02,
                         3.126205178815431e+04,
                         7.824524877232093e+07,
                         2.938604376566697e+02,
                         3.549900501563623e+04,
                         5.000000000000000e+01},

                        {0.0,
                         3.855104502494961e+01,
                         3.953296986903059e+01,
                         2.699309089320672e-01,
                         5.999250595473891e-01,
                         3.182615248447483e+00,
                         1.120309393467088e+00,
                         2.845720217644024e+01,
                         2.960543667875003e+03,
                         2.623968460874250e+03,
                         1.651291227698265e+03,
                         6.551161335845770e+02,
                         1.943435981130448e-06,
                         3.847124199949426e+10,
                         2.923540598672011e+06,
                         1.108997288134785e+03,
                         5.152160000000000e+05,
                         2.947368618589360e+01,
                         9.700646212337040e+02,
                         1.268230698051003e+01,
                         5.987713249475302e+02,
                         5.009945671204667e+07,
                         6.109968728263972e+00,
                         4.850340602749970e+02,
                         1.300000000000000e+01}};

  double number_flops[25] = {0,  5.,  4.,  2.,  2., 2.,  2., 16., 36., 17., 9.,  1., 1.,
                             7., 11., 33., 10., 9., 44., 6., 26., 2.,  17., 11., 1.};

  n = nloops[section][which];
  nspan[section][which] = n;
  n16 = nloops[section][16];
  nflops = number_flops[which];
  xflops[which] = (long)nflops;
  loop = lpass[section][which];
  xloops[section][which] = loop;
  loop = loop * mult;
  MasterSum = sums[section][which];
  count = 0;

  set_progress(callback, which, section);

  init(as1, as2, which);

  /************************************************************************
   *                   Start timing first pass only                       *
   ************************************************************************/

  if (count2 == 0) {
    secs = thread_clock();
  }

  return 0;
} /* parameters */

/************************************************************************
 *          check procedure to check accuracy of calculations           *
 ************************************************************************/

static void check(struct Parameters *as2, long which) {
  long maxs = 16;
  double xm, ym, re, min1, max1;

  xm = MasterSum;
  ym = Checksum[section][which];

  if (xm * ym < 0.0) {
    accuracy[section][which] = 0;
  } else {
    if (xm == ym) {
      accuracy[section][which] = maxs;
    } else {
      xm = fabs(xm);
      ym = fabs(ym);
      min1 = xm;
      max1 = ym;
      if (ym < xm) {
        min1 = ym;
        max1 = xm;
      }
      re = 1.0 - min1 / max1;
      accuracy[section][which] = (long)(fabs(log10(fabs(re))) + 0.5);
    }
  }

  return;
}

/************************************************************************
 *      iqranf procedure - random number generator for Kernel 14        *
 ************************************************************************/

static void iqranf(struct Arrays *as1, struct Parameters *as2) {
  long inset, Mmin, Mmax, nn, i, kk;
  double span, spin, realn, per, scale1, qq, dkk, dp, dq;
  long seed[3] = {256, 12491249, 1499352848};

  nn = 1001;
  Mmin = 1;
  Mmax = 1001;
  kk = seed[section];

  inset = Mmin;
  span = Mmax - Mmin;
  spin = 16807;
  per = 2147483647;
  realn = nn;
  scale1 = 1.00001;
  qq = scale1 * (span / realn);
  dkk = kk;

  for (i = 0; i < nn; i++) {
    dp = dkk * spin;
    dkk = dp - (long)(dp / per) * per;
    dq = dkk * span;
    ix[i] = inset + (long)(dq / per);
    if ((ix[i] < Mmin) | (ix[i] > Mmax)) {
      ix[i] = inset + i + 1 * (long)qq;
    }
  }

  return;
}

static void checkOut(struct Parameters *as2, struct RunParameters *as3, int which) {
  int i = 0;
  int j = 0;
  int errors = 0;
  char chek1[30] = "";
  char chek2[30] = "";

#ifdef WATCOM
  double sumsOut[3][25] = {{0.0,
                            5.114652693224671e+04,
                            1.539721811668384e+03,
                            1.000742883066364e+01,
                            5.999250595473891e-01,
                            4.548871642387267e+03,
                            4.375116344729986e+03,
                            6.104251075174761e+04,
                            1.501268005625795e+05,
                            1.189443609974981e+05,
                            7.310369784325296e+04,
                            3.342910972650109e+07,
                            2.907141294167248e-05,
                            1.202533961842804e+11,
                            3.165553044000335e+09,
                            3.943816690352044e+04,
                            5.650760000000000e+05,
                            1.114641772902486e+03,
                            1.015727037502299e+05,
                            5.421816960147207e+02,
                            3.040644339351238e+07,
                            1.597308280710200e+08,
                            2.938604376566698e+02,
                            3.549900501563624e+04,
                            5.000000000000000e+02},

                           {0.0,
                            5.253344778937972e+02,
                            1.539721811668384e+03,
                            1.009741436578952e+00,
                            5.999250595473891e-01,
                            4.589031939600982e+01,
                            8.631675645333210e+01,
                            6.345586315784055e+02,
                            1.501268005625795e+05,
                            1.189443609974981e+05,
                            7.310369784325296e+04,
                            3.433560407475758e+04,
                            7.127569130821465e-06,
                            9.816387810944356e+10,
                            3.039983465145392e+07,
                            3.943816690352044e+04,
                            6.480410000000000e+05,
                            1.114641772902486e+03,
                            1.015727037502299e+05,
                            5.421816960147207e+02,
                            3.126205178815432e+04,
                            7.824524877232093e+07,
                            2.938604376566698e+02,
                            3.549900501563624e+04,
                            5.000000000000000e+01},

                           {0.0,
                            3.855104502494961e+01,
                            3.953296986903060e+01,
                            2.699309089320672e-01,
                            5.999250595473891e-01,
                            3.182615248447483e+00,
                            1.120309393467088e+00,
                            2.845720217644024e+01,
                            2.960543667875005e+03,
                            2.623968460874250e+03,
                            1.651291227698265e+03,
                            6.551161335845770e+02,
                            1.943435981130448e-06,
                            3.847124199949431e+10,
                            2.923540598672009e+06,
                            1.108997288134785e+03,
                            5.152160000000000e+05,
                            2.947368618589360e+01,
                            9.700646212337040e+02,
                            1.268230698051004e+01,
                            5.987713249475302e+02,
                            5.009945671204667e+07,
                            6.109968728263973e+00,
                            4.850340602749970e+02,
                            1.300000000000000e+01}};
#endif

#ifdef VISUALC
  double sumsOut[3][25] = {{0.0,
                            5.114652693224671e+04,
                            1.539721811668385e+03,
                            1.000742883066364e+01,
                            5.999250595473891e-01,
                            4.548871642387267e+03,
                            4.375116344729986e+03,
                            6.104251075174761e+04,
                            1.501268005625799e+05,
                            1.189443609974981e+05,
                            7.310369784325296e+04,
                            3.342910972650109e+07,
                            2.907141294167248e-05,
                            1.202533961842805e+11,
                            3.165553044000335e+09,
                            3.943816690352044e+04,
                            5.650760000000000e+05,
                            1.114641772902486e+03,
                            1.015727037502299e+05,
                            5.421816960147207e+02,
                            3.040644339351239e+07,
                            1.597308280710200e+08,
                            2.938604376566698e+02,
                            3.549900501563623e+04,
                            5.000000000000000e+02},

                           {0.0,
                            5.253344778937972e+02,
                            1.539721811668385e+03,
                            1.009741436578952e+00,
                            5.999250595473891e-01,
                            4.589031939600982e+01,
                            8.631675645333210e+01,
                            6.345586315784055e+02,
                            1.501268005625799e+05,
                            1.189443609974981e+05,
                            7.310369784325296e+04,
                            3.433560407475758e+04,
                            7.127569130821465e-06,
                            9.816387810944356e+10,
                            3.039983465145392e+07,
                            3.943816690352044e+04,
                            6.480410000000000e+05,
                            1.114641772902486e+03,
                            1.015727037502299e+05,
                            5.421816960147207e+02,
                            3.126205178815431e+04,
                            7.824524877232093e+07,
                            2.938604376566698e+02,
                            3.549900501563623e+04,
                            5.000000000000000e+01},

                           {0.0,
                            3.855104502494961e+01,
                            3.953296986903060e+01,
                            2.699309089320672e-01,
                            5.999250595473891e-01,
                            3.182615248447483e+00,
                            1.120309393467088e+00,
                            2.845720217644024e+01,
                            2.960543667875003e+03,
                            2.623968460874251e+03,
                            1.651291227698265e+03,
                            6.551161335845770e+02,
                            1.943435981130448e-06,
                            3.847124199949431e+10,
                            2.923540598672009e+06,
                            1.108997288134785e+03,
                            5.152160000000000e+05,
                            2.947368618589361e+01,
                            9.700646212337041e+02,
                            1.268230698051004e+01,
                            5.987713249475302e+02,
                            5.009945671204667e+07,
                            6.109968728263973e+00,
                            4.850340602749970e+02,
                            1.300000000000000e+01}};
#endif

#ifdef GCCINTEL32
  double sumsOut[3][25] = {{0.0,
                            5.114652693224706e+04,
                            1.539721811668509e+03,
                            1.000742883066623e+01,
                            5.999250595474070e-01,
                            4.548871642388545e+03,
                            4.375116344743014e+03,
                            6.104251075174961e+04,
                            1.501268005627157e+05,
                            1.189443609975086e+05,
                            7.310369784325988e+04,
                            3.342910972650531e+07,
                            2.907141429123183e-05,
                            1.202533961843096e+11,
                            3.165553044001604e+09,
                            3.943816690352310e+04,
                            5.650760000000000e+05,
                            1.114641772903092e+03,
                            1.015727037502793e+05,
                            5.421816960150400e+02,
                            3.040644339317274e+07,
                            1.597308280710857e+08,
                            2.938604376567100e+02,
                            3.549900501566157e+04,
                            5.000000000000000e+02},

                           {0.0,
                            5.253344778938001e+02,
                            1.539721811668509e+03,
                            1.009741436579188e+00,
                            5.999250595474070e-01,
                            4.589031939602133e+01,
                            8.631675645345986e+01,
                            6.345586315784150e+02,
                            1.501268005627157e+05,
                            1.189443609975086e+05,
                            7.310369784325988e+04,
                            3.433560407476163e+04,
                            7.127569145018442e-06,
                            9.816387817138106e+10,
                            3.039983465147494e+07,
                            3.943816690352310e+04,
                            6.480410000000000e+05,
                            1.114641772903092e+03,
                            1.015727037502793e+05,
                            5.421816960150400e+02,
                            3.126205178811007e+04,
                            7.824524877235141e+07,
                            2.938604376567100e+02,
                            3.549900501566157e+04,
                            5.000000000000000e+01},
                           {0.0,
                            3.855104502494984e+01,
                            3.953296986903387e+01,
                            2.699309089321297e-01,
                            5.999250595474070e-01,
                            3.182615248448272e+00,
                            1.120309393467599e+00,
                            2.845720217644062e+01,
                            2.960543667877650e+03,
                            2.623968460874420e+03,
                            1.651291227698377e+03,
                            6.551161335846538e+02,
                            1.943435981782704e-06,
                            3.847124173932906e+10,
                            2.923540598699676e+06,
                            1.108997288135067e+03,
                            5.152160000000000e+05,
                            2.947368618590714e+01,
                            9.700646212341514e+02,
                            1.268230698051747e+01,
                            5.987713249471802e+02,
                            5.009945671206567e+07,
                            6.109968728264795e+00,
                            4.850340602751676e+02,
                            1.300000000000000e+01}};
#endif

#ifdef CCINTEL32
  double sumsOut[3][25] = {{0.0,
                            5.114652693224706e+04,
                            1.539721811668509e+03,
                            1.000742883066623e+01,
                            5.999250595474070e-01,
                            4.548871642388545e+03,
                            4.375116344743014e+03,
                            6.104251075174961e+04,
                            1.501268005627157e+05,
                            1.189443609975086e+05,
                            7.310369784325988e+04,
                            3.342910972650531e+07,
                            2.907141429123183e-05,
                            1.202533961843096e+11,
                            3.165553044001604e+09,
                            3.943816690352310e+04,
                            5.650760000000000e+05,
                            1.114641772903092e+03,
                            1.015727037502793e+05,
                            5.421816960150400e+02,
                            3.040644339317274e+07,
                            1.597308280710857e+08,
                            2.938604376567100e+02,
                            3.549900501566157e+04,
                            5.000000000000000e+02},

                           {0.0,
                            5.253344778938001e+02,
                            1.539721811668509e+03,
                            1.009741436579188e+00,
                            5.999250595474070e-01,
                            4.589031939602133e+01,
                            8.631675645345986e+01,
                            6.345586315784150e+02,
                            1.501268005627157e+05,
                            1.189443609975086e+05,
                            7.310369784325988e+04,
                            3.433560407476163e+04,
                            7.127569145018442e-06,
                            9.816387817138104e+10,
                            3.039983465147494e+07,
                            3.943816690352310e+04,
                            6.480410000000000e+05,
                            1.114641772903092e+03,
                            1.015727037502793e+05,
                            5.421816960150400e+02,
                            3.126205178811007e+04,
                            7.824524877235141e+07,
                            2.938604376567100e+02,
                            3.549900501566157e+04,
                            5.000000000000000e+01},
                           {0.0,
                            3.855104502494984e+01,
                            3.953296986903387e+01,
                            2.699309089321297e-01,
                            5.999250595474070e-01,
                            3.182615248448272e+00,
                            1.120309393467599e+00,
                            2.845720217644062e+01,
                            2.960543667877650e+03,
                            2.623968460874420e+03,
                            1.651291227698377e+03,
                            6.551161335846538e+02,
                            1.943435981782704e-06,
                            3.847124173932906e+10,
                            2.923540598699676e+06,
                            1.108997288135067e+03,
                            5.152160000000000e+05,
                            2.947368618590714e+01,
                            9.700646212341514e+02,
                            1.268230698051747e+01,
                            5.987713249471802e+02,
                            5.009945671206567e+07,
                            6.109968728264795e+00,
                            4.850340602751676e+02,
                            1.300000000000000e+01}};
#endif

#ifdef GCCINTEL64
  double sumsOut[3][25] = {{0.0,
                            5.114652693224671e+04,
                            1.539721811668385e+03,
                            1.000742883066363e+01,
                            5.999250595473891e-01,
                            4.548871642387267e+03,
                            4.375116344729986e+03,
                            6.104251075174761e+04,
                            1.501268005625795e+05,
                            1.189443609974981e+05,
                            7.310369784325296e+04,
                            3.342910972650109e+07,
                            2.907141294167248e-05,
                            1.202533961842805e+11,
                            3.165553044000335e+09,
                            3.943816690352044e+04,
                            5.650760000000000e+05,
                            1.114641772902486e+03,
                            1.015727037502299e+05,
                            5.421816960147207e+02,
                            3.040644339351239e+07,
                            1.597308280710199e+08,
                            2.938604376566697e+02,
                            3.549900501563623e+04,
                            5.000000000000000e+02},

                           {0.0,
                            5.253344778937972e+02,
                            1.539721811668385e+03,
                            1.009741436578952e+00,
                            5.999250595473891e-01,
                            4.589031939600982e+01,
                            8.631675645333210e+01,
                            6.345586315784055e+02,
                            1.501268005625795e+05,
                            1.189443609974981e+05,
                            7.310369784325296e+04,
                            3.433560407475758e+04,
                            7.127569130821465e-06,
                            9.816387810944356e+10,
                            3.039983465145392e+07,
                            3.943816690352044e+04,

                            6.480410000000000e+05,
                            1.114641772902486e+03,
                            1.015727037502299e+05,
                            5.421816960147207e+02,
                            3.126205178815431e+04,
                            7.824524877232093e+07,
                            2.938604376566697e+02,
                            3.549900501563623e+04,
                            5.000000000000000e+01},

                           {0.0,
                            3.855104502494961e+01,
                            3.953296986903059e+01,
                            2.699309089320672e-01,
                            5.999250595473891e-01,
                            3.182615248447483e+00,
                            1.120309393467088e+00,
                            2.845720217644024e+01,
                            2.960543667875005e+03,
                            2.623968460874250e+03,
                            1.651291227698265e+03,
                            6.551161335845770e+02,
                            1.943435981130448e-06,
                            3.847124199949431e+10,
                            2.923540598672009e+06,
                            1.108997288134785e+03,
                            5.152160000000000e+05,
                            2.947368618589361e+01,
                            9.700646212337041e+02,
                            1.268230698051003e+01,
                            5.987713249475302e+02,
                            5.009945671204667e+07,
                            6.109968728263972e+00,
                            4.850340602749970e+02,
                            1.300000000000000e+01}};
#endif

#ifdef GCCARMDP
  double sumsOut[3][25] = {{0.0,
                            5.114652693224706e+04,
                            1.539721811668509e+03,
                            1.000742883066623e+01,
                            5.999250595474070e-01,
                            4.548871642388545e+03,
                            4.375116344743014e+03,
                            6.104251075174961e+04,
                            1.501268005627157e+05,
                            1.189443609975086e+05,
                            7.310369784325988e+04,
                            3.342910972650531e+07,
                            2.907141429123183e-05,
                            1.202533961843096e+11,
                            3.165553044001604e+09,
                            3.943816690352310e+04,
                            5.650760000000000e+05,
                            1.114641772903092e+03,
                            1.015727037502793e+05,
                            5.421816960150400e+02,
                            3.040644339317274e+07,
                            1.597308280710857e+08,
                            2.938604376567100e+02,
                            3.549900501566157e+04,
                            5.000000000000000e+02},
                           {0.0,
                            5.253344778938001e+02,
                            1.539721811668509e+03,
                            1.009741436579188e+00,
                            5.999250595474070e-01,
                            4.589031939602133e+01,
                            8.631675645345986e+01,
                            6.345586315784150e+02,
                            1.501268005627157e+05,
                            1.189443609975086e+05,
                            7.310369784325988e+04,
                            3.433560407476163e+04,
                            7.127569145018442e-06,
                            9.816387817138104e+10,
                            3.039983465147494e+07,
                            3.943816690352310e+04,
                            6.480410000000000e+05,
                            1.114641772903092e+03,
                            1.015727037502793e+05,
                            5.421816960150400e+02,
                            3.126205178811007e+04,
                            7.824524877235141e+07,
                            2.938604376567100e+02,
                            3.549900501566157e+04,
                            5.000000000000000e+01},
                           {0.0,
                            3.855104502494984e+01,
                            3.953296986903387e+01,
                            2.699309089321297e-01,
                            5.999250595474070e-01,
                            3.182615248448272e+00,
                            1.120309393467599e+00,
                            2.845720217644062e+01,
                            2.960543667877650e+03,
                            2.623968460874420e+03,
                            1.651291227698377e+03,
                            6.551161335846538e+02,
                            1.943435981782704e-06,
                            3.847124173932906e+10,
                            2.923540598699676e+06,
                            1.108997288135067e+03,
                            5.152160000000000e+05,
                            2.947368618590714e+01,
                            9.700646212341514e+02,
                            1.268230698051747e+01,
                            5.987713249471802e+02,
                            5.009945671206567e+07,
                            6.109968728264795e+00,
                            4.850340602751676e+02,
                            1.300000000000000e+01}};
#endif

#ifdef GCCARMPI
  double sumsOut[3][25] = {{0.0,
                            5.114652693224671e+04,
                            1.539721811668385e+03,
                            1.000742883066363e+01,
                            5.999250595473891e-01,
                            4.548871642387267e+03,
                            4.375116344729986e+03,
                            6.104251075174761e+04,
                            1.501268005625795e+05,
                            1.189443609974981e+05,
                            7.310369784325296e+04,
                            3.342910972650109e+07,
                            2.907141294167248e-05,
                            1.202533961842805e+11,
                            3.165553044000335e+09,
                            3.943816690352044e+04,
                            5.650760000000000e+05,
                            1.114641772902486e+03,
                            1.015727037502299e+05,
                            5.421816960147207e+02,
                            3.040644339351239e+07,
                            1.597308280710199e+08,
                            2.938604376566697e+02,
                            3.549900501563623e+04,
                            5.000000000000000e+02},
                           {0.0,
                            5.253344778937972e+02,
                            1.539721811668385e+03,
                            1.009741436578952e+00,
                            5.999250595473891e-01,
                            4.589031939600982e+01,
                            8.631675645333210e+01,
                            6.345586315784055e+02,
                            1.501268005625795e+05,
                            1.189443609974981e+05,
                            7.310369784325296e+04,
                            3.433560407475758e+04,
                            7.127569130821465e-06,
                            9.816387810944356e+10,
                            3.039983465145392e+07,
                            3.943816690352044e+04,
                            6.480410000000000e+05,
                            1.114641772902486e+03,
                            1.015727037502299e+05,
                            5.421816960147207e+02,
                            3.126205178815431e+04,
                            7.824524877232093e+07,
                            2.938604376566697e+02,
                            3.549900501563623e+04,
                            5.000000000000000e+01},
                           {0.0,
                            3.855104502494961e+01,
                            3.953296986903059e+01,
                            2.699309089320672e-01,
                            5.999250595473891e-01,
                            3.182615248447483e+00,
                            1.120309393467088e+00,
                            2.845720217644024e+01,
                            2.960543667875005e+03,
                            2.623968460874250e+03,
                            1.651291227698265e+03,
                            6.551161335845770e+02,
                            1.943435981130448e-06,
                            3.847124199949431e+10,
                            2.923540598672009e+06,
                            1.108997288134785e+03,
                            5.152160000000000e+05,
                            2.947368618589361e+01,
                            9.700646212337041e+02,
                            1.268230698051003e+01,
                            5.987713249475302e+02,
                            5.009945671204667e+07,
                            6.109968728263972e+00,
                            4.850340602749970e+02,
                            1.300000000000000e+01}};
#endif

  if (reliability) {
    i = section;
    j = which;
    if (count2 == 1) {
      failCount = 0;
      sumscomp[i][j] = sumsOut[i][j];
      sprintf(chek1, "%22.15e", Checksum[i][j]);
      sprintf(chek2, "%22.15e", sumscomp[i][j]);
      if (strcmp(chek1, chek2) != 0) {
        nsRes = TRUE;
        sumscomp[i][j] = Checksum[i][j];
        /*fprintf(NULL, " Section %d Test %2d pass %5ld Non-standard
           result was %s expected %s\n", i + 1, j, count2, chek1,
           chek2);*/
      }
    } else {
      sprintf(chek1, "%22.15e", Checksum[i][j]);
      sprintf(chek2, "%22.15e", sumscomp[i][j]);
      if (strcmp(chek1, chek2) != 0) {
        compareFail = compareFail + 1;
        failCount = failCount + 1;
        /*if (compareFail == 1) {
            fprintf(NULL, " ERRORS - maximum of 5 reported per loop\n");
        }
        if (failCount < 6) {
            fprintf(NULL, " Section %d Test %2d pass %5ld Different
        result was %s expected %s\n", i + 1, j, count2, chek1, chek2);
        fflush(NULL);
        }*/
      }
    }
  } else {
    for (i = 0; i < 3; i++) {
      for (j = 1; j < 25; j++) {
        sprintf(chek1, "%22.15e", Checksum[i][j]);
        sprintf(chek2, "%22.15e", sumsOut[i][j]);
        if (strcmp(chek1, chek2) != 0) {
          errors = errors + 1;
          /*   fprintf(NULL, " Section %d Test %2d Non-standard result
             was %s expected %s\n", i + 1, j, chek1, chek2);*/
        }
      }
    }
    /*        if (errors == 0) {
                fprintf(NULL, " Numeric results were as expected\n");
            } else {
                fprintf(NULL, "\n Compiler #define or values in linpack.c
       might to be changed\n");
            }
            fprintf(NULL, "\n");*/
  }
}
