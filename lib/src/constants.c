/*
 * Copyright (c) 2022 waverian. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <waverian/evolution_benchmark/constants.h>

const int EB_KERNEL_COUNT = 24;

const char *const EB_KERNEL_NAMES[] = {"Hydro fragment",
                                       "ICCG excerpt (Incomplete Cholesky Conjugate Gradient)",
                                       "Inner product",
                                       "Banded linear equations",
                                       "Tri-diagonal elimination, below diagonal",
                                       "General linear recurrence equations",
                                       "Equation of state fragment",
                                       "ADI integration",
                                       "Integrate predictors",
                                       "Difference predictors",
                                       "First sum",
                                       "First difference",
                                       "2-D PIC (Particle In Cell)",
                                       "1-D PIC (Particle In Cell)",
                                       "Casual Fortran.  Development version",
                                       "Monte Carlo search loop",
                                       "Implicit, conditional computation",
                                       "2-D explicit hydrodynamics fragment",
                                       "General linear recurrence equations",
                                       "Discrete ordinates transport, conditional recurrence on xx",
                                       "Matrix*matrix product",
                                       "Planckian distribution",
                                       "2-D implicit hydrodynamics fragment",
                                       "Find location of first minimum in array"};

const char *const EB_EXECUTION_MODE_NAMES[] = {"Manual", "Full", "Workstation"};

const char *const EB_OPTIMIZATION_NAMES[] = {"Non optimized", "Optimized"};

const char *const EB_RUN_TYPE_NAMES[] = {"Manual", "Single core", "Multi core", "Quad core", "Workstation"};
