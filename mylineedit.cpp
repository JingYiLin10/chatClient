#include "mylineedit.h"

MyLineEdit::MyLineEdit()
{

}

void MyLineEdit::focusInEvent(QFocusEvent*)
{
    if(this->text() == QString("")){
        this->setPlaceholderText(QString(""));
        if(this->echoMode() == QLineEdit::Password){
            QString style = "QLineEdit{ border:1px solid #D1D1D1; font:8px; border-bottom-right-radius:4px;border-bottom-left-radius:4px;}"
                                   "QLineEdit:hover {border:1px solid #1583DD;}";
            this->setStyleSheet(style);
        }
    }
}

void MyLineEdit::focusOutEvent(QFocusEvent*)
{

    if(this->text() == QString("")){
        if(this->echoMode() == QLineEdit::Password){
            this->setPlaceholderText(QString("密码"));
            QString style = "QLineEdit{ border:1px solid #D1D1D1; font:12px; border-bottom-right-radius:4px;border-bottom-left-radius:4px;}"
                                   "QLineEdit:hover {border:1px solid #1583DD;}";
            this->setStyleSheet(style);
        }
        else{
            this->setPlaceholderText(QString("QQ号码/电话/邮箱"));
        }
        //

        //setCursorVisible
        //this->curs
    }
}
