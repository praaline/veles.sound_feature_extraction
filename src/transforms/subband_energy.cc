/*! @file subband_energy.cc
 *  @brief Calculate the subband energy.
 *  @author Markovtsev Vadim <v.markovtsev@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *  This code partially conforms to <a href="http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml">Google C++ Style Guide</a>.
 *
 *  @section Copyright
 *  Copyright 2013 Samsung R&D Institute Russia
 */

#include "src/transforms/subband_energy.h"
#include <math.h>
#ifdef __AVX__
#include <immintrin.h>
#include "src/primitives/avx_extra.h"
#elif defined(__ARM_NEON__)
#include <arm_neon.h>
#endif
#include "src/primitives/wavelet_filter_bank.h"
#include "src/primitives/energy.h"

namespace SpeechFeatureExtraction {

using Primitives::WaveletFilterBank;

namespace Transforms {

SubbandEnergy::SubbandEnergy()
: UniformFormatTransform(SupportedParameters()),
  treeFingerprint_({ 3, 3, 3, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
                     6, 6, 6, 6, 6, 6, 6, 6 }) {
}

void SubbandEnergy::SetParameter(const std::string& name,
                                 const std::string& value) {
  if (name == "tree") {
    treeFingerprint_ = WaveletFilterBank::ParseDescription(value);
  }
}

void SubbandEnergy::OnFormatChanged() {
  WaveletFilterBank::ValidateLength(treeFingerprint_,
                                    inputFormat_->Size());
  outputFormat_->SetSize(treeFingerprint_.size());
}

void SubbandEnergy::Initialize() const noexcept {
  offsets_.reserve(treeFingerprint_.size());
  int offset = 0;
  for (auto depth : treeFingerprint_) {
    offsets_.push_back(offset);
    offset += (inputFormat_->Size() >> depth);
  }
  offsets_.push_back(offset);
}

void SubbandEnergy::TypeSafeInitializeBuffers(
    const BuffersBase<Formats::WindowF>& in,
    BuffersBase<Formats::WindowF>* buffers) const noexcept {
  buffers->Initialize(in.Size(), treeFingerprint_.size());
}

void SubbandEnergy::TypeSafeDo(
    const BuffersBase<Formats::WindowF>& in,
    BuffersBase<Formats::WindowF> *out) const noexcept {
  for (size_t i = 0; i < in.Size(); i++) {
    auto input = in[i]->Data.get();
    auto output = (*out)[i]->Data.get();
    for (int i = 0; i < (int)offsets_.size() - 1; i++) {
      output[i] = calculate_energy(input + offsets_[i],
                                   offsets_[i + 1] - offsets_[i]);
    }
  }
}

REGISTER_TRANSFORM(SubbandEnergy);

}  // namespace Transforms
}  // namespace SpeechFeatureExtraction