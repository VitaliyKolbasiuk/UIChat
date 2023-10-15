#pragma once
#include <iostream>
#include <boost/asio.hpp>

// messages from server to client
#define MESSAGE_FROM_CMD    "MESSAGE_FROM" // cmd;nick_name;message

// messages from client to server
#define JOIN_TO_CHAT_CMD    "JOIN_TO_CHAT"    // cmd;nick_name
#define EXIT_THE_CHAT_CMD   "EXIT_THE_CHAT"    // cmd;nick_name
#define MESSAGE_TO_ALL_CMD  "MESSAGE_TO_ALL"  // cmd;message

using namespace boost::asio;
using ip::tcp;

class IChatClient
{
public:
    virtual ~IChatClient() = default;

public:
    virtual void handleServerMessage( const std::string& command, boost::asio::streambuf& message ) = 0;
    virtual bool sendUserMessage( const std::string& message ) = 0;
    virtual void saveClientInfo( const std::string& chatClientName, const std::string& chatRoomName ) = 0;
    virtual void closeConnection() = 0;
};

class ITerminal
{
    virtual void displayMessage(std::string message) = 0;
};

