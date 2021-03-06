/*! @file window.cc
 *  @brief Tests for src/primitives/window.h.
 *  @author Markovtsev Vadim <v.markovtsev@samsung.com>
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

#include <gtest/gtest.h>
#include <vector>
#include "src/primitives/window.h"

#define EPSILON 0.0001f

#define ASSERT_EQF(a, b) ASSERT_NEAR(a, b, EPSILON)

using sound_feature_extraction::WindowType;

TEST(Window, Rectangular) {
  ASSERT_EQ(1.0f, WindowElement(WindowType::kWindowTypeRectangular, 4, 0));
  ASSERT_EQ(1.0f, WindowElement(WindowType::kWindowTypeRectangular, 5, 2));
  ASSERT_EQ(1.0f, WindowElement(WindowType::kWindowTypeRectangular, 10, 8));
}

TEST(Window, Hamming) {
  std::vector<float> filter(20);
  std::vector<float> valid { 0.0800f, 0.7700f, 0.7700f, 0.0800f };  // NOLINT(*)
  for (size_t i = 0; i < valid.size(); i++) {
    filter[i] = WindowElement(WindowType::kWindowTypeHamming, 4, i);
  }
  for (size_t i = 0; i < valid.size(); i++) {
    ASSERT_EQF(valid[i], filter[i]);
  }

  valid = { 0.0800f, 0.5400f, 1.0000f, 0.5400f, 0.0800f };
  for (size_t i = 0; i < valid.size(); i++) {
    filter[i] = WindowElement(WindowType::kWindowTypeHamming, 5, i);
  }
  for (size_t i = 0; i < valid.size(); i++) {
    ASSERT_EQF(valid[i], filter[i]);
  }

  valid = { 0.0800f, 0.1077f, 0.1876f, 0.3100f, 0.4601f,
            0.6199f, 0.7700f, 0.8924f, 0.9723f, 1.0000f,
            0.9723f, 0.8924f, 0.7700f, 0.6199f, 0.4601f,
            0.3100f, 0.1876f, 0.1077f, 0.0800f };
  for (size_t i = 0; i < valid.size(); i++) {
    filter[i] = WindowElement(WindowType::kWindowTypeHamming, 19, i);
  }
  for (size_t i = 0; i < valid.size(); i++) {
    ASSERT_EQF(valid[i], filter[i]);
  }

  valid = { 0.0800f, 0.1049f, 0.1770f, 0.2884f, 0.4271f,
            0.5780f, 0.7248f, 0.8515f, 0.9446f, 0.9937f,
            0.9937f, 0.9446f, 0.8515f, 0.7248f, 0.5780f,
            0.4271f, 0.2884f, 0.1770f, 0.1049f, 0.0800f };
  for (size_t i = 0; i < valid.size(); i++) {
    filter[i] = WindowElement(WindowType::kWindowTypeHamming, 20, i);
  }
  for (size_t i = 0; i < valid.size(); i++) {
    ASSERT_EQF(valid[i], filter[i]);
  }
}

TEST(Window, HalfHanningRight) {
  std::vector<float> filter(20);
  std::vector<float> valid { 1.0000f, 0.7500f, 0.2500f, 0.0000f };
  for (size_t i = 0; i < valid.size(); i++) {
    filter[i] = WindowElement(WindowType::kWindowTypeHalfHanningRight, 4, i);
  }
  for (size_t i = 0; i < valid.size(); i++) {
    ASSERT_EQF(valid[i], filter[i]);
  }

  valid = { 1.0000f, 0.8535f, 0.5000f, 0.1464f, 0.0000f };
  for (size_t i = 0; i < valid.size(); i++) {
    filter[i] = WindowElement(WindowType::kWindowTypeHalfHanningRight, 5, i);
  }
  for (size_t i = 0; i < valid.size(); i++) {
    ASSERT_EQF(valid[i], filter[i]);
  }

  valid = { 1.0000f, 0.9924f, 0.9698f, 0.9330f, 0.8830f,
            0.8213f, 0.7500f, 0.6710f, 0.5868f, 0.5000f,
            0.4131f, 0.3289f, 0.2500f, 0.1786f, 0.1169f,
            0.0669f, 0.0301f, 0.0075f, 0.0000f};
  for (size_t i = 0; i < valid.size(); i++) {
    filter[i] = WindowElement(WindowType::kWindowTypeHalfHanningRight, 19, i);
  }
  for (size_t i = 0; i < valid.size(); i++) {
    ASSERT_EQF(valid[i], filter[i]);
  }

  valid = { 1.0000f, 0.9931f, 0.9729f, 0.9397f, 0.8945f,
            0.8386f, 0.7734f, 0.7008f, 0.6227f, 0.5412f,
            0.4587f, 0.3772f, 0.2991f, 0.2265f, 0.1613f,
            0.1054f, 0.0602f, 0.0270f, 0.0068f, 0.0000f };
  for (size_t i = 0; i < valid.size(); i++) {
    filter[i] = WindowElement(WindowType::kWindowTypeHalfHanningRight, 20, i);
  }
  for (size_t i = 0; i < valid.size(); i++) {
    ASSERT_EQF(valid[i], filter[i]);
  }
}

TEST(Window, Hanning) {
  std::vector<float> filter(20);
  std::vector<float> valid { 0.0f, 0.7500f, 0.7500f, 0.0f };
  for (size_t i = 0; i < valid.size(); i++) {
    filter[i] = WindowElement(WindowType::kWindowTypeHanning, 4, i);
  }
  for (size_t i = 0; i < valid.size(); i++) {
      ASSERT_EQF(valid[i], filter[i]);
  }

  valid = { 0.0f, 0.5000f, 1.0000f, 0.5000f, 0.000f };
  for (size_t i = 0; i < valid.size(); i++) {
    filter[i] = WindowElement(WindowType::kWindowTypeHanning, 5, i);
  }
  for (size_t i = 0; i < valid.size(); i++) {
    ASSERT_EQF(valid[i], filter[i]);
  }

  valid = { 0.0000f, 0.0301f, 0.1169f, 0.2500f, 0.4131f,
            0.5868f, 0.7500f, 0.8830f, 0.9698f, 1.0000f,
            0.9698f, 0.8830f, 0.7500f, 0.5868f, 0.4131f,
            0.2500f, 0.1169f, 0.0301f, 0.0000f};
  for (size_t i = 0; i < valid.size(); i++) {
    filter[i] = WindowElement(WindowType::kWindowTypeHanning, 19, i);
  }
  for (size_t i = 0; i < valid.size(); i++) {
    ASSERT_EQF(valid[i], filter[i]);
  }

  valid = { 0.0000f, 0.0270f, 0.1054f, 0.2265f, 0.3772f,
            0.5412f, 0.7008f, 0.8386f, 0.9397f, 0.9931f,
            0.9931f, 0.9397f, 0.8386f, 0.7008f, 0.5412f,
            0.3772f, 0.2265f, 0.1054f, 0.0270f, 0.0000f };
  for (size_t i = 0; i < valid.size(); i++) {
    filter[i] = WindowElement(WindowType::kWindowTypeHanning, 20, i);
  }
  for (size_t i = 0; i < valid.size(); i++) {
    ASSERT_EQF(valid[i], filter[i]);
  }
}


TEST(Window, Blackman) {
  std::vector<float> filter(20);
  std::vector<float> valid = {
       0.f, 0.0102f, 0.0451f, 0.1144f,
    0.2269f, 0.3824f, 0.5667f, 0.7520f,
    0.9035f, 0.9888f, 0.9888f, 0.9035f,
    0.7520f, 0.5667f, 0.3824f, 0.2269f,
    0.1144f, 0.0451f, 0.0102f, 0.f };
  for (size_t i = 0; i < valid.size(); i++) {
    filter[i] = WindowElement(WindowType::kWindowTypeBlackman, 20, i);
  }
  for (size_t i = 0; i < valid.size(); i++) {
    ASSERT_EQF(valid[i], filter[i]);
  }
}

#include "tests/google/src/gtest_main.cc"
