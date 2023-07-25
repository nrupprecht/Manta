//
// Created by Nathaniel Rupprecht on 1/30/21.
//

#pragma once

#include <utility>

#include "utility.hpp"

namespace manta::utility {

class IStreamContainer {
private:
  //! \brief Abstract base class for the IStreamContainer's impl.
  struct container {
    virtual std::istream& get_stream() const = 0;
    virtual bool is_good() const = 0;
  };

  //! \brief Container that has a shared pointer to an istream.
  struct container_ptr : public container {
    explicit container_ptr(std::shared_ptr<std::istream> ptr)
        : in_(std::move(ptr)) {};

    NO_DISCARD std::istream& get_stream() const override { return *in_; }
    NO_DISCARD bool is_good() const override { return in_ != nullptr; }

    std::shared_ptr<std::istream> in_;
  };

  //! \brief Container that has a reference to an istream.
  struct container_ref : public container {
    explicit container_ref(std::istream& in)
        : in_(in) {};

    NO_DISCARD std::istream& get_stream() const override { return in_; }
    NO_DISCARD bool is_good() const override { return true; }

    std::istream& in_;
  };

  //! \brief The stream container, which serves as the private implementation of the
  //! IStreamContainer, and allows for different 'backends' for interacting with streams.
  std::shared_ptr<container> stream_container_;

  explicit IStreamContainer(std::shared_ptr<container>& ptr)
      : stream_container_(std::move(ptr)) {};

public:
  //! \brief Default constructor.
  IStreamContainer() = default;

  static IStreamContainer OpenFile(const std::string& filename) {
    std::shared_ptr<std::istream> strm = std::make_shared<std::ifstream>(filename);
    std::shared_ptr<container> con = std::make_shared<container_ptr>(strm);
    return IStreamContainer(con);
  }

  static IStreamContainer StreamString(const std::string& sentence) {
    std::shared_ptr<std::stringstream> sstrm = std::make_shared<std::stringstream>();
    (*sstrm) << sentence;
    std::shared_ptr<std::istream> strm = sstrm;
    std::shared_ptr<container> con = std::make_shared<container_ptr>(strm);
    return IStreamContainer(con);
  }

  NO_DISCARD bool IsGood() const {
    return stream_container_ && stream_container_->is_good();
  }

  IStreamContainer& operator=(std::istream& stream) {
    stream_container_ = std::make_shared<container_ref>(stream);
    return *this;
  }

  IStreamContainer& operator=(const IStreamContainer& stream) {
    stream_container_ = stream.stream_container_;
    return *this;
  }

  std::istream* operator->() const { return &stream_container_->get_stream(); }
};

}  // namespace manta::utility
