#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QDebug>
#include <QWidget>
#include <QLineEdit>
class MyLineEdit : public QLineEdit
{
public:
    MyLineEdit();
private:
//    void inTextStyle();
//    void outTextStyle();
protected:
    virtual void focusInEvent(QFocusEvent *e);
    virtual void focusOutEvent(QFocusEvent *e);
};

#endif // MYLINEEDIT_H
