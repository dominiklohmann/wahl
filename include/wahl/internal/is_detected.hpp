// SPDX-License-Identifier: BSL-1.0

#pragma once

#if __has_include(<experimental/type_traits>)

#include <experimental/type_traits>

namespace wahl::internal {

using std::experimental::detected_or;
using std::experimental::detected_or_t;
using std::experimental::detected_t;
using std::experimental::is_detected;
using std::experimental::is_detected_v;
using std::experimental::nonesuch;

} // namespace wahl::internal

#else

#include <type_traits>

namespace wahl::internal {

namespace detail {

#if defined(_MSC_VER)
template <typename... Ts> struct void_t_impl { using type = void; };
template <typename... Ts>
using void_t = typename detail::void_t_impl<Ts...>::type;
#else
using std::void_t;
#endif

template <class Default, class AlwaysVoid, template <class...> class Op,
          class... Args>
struct detector {
  using value_t = std::false_type;
  using type = Default;
};

template <class Default, template <class...> class Op, class... Args>
struct detector<Default, void_t<Op<Args...>>, Op, Args...> {
  using value_t = std::true_type;
  using type = Op<Args...>;
};

} // namespace detail

struct nonesuch {
  nonesuch() = delete;
  ~nonesuch() = delete;
  nonesuch(const nonesuch &) = delete;
  void operator=(const nonesuch &) = delete;
};

template <template <class...> class Op, class... Args>
using is_detected =
    typename detail::detector<nonesuch, void, Op, Args...>::value_t;

template <template <class...> class Op, class... Args>
inline constexpr bool is_detected_v = is_detected<Op, Args...>::value;

template <template <class...> class Op, class... Args>
using detected_t = typename detail::detector<nonesuch, void, Op, Args...>::type;

template <class Default, template <class...> class Op, class... Args>
using detected_or = detail::detector<Default, void, Op, Args...>;

template <class Default, template <class...> class Op, class... Args>
using detected_or_t =
    typename detail::detector<Default, void, Op, Args...>::type;

} // namespace wahl::internal

#endif
