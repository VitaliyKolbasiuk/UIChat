#pragma once
#include <iostream>
#include <boost/asio.hpp>

// messages from server to client
#define MESSAGE_FROM_CMD     "MESSAGE_FROM"      // cmd;message;chat_room;username
#define UPDATE_THE_USER_TABLE_CMD "UPDATE_THE_USER_TABLE" // cmd;message;

// messages from client to server
#define JOIN_TO_CHAT_CMD     "JOIN_TO_CHAT"      // cmd;chat_room;username
#define MESSAGE_TO_ALL_CMD   "MESSAGE_TO_ALL"    // cmd;message;chat_room;username
#define EXIT_THE_CHAT_CMD    "EXIT_THE_CHAT"     // cmd;chatroom;username

using namespace boost::asio;
using ip::tcp;


class ISession
{
public:
    virtual ~ISession() = default;

    //virtual void sendMessage( std::string message ) = 0;
    virtual void sendMessage( std::shared_ptr<boost::asio::streambuf> wrStreambuf ) = 0;

};

class IChat
{
protected:
    virtual ~IChat() = default;

public:
    virtual void handleMessage( ISession&, boost::asio::streambuf& message ) = 0;
    //virtual void closeConnection(ISession& client) = 0;
};

class IServer
{
public:
    virtual void execute() = 0;
};

IServer* createServer(io_context& ioContext, IChat& chat, int port);
