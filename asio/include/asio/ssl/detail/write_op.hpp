//
// ssl/detail/write_op.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2022 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_SSL_DETAIL_WRITE_OP_HPP
#define ASIO_SSL_DETAIL_WRITE_OP_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"

#include "asio/detail/buffer_sequence_adapter.hpp"
#include "asio/ssl/detail/engine.hpp"

#include "asio/detail/push_options.hpp"

namespace ModioAsio {
namespace ssl {
namespace detail {

template <typename ConstBufferSequence>
class write_op
{
public:
  static ASIO_CONSTEXPR const char* tracking_name()
  {
    return "ssl::stream<>::async_write_some";
  }

  write_op(const ConstBufferSequence& buffers)
    : buffers_(buffers)
  {
  }

  engine::want operator()(engine& eng,
      ModioAsio::error_code& ec,
      std::size_t& bytes_transferred) const
  {
    unsigned char storage[
      ModioAsio::detail::buffer_sequence_adapter<ModioAsio::const_buffer,
        ConstBufferSequence>::linearisation_storage_size];

    ModioAsio::const_buffer buffer =
      ModioAsio::detail::buffer_sequence_adapter<ModioAsio::const_buffer,
        ConstBufferSequence>::linearise(buffers_, ModioAsio::buffer(storage));

    return eng.write(buffer, ec, bytes_transferred);
  }

  template <typename Handler>
  void call_handler(Handler& handler,
      const ModioAsio::error_code& ec,
      const std::size_t& bytes_transferred) const
  {
    ASIO_MOVE_OR_LVALUE(Handler)(handler)(ec, bytes_transferred);
  }

private:
  ConstBufferSequence buffers_;
};

} // namespace detail
} // namespace ssl
} // namespace ModioAsio

#include "asio/detail/pop_options.hpp"

#endif // ASIO_SSL_DETAIL_WRITE_OP_HPP
