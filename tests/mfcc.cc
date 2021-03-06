/*! @file mfcc.cc
 *  @brief Test which checks the calculation of MFCC.
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
#include "src/transform_tree.h"
#include "src/transform_registry.h"
#include "src/formats/array_format.h"
#include "tests/speech_sample.inc"

using sound_feature_extraction::TransformTree;
using sound_feature_extraction::BuffersBase;

TEST(Features, MFCC) {
  TransformTree tt( { 48000, 16000 } );  // NOLINT(*)
  tt.set_validate_after_each_transform(true);
  tt.AddFeature("MFCC", { { "Window", "length=512" }, { "RDFT", "" },
      { "SpectralEnergy", "" }, { "FilterBank", "squared=true" },
      { "Log", "" }, { "Square", "" }, { "DCT", "" },
      { "Selector", "length=16" } });
  int16_t* buffers = new int16_t[48000];
  memcpy(buffers, data, sizeof(data));
  tt.PrepareForExecution();
  auto res = tt.Execute(buffers);
  delete[] buffers;
  ASSERT_EQ(1U, res.size());
  res["MFCC"]->Validate();
  tt.Dump("/tmp/mfcc.dot");
  auto report = tt.ExecutionTimeReport();
  for (auto r : report) {
    printf("%s:\t%f\n", r.first.c_str(), r.second);
  }
}

TEST(Features, MFCCTrivial) {
  TransformTree tt( { 48000, 16000 } );  // NOLINT(*)
  tt.set_validate_after_each_transform(true);
  // tt.SetDumpBuffersAfterEachTransform(true);
  tt.AddFeature("MFCC", { { "Window", "length=512" }, { "RDFT", "" },
      { "SpectralEnergy", "" }, { "FilterBank", "squared=true" },
      { "Log", "" }, { "Square", "" }, { "DCT", "" },
      { "Selector", "length=16" } });
  int16_t* buffers = new int16_t[48000];
  for (int i = 0; i < 48000; i++) {
    buffers[i] = 128;
  }
  tt.PrepareForExecution();
  auto res = tt.Execute(buffers);
  delete[] buffers;
  ASSERT_EQ(1U, res.size());
  res["MFCC"]->Validate();
}

#include "tests/google/src/gtest_main.cc"
