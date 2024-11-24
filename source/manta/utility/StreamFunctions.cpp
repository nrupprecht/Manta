//
// Created by Nathaniel Rupprecht on 7/30/23.
//

#include "manta/utility/StreamFunctions.h"

namespace manta::utility {

std::string GetUntil(IStreamContainer& stream_container, char terminator) {
  std::string output;

  char c {};
  do {
    stream_container->get(c);

    if (stream_container->eof()) {
      if (terminator == static_cast<char>(0)) {
        return output;
      }
      MANTA_FAIL("did not find the character terminator");
    }

    if (c == terminator) {
      return output;
    }

    if (c == '\\') {
      // Get the next character, even if it is a terminator.
      MANTA_REQUIRE(!stream_container->eof(), "cannot escape an EOF");
      stream_container->get(c);
      if (c != terminator) {
        // Keep any characters that were not the EOF escaped.
        output.push_back('\\');
      }
      output.push_back(c);
    }
    else {
      output.push_back(c);
    }
  } while (true);
}

}  // namespace manta::utility
