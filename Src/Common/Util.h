#ifndef SRC_COMMON_UTIL_H_
#define SRC_COMMON_UTIL_H_

#include <type_traits>

template <typename Enumeration>
auto as_integer(Enumeration const value)
    -> typename std::underlying_type<Enumeration>::type
{
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}


#endif /* SRC_COMMON_UTIL_H_ */
