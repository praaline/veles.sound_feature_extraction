/*! @file fixed_array.h
 *  @brief Array with compile-time size.
 *  @author Vadim Markovtsev <v.markovtsev@samsung.com>
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

#ifndef SRC_FORMATS_FIXED_ARRAY_H_
#define SRC_FORMATS_FIXED_ARRAY_H_

#include <array>
#include <cstdint>
#include "src/buffers_base.h"

namespace sound_feature_extraction {
namespace formats {

template<std::uint8_t L, typename F = float>
struct FixedArray : public std::array<F, L> {
  static_assert(std::is_arithmetic<F>(), "F must be an arithmetic type");

  bool Validate() const noexcept {
    for (int i = 0; i < L; i++) {
      if (!validation::Validator<F>::Validate((*this)[i])) {
        return false;
      }
    }
    return true;
  }

  bool operator== (F value) const noexcept {
    for (int i = 0; i < L; i++) {
      if ((*this)[i] != value) {
        return false;
      }
    }
    return true;
  }
};

}  // namespace formats

namespace validation {
  template<std::uint8_t L>
  struct Validator<formats::FixedArray<L>> {
    static bool Validate(const formats::FixedArray<L>& value) noexcept {
      return value.Validate();
    }

    static bool Validate(formats::FixedArray<L>&& value) noexcept {
      return value.Validate();
    }
  };
}  // namespace validation

}  // namespace sound_feature_extraction

namespace std {
  template<std::uint8_t L, typename F>
  inline string to_string(
      const sound_feature_extraction::formats::FixedArray<L, F>& __val) {
    std::string res("[");
    for (int i = 0; i < L; i++) {
      res += std::to_string(__val[i]) + ",\t";
    }
    res = res.substr(0, res.size() - 2);
    res += "]";
    return res;
  }
}  // namespace std

#endif  // SRC_FORMATS_FIXED_ARRAY_H_
