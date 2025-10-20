//
// ip/impl/address.hpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2022 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_IP_IMPL_ADDRESS_HPP
#define ASIO_IP_IMPL_ADDRESS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#if !defined(ASIO_NO_IOSTREAM)

#include "asio/detail/throw_error.hpp"

#include "asio/detail/push_options.hpp"

namespace ModioAsio {
namespace ip {

#if !defined(ASIO_NO_DEPRECATED)

inline address address::from_string(const char* str)
{
  return ModioAsio::ip::make_address(str);
}

inline address address::from_string(
    const char* str, ModioAsio::error_code& ec)
{
  return ModioAsio::ip::make_address(str, ec);
}

inline address address::from_string(const std::string& str)
{
  return ModioAsio::ip::make_address(str);
}

inline address address::from_string(
    const std::string& str, ModioAsio::error_code& ec)
{
  return ModioAsio::ip::make_address(str, ec);
}

#endif // !defined(ASIO_NO_DEPRECATED)

template <typename Elem, typename Traits>
std::basic_ostream<Elem, Traits>& operator<<(
    std::basic_ostream<Elem, Traits>& os, const address& addr)
{
  return os << addr.to_string().c_str();
}

} // namespace ip
} // namespace ModioAsio

#include "asio/detail/pop_options.hpp"

#endif // !defined(ASIO_NO_IOSTREAM)

#endif // ASIO_IP_IMPL_ADDRESS_HPP
