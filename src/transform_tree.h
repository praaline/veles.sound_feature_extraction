/*! @file transform_tree.h
 *  @brief New file description.
 *  @author Markovtsev Vadim <v.markovtsev@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *  This code partially conforms to <a href="http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml">Google C++ Style Guide</a>.
 *
 *  @section Copyright
 *  Copyright 2013 Samsung R&D Institute Russia
 */

#ifndef SRC_TRANSFORM_TREE_H_
#define SRC_TRANSFORM_TREE_H_

#include <chrono>
#include <vector>
#include "src/formats/raw_format.h"
#include "src/exceptions.h"
#include "src/logger.h"
#include "src/transform.h"
#include "src/allocators/buffers_allocator.h"

namespace SoundFeatureExtraction {

class ChainNameAlreadyExistsException : public ExceptionBase {
 public:
  explicit ChainNameAlreadyExistsException(const std::string& name)
  : ExceptionBase("Chain name \"" + name + "\" already exists.") {
  }
};

class ChainAlreadyExistsException : public ExceptionBase {
 public:
  explicit ChainAlreadyExistsException(const std::string& nameOld,
                              const std::string& nameNew)
  : ExceptionBase("Chain \"" + nameNew + "\" is identical to previously "
                  "added \"" + nameOld + "\".") {
  }
};

class TransformNotRegisteredException : public ExceptionBase {
 public:
  explicit TransformNotRegisteredException(const std::string& name)
  : ExceptionBase("Transform \"" + name + "\" is not registered.") {
  }
};

class IncompatibleTransformFormatException : public ExceptionBase {
 public:
  IncompatibleTransformFormatException(const Transform& parent,
                                       const std::string& child)
  : ExceptionBase("Transform \"" + child + "\" is incompatible with "
                  "output format \"" + parent.OutputFormat()->Id() +
                  "\" of transform \"" + parent.Name() + "\".") {
  }
};

class TreeIsEmptyException : public ExceptionBase {
 public:
  TreeIsEmptyException()
  : ExceptionBase("Transform tree is empty.") {
  }
};

class DependencyParameterUnknownException : public ExceptionBase {
 public:
  DependencyParameterUnknownException(const std::string& pname,
                                      const std::string& parentTransformName,
                                      const std::string& childTransformName)
  : ExceptionBase("Transform \"" + parentTransformName + "\" does not "
                  "have a registered parameter \"" + pname + "\" checked "
                  " by transform \"" + childTransformName + "\".") {
  }
};

class TreeIsPreparedException : public ExceptionBase {
 public:
  TreeIsPreparedException()
  : ExceptionBase("Transform tree has previously been prepared for execution "
      "and no more chains may be added.") {
  }
};

class TreeIsNotPreparedException : public ExceptionBase {
 public:
  TreeIsNotPreparedException()
  : ExceptionBase("Transform tree has not been prepared for execution.") {
  }
};

class TreeAlreadyPreparedException : public ExceptionBase {
 public:
  TreeAlreadyPreparedException()
  : ExceptionBase("Transform tree has already been prepared for execution.") {
  }
};

class TransformResultedInInvalidBuffersException : public ExceptionBase {
 public:
  TransformResultedInInvalidBuffersException(const std::string& transform,
                                             const std::string& inner)
  : ExceptionBase("Transform " + transform +
                  " resulted in invalid buffers. " + inner) {
  }
};

class InvalidInputBuffersException : public ExceptionBase {
 public:
  explicit InvalidInputBuffersException(const std::string& message)
  : ExceptionBase("Invalid input data. " + message) {
  }
};

class FailedToAllocateBuffersException : public std::bad_alloc {
 public:
  explicit FailedToAllocateBuffersException(const char* message) noexcept
  : message_(message) {}

  explicit FailedToAllocateBuffersException(const std::string& message) noexcept
  : message_(message) {}

  virtual const char* what() const noexcept {
    return message_.c_str();
  }

 private:
  std::string message_;
};

class TransformTree : public Logger {
 public:
  explicit TransformTree(Formats::RawFormat16&& rootFormat) noexcept;
  explicit TransformTree(
      const std::shared_ptr<Formats::RawFormat16>& rootFormat) noexcept;
  virtual ~TransformTree() noexcept;

  std::shared_ptr<Formats::RawFormat16> RootFormat() const noexcept;

  void AddFeature(
      const std::string& name,
      const std::vector<std::pair<std::string, std::string>>& transforms);

  void PrepareForExecution();

  std::unordered_map<std::string, std::shared_ptr<Buffers>> Execute(
      const int16_t* in);

  std::unordered_map<std::string, float> ExecutionTimeReport() const noexcept;
  void Dump(const std::string& dotFileName) const;

  bool ValidateAfterEachTransform() const noexcept;
  void SetValidateAfterEachTransform(bool value) noexcept;

  bool DumpBuffersAfterEachTransform() const noexcept;
  void SetDumpBuffersAfterEachTransform(bool value) noexcept;

 private:
  class Node : public Logger {
   public:
    Node(Node* parent, const std::shared_ptr<Transform>& boundTransform,
         TransformTree* host) noexcept;

    std::shared_ptr<Node> FindIdenticalChildTransform(const Transform& base)
        const noexcept;

    void ActionOnEachTransformInSubtree(
        const std::function<void(const Transform&)> action) const;
    void ActionOnSubtree(
        const std::function<void(const Node&)> action) const;
    void ActionOnEachImmediateChild(
        const std::function<void(Node&)> action);
    void ActionOnEachImmediateChild(
        const std::function<void(const Node&)> action) const;
    void ActionOnEachParent(
        const std::function<void(const Node&)> action) const;

    void BuildAllocationTree(size_t buffersCount,
                             MemoryAllocation::Node* node) const noexcept;

    void ApplyAllocationTree(size_t buffersCount,
                             const MemoryAllocation::Node& node,
                             void* allocatedMemory) noexcept;

    void Execute();

    size_t ChildrenCount() const noexcept;

    Node* Parent;
    const std::shared_ptr<Transform> BoundTransform;
    std::shared_ptr<Buffers> BoundBuffers;
    std::unordered_map<std::string,
                       std::vector<std::shared_ptr<Node>>> Children;
    Node* Next;
    TransformTree* Host;
    std::chrono::high_resolution_clock::duration ElapsedTime;
    std::vector<std::string> RelatedFeatures;
  };

  struct TransformCacheItem {
    std::shared_ptr<Transform> BoundTransform;
    std::chrono::high_resolution_clock::duration ElapsedTime;
  };

  void AddTransform(const std::string& name,
                    const std::string& parameters,
                    const std::string& relatedFeature,
                    std::shared_ptr<Node>* currentNode);
  std::shared_ptr<Transform> FindIdenticalTransform(
      const Transform& base) noexcept;
  void SaveTransformToCache(const std::shared_ptr<Transform>& t) noexcept;

  static float ConvertDuration(
      const std::chrono::high_resolution_clock::duration& d) noexcept;

  std::shared_ptr<Node> root_;
  std::shared_ptr<Formats::RawFormat16> rootFormat_;
  bool treeIsPrepared_;
  std::unordered_map<std::string, std::shared_ptr<Node>> features_;
  std::unordered_map<std::string, TransformCacheItem> transformsCache_;
  bool validateAfterEachTransform_;
  bool dumpBuffersAfterEachTransform_;
  std::shared_ptr<void> allocatedMemory_;
};

}  // namespace SoundFeatureExtraction
#endif  // SRC_TRANSFORM_TREE_H_
