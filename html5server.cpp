#include "html5server.hpp"
#include <iostream>
#include <vector>
#include <boost/bind.hpp>

namespace websocket {
    namespace applications {
        namespace chat {

        server::server(boost::asio::io_service& io_service,
            const boost::asio::ip::tcp::endpoint& endpoint)
            : acceptor_(io_service, endpoint),
              io_service_(io_service),
              context_(io_service, boost::asio::ssl::context::sslv23)
        {
            context_.set_options(
                boost::asio::ssl::context::default_workarounds
                    | boost::asio::ssl::context::no_sslv2
                    | boost::asio::ssl::context::single_dh_use);
                context_.set_password_callback(boost::bind(&server::get_password, this));
                context_.use_certificate_chain_file("/etc/oob/server.crt");
                context_.use_private_key_file("/etc/oob/server.key", boost::asio::ssl::context::pem);
                context_.use_tmp_dh_file("/etc/oob/dh2048.pem");

            start_accept();
        }

        std::string server::get_password() const
        {
            return "test";
        }
        void server::start_accept()
        {
            auto new_session = std::make_shared<session>(io_service_, context_);
            acceptor_.async_accept(new_session->socket(),
                boost::bind(&server::handle_accept, this, new_session,
                boost::asio::placeholders::error));
        }

        void server::handle_accept(
        std::shared_ptr<session> new_session,
        const boost::system::error_code& error)
        {            
            if (!error)
            {
                new_session->start();
            }
            else
            {
                std::cerr << error.message() << std::endl;
            }

            start_accept();
        }
        } // namespace chat
    } // namespace applications
} // namespace websocket