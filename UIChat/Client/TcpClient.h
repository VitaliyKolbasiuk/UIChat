#pragma once
#include <iostream>
#include <boost/asio.hpp>
#include "../Client/ClientInterfaces.h"

using namespace boost::asio;
using ip::tcp;

class TcpClient
{
    io_context&  m_ioContext;
    tcp::socket m_socket;
    std::shared_ptr<IChatClient> m_client;

public:    
    TcpClient( io_context&  ioContext, std::shared_ptr<IChatClient> client ) :
        m_ioContext(ioContext),
        m_socket(m_ioContext),
        m_client(client)
    {}

    ~TcpClient()
    {
        std::cout << "!!!! ~Client(): " << std::endl;
    }

    void execute( std::string addr, int port)
    {
        auto endpoint = tcp::endpoint(ip::address::from_string( addr.c_str()), port);

        m_socket.async_connect(endpoint, [this] (const boost::system::error_code& error)
                               {
                                   if ( error )
                                   {
                                       std::cout << "Connection error: " << error.message() << std::endl;
                                   }
                                   else
                                   {
                                       std::shared_ptr<boost::asio::streambuf> wrStreambuf = std::make_shared<boost::asio::streambuf>();
                                       std::ostream os(&(*wrStreambuf));

                                       sendMessageToServer( wrStreambuf );
                                   }
                               });
    }

    void sendMessageToServer( std::shared_ptr<boost::asio::streambuf> streambuf )
    {
        async_write( m_socket, *streambuf,
                    [this,streambuf] ( const boost::system::error_code& error, std::size_t bytes_transferred )
                    {
                        if ( error )
                        {
                            std::cout << "Client write error: " << error.message() << std::endl;
                        }
                        else
                        {
                            readResponse();
                        }
                    });
    }

    void readResponse()
    {
        // Receive the response from the server
        std::shared_ptr<boost::asio::streambuf> streambuf = std::make_shared<boost::asio::streambuf>();

        boost::asio::async_read_until( m_socket, *streambuf, '\n',
                                      [streambuf,this]( const boost::system::error_code& error_code, std::size_t bytes_transferred )
                                      {
                                          if ( error_code )
                                          {
                                              std::cout << "Client read error: " << error_code.message() << std::endl;
                                          }
                                          else
                                          {
                                              {
                                                  std::istream response( &(*streambuf) );

                                                  std::string command;
                                                  std::getline( response, command, ';' );

                                                  m_client->handleServerMessage( command, *streambuf );

                                              }

                                              streambuf->consume(bytes_transferred);
                                              readResponse();
                                          }
                                      });
    }
};
