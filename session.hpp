#ifndef WEBSOCKET_APPLICATIONS_CHAT_SESSION_HPP
#define WEBSOCKET_APPLICATIONS_CHAT_SESSION_HPP

#include <string>
#include <vector>
#include <memory>
#include <beast/websocket/ssl.hpp>
#include <beast/websocket.hpp>
//#include <beast/core/placeholders.hpp>
#include <beast/core/streambuf.hpp>

#include <boost/bind.hpp>  
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/deadline_timer.hpp>

namespace websocket {
    namespace applications {
        namespace chat {
        typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket;
        typedef beast::websocket::stream<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> beastsslsocket;
        class session
            : 
              public std::enable_shared_from_this<session>,
              private boost::noncopyable
        {
        public:
            explicit session(boost::asio::io_service& io_service, 
                boost::asio::ssl::context& context);
            ssl_socket::lowest_layer_type& socket();
            void start();
        private:
            void http_handle_read();//const boost::system::error_code& error);
            void do_read_header();
            void handle_handshake(const boost::system::error_code& error);
            void http_do_read(const boost::system::error_code& error);
            void http_handle_read(const boost::system::error_code& error);
            
            boost::asio::io_service::strand strand_;            
            boost::asio::streambuf buff_;
            //beast::streambuf buff_;
            boost::asio::io_service& io_service_;
            beastsslsocket wssocket_;
            beast::websocket::opcode op_;
        };
        } // namespace chat
    } // namespace server
} // namespace websocket

#endif // WEBSOCKET_APPLICATIONS_CHAT_SESSION_HPP
