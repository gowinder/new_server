//
// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

static const int max_buffer_len = 1024 * 512;
using namespace boost::asio;
using boost::asio::ip::tcp;
using namespace std;
class session
{
public:
  session(boost::asio::io_service& io_service)
    : socket_(io_service)
  {
  }
  tcp::socket& socket()
  {
    return socket_;
  }
  void start()
  {
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
        boost::bind(&session::handle_read, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  }
  void handle_read(const boost::system::error_code& error,
      size_t bytes_transferred)
  {
    if (!error)
    {
      boost::asio::async_write(socket_,
          boost::asio::buffer(data_, bytes_transferred),
          boost::bind(&session::handle_write, this,
            boost::asio::placeholders::error));

      cout << "thread " << boost::this_thread::get_id() << "read" << endl;
//
//      boost::this_thread::get_id();
    }
    else
    {
      delete this;
    }
  }
  void handle_write(const boost::system::error_code& error)
  {
    if (!error)
    {
      socket_.async_read_some(boost::asio::buffer(data_, max_length),
          boost::bind(&session::handle_read, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }
    else
    {
      delete this;
    }
  }
private:
  tcp::socket socket_;
  enum { max_length = max_buffer_len };
  char data_[max_length];
};
class server
{
public:
  server(boost::asio::io_service& io_service, short port)
    : io_service_(io_service),
      acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
  {
    session* new_session = new session(io_service_);
    acceptor_.async_accept(new_session->socket(),
        boost::bind(&server::handle_accept, this, new_session,
          boost::asio::placeholders::error));
  }
  void handle_accept(session* new_session,
      const boost::system::error_code& error)
  {
    if (!error)
    {
      new_session->start();
      new_session = new session(io_service_);
      acceptor_.async_accept(new_session->socket(),
          boost::bind(&server::handle_accept, this, new_session,
            boost::asio::placeholders::error));
    }
    else
    {
      delete new_session;
    }
  }
private:
  boost::asio::io_service& io_service_;
  tcp::acceptor acceptor_;
};




int main(int argc, char* argv[])
{
  try
  {
	  typedef std::vector<boost::shared_ptr<boost::thread> > SP_VECT_THREAD;
	  SP_VECT_THREAD	g_vect_thread;

//    if (argc != 2)
//    {
//      std::cerr << "Usage: async_tcp_echo_server <port>/n";
//      return 1;
//    }
	int port = 2234;
    boost::asio::io_service io_service;
    using namespace std; // For atoi.
    server s(io_service, port/*atoi(argv[1])*/);

    const int TRHEAD_COUNT = 4;
    for(int i = 0; i < TRHEAD_COUNT; i++)
    {
        boost::shared_ptr<boost::thread> sp(new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service)));
        g_vect_thread.push_back(sp);
    }
    getchar();
    for(int i = 0; i < TRHEAD_COUNT; i++)
    {
    	g_vect_thread[i]->join();
    }
  //  io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "/n";
  }
  return 0;
}
