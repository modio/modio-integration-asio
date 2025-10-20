//
// socks4.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2022 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef SOCKS4_HPP
#define SOCKS4_HPP

#include <string>
#include <asio.hpp>
#include <boost/array.hpp>

namespace socks4 {

const unsigned char version = 0x04;

class request
{
public:
  enum command_type
  {
    connect = 0x01,
    bind = 0x02
  };

  request(command_type cmd, const ModioAsio::ip::tcp::endpoint& endpoint,
      const std::string& user_id)
    : version_(version),
      command_(cmd),
      user_id_(user_id),
      null_byte_(0)
  {
    // Only IPv4 is supported by the SOCKS 4 protocol.
    if (endpoint.protocol() != ModioAsio::ip::tcp::v4())
    {
      throw ModioAsio::system_error(
          ModioAsio::error::address_family_not_supported);
    }

    // Convert port number to network byte order.
    unsigned short port = endpoint.port();
    port_high_byte_ = (port >> 8) & 0xff;
    port_low_byte_ = port & 0xff;

    // Save IP address in network byte order.
    address_ = endpoint.address().to_v4().to_bytes();
  }

  boost::array<ModioAsio::const_buffer, 7> buffers() const
  {
    boost::array<ModioAsio::const_buffer, 7> bufs =
    {
      {
        ModioAsio::buffer(&version_, 1),
        ModioAsio::buffer(&command_, 1),
        ModioAsio::buffer(&port_high_byte_, 1),
        ModioAsio::buffer(&port_low_byte_, 1),
        ModioAsio::buffer(address_),
        ModioAsio::buffer(user_id_),
        ModioAsio::buffer(&null_byte_, 1)
      }
    };
    return bufs;
  }

private:
  unsigned char version_;
  unsigned char command_;
  unsigned char port_high_byte_;
  unsigned char port_low_byte_;
  ModioAsio::ip::address_v4::bytes_type address_;
  std::string user_id_;
  unsigned char null_byte_;
};

class reply
{
public:
  enum status_type
  {
    request_granted = 0x5a,
    request_failed = 0x5b,
    request_failed_no_identd = 0x5c,
    request_failed_bad_user_id = 0x5d
  };

  reply()
    : null_byte_(0),
      status_()
  {
  }

  boost::array<ModioAsio::mutable_buffer, 5> buffers()
  {
    boost::array<ModioAsio::mutable_buffer, 5> bufs =
    {
      {
        ModioAsio::buffer(&null_byte_, 1),
        ModioAsio::buffer(&status_, 1),
        ModioAsio::buffer(&port_high_byte_, 1),
        ModioAsio::buffer(&port_low_byte_, 1),
        ModioAsio::buffer(address_)
      }
    };
    return bufs;
  }

  bool success() const
  {
    return null_byte_ == 0 && status_ == request_granted;
  }

  unsigned char status() const
  {
    return status_;
  }

  ModioAsio::ip::tcp::endpoint endpoint() const
  {
    unsigned short port = port_high_byte_;
    port = (port << 8) & 0xff00;
    port = port | port_low_byte_;

    ModioAsio::ip::address_v4 address(address_);

    return ModioAsio::ip::tcp::endpoint(address, port);
  }

private:
  unsigned char null_byte_;
  unsigned char status_;
  unsigned char port_high_byte_;
  unsigned char port_low_byte_;
  ModioAsio::ip::address_v4::bytes_type address_;
};

} // namespace socks4

#endif // SOCKS4_HPP
