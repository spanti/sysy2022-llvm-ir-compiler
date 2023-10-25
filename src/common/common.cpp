#include <cassert>
#include <cctype>
#include <climits>
#include <sstream>
#include "commom.h"
#include "errors.h"
int32_t concat(int32_t bottom, int32_t top) {
  uint32_t s =
      static_cast<uint32_t>(bottom) | (static_cast<uint32_t>(top) << 16);
  if (s <= static_cast<uint32_t>(std::numeric_limits<int32_t>::max()))
    return static_cast<int32_t>(s);
  else if (s == static_cast<uint32_t>(std::numeric_limits<int32_t>::min()))
    return std::numeric_limits<int32_t>::min();
  else
    return -static_cast<int32_t>(std::numeric_limits<uint32_t>::max() - s + 1);
}

bool startswith(const std::string &s1, const std::string &s2) {
  if (s1.length() < s2.length())
    return false;
  for (size_t i = 0; i < s2.length(); ++i)
    if (s1[i] != s2[i])
      return false;
  return true;
}

#define check_int32(x)                                                         \
  if ((x) > 2147483648ll)                                                      \
  throw InvalidLiteral("integer literal out of range")

int32_t parse_int32_literal(const std::string &s) {
  int64_t ret = 0;
  if (startswith(s, "0x") || startswith(s, "0X")) {
    for (size_t i = 2; i < s.length(); ++i) {
      if (s[i] >= '0' && s[i] <= '9') {
        ret = ret * 16 + s[i] - '0';
      } else if (s[i] >= 'a' && s[i] <= 'f') {
        ret = ret * 16 + s[i] - 'a' + 10;
      } else {
        assert(s[i] >= 'A' && s[i] <= 'F');
        ret = ret * 16 + s[i] - 'A' + 10;
      }
      check_int32(ret);
    }
  } else if (startswith(s, "0")) {
    for (size_t i = 1; i < s.length(); ++i) {
      if (s[i] >= '0' && s[i] <= '7') {
        ret = ret * 8 + s[i] - '0';
      } else
        throw InvalidLiteral("invalid octal interger literal");
      check_int32(ret);
    }
  } else {
    for (char ch : s) {
      assert(ch >= '0' && ch <= '9');
      ret = ret * 10 + ch - '0';
      check_int32(ret);
    }
  }
  if (ret <= INT_MAX)
    return static_cast<int32_t>(ret);
  else
    return INT_MIN;
}
