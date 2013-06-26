/*! @file intF_to_int16.h
 *  @brief float to int16 converter.
 *  @author Markovtsev Vadim <v.markovtsev@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *  This code partially conforms to <a href="http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml">Google C++ Style Guide</a>.
 *
 *  @section Copyright
 *  Copyright 2013 Samsung R&D Institute Russia
 */

#ifndef SRC_FORMATS_FLOAT_TO_INT16_H_
#define SRC_FORMATS_FLOAT_TO_INT16_H_

#include "src/formats/raw_format.h"
#include "src/formats/raw_format_converter_base.h"

namespace SoundFeatureExtraction {
namespace Formats {

class FloatToInt16Raw
    : public RawFormatConverterBase<RawFormatF, RawFormat16> {
 protected:
  virtual void Do(const float* in,
                  int16_t* out) const noexcept override;
};

}  // namespace Formats
}  // namespace SoundFeatureExtraction
#endif  // SRC_FORMATS_FLOAT_TO_INT16_H_
