#include "chatsoft.h"
#include "ui_chatsoft.h"

ChatSoft::ChatSoft(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatSoft)
{
    ui->setupUi(this);
    loginInterfase = new LoginInterfase();
}

ChatSoft::~ChatSoft()
{
    delete ui;
    delete loginInterfase;
}

void ChatSoft::start()
{
    loginInterfase->show();
}
