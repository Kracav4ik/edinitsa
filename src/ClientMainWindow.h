#pragma once

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>

#include "ui_edinitsa.h"
#include "transport.h"

class HandWidget;

class ClientMainWindow : public QMainWindow, private Ui::ClientMainWindow {
Q_OBJECT
private:
    QTcpSocket* client;
    MessageReader reader;
    HandWidget* handWidget;
    bool isConnected();

    template<typename MsgClass, typename... ArgTypes>
    void sendMessage(ArgTypes... args);

public:
    ClientMainWindow();

public slots:
    void on_socket_readyRead();
    void on_socket_connected();
    void on_socket_error(QAbstractSocket::SocketError error);
    void on_socket_stateChanged(QAbstractSocket::SocketState state);

    void on_buttonSend_clicked();
    void on_buttonConnect_clicked();

    void on_hand_placeCardClicked(uint32_t cardUid);
};

