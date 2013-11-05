/*! @file buffers.h
 *  @brief Buffers class definition.
 *  @author Markovtsev Vadim <v.markovtsev@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *  This code partially conforms to <a href="http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml">Google C++ Style Guide</a>.
 *
 *  @section Copyright
 *  Copyright 2013 Samsung R&D Institute Russia
 */

#ifndef SRC_BUFFERS_H_
#define SRC_BUFFERS_H_

#include <memory>
#include "src/buffer_format.h"

namespace sound_feature_extraction {

class Buffers {
 public:
  Buffers(const std::shared_ptr<BufferFormat>& format,
          size_t count = 0, void* reusedMemory = nullptr) noexcept;
  Buffers& operator=(const Buffers& other);

  virtual ~Buffers() {
  }

  size_t Count() const noexcept;
  size_t SizeInBytes() const noexcept;

  void* operator[](size_t index) noexcept;
  const void* operator[](size_t index) const noexcept;

  std::shared_ptr<BufferFormat> Format() const noexcept;

  void Validate() const;

  std::string Dump() const noexcept;
  std::string Dump(size_t index) const noexcept;
  std::string ToString() const noexcept;

 protected:
  void* Data() noexcept;
  const void* Data() const noexcept;

 private:
  std::shared_ptr<BufferFormat> format_;
  std::shared_ptr<void> buffers_;
  size_t count_;
};

}  // namespace sound_feature_extraction

namespace sound_feature_extraction {

/// @brief This exception is thrown when an attempt is made to assign "my"
/// buffers to "yours" of a bigger size.
class InsufficientAllocatedMemoryException : public ExceptionBase {
 public:
  explicit InsufficientAllocatedMemoryException(const Buffers& my,
                                                const Buffers& yours)
  : ExceptionBase("Attempted to assign " + my.ToString() + " to " +
                  yours.ToString() + ".") {
  }
};

}  // namespace sound_feature_extraction
#endif  // SRC_BUFFERS_H_
