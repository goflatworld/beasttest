#include "session.hpp"
#include <iostream>
#include <vector>

#include <boost/uuid/sha1.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>

namespace websocket {
    namespace applications {
        namespace chat {
            
            session::session(boost::asio::io_service& io_service, 
                boost::asio::ssl::context& context)
                : socket_(io_service,context), 
                  io_service_(io_service),
                  strand_(io_service),
                  wssocket_(io_service,context)
            {
            }
            
            ssl_socket::lowest_layer_type& session::socket()
            {
                return socket_.lowest_layer();
            }

            void session::start()
            {
                socket_.async_handshake(boost::asio::ssl::stream_base::server,
                    boost::bind(&session::handle_handshake, shared_from_this(),
                      boost::asio::placeholders::error));    
            }
 
            void session::handle_handshake(const boost::system::error_code& error)
            {
                if (!error)
                {
                    do_read_header();
                }
                else
                {
                }
            }
        
            void session::do_read_header()
            {
            
                        beast::http::async_read(socket_, buff_, request_,
                        strand_.wrap(
                        boost::bind(&session::http_handle_read, shared_from_this(),
                        boost::asio::placeholders::error)));
               
            }
            
            void session::http_handle_read(const boost::system::error_code& error)
            {
                if (!error)
                {
                        bool result=beast::http::is_upgrade(request_);
                        if (result)
                        {
                   //          wssocket_.async_accept(buff_.data());
                        }
                    
                }
            }

        }
    
    }
}
