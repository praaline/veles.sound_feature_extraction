/*! @file magnitude.h
 *  @brief Calculate the magnitude of each complex number.
 *  @author Markovtsev Vadim <v.markovtsev@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *  This code partially conforms to <a href="http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml">Google C++ Style Guide</a>.
 *
 *  @section Copyright
 *  Copyright 2013 Samsung R&D Institute Russia
 */

#ifndef MAGNITUDE_H_
#define MAGNITUDE_H_

#include "src/formats/window_format.h"
#include "src/uniform_format_transform.h"

namespace SpeechFeatureExtraction {
namespace Transforms {

class Magnitude
    : public UniformFormatTransform<Formats::WindowFormatF> {
 public:
  Magnitude();

  TRANSFORM_INTRO("Magnitude",
                  "Calculates the magnitude of each complex number, that is, "
                  " a square root of the sum of squared real and imaginary "
                  "parts.")

  TRANSFORM_PARAMETERS()

 protected:
  virtual void TypeSafeInitializeBuffers(
      const BuffersBase<Formats::WindowF>& in,
      BuffersBase<Formats::WindowF>* buffers)
  const noexcept;

  virtual void TypeSafeDo(const BuffersBase<Formats::WindowF>& in,
                          BuffersBase<Formats::WindowF> *out) const noexcept;
};

}  // namespace Transforms
}  // namespace SpeechFeatureExtraction
#endif  // INCLUDE_MAGNITUDE_H_
