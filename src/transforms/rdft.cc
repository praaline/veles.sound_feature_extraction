/*! @file dft.cc
 *  @brief Discrete Fourier Transform using FFT.
 *  @author Markovtsev Vadim <v.markovtsev@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *  This code partially conforms to <a href="http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml">Google C++ Style Guide</a>.
 *
 *  @section Copyright
 *  Copyright 2013 Samsung R&D Institute Russia
 */

#include "src/transforms/rdft.h"
#include <fftf/api.h>
#include <vector>
#include <simd/arithmetic-inl.h>

namespace SoundFeatureExtraction {
namespace Transforms {

BuffersCountChange RDFT::OnFormatChanged() {
  if (!IsInverse()) {
    outputFormat_->SetSize(inputFormat_->Size() + 2);
  } else {
    outputFormat_->SetSize(inputFormat_->Size() - 2);
  }
  return BuffersCountChange::Identity;
}

void RDFT::Do(
    const BuffersBase<float*>& in,
    BuffersBase<float*>* out) const noexcept {
  int length = inputFormat_->Size();
  std::vector<const float*> inputs(in.Count());
  std::vector<float*> outputs(in.Count());
  for (size_t i = 0; i < in.Count(); i++) {
    inputs[i] = in[i];
    outputs[i] = (*out)[i];
  }
  auto fftPlan = std::unique_ptr<FFTFInstance, void (*)(FFTFInstance *)>(
      fftf_init_batch(
          FFTF_TYPE_REAL,
          FFTF_DIRECTION_FORWARD,
          FFTF_DIMENSION_1D,
          &length,
          FFTF_NO_OPTIONS,
          in.Count(),
          &inputs[0], &outputs[0]),
      fftf_destroy);

  fftf_calc(fftPlan.get());
}

void RDFT::DoInverse(
    const BuffersBase<float*>& in,
    BuffersBase<float*>* out) const noexcept {
  int length = outputFormat_->Size();
  std::vector<const float*> inputs(in.Count());
  std::vector<float*> outputs(in.Count());
  for (size_t i = 0; i < in.Count(); i++) {
    inputs[i] = in[i];
    outputs[i] = (*out)[i];
  }
  auto fftPlan = std::unique_ptr<FFTFInstance, void (*)(FFTFInstance *)>(
      fftf_init_batch(
          FFTF_TYPE_REAL,
          FFTF_DIRECTION_BACKWARD,
          FFTF_DIMENSION_1D,
          &length,
          FFTF_NO_OPTIONS,
          in.Count(),
          &inputs[0], &outputs[0]),
      fftf_destroy);

  fftf_calc(fftPlan.get());
  for (size_t i = 0; i < in.Count(); i++) {
    real_multiply_scalar(outputs[i], length, 1.0f / length, outputs[i]);
  }
}

REGISTER_TRANSFORM(RDFT);

}  // namespace Transforms
}  // namespace SoundFeatureExtraction
