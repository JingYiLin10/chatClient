#ifndef LOGININTERFASE_H
#define LOGININTERFASE_H

#include <QPushButton>
#include "mylineedit.h"
#include <QCheckBox>
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QGridLayout>
#include <QPoint>
#include <QPalette>
#include <QWidget>
#include <QBrush>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpSocket>
#include "chatinterfase.h"

class LoginInterfase : public QWidget
{
    Q_OBJECT
public:
    explicit LoginInterfase(QWidget *parent = 0);
    ~LoginInterfase();

signals:

public slots:
    void login();
    void setCheckBoxState(int);
    void readMessHandle();
    void windowMinimize();

private:
    void setWindowTitleStyle(Qt::WindowFlags, int, int);
    void setControlsAttributes();
    void setControlsLayout();
    void setAllTabOrder();
    QString strHandle(const User &);

    QPushButton *miniBtn;
    QPushButton *closeBtn;
    QPushButton *loginBtn;
    QCheckBox *rememberPwd;
    QCheckBox *autoLogin;
    MyLineEdit *IMLine;
    MyLineEdit *pwdLine;

    User *user;
    QTcpSocket *tcpSocket;
    CharInterfase *chatInterfase;

    QVBoxLayout *mainLayout;
    QHBoxLayout *navLayout;
    QHBoxLayout *checkLayout;
    QGridLayout *loginInfoLayout;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    bool pressFlag;
    QPoint beginPos;

};

#endif // LOGININTERFASE_H
