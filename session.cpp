#include "session.hpp"
#include <iostream>
#include <vector>

namespace websocket {
    namespace applications {
        namespace chat {            
            session::session(boost::asio::io_service& io_service, 
                boost::asio::ssl::context& context)
                : io_service_(io_service),
                  strand_(io_service),
                  wssocket_(io_service,context)
            {
            }
            
            ssl_socket::lowest_layer_type& session::socket()
            {
                return wssocket_.lowest_layer();
            }

            void session::start()
            {
                    wssocket_.next_layer().async_handshake(
                        boost::asio::ssl::stream_base::server, 
                            boost::bind(&session::handle_handshake, shared_from_this(),
                                boost::asio::placeholders::error));
            }
 
            void session::handle_handshake(const boost::system::error_code& error)
            {
                if (!error)
                {
                    wssocket_.async_accept(strand_.wrap(
                        boost::bind(&session::http_do_read, shared_from_this(),
                        boost::asio::placeholders::error)));
                }
                else
                {
                    std::cout<<error.message()<<std::endl;
                }
            }
            
            void session::http_do_read(const boost::system::error_code& error)
            {
                if (!error)
                {
                    wssocket_.async_read(op_, buff_,
                        strand_.wrap(
                        boost::bind(&session::http_handle_read, shared_from_this(),
                        boost::asio::placeholders::error)));
                } else
                {
                    std::cout<<error.message()<<std::endl;
                }
            }
            
            void session::http_handle_read(const boost::system::error_code& error)
            {
                wssocket_.set_option(beast::websocket::message_type(beast::websocket::opcode::text));
                if (!error)
                {
                    std::string msg="Hello World";
                    
                    wssocket_.async_write(
                        //buff_.data(),       //Works if I send it back directly
                       // boost::asio::buffer("Hello World!"),  //Works if I directly put string in here.
                       boost::asio::buffer(msg),   //Doesn't work using string var
                        strand_.wrap(boost::bind(&session::http_do_read, shared_from_this(),
                        boost::asio::placeholders::error))); 
                    
                    buff_.consume(buff_.size());
                } else
                    std::cout<<error.message()<<std::endl;
            }
        }
    }
}
