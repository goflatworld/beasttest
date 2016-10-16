#include "html5server.hpp"
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>

int main(int argc, char* argv[])
{

  try
  {
    if (argc <2)
    { 
        std::cerr << "Usage: central_server <port> [<port> ...]\n";
        return 1;
    }

    boost::asio::io_service io_service;

    std::list<websocket::applications::chat::server> servers;
    for (int i = 1; i < argc; ++i)
    {
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), std::atoi(argv[i]));
        servers.emplace_back(io_service, endpoint);
    }
    io_service.run();
  }
  catch (std::exception& e)
  {
  }
  return 0;
}
