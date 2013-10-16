/*! @file mix_stereo.h
 *  @brief Mix stereo audio channels together.
 *  @author Markovtsev Vadim <v.markovtsev@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *  This code partially conforms to <a href="http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml">Google C++ Style Guide</a>.
 *
 *  @section Copyright
 *  Copyright 2013 Samsung R&D Institute Russia
 */

#ifndef SRC_TRANSFORMS_MIX_STEREO_H_
#define SRC_TRANSFORMS_MIX_STEREO_H_

#include "src/formats/raw_format.h"
#include "src/omp_transform_base.h"

namespace SoundFeatureExtraction {
namespace Transforms {

class MixStereo : public OmpUniformFormatTransform<Formats::RawFormat16> {
 public:
  TRANSFORM_INTRO("Mix", "Mix stereo audio channels together.")

  OMP_TRANSFORM_PARAMETERS()

 protected:
  virtual size_t OnFormatChanged(size_t buffersCount) override;

  virtual void Do(const int16_t* in, int16_t* out) const noexcept override;
};

}  // namespace Transforms
}  // namespace SoundFeatureExtraction
#endif  // SRC_TRANSFORMS_MIX_STEREO_H_
