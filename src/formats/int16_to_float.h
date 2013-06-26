/*! @file int16_to_float.h
 *  @brief int16 to float converter.
 *  @author Markovtsev Vadim <v.markovtsev@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *  This code partially conforms to <a href="http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml">Google C++ Style Guide</a>.
 *
 *  @section Copyright
 *  Copyright 2013 Samsung R&D Institute Russia
 */

#ifndef SRC_FORMATS_INT16_TO_FLOAT_H_
#define SRC_FORMATS_INT16_TO_FLOAT_H_

#include "src/formats/raw_format.h"
#include "src/formats/raw_format_converter_base.h"

namespace SoundFeatureExtraction {
namespace Formats {

class Int16ToFloatRaw
    : public RawFormatConverterBase<RawFormat16, RawFormatF> {
 protected:
  virtual void Do(const int16_t* in,
                  float* out) const noexcept override;
};

}  // namespace Formats
}  // namespace SoundFeatureExtraction
#endif  // SRC_FORMATS_INT16_TO_FLOAT_H_
