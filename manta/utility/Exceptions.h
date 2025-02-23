//
// Created by Nathaniel Rupprecht on 7/17/21.
//
// Defines exception related functionality, namely, quick ways to define exceptions.
//

#pragma once

#include <exception>
#include <stdexcept> // For std::runtime_error
#include <string>
#include <Lightning/Lightning.h>

//! \brief Macro alias for nodiscard specifier
#define NO_DISCARD [[nodiscard]]

//! \brief Macro alias for const noexcept
#define CONSTNOEXCEPT const noexcept

//! \brief Define the "what" function of an exception
#define EXCEPTION_WHAT(message) \
  NO_DISCARD const char* what() CONSTNOEXCEPT override { \
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

//! \brief Exception class for Manta. These record the line and file that caused the exception.
class MantaException : public std::runtime_error {
 public:
  explicit MantaException(std::string message, const char* file, const std::string& function_name, int line)
      : std::runtime_error(formatMessage(message, file, function_name, line))
        , file_(file)
        , function_name_(function_name)
        , line_(line) {}

  NO_DISCARD const std::string& GetFileName() const { return file_; }
  NO_DISCARD const std::string GetFunctionName() const { return function_name_; }
  NO_DISCARD int GetLine() const { return line_; }

 private:
  //! \brief Create the exception message from the input message, file, and line.
  static std::string formatMessage(const std::string& message,
                                   const std::string& file,
                                   const std::string& function_name,
                                   int line);

  const std::string file_{};
  const std::string function_name_{};
  const int line_;
};

} // namespace manta


//! \brief Macro for getting the function name.
// TODO: This needs work, since things like __PRETTY_FUNCTION__ are not macros, so we actually can't do this this way.
#ifdef __PRETTY_FUNCTION__
#   define __FUNCTION_NAME__ __PRETTY_FUNCTION__
#endif
#ifndef __FUNCTION_NAME__
#   ifdef __FUNCSIG__
#     define __FUNCTION_NAME__ __FUNCSIG__
#   endif
#endif
#ifndef __FUNCTION_NAME__
#   ifdef __FUNCTION__
#     define __FUNCTION_NAME__ __FUNCTION__
#   endif
#endif
#ifndef __FUNCTION_NAME__
#   ifdef __func__
#     define __FUNCTION_NAME__ __func__
#   endif
#endif
#ifndef __FUNCTION_NAME__
#   define __FUNCTION_NAME__ "<unknown function>"
#endif

#define __MANTA_EXCEPTION(message) do { \
  std::ostringstream _strm_; \
  _strm_ << __FILE__ << ":" << __LINE__ << "\n" << message; \
  throw ::manta::MantaException(_strm_.str(), __FILE__, __FUNCTION_NAME__, __LINE__); \
} while (false)

//! \brief Contract macro for function preconditions.
#define MANTA_REQUIRE(condition, message) if (!(condition)) __MANTA_EXCEPTION(message)

//! \brief Macro for general condition checking.
#define MANTA_ASSERT(condition, message) if (!(condition)) __MANTA_EXCEPTION(message)

//! \brief Macro for raising a runtime error.
#define MANTA_FAIL(message) __MANTA_EXCEPTION(message)

//! \brief Macro for raising a specific type of error.
#define MANTA_THROW(exception_type, message) \
  { \
    std::ostringstream _strm_; \
    _strm_ << __FILE__ << ":" << __LINE__ << "\n" << message; \
    throw std::runtime_error(_strm_.str()); \
  }

namespace std {

inline void format_logstream(const exception& ex, lightning::RefBundle& handler) {
  using namespace lightning;
  using namespace lightning::formatting;

  handler << NewLineIndent
          << AnsiColor8Bit(R"(""")", AnsiForegroundColor::Red)
          << AnsiColorSegment(AnsiForegroundColor::Yellow); // Exception in yellow.
  const char* begin = ex.what(), *end = ex.what();
  while (*end) {
    for (; *end && *end != '\n'; ++end); // Find next newline.
    handler << NewLineIndent << string_view(begin, end - begin);
    for (; *end && *end == '\n'; ++end); // Pass any number of newlines.
    begin = end;
  }
  handler << AnsiResetSegment
          << NewLineIndent // Reset colors to default.
          << AnsiColor8Bit(R"(""")", AnsiForegroundColor::Red);
}

} // namespace std