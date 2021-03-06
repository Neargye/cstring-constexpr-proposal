// Licensed under the MIT License <http://opensource.org/licenses/MIT>.
// SPDX-License-Identifier: MIT
// Copyright (c) 2019 - 2020 Daniil Goncharov <neargye@gmail.com>.
//
// Permission is hereby  granted, free of charge, to any  person obtaining a copy
// of this software and associated  documentation files (the "Software"), to deal
// in the Software  without restriction, including without  limitation the rights
// to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
// copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
// IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
// FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
// AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
// LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <cstddef> // NULL, size_t
#include <cstring>
#include "detail.hpp"

namespace nstd {

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4996)
#endif

constexpr char* strcpy(char* dest, const char* src) {
  if (detail::is_constant_evaluated()) {
    // https://github.com/freebsd/freebsd/blob/master/sys/libkern/strcpy.c
    if (dest != nullptr && src != nullptr) {
      char* d = dest;
      for (; (*d = *src) != '\0'; ++d, ++src) {
      }
    }

    return dest;
  } else {
    return std::strcpy(dest, src);
  }
}

constexpr char* strncpy(char* dest, const char* src, std::size_t count) {
  if (detail::is_constant_evaluated()) {
    // https://github.com/freebsd/freebsd/blob/master/sys/libkern/strncpy.c
    if (dest != nullptr && src != nullptr && count != 0) {
      char* d = dest;
      do {
        if ((*d++ = *src++) == '\0') {
          while (--count != 0) {
            *d++ = '\0';
          }
          break;
        }
      } while (--count != 0);
    }

    return dest;
  } else {
    return std::strncpy(dest, src, count);
  }
}

constexpr char* strcat(char* dest, const char* src) {
  if (detail::is_constant_evaluated()) {
    // https://github.com/freebsd/freebsd/blob/master/sys/libkern/strcat.c
    if (dest != nullptr && src != nullptr) {
      char* d = dest;
      for (; *d != '\0'; ++d) {
      }
      while ((*d++ = *src++) != '\0') {
      }
    }

    return dest;
  } else {
    return std::strcat(dest, src);
  }
}

constexpr char* strncat(char* dest, const char* src, std::size_t count) {
  if (detail::is_constant_evaluated()) {
    // https://github.com/freebsd/freebsd/blob/master/sys/libkern/strncat.c
    if (dest != nullptr && src != nullptr && count != 0) {
      char* d = dest;
      for (; *d != '\0'; ++d) {
      }
      do {
        if ((*d = *src++) == '\0') {
          break;
        }
        d++;
      } while (--count != 0);
      *d = '\0';
    }

    return dest;
  } else {
    return std::strncat(dest, src, count);
  }
}

constexpr std::size_t strlen(const char* str) {
  if (detail::is_constant_evaluated()) {
#if defined(__GNUC__) && !defined(__clang__)
    // https://github.com/gcc-mirror/gcc/blob/5c7634a0e5f202935aa6c11b6ea953b8bf80a00a/libstdc%2B%2B-v3/include/bits/char_traits.h#L329
    if (str != nullptr) {
      std::size_t i = 0;
      while (str[i] != '\0') {
        ++i;
      }

      return i;
    }

    return 0;
#elif defined(__clang__) || defined(_MSC_VER)
    return __builtin_strlen(str);
#endif
  } else {
    return std::strlen(str);
  }
}

constexpr int strcmp(const char* lhs, const char* rhs) {
  if (detail::is_constant_evaluated()) {
    // https://github.com/freebsd/freebsd/blob/master/sys/libkern/strcmp.c
    if (lhs == nullptr || rhs == nullptr) {
      return rhs != nullptr ? -1 : 1;
    }

    while (*lhs == *rhs++) {
      if (*lhs++ == '\0') {
        return 0;
      }
    }

    return static_cast<unsigned int>(*lhs) -static_cast<unsigned int>(*(rhs - 1));
  } else {
    return std::strcmp(lhs, rhs);
  }
}

constexpr int strncmp(const char* lhs, const char* rhs, std::size_t count) {
  if (detail::is_constant_evaluated()) {
#if defined(__GNUC__) && !defined(__clang__)
    // https://github.com/gcc-mirror/gcc/blob/13b9cbfc32fe3ac4c81c4dd9c42d141c8fb95db4/libstdc%2B%2B-v3/include/bits/char_traits.h#L655
    if (lhs == nullptr || rhs == nullptr) {
      return rhs != nullptr ? -1 : 1;
    }

    for (std::size_t i = 0; i < count; ++i) {
      if (lhs[i] != rhs[i]) {
        return lhs[i] < rhs[i] ? -1 : 1;
      } else if (lhs[i] == '\0') {
        return 0;
      }
    }

    return 0;
#elif defined(__clang__) || defined(_MSC_VER)
    return __builtin_memcmp(lhs, rhs, count);
#endif
  } else {
    return std::strncmp(lhs, rhs, count);
  }
}

constexpr const char* strchr(const char* str, int ch) {
  if (detail::is_constant_evaluated()) {
    // Naive implementation.
    if (str != nullptr) {
      for (std::size_t i = 0; str[i] != '\0'; ++i) {
        if (str[i] == ch) {
          return &str[i];
        }
      }
    }

    return nullptr;
  } else {
    return std::strchr(str, ch);
  }
}

constexpr char* strchr(char* str, int ch) {
  if (detail::is_constant_evaluated()) {
    // Naive implementation.
    if (str != nullptr) {
      for (std::size_t i = 0; str[i] != '\0'; ++i) {
        if (str[i] == ch) {
          return &str[i];
        }
      }
    }

    return nullptr;
  } else {
    return std::strchr(str, ch);
  }
}

constexpr const char* strrchr(const char* str, int ch) {
  if (detail::is_constant_evaluated()) {
    // Naive implementation.
    int p = -1;
    if (str != nullptr) {
      for (int i = 0; str[i] != '\0'; ++i) {
        if (str[i] == ch) {
          p = i;
        }
      }
    }

    return p != -1 ? &str[p] : nullptr;
  } else {
    return std::strrchr(str, ch);
  }
}

constexpr char* strrchr(char* str, int ch) {
  if (detail::is_constant_evaluated()) {
    // Naive implementation.
    int p = -1;
    if (str != nullptr) {
      for (int i = 0; str[i] != '\0'; ++i) {
        if (str[i] == ch) {
          p = i;
        }
      }
    }


    return p != -1 ? &str[p] : nullptr;
  } else {
    return std::strrchr(str, ch);
  }
}

constexpr std::size_t strspn(const char* dest, const char* src) {
  if (detail::is_constant_evaluated()) {
    // Naive implementation.
    std::size_t i = 0;
    if (dest != nullptr) {
      while (dest[i] != '\0' && strchr(src, dest[i]) != nullptr) {
        ++i;
      }
    }

    return i;
  } else {
    return std::strspn(dest, src);
  }
}

constexpr std::size_t strcspn(const char* dest, const char* src) {
  if (detail::is_constant_evaluated()) {
    // Naive implementation.
    std::size_t i = 0;
    if (dest != nullptr) {
      while (dest[i] != '\0' && strchr(src, dest[i]) == nullptr) {
        ++i;
      }
    }

    return i;
  } else {
    return std::strcspn(dest, src);
  }
}

constexpr const char* strpbrk(const char* dest, const char* breakset) {
  if (detail::is_constant_evaluated()) {
    // Naive implementation.
    if (dest != nullptr && breakset != nullptr) {
      for (std::size_t i = 0; dest[i] != '\0'; ++i) {
        if (strchr(breakset, dest[i]) != nullptr) {
          return &dest[i];
        }
      }
    }

    return nullptr;
  } else {
    return std::strpbrk(dest, breakset);
  }
}

constexpr char* strpbrk(char* dest, const char* breakset) {
  if (detail::is_constant_evaluated()) {
    // Naive implementation.
    if (dest != nullptr && breakset != nullptr) {
      for (std::size_t i = 0; dest[i] != '\0'; ++i) {
        if (strchr(breakset, dest[i]) != nullptr) {
          return &dest[i];
        }
      }
    }

    return nullptr;
  } else {
    return std::strpbrk(dest, breakset);
  }
}

constexpr const char* strstr(const char* str, const char* target) {
  if (detail::is_constant_evaluated()) {
    // https://github.com/freebsd/freebsd/blob/master/sys/libkern/strstr.c
    if (char c = target != nullptr ? *target++ : '\0'; c != '\0' && str != nullptr) {
      std::size_t len = strlen(target);
      do {
        char sc = {};
        do {
          if ((sc = *str++) == '\0') {
            return nullptr;
          }
        } while (sc != c);
      } while (strncmp(str, target, len) != 0);
      --str;
    }

    return str;
  } else {
    return std::strstr(str, target);
  }
}

constexpr char* strstr(char* str, const char* target) {
  if (detail::is_constant_evaluated()) {
    // https://github.com/freebsd/freebsd/blob/master/sys/libkern/strstr.c
    if (char c = target != nullptr ? *target++ : '\0'; c != '\0' && str != nullptr) {
      std::size_t len = strlen(target);
      do {
        char sc = {};
        do {
          if ((sc = *str++) == '\0') {
            return nullptr;
          }
        } while (sc != c);
      } while (strncmp(str, target, len) != 0);
      --str;
    }

    return str;
  } else {
    return std::strstr(str, target);
  }
}

// Unlike std::strtok, this function does not update static storage: it stores the parser state in the user-provided location.
constexpr char* strtok(char* str, const char* delim, char** ptr) {
  // Naive implementation.
  if (str == nullptr && (str = *ptr) == nullptr) {
    return nullptr;
  }

  str += strspn(str, delim);
  if (*str == '\0') {
    return *ptr = nullptr;
  }

  *ptr = str + strcspn(str, delim);
  if (**ptr != '\0') {
    *(*ptr)++ = '\0';
  } else {
    *ptr = nullptr;
  }

  return str;
}

constexpr const void* memchr(const void* ptr, int ch, std::size_t count) {
  if (detail::is_constant_evaluated()) {
#if defined(__GNUC__) && !defined(__clang__) || defined(_MSC_VER)
    (void)ptr;
    (void)ch;
    (void)count;
    return 0; //TODO
#elif defined(__clang__)
    return __builtin_memchr(ptr, ch, count);
#endif
  } else {
    return std::memchr(ptr, ch, count);
  }
}

constexpr void* memchr(void* ptr, int ch, std::size_t count) {
  if (detail::is_constant_evaluated()) {
#if defined(__GNUC__) && !defined(__clang__) || defined(_MSC_VER)
    (void)ptr;
    (void)ch;
    (void)count;
    return 0; //TODO
#elif defined(__clang__)
    return __builtin_memchr(ptr, ch, count);
#endif
  } else {
    return std::memchr(ptr, ch, count);
  }
}

constexpr int memcmp(const void* lhs, const void* rhs, std::size_t count) {
  if (detail::is_constant_evaluated()) {
#if defined(__GNUC__) && !defined(__clang__)
    (void)lhs;
    (void)rhs;
    (void)count;
    return 0; //TODO
#elif defined(__clang__) || defined(_MSC_VER)
    return __builtin_memcmp(lhs, rhs, count);
#endif
  } else {
    return std::memcmp(lhs, rhs, count);
  }
}

constexpr void* memset(void* dest, int ch, std::size_t count) {
  if (detail::is_constant_evaluated()) {
#if defined(__GNUC__) || defined(__clang__) || defined(_MSC_VER)
    (void)dest;
    (void)ch;
    (void)count;
    return 0; //TODO
#endif
  } else {
    return std::memset(dest, ch, count);
  }
}

constexpr void* memcpy(void* dest, const void* src, std::size_t count) {
  if (detail::is_constant_evaluated()) {
#if defined(__GNUC__) && !defined(__clang__) || defined(_MSC_VER)
    (void)dest;
    (void)src;
    (void)count;
    return 0; //TODO
#elif defined(__clang__)
    return __builtin_memcpy(dest, src, count);
#endif
  } else {
    return std::memcpy(dest, src, count);
  }
}

constexpr void* memmove(void* dest, const void* src, std::size_t count) {
  if (detail::is_constant_evaluated()) {
#if defined(__GNUC__) && !defined(__clang__) || defined(_MSC_VER)
    (void)dest;
    (void)src;
    (void)count;
    return 0; //TODO
#elif defined(__clang__)
    return __builtin_memmove(dest, src, count);
#endif
  } else {
    return std::memmove(dest, src, count);
  }
}

#if defined(_MSC_VER)
#  pragma warning(pop)
#endif

} // namespace nstd
