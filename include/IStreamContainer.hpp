//
// Created by Nathaniel Rupprecht on 1/30/21.
//

#ifndef MANTA_ISTREAM_CONTAINER_H
#define MANTA_ISTREAM_CONTAINER_H

#include <utility>

#include "utility.hpp"

class IStreamContainer {
 private:
  struct container {
    virtual std::istream &get_stream() const = 0;
    virtual bool is_good() const = 0;
  };

  struct container_ptr : public container {
    explicit container_ptr(std::shared_ptr<std::istream> ptr) : in_(std::move(ptr)) {};

    NO_DISCARD std::istream &get_stream() const override {
      return *in_;
    }

    NO_DISCARD bool is_good() const override {
      return in_ != nullptr;
    }

    std::shared_ptr<std::istream> in_;
  };

  struct container_ref : public container {
    explicit container_ref(std::istream &in) : in_(in) {};

    NO_DISCARD std::istream &get_stream() const override {
      return in_;
    }

    NO_DISCARD bool is_good() const override {
      return true;
    }

    std::istream &in_;
  };

  std::shared_ptr<container> stream_container;

  explicit IStreamContainer(std::shared_ptr<container> &ptr)
      : stream_container(std::move(ptr)) {};

 public:
  //! \brief Default constructor.
  IStreamContainer() = default;

  static IStreamContainer OpenFile(const std::string &filename) {
    std::shared_ptr<std::istream> strm = std::make_shared<std::ifstream>(filename);
    std::shared_ptr<container> con = std::make_shared<container_ptr>(strm);
    return IStreamContainer(con);
  }

  static IStreamContainer StreamString(const std::string &sentence) {
    std::shared_ptr<std::stringstream> sstrm = std::make_shared<std::stringstream>();
    (*sstrm) << sentence;
    std::shared_ptr<std::istream> strm = sstrm;
    std::shared_ptr<container> con = std::make_shared<container_ptr>(strm);
    return IStreamContainer(con);
  }

  NO_DISCARD bool IsGood() const {
    return stream_container != nullptr && stream_container->is_good();
  }

  IStreamContainer &operator=(std::istream &stream) {
    stream_container = std::make_shared<container_ref>(stream);
    return *this;
  }

  IStreamContainer &operator=(const IStreamContainer &stream) {
    stream_container = stream.stream_container;
    return *this;
  }

  std::istream *operator->() const {
    return &stream_container->get_stream();
  }
};

#endif //MANTA_ISTREAM_CONTAINER_H
