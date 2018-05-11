#ifndef CHATSOFT_H
#define CHATSOFT_H
#include "chatinterfase.h"
#include "logininterfase.h"
#include <QWidget>

namespace Ui {
class ChatSoft;
}

class ChatSoft : public QWidget
{
    Q_OBJECT

public:
    explicit ChatSoft(QWidget *parent = 0);
    ~ChatSoft();
    void start();

private:
    Ui::ChatSoft *ui;
    LoginInterfase *loginInterfase;
    CharInterfase *charInterfase;
};

#endif // CHATSOFT_H
