#pragma once

#include <iostream>
#include <thread>
#include <string>
#include <boost/tokenizer.hpp>

#include "ClientInterfaces.h"
//#include "../Log.h"

// connect <room> as <username>
// <message>
// !q

class Terminal : public ITerminal
{
    IChatClient& m_userClient;
public:
    Terminal(IChatClient& client) : m_userClient(client){}

    std::string parseInput(std::string input)
    {
        boost::char_separator<char> sep(" ");
        boost::tokenizer<boost::char_separator<char>> tokens(input, sep);
        auto token = tokens.begin();
        if (token == tokens.end())
        {
            std::cout << "invalid command" << std::endl;
            return "";
        }
        if (*token == "connect")
        {
            auto roomName = (++token);
            if (roomName == tokens.end())
            {
                std::cout << "invalid command" << std::endl;
                return "";
            }
            if (*(++token) != "as")
            {
                std::cout << "invalid command" << std::endl;
                return "";
            }
            auto username = (++token);
            if (username == tokens.end())
            {
                std::cout << "invalid command" << std::endl;
                return "";
            }
            m_userClient.saveClientInfo(*username, *roomName);
            return JOIN_TO_CHAT_CMD ";";
        }
        else{
            return MESSAGE_TO_ALL_CMD ";" + input + ";";
        }

    }

    void run()
    {
        std::thread inputThread([this]() {
            for (;;)
            {
                std::string input;
                std::getline(std::cin, input);
                if (input != "") {
                    if (input == "!q")
                    {
                        m_userClient.closeConnection();
                        return;
                    }
                    std::string message = parseInput(input);
                    if (!message.empty())
                    {
                        m_userClient.sendUserMessage(message);
                    }
                }
            }
        });
        inputThread.detach();
    }
};
