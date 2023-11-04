#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "Client/ClientInterfaces.h"
#include "Client/ChatClient.h"
#include "Settings.h"
#include "Utils.h"
#include "CreateChatRoom.h"

#include <QDir>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    configureUI();

    m_chatClient = std::make_shared<QChatClient>();
    connect(m_chatClient.get(), &QChatClient::OnMessageReceived, this, [this](auto username, auto message){
        ui->textBrowser->append(username + message + "<br>");
    });

    connect(m_chatClient.get(), &QChatClient::OnTableChanged, this, [this](auto username){
        for (int i = 0; i < ui->UsersInChatRoom->rowCount(); i++)
        {
            for (int j = 0; j < ui->UsersInChatRoom->columnCount(); j++)
            {
                if(ui->UsersInChatRoom->item(i, j) && ui->UsersInChatRoom->item(i, j)->text() == username)
                {
                    ui->UsersInChatRoom->removeRow(i);
                    return;
                }
            }
        }
        ui->UsersInChatRoom->insertRow(ui->UsersInChatRoom->rowCount());
        ui->UsersInChatRoom->setItem(ui->UsersInChatRoom->rowCount() - 1, 0, new QTableWidgetItem(username));
    });

    //qDebug() << QDir::homePath();
    m_settings = new Settings("settings2.bin");
    //system("dir");
    m_chatClient->saveClientInfo(m_settings->m_username, "");
    ui->TextUsername->setText(QString::fromStdString(m_settings->m_username));

    m_tcpClient = std::make_shared<TcpClient>(m_ioContext1, m_chatClient);
    m_chatClient->setTcpClient(m_tcpClient);
    m_tcpClient->execute("127.0.0.1", 1234);

    m_chatClient->sendUserMessage(CONNECT_CMD ";" + arrayToHexString(m_settings->m_userKey) + ";" + arrayToHexString(m_settings->m_deviceKey) + ";");

    std::thread ([this]{
        m_ioContext1.run();
    }).detach();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_settings;
}

void MainWindow::configureUI()
{
    QString style = "background-color: #282e33; color : #e9f2f4; font : bold; QLabel { text-align: center; }";
    ui->centralwidget->setStyleSheet("background-color: #18191d");
    ui->TextChatRoomName->setStyleSheet(style);
    ui->TextUsername->setStyleSheet(style);
    ui->textBrowser->setStyleSheet(style);
    ui->Username->setStyleSheet(style);
    ui->ChatRoomName->setStyleSheet(style);
    ui->Exit->setStyleSheet(style);
    ui->Join->setStyleSheet(style);
    ui->SaveSettings->setStyleSheet(style);
    ui->UserMessage->setStyleSheet(style);
    ui->SendMessage->setStyleSheet(style);
    ui->m_CreateRoom->setStyleSheet(style);

    ui->UsersInChatRoom->setColumnCount(1);
    QString newHeaderText = "Users";
    ui->UsersInChatRoom->setHorizontalHeaderLabels(QStringList() << newHeaderText);
    ui->UsersInChatRoom->setStyleSheet(style);

    ui->SendMessage->setVisible(false);
    ui->UserMessage->setVisible(false);
}

void MainWindow::on_Join_released()
{
    QString chatRoomName = ui->TextChatRoomName->text();
    QString username = ui->TextUsername->text();
    if (!chatRoomName.isEmpty() && !username.isEmpty())
    {
        m_chatClient->saveClientInfo(username.toStdString(), chatRoomName.toStdString());
        m_chatClient->sendUserMessage(JOIN_TO_CHAT_CMD ";");
        ui->TextChatRoomName->setVisible(false);
        ui->TextUsername->setVisible(false);
        ui->Join->setVisible(false);
        ui->ChatRoomName->setVisible(false);
        ui->Username->setVisible(false);
        ui->SaveSettings->setVisible(false);
        ui->SendMessageW->setVisible(true);
        ui->UserMessage->setVisible(true);
    }
}




void MainWindow::on_SaveSettings_released()
{
    m_settings->m_username = ui->TextUsername->text().toStdString();
    m_settings->saveSettings("settings.bin");
}


void MainWindow::on_SendMessage_released()
{
    QString userMessage = ui->UserMessage->text();
    if (!userMessage.isEmpty())
    {
        std::string message = MESSAGE_TO_ALL_CMD ";" + userMessage.toStdString() + ";";
        m_chatClient->sendUserMessage(message);
        ui->UserMessage->clear();
    }
}


void MainWindow::on_Exit_released()
{
    m_chatClient->closeConnection();
}



void MainWindow::on_m_CreateRoom_released()
{
    CreateChatRoom createChatRoom(*m_chatClient);
    createChatRoom.setModal(true);
    createChatRoom.exec();
}

