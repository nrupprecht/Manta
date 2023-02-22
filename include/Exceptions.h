//
// Created by Nathaniel Rupprecht on 7/17/21.
//
// Defines exception related functionality, namely, quick ways to define exceptions.
//

#ifndef MANTACLION_EXCEPTIONS_H
#define MANTACLION_EXCEPTIONS_H

#include <exception>

//! \brief Macro alias for nodiscard specifier
#define NO_DISCARD [[nodiscard]]

//! \brief Macro alias for const noexcept
#define CONSTNOEXCEPT const noexcept

//! \brief Define the "what" function of an exception
#define EXCEPTION_WHAT(message) [[nodiscard]] const char* what() CONSTNOEXCEPT override \
    { return (message); }

//! \brief Define an exception class and what the "what" function will return.
#define EXCEPTION(class_name, message) class class_name : public std::exception { EXCEPTION_WHAT(message); };

//! \brief Define an exception class whose constructor takes the message the "what" function will return.
#define EXCEPTION_MESSAGE_CTOR(class_name) class class_name : public std::exception { \
    public:                                                                           \
    explicit class_name(std::string message) : message_(std::move(message)) {}        \
    EXCEPTION_WHAT(message_.c_str())                                                  \
    private:                                                                          \
    const std::string message_;                                                       \
    };

#define MANTA_REQUIRE(condition, message) { \
  if (!(condition)) {                       \
    std::ostringstream _strm_; \
    _strm_ << message; \
    throw std::runtime_error(_strm_.str()); \
  }                                         \
}

#define MANTA_ASSERT(condition, message) { \
  if (!(condition)) {                       \
    std::ostringstream _strm_; \
    _strm_ << message; \
    throw std::runtime_error(_strm_.str()); \
  }                                         \
}

#define MANTA_FAIL(message) { \
  std::ostringstream _strm_; \
  _strm_ << message; \
  throw std::runtime_error(_strm_.str()); \
}


#endif //MANTACLION_EXCEPTIONS_H

