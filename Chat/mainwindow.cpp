#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "Client/ClientInterfaces.h"
#include "Client/ChatClient.h"
#include "Settings.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_chatClient = std::make_shared<QChatClient>();
    connect(m_chatClient.get(), &QChatClient::OnMessageReceived, this, [this](auto username, auto message){
        ui->textBrowser->append("<HTML> <font color=\"blue\">" + username + ": </font>" + message + "<br>");
    });

    m_settings = new Settings("settings.bin");
    m_chatClient->saveClientInfo(m_settings->m_username, "");
    ui->TextUsername->setText(QString::fromStdString(m_settings->m_username));

    m_tcpClient = std::make_shared<TcpClient>(m_ioContext1, m_chatClient);
    m_chatClient->setTcpClient(m_tcpClient);
    m_tcpClient->execute("127.0.0.1", 1234);

    std::thread ([this]{
        m_ioContext1.run();
    }).detach();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_settings;
}

void MainWindow::on_SendMessage_clicked()
{
    QString userMessage = ui->UserMessage->text();
    if (!userMessage.isEmpty())
    {
        std::string message = MESSAGE_TO_ALL_CMD ";" + userMessage.toStdString() + ";";
        m_chatClient->sendUserMessage(message);
    }
}


void MainWindow::on_Join_released()
{
    QString chatRoomName = ui->TextChatRoomName->text();
    QString username = ui->TextUsername->text();
    if (!chatRoomName.isEmpty() && !username.isEmpty())
    {
        m_chatClient->saveClientInfo(username.toStdString(), chatRoomName.toStdString());
        m_chatClient->sendUserMessage(JOIN_TO_CHAT_CMD ";");
    }
}




void MainWindow::on_SaveSettings_released()
{
    m_settings->m_username = ui->TextUsername->text().toStdString();
    m_settings->saveSettings("settings.bin");
}

