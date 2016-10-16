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
                    wssocket_.next_layer().async_handshake(
        boost::asio::ssl::stream_base::server, strand_.wrap(
            boost::bind(&session::handle_handshake, shared_from_this(),
                boost::asio::placeholders::error)));
                
              //  socket_.async_handshake(boost::asio::ssl::stream_base::server,
                //    boost::bind(&session::handle_handshake, shared_from_this(),
                  //    boost::asio::placeholders::error));    
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
        
            void session::do_read_header()
            {
                
               
            }
            
            
            void session::http_do_read(const boost::system::error_code& error)
            {
                std::cout<<"Inside http_do-read, checking wsssocket_.accept result" <<std::endl;
                if (!error)
                {
                    std::cout<<"Accepted HTTP connection, should write to it"<<std::endl;
               //     buff_.consume(buff_.size());
            //        wssocket_.read(op_,buff_);
             //       std::cout<<"After wssocket_.read"<<std::endl;
           //         wssocket_.set_option(beast::websocket::message_type{op_});
                //    wssocket_.write("asdfdf");
                //    buff_.consum(buff_.size());
                } else
                {
                    std::cout<<error.message()<<std::endl;
                    
                }
            }
            
        }
    
    }
}
