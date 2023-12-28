//
// Created by Nathaniel Rupprecht on 7/17/21.
//
// Defines exception related functionality, namely, quick ways to define exceptions.
//

#pragma once

#include <exception>
#include <string>

//! \brief Macro alias for nodiscard specifier
#define NO_DISCARD [[nodiscard]]

//! \brief Macro alias for const noexcept
#define CONSTNOEXCEPT const noexcept

//! \brief Define the "what" function of an exception
#define EXCEPTION_WHAT(message) \
  [[nodiscard]] const char* what() CONSTNOEXCEPT override { \
    return (message); \
  }

//! \brief Define an exception class and what the "what" function will return.
#define EXCEPTION(class_name, message) \
  class class_name : public std::exception { \
    EXCEPTION_WHAT(message); \
  };

//! \brief Define an exception class whose constructor takes the message the "what" function will return.
#define EXCEPTION_MESSAGE_CTOR(class_name) \
  class class_name : public std::exception { \
  public: \
    explicit class_name(std::string message) \
        : message_(std::move(message)) {} \
    EXCEPTION_WHAT(message_.c_str()) \
  private: \
    const std::string message_; \
  };

namespace manta {

class MantaException : public std::runtime_error {
 public:
  explicit MantaException(std::string message, const char* file, int line)
      : std::runtime_error(formatMessage(message, file, line)), file_(file), line_(line) {}

  NO_DISCARD const std::string& File() const {
    return file_;
  }

  NO_DISCARD int Line() const {
    return line_;
  }

 private:
  //! \brief Create the exception message from the input message, file, and line.
  std::string formatMessage(const std::string& message, const std::string& file, int line);

  std::string file_;
  int line_;
};

} // namespace manta

//! \brief Contract macro for function preconditions.
#define MANTA_REQUIRE(condition, message) \
  { \
    if (!(condition)) { \
      std::ostringstream _strm_; \
      _strm_ << message; \
      throw ::manta::MantaException(_strm_.str(), __FILE__, __LINE__); \
    } \
  }

//! \brief Macro for general condition checking.
#define MANTA_ASSERT(condition, message) \
  { \
    if (!(condition)) { \
      std::ostringstream _strm_; \
      _strm_ << message; \
      throw ::manta::MantaException(_strm_.str(), __FILE__, __LINE__); \
    } \
  }

//! \brief Macro for raising a runtime error.
#define MANTA_FAIL(message) \
  { \
    std::ostringstream _strm_; \
    _strm_ << message; \
    throw ::manta::MantaException(_strm_.str(), __FILE__, __LINE__); \
  }

//! \brief Macro for raising a specific type of error.
#define MANTA_THROW(exception_type, message) \
  { \
    std::ostringstream _strm_; \
    _strm_ << message; \
    throw ::manta::MantaException(_strm_.str(), __FILE__, __LINE__); \
  }
