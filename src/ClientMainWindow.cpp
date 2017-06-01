#include "ClientMainWindow.h"
#include "transport.h"
#include "enums.h"
#include "messages.h"
#include "widgets/HandWidget.h"
#include "widgets/CardWidget.h"
#include <QInputDialog>

ClientMainWindow::ClientMainWindow()
        : client(new QTcpSocket(this))
        , handWidget(new HandWidget(this)) {
    client->setObjectName("socket");
    handWidget->setObjectName("hand");
    setupUi(this);

    addDockWidget(Qt::BottomDockWidgetArea, handWidget);
    menuView->addAction(handWidget->toggleViewAction());

    show();
}

void ClientMainWindow::on_socket_readyRead() {
    reader.processBytes(client->readAll(), {
            {STRING_MESSAGE, [this](const QByteArray& message){
                StringMessage m(message);
                textEdit->append(m.str);
            }},
            {SET_CLIENT_NAME_MESSAGE, [this](const QByteArray& message){
                SetClientNameMessage m(message);
                setWindowTitle("Name is " + m.name);
            }},
            {CARD_PLACED_MESSAGE, [this](const QByteArray& message){
                CardPlacedMessage m(message);
                handWidget->deleteCard(m.uid);
                printf("CardPlacedMessage card id %d\n", m.uid);
                QLayout* tableLayout = table->layout();
                for (int i = 0; i < tableLayout->count(); ++i) {
                    tableLayout->itemAt(i)->widget()->deleteLater();
                }
                tableLayout->addWidget(new CardWidget(nullptr, m.card));
            }},
            {CARD_ADDED_MESSAGE, [this](const QByteArray& message){
                CardAddedMessage m(message);
                handWidget->appendCard(m.uid, m.card);
            }},
    });
}

void ClientMainWindow::on_socket_connected() {
    buttonSend->setEnabled(true);
    printf("Connected\n");
}

void ClientMainWindow::on_socket_error(QAbstractSocket::SocketError error) {
    printf("Errrrrrror: %d\n", error);
}

void ClientMainWindow::on_socket_stateChanged(QAbstractSocket::SocketState state) {
    printf("Staaaaaate: %d\n", state);
    switch (state){
        case QAbstractSocket::UnconnectedState:
            buttonSend->setEnabled(false);
            buttonConnect->setEnabled(true);
            handWidget->clearHand();
            break;
        default:
            break;
    }
}

bool ClientMainWindow::isConnected() {
    return buttonSend->isEnabled();
}

template<typename MsgClass, typename... ArgTypes>
void ClientMainWindow::sendMessage(ArgTypes... args) {
    if(!isConnected()){
        return;
    }
    client->write(MsgClass(args...).encodeMessage());
    client->flush();
}

void ClientMainWindow::on_buttonSend_clicked(){
    if(lineEdit->text().isEmpty()){
        return;
    }

    sendMessage<StringMessage>(lineEdit->text());
    lineEdit->setText("");
}

void ClientMainWindow::on_buttonConnect_clicked(){
    buttonConnect->setEnabled(false);
    client->connectToHost("localhost", 9000);
}

void ClientMainWindow::on_hand_placeCardClicked(uint32_t cardUid) {
    sendMessage<PlaceCardMessage>(cardUid);
}
