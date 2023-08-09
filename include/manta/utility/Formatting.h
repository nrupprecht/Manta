//
// Created by Nathaniel Rupprecht on 7/30/23.
//

#pragma once

#include <Lightning/Lightning.h>

namespace manta::formatting {

#define LCOL8(data, color8bit) \
  lightning::AnsiColor8Bit((data), lightning::formatting::AnsiForegroundColor::color8bit)

template<typename T>
auto CLBB(const T& data) {
  return LCOL8(data, BrightBlue);
}

template<typename T>
auto CLX(const T& data) {
  return LCOL8(data, BrightBlack);
}

template<typename T>
auto CLW(const T& data) {
  return LCOL8(data, White);
}

template<typename T>
auto CLB(const T& data) {
  return LCOL8(data, Blue);
}

template<typename T>
auto CLM(const T& data) {
  return LCOL8(data, Magenta);
}

template<typename T>
auto CLBG(const T& data) {
  return LCOL8(data, BrightGreen);
}

template<typename T>
auto CLY(const T& data) {
  return LCOL8(data, Yellow);
}

}  // namespace manta::formatting
