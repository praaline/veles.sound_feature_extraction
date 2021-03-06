/*! @file rectify.cc
 *  @brief Wave rectification.
 *  @author Shapichev Alexey <a.shapichev@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *  This code partially conforms to <a href="http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml">Google C++ Style Guide</a>.
 *
 *  @section Copyright
 *  Copyright © 2013 Samsung R&D Institute Russia
 *
 *  @section License
 *  Licensed to the Apache Software Foundation (ASF) under one
 *  or more contributor license agreements.  See the NOTICE file
 *  distributed with this work for additional information
 *  regarding copyright ownership.  The ASF licenses this file
 *  to you under the Apache License, Version 2.0 (the
 *  "License"); you may not use this file except in compliance
 *  with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing,
 *  software distributed under the License is distributed on an
 *  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 *  KIND, either express or implied.  See the License for the
 *  specific language governing permissions and limitations
 *  under the License.
 */

#include "src/transforms/rectify.h"
#include <cmath>
#include <simd/instruction_set.h>

namespace sound_feature_extraction {
namespace transforms {

void Rectify::Do(const float* in, float* out) const noexcept {
  Do(use_simd(), in, input_format_->Size(), out);
}

void Rectify::Do(bool simd, const float* input, int length,
                     float* output) noexcept {
  if (simd) {
#ifdef __AVX__
    const __m256 SIGNMASK =  _mm256_set1_ps(-0.f);
    // Unroll 1 time
    for (int i = 0; i < length - 15; i += 16) {
      __m256 vec1 = _mm256_load_ps(input + i);
      __m256 vec2 = _mm256_load_ps(input + i + 8);
      __m256 vecabs1 = _mm256_andnot_ps(SIGNMASK, vec1);
      __m256 vecabs2 = _mm256_andnot_ps(SIGNMASK, vec2);
      _mm256_store_ps(output + i, vecabs1);
      _mm256_store_ps(output + i + 8, vecabs2);
    }
    for (int i = (length & ~0xF); i < length; i++) {
      output[i] = fabsf(input[i]);
    }
  } else {
#elif defined(__ARM_NEON__)
    for (int i = 0; i < length - 7; i += 8) {
      float32x4_t vec1 = vld1q_f32(input + i);
      float32x4_t vec2 = vld1q_f32(input + i + 4);
      float32x4_t res1 = vabsq_f32(vec1);
      float32x4_t res2 = vabsq_f32(vec2);
      vst1q_f32(output + i, res1);
      vst1q_f32(output + i + 4, res2);
    }
    for (int i = (length & ~0x7); i < length; i++) {
      output[i] = fabsf(input[i]);
    }
  } else {
#else
  } {
#endif
    for (int i = 0; i < length; i++) {
      output[i] = abs(input[i]);
    }
  }
}

REGISTER_TRANSFORM(Rectify);

}  // namespace transforms
}  // namespace sound_feature_extraction
