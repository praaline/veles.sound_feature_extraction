/*! @file zero_padding.cc
 *  @brief Tests for SoundFeatureExtraction::Transforms::ZeroPadding.
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
#include "src/transforms/zero_padding.h"

using SoundFeatureExtraction::Formats::WindowF;
using SoundFeatureExtraction::Formats::WindowFormatF;
using SoundFeatureExtraction::BuffersBase;
using SoundFeatureExtraction::Transforms::ZeroPadding;

class ZeroPaddingTest : public ZeroPadding, public testing::Test {
 public:
  BuffersBase<WindowF> Input;
  BuffersBase<WindowF> Output;
  int Size;

  ZeroPaddingTest()
      : Input(inputFormat_),
        Output(outputFormat_) {
  }

  virtual void SetUp() {
    Size = 501;
    Input.Initialize(1, Size * 2);
    for (int i = 0; i < Size; i++) {
      Input[0]->Data.get()[i] = (i - Size / 2.0f) / Size;
    }
    auto format = std::make_shared<WindowFormatF>(Size * 1000 / 16000, 16000);
    format->SetAllocatedSize(Size * 2);
    SetInputFormat(format);
    InitializeBuffers(Input, &Output);
  }
};

TEST_F(ZeroPaddingTest, Do) {
  ASSERT_EQ(512, outputFormat_->Size());
  Do(Input, &Output);
  for (int i = Size; i < 512; i++) {
    ASSERT_EQ(0.f, Output[0]->Data.get()[i]);
  }
}

#include "tests/google/src/gtest_main.cc"
