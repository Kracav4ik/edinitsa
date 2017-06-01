#include "HandWidget.h"
#include "CardWidget.h"

HandWidget::HandWidget(QWidget* parent)
        : QDockWidget(parent) {
    setupUi(this);
}

void HandWidget::appendCard(uint32_t uid, Card::Data card) {
    CardWidget* widget = new CardWidget(this, card);
    uidToCard[uid] = widget;

    QVBoxLayout* boxLayout = getButtonsLayout();
    boxLayout->insertWidget(boxLayout->count() - 1, widget);

    connect(widget, &CardWidget::clickPlaceCard, [this, uid]() {
        emit placeCardClicked(uid);
    });
}

void HandWidget::deleteCard(uint32_t uid) {
    QVBoxLayout* boxLayout = getButtonsLayout();
    CardWidget* card = uidToCard.take(uid);
    boxLayout->removeWidget(card);
    card->deleteLater();
}

QVBoxLayout* HandWidget::getButtonsLayout() {
    return reinterpret_cast<QVBoxLayout*>(cards->layout());
}

void HandWidget::clearHand() {
    for (uint32_t cardsUid : uidToCard.keys()) {
        deleteCard(cardsUid);
    }
}
