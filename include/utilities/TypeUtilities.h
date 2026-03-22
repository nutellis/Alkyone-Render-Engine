//
// Created by Nutellis on 08-Mar-26.
//

#ifndef ALKYONERENDERENGINE_TYPEUTILITIES_H
#define ALKYONERENDERENGINE_TYPEUTILITIES_H

#include <type_traits>

//TODO: hide this behind implementation file to avoid recompilation every time there is a change


template<typename T>
constexpr std::enable_if_t<std::is_enum_v<T>, std::underlying_type_t<T>>Underlying(T value) noexcept {
    return static_cast<std::underlying_type_t<T>>(value);
}

template<typename T>
requires std::is_enum_v<T> && std::is_unsigned_v<std::underlying_type_t<T>>
constexpr T operator|(T a, T b) noexcept {
    return static_cast<T>(Underlying(a) | Underlying(b));
}

template<typename T>
requires std::is_enum_v<T> && std::is_unsigned_v<std::underlying_type_t<T>>
constexpr T operator&(T a, T b) noexcept {
    return static_cast<T>(Underlying(a) & Underlying(b));
}

#endif //ALKYONERENDERENGINE_TYPEUTILITIES_H