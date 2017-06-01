#pragma once

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>

#include "ui_edinitsa_server.h"
#include "transport.h"
#include "CardData.h"

struct ClientInfo{
    QString name;

    ClientInfo(const QString &name) : name(name) {}
};

class ServerMainWindow : public QMainWindow, private Ui::ServerMainWindow {
Q_OBJECT
private:
    QTcpServer srv;
    QMap<QTcpSocket*, ClientInfo*> clients;
    MessageReader reader;
    QHash<uint32_t, Card::Data> cards;
    uint32_t nextCard = 1;

    HandlePair::CallbackType multicastFunc(uint32_t messageType, QTcpSocket* ignoreSocket=nullptr);
    void sendCard(QTcpSocket* socket, Card::Data card);

public:
    ServerMainWindow();

public slots:
    void readyToRead();
    void acceptConnection();
    void clientDisconnected();
};

