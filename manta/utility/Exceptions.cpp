//
// Created by Nathaniel Rupprecht on 12/27/23.
//

#include "manta/utility/Exceptions.h"
// Other files.
#include <Lightning/Lightning.h>

namespace manta {

std::string MantaException::formatMessage(const std::string& message,
                                          const std::string& file,
                                          const std::string& function_name,
                                          int line) {
  return lightning::formatting::Format("exception raised from {}:{}, {}\n{}", file, line, function_name, message);
}

} // namespace manta
