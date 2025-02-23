//
// Created by Nathaniel Rupprecht on 7/30/23.
//

#pragma once

#include <string>
#include "manta/utility/IStreamContainer.h"


namespace manta::utility {

std::string GetUntil(IStreamContainer& stream_container, char terminator);

}  // namespace manta::utility
