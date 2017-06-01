#pragma once

#include <QFrame>

#include "CardData.h"
#include "ui_card.h"

class CardWidget: public QFrame, private Ui::CardWidget {
Q_OBJECT
private:
    const Card::Data card;

private slots:
    void on_send_clicked();

public:
    CardWidget(QWidget* parent, Card::Data card);

protected:
    void paintEvent(QPaintEvent* event) override;

signals:
    void clickPlaceCard();
};
