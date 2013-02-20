/*! @file convolute.cc
 *  @brief Tests for src/primitives/convolute.cc.
 *  @author Markovtsev Vadim <v.markovtsev@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *  This code partially conforms to <a href="http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml">Google C++ Style Guide</a>.
 *
 *  @section Copyright
 *  Copyright 2013 Samsung R&D Institute Russia
 */

#include <gtest/gtest.h>
#include "src/primitives/convolute.h"
#include "src/primitives/memory.h"
#include "src/primitives/arithmetic.h"

void convolute_reference(const float *__restrict x, size_t xLength,
                         const float *__restrict h, size_t hLength,
                         float *__restrict result) {
  for (int n = 0; n < (int)xLength; n++) {
    float sum = .0f;
    for (int m = 0; m < (int)hLength && m <= n; m++) {
      sum += h[m] * x[n - m];
    }
    result[n] = sum;
  }
}

void DebugPrintConvolution(const char* name, const float* vec) {
  printf("%s\t", name);
  for (int i = 0; i < 10; i++) {
    printf("%f  ", vec[i]);
  }
  printf("\n");
}

TEST(convolute, convolute) {
  float x[1024];
  for (int i = 0; i < (int)(sizeof(x) / sizeof(float)); i++) {
    x[i] = 1.0f;
  }
  float h[50];
  for (int i = 0; i < (int)(sizeof(h) / sizeof(float)); i++) {
    h[i] = i / (sizeof(h) / sizeof(float) - 1.0f);
  }

  float verif[sizeof(x) / sizeof(float)];
  convolute_reference(x, sizeof(x) / sizeof(float),
                      h, sizeof(h) / sizeof(float), verif);
  DebugPrintConvolution("BRUTE-FORCE", verif);

  float res[sizeof(x) / sizeof(float)];
  convolute(x, sizeof(x) / sizeof(float), h, sizeof(h) / sizeof(float), res);
  DebugPrintConvolution("OVERLAP-SAVE", res);

  int firstDifferenceIndex = -1;
  for (int i = 0; i < (int)(sizeof(x) / sizeof(float)); i++) {
    float delta = res[i] - verif[i];
    if (delta * delta > 1E-10 && firstDifferenceIndex == -1) {
      firstDifferenceIndex = i;
    }
  }
  ASSERT_EQ(-1, firstDifferenceIndex);
}

#include "tests/google/src/gtest_main.cc"
