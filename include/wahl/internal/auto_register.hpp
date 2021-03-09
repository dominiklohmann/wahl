// SPDX-License-Identifier: BSL-1.0

#pragma once

namespace wahl::internal {

template <class T, class F> struct auto_register {
  static bool auto_register_reg_;
  static bool auto_register_reg_init_() {
    F::template apply<T>();
    return true;
  }

  auto_register() { (void)auto_register_reg_; }
};

template <class T, class F>
bool auto_register<T, F>::auto_register_reg_ =
    auto_register<T, F>::auto_register_reg_init_();

} // namespace wahl::internal
