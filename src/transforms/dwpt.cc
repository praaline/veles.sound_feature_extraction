/*! @file dwpt.cc
 *  @brief Discrete wavelet packet transform.
 *  @author Markovtsev Vadim <v.markovtsev@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *  This code partially conforms to <a href="http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml">Google C++ Style Guide</a>.
 *
 *  @section Copyright
 *  Copyright 2013 Samsung R&D Institute Russia
 */

#include "src/transforms/dwpt.h"
#include <string>
#include "src/primitives/wavelet_filter_bank.h"

namespace SpeechFeatureExtraction {

using Primitives::WaveletFilterBank;

namespace Transforms {

DWPT::DWPT()
  : UniformFormatTransform(SupportedParameters()),
    treeFingerprint_({ 3, 3, 3, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
                       6, 6, 6, 6, 6, 6, 6, 6 }),
    waveletType_(WAVELET_TYPE_DAUBECHIES),
    waveletOrder_(DEFAULT_WAVELET_ORDER) {
}

bool DWPT::HasInverse() const noexcept {
  return true;
}

void DWPT::SetParameter(const std::string& name, const std::string& value) {
  if (name == "tree") {
    treeFingerprint_ = WaveletFilterBank::ParseDescription(value);
  } else if (name == "type") {
    waveletType_ = WaveletFilterBank::ParseWaveletType(value);
  } else if (name == "order") {
    int order = Parse<int>(name, value);
    WaveletFilterBank::ValidateOrder(waveletType_, order);
    waveletOrder_ = order;
  }
}

void DWPT::OnFormatChanged() {
  WaveletFilterBank::ValidateLength(treeFingerprint_,
                                    inputFormat_->Size());
}

void DWPT::TypeSafeInitializeBuffers(
    const BuffersBase<Formats::WindowF>& in,
    BuffersBase<Formats::WindowF>* buffers) const noexcept {
  buffers->Initialize(in.Size(), inputFormat_->Size());
}

void DWPT::Initialize() const noexcept {
  filterBank_ = std::make_shared<WaveletFilterBank>(
      waveletType_, waveletOrder_, treeFingerprint_);
}

void DWPT::TypeSafeDo(
    const BuffersBase<Formats::WindowF>& in,
    BuffersBase<Formats::WindowF> *out) const noexcept {
  assert(!IsInverse() && "Not implemented yet");
  assert(filterBank_ != nullptr && "Initialize() was not called");
  size_t length = inputFormat_->Size();

  #pragma omp parallel for
  for (size_t i = 0; i < in.Size(); i++) {
    auto input = in[i]->Data.get();
    auto output = (*out)[i]->Data.get();
    filterBank_->Apply(input, length, output);
  }
}

REGISTER_TRANSFORM(DWPT);

}  // namespace Transforms
}  // namespace SpeechFeatureExtraction