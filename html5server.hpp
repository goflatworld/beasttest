#ifndef WEBSOCKET_APPLICATIONS_CHAT_SERVER_HPP
#define WEBSOCKET_APPLICATIONS_CHAT_SERVER_HPP

#include <string>
#include <vector>
#include <beast/websocket/ssl.hpp>
#include <beast/websocket.hpp>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio/ssl.hpp>
#include "session.hpp"

namespace websocket {
    namespace applications {
        namespace chat {
        class server
        {
        public:
            explicit server(boost::asio::io_service& io_service,
                    const boost::asio::ip::tcp::endpoint& endpoint);
            /// Run the server's io_service loop.
            void run();

        private:
            void start_accept();
            std::string get_password() const;
            void handle_accept( std::shared_ptr<session> new_session,const boost::system::error_code& e);
            void handle_stop();
            boost::asio::io_service& io_service_;
            boost::asio::ip::tcp::acceptor acceptor_;
            
            boost::asio::ssl::context context_;
        };
        } // namespace chat
    } // namespace applications
} // namespace websocket
#endif // WEBSOCKET_APPLICATIONS_CHAT_SERVER_HPP