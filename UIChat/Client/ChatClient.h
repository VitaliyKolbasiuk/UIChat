#pragma once

#include <iostream>
#include <thread>
#include <string>
#include <boost/tokenizer.hpp>
#include <QObject>

#include "ClientInterfaces.h"
#include "TcpClient.h"
//#include "../Log.h"

class QChatClient : public QObject, public IChatClient
{
   Q_OBJECT

public:

signals:
    void OnMessageReceived(QString username, QString message);

    void OnTableChanged(QString username);

private:
    std::weak_ptr<TcpClient>  m_tcpClient;
    std::string m_chatClientName;
    std::string m_chatRoomName;

public:
    QChatClient() {

    }

    void setTcpClient(std::weak_ptr<TcpClient> tcpClient)
    {
        m_tcpClient = tcpClient;
    }

    virtual void saveClientInfo(const std::string& chatClientName, const std::string& chatRoomName) override
    {
        m_chatClientName = chatClientName;
        m_chatRoomName = chatRoomName;
    }

    virtual void handleServerMessage(const std::string& command, boost::asio::streambuf& message) override
    {
        if ( message.size() <= 0 )
        {
            return;
        }

        std::istringstream input;
        input.str(std::string((const char*)message.data().data(), message.size()));

        if (command == MESSAGE_FROM_CMD)
        {
            std::string username;
            std::string message;

            std::getline(input, message, ';');
            std::getline(input, username, ';');

            if (username != m_chatClientName)
            {
                emit OnMessageReceived(QString::fromStdString("<HTML> <font color=\"yellow\">" + username + " </font>" ), QString::fromStdString(message));
            }
            else
            {
                emit OnMessageReceived(QString::fromStdString("<HTML> <font color=\"green\">" + username + ": </font>"), QString::fromStdString(message));
            }
        }
        else if (command == UPDATE_THE_USER_TABLE_CMD)
        {
            std::string username;
            std::getline(input, username, ';');
            emit OnTableChanged(QString::fromStdString(username));
        }
    }
    virtual bool sendUserMessage(const std::string& message) override
    {
        std::shared_ptr<boost::asio::streambuf> wrStreambuf = std::make_shared<boost::asio::streambuf>();
        std::ostream os(&(*wrStreambuf));
        os << message + m_chatRoomName + ";" + m_chatClientName + ";\n";
        if (auto tcpClient = m_tcpClient.lock(); tcpClient )
        {
            tcpClient->sendMessageToServer(wrStreambuf);
            return true;
        }
        else
        {
            return false;
        }
    }

    virtual void closeConnection() override
    {
        sendUserMessage(EXIT_THE_CHAT_CMD ";");
        exit(1);
    }
};
