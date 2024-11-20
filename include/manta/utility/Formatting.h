//
// Created by Nathaniel Rupprecht on 7/30/23.
//

#pragma once

#include <Lightning/Lightning.h>

namespace manta::formatting {

#define LCOL8(data, color8bit) \
  ::lightning::AnsiColor8Bit((data), ::lightning::formatting::AnsiForegroundColor::color8bit)

#define FMT_COL(name, color) \
  template<typename T> \
  inline auto name (const T& data) { \
    return LCOL8(data, color); \
  }

FMT_COL(CLR, Red);
FMT_COL(CLBR, BrightRed);
FMT_COL(CLB, Blue);
FMT_COL(CLBB, BrightBlue);
FMT_COL(CLX, BrightBlack);
FMT_COL(CLW, White);
FMT_COL(CLBW, BrightWhite);
FMT_COL(CLM, Magenta);
FMT_COL(CLG, Green);
FMT_COL(CLBG, BrightGreen);
FMT_COL(CLY, Yellow);
FMT_COL(CLBY, BrightYellow);

inline std::size_t DetectIndentation(std::string_view body) {
  std::size_t indentation = std::numeric_limits<std::size_t>::max();
  while (!body.empty()) {
    // Detect indentation.
    auto whitespace_index = body.find_first_not_of(" \t");
    if (whitespace_index != std::string::npos) {
      if (body[whitespace_index] != '\n') {
        indentation = std::min(whitespace_index, indentation);
      }
      body = body.substr(whitespace_index);
    }
    else {
      return indentation == std::numeric_limits<std::size_t>::max() ? 0 : indentation;
    }

    const auto index = body.find_first_of('\n');
    if (index == std::string::npos) {
      break;
    }
    body = body.substr(index + 1);
  }

  return indentation == std::numeric_limits<std::size_t>::max() ? 0 : indentation;
}

//! \brief Take a body of text that is indented in some way, and format it to have different indentation.
//!
//! \param stream The stream to write the formatted text to.
//! \param body The body of text to format.
//! \param indent The number of spaces to indent each line.
//! \param strip_leading_whitespace If true, strip leading whitespace from each line.
//! \param detect_whitespace_to_strip If true, detect the whitespace to strip from each line, and strip the
//!                                   same number from each line.
//! \param remove_leading_empty_lines If true, remove any lines that are all whitespaces and occur before any
//!                                   line with anything other than a whitespace.
//! \param remove_trailing_empty_lines If true, remove a last line that is all whitespaces, not injecting a
//!                                    newline at the end.
inline void HandleIndentation(std::ostream& stream,
                              std::string_view body,
                              std::size_t indent,
                              bool strip_leading_whitespace    = true,
                              bool detect_whitespace_to_strip  = true,
                              bool remove_leading_empty_lines  = false,
                              bool remove_trailing_empty_lines = false) {
  std::size_t indentation = std::numeric_limits<std::size_t>::max();
  if (detect_whitespace_to_strip) {
    indentation = DetectIndentation(body);
  }

  // Inject indentation at the start of each line.
  bool first_line = true;
  while (!body.empty()) {
    if (strip_leading_whitespace) {
      // Have to be careful of lines that are composed entirely of whitespaces.
      auto whitespace_index = body.find_first_not_of(" \t");
      if (whitespace_index == std::string::npos) {
        // Last line is all whitespaces.
        if (!remove_trailing_empty_lines) {
          stream << "\n";
        }
        return;
      }
      if (body[whitespace_index] == '\n') {
        // A line other than the last line is all whitespaces.
        body = body.substr(whitespace_index + 1);
        if (!first_line || !remove_leading_empty_lines) {
          stream << "\n";
        }
        continue;
      }

      if (indentation != std::numeric_limits<std::size_t>::max()) {
        body = body.substr(indentation);
      }
      // Skip spaces and tabs, but don't skip newlines.
      else if (whitespace_index = body.find_first_not_of(" \t"); whitespace_index != std::string::npos) {
        body = body.substr(whitespace_index);
      }
    }

    if (!first_line) {
      stream << "\n";
    }
    first_line = false;

    const auto index = body.find_first_of('\n');
    for (std::size_t i = 0; i < indent; ++i) {
      stream << " ";
    }
    stream << body.substr(0, index);
    if (index == std::string::npos) {
      break;
    }
    body = body.substr(index + 1);
  }

  if (!remove_trailing_empty_lines) {
    stream << "\n";
  }
}

}  // namespace manta::formatting
