//
// Created by Nathaniel Rupprecht on 7/30/23.
//

#pragma once

#include "manta/utility/IStreamContainer.hpp"

namespace manta::utility {

std::string GetUntil(IStreamContainer& stream_container, char terminator);

}  // namespace manta::utility
