#include "logininterfase.h"

LoginInterfase::LoginInterfase(QWidget *parent) : QWidget(parent)
{
    miniBtn = new QPushButton;
    closeBtn = new QPushButton;
    loginBtn = new QPushButton;
    rememberPwd = new QCheckBox;
    autoLogin = new QCheckBox;
    IMLine = new MyLineEdit;
    pwdLine = new MyLineEdit;

    user = new User;
    tcpSocket = new QTcpSocket;
    chatInterfase = nullptr;

    mainLayout = new QVBoxLayout;
    navLayout = new QHBoxLayout;
    checkLayout = new QHBoxLayout;
    loginInfoLayout = new QGridLayout;

    setWindowTitleStyle(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint, 430, 330);
    setControlsAttributes();
    setControlsLayout();
    setAllTabOrder();

    connect(closeBtn, QPushButton::clicked, this, QWidget::close);
    connect(miniBtn, QPushButton::clicked, this, windowMinimize);
    connect(loginBtn, QPushButton::clicked, this, login);
    connect(tcpSocket, QTcpSocket::readyRead, this, readMessHandle);
    connect(autoLogin, QCheckBox::stateChanged, this, setCheckBoxState);
    connect(rememberPwd, QCheckBox::stateChanged, this, setCheckBoxState);
}

LoginInterfase::~LoginInterfase()
{

}

//---------------slot-----------------
void LoginInterfase::login()
{
    user->id = IMLine->text();
    user->pwd = pwdLine->text();
    if(!tcpSocket->isOpen()){
        tcpSocket->abort();
        tcpSocket->connectToHost(QHostAddress(QString("127.0.0.1")),2001);
    }
    QString userStr = strHandle(*user);
    tcpSocket->write(userStr.toLatin1(),50);
}

void LoginInterfase::setCheckBoxState(int)
{
    QCheckBox *senderCBox = qobject_cast<QCheckBox *>(sender());
    if(senderCBox == rememberPwd){
        if(autoLogin->isChecked() && !rememberPwd->isChecked())
            autoLogin->setChecked(false);
    }
    else{
        if(!rememberPwd->isChecked() && autoLogin->isChecked())
            rememberPwd->setChecked(true);
    }
}

void LoginInterfase::readMessHandle()
{
    int len = 0;
    QString result = "";
    while(len < 50){
        QString str = QString(tcpSocket->read(50));
        result += str;
        len += str.size();
    }
    if(result[49] != 'e'){
        chatInterfase = new CharInterfase(*user);
        chatInterfase->show();
        disconnect(tcpSocket, QTcpSocket::readyRead, this, readMessHandle);
        this->close();
    }
    else{
        IMLine->setText("");
        pwdLine->setText("");
        IMLine->setFocus();
        pwdLine->clearFocus();
    }
}

void LoginInterfase::windowMinimize()
{
    if(windowState() != Qt::WindowMinimized){
        setWindowState(Qt::WindowMinimized);
    }
}

//---------------Function-----------------
void LoginInterfase::setWindowTitleStyle(Qt::WindowFlags type, int width, int height)
{
    this->setWindowFlags(type);
    this->resize(width, height);
    QPalette pal;
    QString imgname = "C:\\Users\\wjingbo\\Desktop\\GraduationProject\\img\\loginInterfasebg.jpg";
    QPixmap pixmap(imgname);
    pal.setBrush(QPalette::Background, QBrush(pixmap));
    this->setPalette(pal);
    miniBtn->setFixedSize(20, 20);
    closeBtn->setFixedSize(20, 20);

    IMLine->setFixedSize(190, 30);
    pwdLine->setFixedSize(190, 30);
    loginBtn->setFixedSize(190, 32);
}

void LoginInterfase::setControlsAttributes()
{
    QString miniBtnStyle = "QPushButton{background:transparent;}";
    miniBtn->setStyleSheet(miniBtnStyle);
    miniBtn->setIcon(QIcon("C:\\Users\\wjingbo\\Desktop\\GraduationProject\\img\\mini.png"));
    miniBtn->setIconSize(QSize(20, 17));

    QString closeBtnStyle = "QPushButton{ background:transparent;}";
    closeBtn->setStyleSheet(closeBtnStyle);
    closeBtn->setIcon(QIcon("C:\\Users\\wjingbo\\Desktop\\GraduationProject\\img\\close.png"));
    closeBtn->setIconSize(QSize(20, 20));

    loginBtn->setText(QObject::tr("安 全 登 录"));
    QString loginBtnStyle = "QPushButton{border-radius:4px; background:#00A3FF; color:white; font-family:宋体; font:12px; font-weight:bold;}"
                           "QPushButton:hover{background:#2AB9F8;}";
    loginBtn->setStyleSheet(loginBtnStyle);
    rememberPwd->setText(QObject::tr("记住密码"));
    rememberPwd->setChecked(true);
    autoLogin->setText(QObject::tr("自动登录"));
    QString checkBtnStyle = "QCheckBox {font:12px; color:#8D9195;}"
                            "QCheckBox::indicator{width:13px; height:13px;}";
    rememberPwd->setStyleSheet(checkBtnStyle);
    autoLogin->setStyleSheet(checkBtnStyle);
    IMLine->setPlaceholderText(tr("QQ号码/手机/邮箱"));
    QString IMLineStyle = "QLineEdit{border:1px solid #D1D1D1; font:12px; border-top-right-radius:4px; border-top-left-radius:4px;}"
                           "QLineEdit:hover{border:1px solid #1583DD;}";
    IMLine->setStyleSheet(IMLineStyle);
    pwdLine->setPlaceholderText(tr("密码"));
    QString pwdLineStyle = "QLineEdit{border:1px solid #D1D1D1; font:12px; border-bottom-right-radius:4px; border-bottom-left-radius:4px;}"
                           "QLineEdit:hover{border:1px solid #1583DD;}";
    pwdLine->setStyleSheet(pwdLineStyle);
    pwdLine->setEchoMode(QLineEdit::Password);
}

void LoginInterfase::setControlsLayout(){
    navLayout->addStretch(368);
    navLayout->addWidget(miniBtn);
    navLayout->addStretch(2);
    navLayout->addWidget(closeBtn);
    navLayout->setMargin(0);
    navLayout->setSpacing(0);

    checkLayout->addWidget(rememberPwd);
    checkLayout->addWidget(autoLogin);
    checkLayout->setSpacing(54);
    checkLayout->setContentsMargins(0, 5, 0, 11);

    loginInfoLayout->addWidget(IMLine, 0, 0, 1, 1);
    loginInfoLayout->addWidget(pwdLine, 1, 0, 1, 1);
    loginInfoLayout->addLayout(checkLayout, 2, 0, 1, 1);
    loginInfoLayout->addWidget(loginBtn, 3, 0, 1, 1);
    loginInfoLayout->setMargin(0);
    loginInfoLayout->setSpacing(0);

    mainLayout->addLayout(navLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(loginInfoLayout);

    setLayout(mainLayout);
}

void LoginInterfase::setAllTabOrder()
{
    setTabOrder(miniBtn,closeBtn);
    setTabOrder(closeBtn,IMLine);
    setTabOrder(IMLine,pwdLine);
    setTabOrder(pwdLine,rememberPwd);
    setTabOrder(rememberPwd,autoLogin);
    setTabOrder(autoLogin,loginBtn);
    setTabOrder(loginBtn,miniBtn);
}

QString LoginInterfase::strHandle(const User &user)
{
    QString id = user.id;
    id.resize(10, char(1));
    QString pwd = user.pwd;
    QString result = id + pwd;
    result.resize(50, char(1));
    result[49] = 'l';
    return result;
}

//---------------MouseEvent-----------------
void LoginInterfase::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        pressFlag = true;
        beginPos = event->pos();
    }
    QWidget::mousePressEvent(event);
}

void LoginInterfase::mouseMoveEvent(QMouseEvent *event)
{

    if(pressFlag){
        QPoint relPos(QCursor::pos() - beginPos);
        move(relPos);
    }
    QWidget::mouseMoveEvent(event);
}

void LoginInterfase::mouseReleaseEvent(QMouseEvent *event)
{
    IMLine->clearFocus();
    pwdLine->clearFocus();
    pressFlag = false;
    QWidget::mouseReleaseEvent(event);
}
