#include "chatinterfase.h"

CharInterfase::CharInterfase(User user, QWidget *parent) : QWidget(parent)
{   //对象创建
    setUser(user);
    tcpSocket = new QTcpSocket;
    tcpSocket->abort();
    tcpSocket->connectToHost(QHostAddress(QString("127.0.0.1")),2001);
    this->createChatInterfaseObj();
    this->createAddFriObj();
    this->createMainObj();

    this->setChatInterfaseAttributes();
    this->setChatInterfaseLayout();
    this->setAddFriAttributes();
    this->setAddFriLayout();

    this->setWindowControlsAttributes(Qt::FramelessWindowHint|Qt::WindowMinimizeButtonHint,906,602);
    this->setWindowControlsLayout();
    this->setStackedLayoutAttributes();

    connect(tcpSocket, QTcpSocket::readyRead, this, readMessHandle);
    connect(closeBtn,QPushButton::clicked, this, QWidget::close);
    connect(miniBtn,QPushButton::clicked, this, windowMinimize);
    connect(closeBtn,QPushButton::clicked, this, tcpClose);
    connect(chatInterfaseBtn, QPushButton::clicked, this, chat_add_SwitchPage);
    connect(addFriendInfoBtn, QPushButton::clicked, this, chat_add_SwitchPage);
    connect(sendBtn,QPushButton::clicked,this,createChatInfo1);
    connect(middleFindBtn, QPushButton::clicked, this, findFriendSlot);
    connect(newFriendAddBtn, QPushButton::clicked, this, addFriend);
    connect(recentChatList,SIGNAL(itemActivated(QListWidgetItem*)), this,SLOT(setChatObj(QListWidgetItem*)));
}

CharInterfase::~CharInterfase()
{
    tcpSocket->close();
}

void CharInterfase::setUser(User user)
{
    this->user = new User;
    this->user->id = user.id;
    this->user->pwd = user.pwd;
}

void CharInterfase::tcpClose()
{
    QString str1 = user->id;
    str1.resize(10,char(1));
    QString str2 = str1 + user->pwd;
    str2.resize(50, char(1));
    str2[49] = 'q';
    tcpSocket->write(str2.toLatin1());
}
//----------------------------------------------------
void CharInterfase:: readMessHandle()
{
    int len = 0;
    QString messInfo = "";
    while(len < 50){
        QString str = QString(tcpSocket->read(50));
        messInfo += str;
        len += str.size();
    }

    bool aimsFri = false;
    if(messInfo[49] == 'y'){
        QString id = middleFindLine->text();
        AddFri *friendInfo = friendInfoHandle(messInfo);
        if(friendInfo->aimsId == user->id){
            id = friendInfo->id;
            aimsFri = true;
        }
        else{
            chatObjName->setText(id);
        }
        recentChatList->addItem(getFriendItemObj(id));

        if(friendInfo->aimsId != user->id){
            recentChatList->setItemSelected(recentChatList->item(recentChatList->count() - 1),true);
            QVector<QPair<QString,bool>> vs = resultMessDB(middleFindLine->text());
            addChatInfo(vs);
        }
    }
    if(messInfo[49] == 'n'){
        QString friendName = middleFindLine->text();
        if(user->id != friendName){
            for(int i = 0; i < recentChatList->count(); ++i){
                if(recentChatList->item(i)->text() == friendName){
                    recentChatList->setItemSelected(recentChatList->item(i),true);
                    chatObjName->setText(friendName);
                    break;
                }
            }
        }
    }
    if(messInfo[49] == 'm'){
        Mess *mess  = new Mess;
        QString id = messInfo.left(10);
        QString aimsId = messInfo.left(20).right(10);
        QString message = messInfo.right(30).left(29);
        mess->id = trim(id);
        mess->aimsId = trim(aimsId);
        mess->mess = trim(message);
        if(mess->id == chatObjName->text() && mess->aimsId == user->id){
            QHBoxLayout *chatRecordItem = createChatInfo2(mess->mess, false);
            if(chatMessCount == 8){
                int index = chatMessLayoutFlag[chatMessRmIndex] ? 1 : 0;
                QLayoutItem *it = chatMessLayoutArr[chatMessRmIndex]->layout()->itemAt(index);
                QLabel *label = qobject_cast<QLabel *>(it->widget());
                label->hide();
                chatRecordLayout->removeItem(chatMessLayoutArr[chatMessRmIndex]);
                delete chatMessLayoutArr[chatMessRmIndex];
                chatMessLayoutArr[chatMessRmIndex] = chatRecordItem;
                chatMessLayoutFlag[chatMessRmIndex] = false;
                if(chatMessRmIndex == 7){
                    chatMessRmIndex = 0;
                }
                else{
                    chatMessRmIndex += 1;
                }
            }
            else{
                chatMessLayoutArr[chatMessCount] = chatRecordItem;
                chatMessLayoutFlag[chatMessCount] = false;
                chatMessCount += 1;
            }
            chatRecordLayout->addLayout(chatRecordItem);
        }
    }
    chat_add_StackLayout->setCurrentIndex(0);
    middleFindLine->setText(QString(""));
    findFriendWidget->hide();
    setFriendLayout1();
    if(messInfo[49] =='n' || messInfo[49] == 'y' || messInfo[49] == 's'){
        chatInterfaseBtn->setIcon(QIcon("C:\\Users\\wjingbo\\Desktop\\GraduationProject\\img\\user22.png"));
        addFriendInfoBtn->setIcon(QIcon("C:\\Users\\wjingbo\\Desktop\\GraduationProject\\img\\add11.png"));
    }
    if(justLogin && (messInfo[49] =='n' || messInfo[49] =='y') && !aimsFri){
        justLoginAfter();
        justLogin = false;
        lookWorldImg->hide();
    }
}

//---------------------------------------------------------

void CharInterfase::chat_add_SwitchPage()  //stackedLayout 切换页面
{
    //调用sender()函数返回信号发送对象
    QPushButton *senderBtn = qobject_cast<QPushButton *>(sender());
    int nIndex = chat_add_StackLayout->currentIndex();
    if(senderBtn == chatInterfaseBtn && nIndex == 1){
        nIndex = 0;
        chatInterfaseBtn->setIcon(QIcon("C:\\Users\\wjingbo\\Desktop\\GraduationProject\\img\\user22.png"));
        addFriendInfoBtn->setIcon(QIcon("C:\\Users\\wjingbo\\Desktop\\GraduationProject\\img\\add11.png"));
    }
    else if(senderBtn == addFriendInfoBtn && nIndex == 0){
        nIndex = 1;
        chatInterfaseBtn->setIcon(QIcon("C:\\Users\\wjingbo\\Desktop\\GraduationProject\\img\\user11.png"));
        addFriendInfoBtn->setIcon(QIcon("C:\\Users\\wjingbo\\Desktop\\GraduationProject\\img\\add22.png"));
    }
    chat_add_StackLayout->setCurrentIndex(nIndex);
}

void CharInterfase::addFriend()
{
    AddFri af;
    af.id = user->id;
    af.aimsId = middleFindLine->text();
    QString str = strHandle(af);
    tcpSocket->write(str.toLatin1());
}

void CharInterfase::createChatInfo1()
{
    QString string = chatText->toPlainText();
    if(string != ""){
        QHBoxLayout *chatRecordItem = createChatInfo2(string, true);
        chatText->setText(QString(""));
        qDebug() << chatMessCount;
        if(chatMessCount == 8){
            int index = chatMessLayoutFlag[chatMessRmIndex] ? 1 : 0;
            qDebug() << index;
            QLayoutItem *it = chatMessLayoutArr[chatMessRmIndex]->layout()->itemAt(index);
            QLabel *label = qobject_cast<QLabel *>(it->widget());
            label->hide();
            chatRecordLayout->removeItem(chatMessLayoutArr[chatMessRmIndex]);
            delete chatMessLayoutArr[chatMessRmIndex];
            chatMessLayoutArr[chatMessRmIndex] = chatRecordItem;
            chatMessLayoutFlag[chatMessRmIndex] = true;
            if(chatMessRmIndex == 7){
                chatMessRmIndex = 0;
            }
            else{
                chatMessRmIndex += 1;
            }
        }
        else{
            chatMessLayoutArr[chatMessCount] = chatRecordItem;
            chatMessLayoutFlag[chatMessCount] = true;
            chatMessCount += 1;
        }
        chatRecordLayout->addLayout(chatRecordItem);
        Mess mess;
        mess.mess = string;
        mess.id = user->id;
        mess.aimsId = chatObjName->text();
        QString str = strHandle(mess);
        tcpSocket->write(str.toLatin1());
    }
}

void CharInterfase::findFriendSlot()
{
   findFriendWidget->show();
    bool flag = false;
    if(middleFindLine->text() != ""){
        QString id = middleFindLine->text();
        bool isUser = findUserDB(id);   //%查询数据库
        if(isUser){
            flag = true;
            newFriendInfo->setText(id);
        }
    }
    if(flag){
        setNotFriendLayout1();
        setFriendLayout2(); //更新总布局
    }
    else{
        setNotFriendLayout2();
        setFriendLayout2();
    }

}

void CharInterfase::setChatObj(QListWidgetItem *item)
{
    chatObjName->setText(item->text());
    QVector<QPair<QString,bool>> vs = resultMessDB(item->text());
    addChatInfo(vs);
    if(justLogin){
        justLoginAfter();
        justLogin = false;
        lookWorldImg->hide();
    }
}

void CharInterfase::windowMinimize()
{
    if(windowState() != Qt::WindowMinimized){
        setWindowState(Qt::WindowMinimized);
    }
}

//------------------slot--------------

//--------------主界面相关函数-----------
void CharInterfase::createMainObj() //主界面对象生成
{
    chatNav = new QWidget;
    addFriendInfoBtn = new QPushButton;
    chatInterfaseBtn = new QPushButton;
    miniBtn = new QPushButton;
    maxBtn = new QPushButton;
    closeBtn = new QPushButton;
    chat_add_StackLayout = new QStackedLayout;
    //Layout
    mainLayout = new QVBoxLayout;
    navLayout = new QHBoxLayout;
}

//主界面设置显示风格属性
void CharInterfase::setWindowControlsAttributes(Qt::WindowFlags type, int width, int height)    //主页面控件风格
{

    this->setWindowFlags(type);
    this->resize(width, height);
    chatNav->setFixedSize(906,56);//适当取值56-58
    QString chatNavStyle = "QWidget{margin-right:8px;}";
    chatNav->setStyleSheet(chatNavStyle);
    QPalette pal;
    QString imgname = "C:\\Users\\wjingbo\\Desktop\\GraduationProject\\img\\chatInterfaseTitle.png";
    QPixmap pixmap(imgname);
    pal.setBrush(QPalette::Background, QBrush(pixmap));
    this->setPalette(pal);

    miniBtn->setFixedSize(20,20);
    QString miniStyle = "QPushButton{color:#96A0D3;font-family:黑体;background:transparent;font-weight:bold;}QPushButton:hover{color:white;}";
    miniBtn->setStyleSheet(miniStyle);
    miniBtn->setIcon(QIcon("C:\\Users\\wjingbo\\Desktop\\GraduationProject\\img\\mini.png"));
    miniBtn->setIconSize(QSize(20, 17));

    maxBtn->setFixedSize(20,20);
    QString maxStyle = "QPushButton{background:transparent;}";
    maxBtn->setStyleSheet(maxStyle);
    maxBtn->setIcon(QIcon("C:\\Users\\wjingbo\\Desktop\\GraduationProject\\img\\setting.png"));
    maxBtn->setIconSize(QSize(20,20));

    closeBtn->setFixedSize(20, 20);
    QString closeStyle = "QPushButton{background:transparent;}";
    closeBtn->setStyleSheet(closeStyle);
    closeBtn->setIcon(QIcon("C:\\Users\\wjingbo\\Desktop\\GraduationProject\\img\\close.png"));
    closeBtn->setIconSize(QSize(20, 20));

    chatInterfaseBtn->setFixedSize(50,50);
    QString chatInterfaseBtnStyle = "QPushButton{background:transparent;}";
                                    //"QPushButton:hover{background-image:url(C:\\Users\\wjingbo\\Desktop\\GraduationProject\\img\\user22.png);}";
    chatInterfaseBtn->setStyleSheet(chatInterfaseBtnStyle);

    chatInterfaseBtn->setIcon(QIcon("C:\\Users\\wjingbo\\Desktop\\GraduationProject\\img\\user22.png"));
    chatInterfaseBtn->setIconSize(QSize(50,50));

    addFriendInfoBtn->setFixedSize(50,50);
    QString addFriendInfoBtnStyle = "QPushButton{background:transparent;}";
    addFriendInfoBtn->setStyleSheet(addFriendInfoBtnStyle);
    addFriendInfoBtn->setIcon(QIcon("C:\\Users\\wjingbo\\Desktop\\GraduationProject\\img\\add11.png"));
    addFriendInfoBtn->setIconSize(QSize(50,50));

}

//主页面设置布局
void CharInterfase::setWindowControlsLayout()   //主页面布局
{
    //标题导航栏
    navLayout->addStretch(340); //310
    navLayout->addWidget(chatInterfaseBtn);
    navLayout->addStretch(30);
    navLayout->addWidget(addFriendInfoBtn);
    navLayout->addStretch(280);//310
    navLayout->addWidget(miniBtn);
    navLayout->addWidget(maxBtn);
    navLayout->addWidget(closeBtn);
    navLayout->setSpacing(5);
    navLayout->setMargin(3);

    chatNav->setLayout(navLayout);
    //stackedLayout
    chat_add_StackLayout->addWidget(chatInterfasePage);
    chat_add_StackLayout->addWidget(addFriendPage);
    chat_add_StackLayout->setSpacing(0);

    //主布局
    mainLayout->addWidget(chatNav);
    //mainLayout->addStretch();
    mainLayout->addLayout(chat_add_StackLayout);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    this->setLayout(mainLayout);

}

//设置主页面stackedLayout属性
void CharInterfase::setStackedLayoutAttributes()    //子页面控制
{
    addFriendPage->setFixedSize(906,546);
    addFriendPage->setLayout(addFriendPageMainLayout1);
    chatInterfasePage->setFixedSize(906,546);
    chatInterfasePage->setLayout(chatInterfaseMainLayout);
    chat_add_StackLayout->setCurrentIndex(0);
    connectDB();
    friendDB();
}



//---------------1页相关函数------------
void CharInterfase::createChatInterfaseObj()    //聊天页面对象生成
{
    justLogin = true;
    chatMessCount = 0;
    chatMessRmIndex = 0;
    recentChatList = new QListWidget;
    chatObjName = new QLabel;
    chatInterfasePage = new QWidget;
    chatText = new QTextEdit;
    chatRecord = new QWidget;
    sendBtn = new QPushButton;

    lookWorldImg = new QPushButton;
    lookWorldLayout = new QHBoxLayout;

    chatRecordLayout = new QVBoxLayout;
    chatWinTitleLayout = new QHBoxLayout;
    chatWinSendLayout = new QHBoxLayout;
    chatWinLayout = new QVBoxLayout;
    chatInterfaseMainLayout = new QHBoxLayout;
}

//1页属性设置
void CharInterfase::setChatInterfaseAttributes()        //设置聊天页面控件风格
{
    QString chatInterfasePageStyle = "QWidget{background:white;}";
    chatInterfasePage->setStyleSheet(chatInterfasePageStyle);

    recentChatList->setFixedWidth(200);
    recentChatList->setIconSize(QSize(40,40));
    QString recentChatListStyle = "QListWidget{background:#FAFAFA; border:none solid #AEAEAE; outline:0px;}"
                                  "QListWidget::item::selected{background:#EBEBEC; color:black;}"
                                  "QListWidget::item:hover{background:#EBEBEC;}"
                                  "QListWidget::item{height:60px;}";
    recentChatList->setStyleSheet(recentChatListStyle);
    chatRecord->setFixedHeight(375);
    chatRecord->setLayout(chatRecordLayout);
    QString chatRecordStyle = "QWidget{border-top:1px solid #EAEAEA;}";
    chatRecord->setStyleSheet(chatRecordStyle);

    lookWorldImg->setFixedSize(470,470);
    lookWorldImg->setIcon(QIcon("C:\\Users\\wjingbo\\Desktop\\GraduationProject\\img\\rightImg.png"));
    lookWorldImg->setIconSize(QSize(470,470));

    //-------------------
    chatObjName->setText(tr("Title"));
    QString chatObjNameStyle = "QLabel{font:18px; color:black; font-family:黑体;}";
    chatObjName->setStyleSheet(chatObjNameStyle);

    chatText->setFixedHeight(92);
    chatText->verticalScrollBar()->hide();
    QString chatTextStyle = "QTextEdit{font:14px; font-family:宋体; border-top:1px solid #EAEAEA;}";
    chatText->setStyleSheet(chatTextStyle);

    sendBtn->setText(tr("发送"));
    sendBtn->setFixedSize(65,25);
    QString sendBtnStyle = "QPushButton {background:#00A3FF; border-radius:2px; color:white;}"
                           "QPushButton:hover {background:#2AB9F8;}";
    sendBtn->setStyleSheet(sendBtnStyle);

}

//1页页面布局
void CharInterfase::setChatInterfaseLayout()    //设置聊天页面布局
{
    chatRecordLayout->addStretch();
    chatRecordLayout->setMargin(25);
    chatRecordLayout->setSpacing(12);

//    recentChatLayout->setMargin(0);
//    recentChatLayout->setSpacing(0);
//    recentChatLayout->addWidget(recentChatList);

    chatWinTitleLayout->setMargin(15);
    chatWinTitleLayout->setSpacing(0);
    chatWinTitleLayout->addWidget(chatObjName);

    chatWinSendLayout->setMargin(0);
    chatWinSendLayout->setSpacing(0);
    chatWinSendLayout->setContentsMargins(0,0,14,15);
    chatWinSendLayout->addStretch();
    chatWinSendLayout->addWidget(sendBtn);

    chatWinLayout->setMargin(0);
    chatWinLayout->setSpacing(0);
    chatWinLayout->addLayout(chatWinTitleLayout);
    chatWinLayout->addWidget(chatRecord);
    chatWinLayout->addWidget(chatText);
    chatWinLayout->addLayout(chatWinSendLayout);

    lookWorldLayout->setMargin(0);
    lookWorldLayout->setSpacing(0);
    lookWorldLayout->addStretch();
    lookWorldLayout->addWidget(lookWorldImg);
    lookWorldLayout->addStretch();

    chatInterfaseMainLayout->setSpacing(0);
    chatInterfaseMainLayout->setMargin(0);
    chatInterfaseMainLayout->addWidget(recentChatList);
    chatInterfaseMainLayout->addLayout(lookWorldLayout);//开局先设置lookworld
}


//---------------2页相关函数------------
void CharInterfase::createAddFriObj() //添加好友页面对象生成
{
    addFriendPage = new QWidget;
    middleTopBg = new QLabel;
    middleTitleInfo = new QLabel;
    middleFindLine = new QLineEdit;
    middleFindBtn = new QPushButton;
    middleWidget = new QWidget;

    middleTitleInfoLayout = new QHBoxLayout;
    middleFindLayout = new QHBoxLayout;
    middleLayout = new QVBoxLayout;
    addFriendPageMainLayout1 = new QHBoxLayout;
    addFriendPageMainLayout2 = new QVBoxLayout;

    findFriendWidget = new QWidget;
    newFriendIcon = new QPushButton;
    newFriendAddBtn = new QPushButton;
    newFriendInfo = new QLabel;
    notFindInfo = new QLabel;

    middleFindResultLayout = new QHBoxLayout;
}

//设置加好友页风格
void CharInterfase::setAddFriAttributes()    //设置添加好友页面控件风格
{

    middleTopBg->setFixedSize(620,6);//查找框标题背景颜色
    QString middleTopBgStyle = "QLabel{background:#118DEE; border-top-left-radius:4px; border-top-right-radius:4px;}";
    middleTopBg->setStyleSheet(middleTopBgStyle);

    middleTitleInfo->setText(tr("添加好友"));
    QString middleTitleInfoStyle = "QLabel{color:#1792FB; font:20px; font-family:黑体}";
    middleTitleInfo->setStyleSheet(middleTitleInfoStyle);

    QString middleFindLineStyle = "QLineEdit{border:1px solid #AEAEAE; width:450px; height:30px; border-radius:15px; font:14px; color:#7F7F7F;padding-left:10px;}";
    middleFindLine->setStyleSheet(middleFindLineStyle);
    middleFindLine->setPlaceholderText(tr("请输入所查找的好友id"));
    middleFindBtn->setText(tr("查找"));//查找按钮颜色 //原#0188FB #289CFF
    QString middleFindBtnStyle = "QPushButton{background:#00A3FF; height:28px; width:65px; border-radius:3px; color:white;}"
                           "QPushButton:hover{background:#2AB9F8;}";
    middleFindBtn->setStyleSheet(middleFindBtnStyle);

    middleWidget->setFixedSize(620,202);//202
    QString middleWidgetStyle = "QWidget{background:white; border-top-left-radius:4px; border-top-right-radius:4px;}";
    middleWidget->setStyleSheet(middleWidgetStyle);
    middleWidget->setLayout(middleLayout);
    QString addFriendPageStyle = "QWidget{background:#FAFAFA;}";
    addFriendPage->setStyleSheet(addFriendPageStyle);

    findFriendWidget->setLayout(middleFindResultLayout);

    QString findFriendStyle = "QWidget{background:#DFDFDF;}";   //查找结果背景颜色
    findFriendWidget->setFixedSize(620,70);
    findFriendWidget->setStyleSheet(findFriendStyle);

    QString newFriendIconStyle = "QPushButton{border-radius:30px;}";
    newFriendIcon->setFixedSize(60,60);
    newFriendIcon->setIcon(QIcon("C:\\Users\\wjingbo\\Desktop\\GraduationProject\\img\\headIcon.png"));
    newFriendIcon->setIconSize(QSize(60,60));
    newFriendIcon->setStyleSheet(newFriendIconStyle);

    newFriendInfo->setFixedWidth(60);
    QString newFriendInfoStyle = "QLabel{font:18px; font-family:黑体; color:white;}";
    newFriendInfo->setStyleSheet(newFriendInfoStyle);
    newFriendAddBtn->setText(tr("添加")); //查找按钮设置颜色
    QString newFriendAddBtnStyle = "QPushButton{background:#00A3FF; border-radius:3px; color:white;}"
                         "QPushButton:hover{background:#2AB9F8;}";
    newFriendAddBtn->setFixedSize(65,28);
    newFriendAddBtn->setStyleSheet(newFriendAddBtnStyle);
    notFindInfo->setText(tr("暂无此用户!"));
    QString notFindInfoStyle = "QLabel{font:12px; font-family:宋体; color:white; }";
    notFindInfo->setStyleSheet(notFindInfoStyle);
}

void CharInterfase::setAddFriLayout()    //设置添加好友页面布局
{
    middleFindLayout->setSpacing(15);
    middleFindLayout->addStretch();
    middleFindLayout->addWidget(middleFindLine);
    middleFindLayout->addWidget(middleFindBtn);
    middleFindLayout->addStretch();

    middleTitleInfoLayout->addStretch();
    middleTitleInfoLayout->addWidget(middleTitleInfo);
    middleTitleInfoLayout->addStretch();

    middleLayout->setMargin(0);
    middleLayout->setSpacing(0);
    middleLayout->addWidget(middleTopBg);
    middleLayout->addStretch(1);
    middleLayout->addLayout(middleTitleInfoLayout);
    middleLayout->addStretch(1);
    middleLayout->addLayout(middleFindLayout);
    middleLayout->addStretch(2);

    middleFindResultLayout->setMargin(5);

    addFriendPageMainLayout2->setMargin(0);
    addFriendPageMainLayout2->setSpacing(0);

    setFriendLayout1();
    addFriendPageMainLayout1->setMargin(0);
    addFriendPageMainLayout1->addStretch();
    addFriendPageMainLayout1->addLayout(addFriendPageMainLayout2);
    addFriendPageMainLayout1->addStretch();

}

QString CharInterfase::trim(QString str)
{
    QString result = "";
    for(int i = str.length() - 1; i >= 0; i--){
        char c = str.at(i).toLatin1();
       if(c != char(1)){
            result = str.left(i + 1);
            break;
       }
    }
    return result;
}

QHBoxLayout *CharInterfase::createChatInfo2(QString str, bool flag)
{
    QHBoxLayout *chatRecordItem = new QHBoxLayout;
    QLabel *recordItem = new QLabel;
    recordItem->setText(QString(str));
    recordItem->setAlignment(Qt::AlignCenter);
    recordItem->setAlignment(Qt::AlignVCenter);
    recordItem->setMinimumWidth(25);
    recordItem->setMaximumWidth(550);
    recordItem->setMinimumHeight(30);
    QString recordItemStyle;
    if(flag){
         recordItemStyle = "QLabel{padding:6px; color:white;  border-top-left-radius:10px; font:13px;  border-top-right-radius: 10px; border-bottom-left-radius:10px; background:#2683F5;}";
         chatRecordItem->addStretch();
         chatRecordItem->addWidget(recordItem);
    }
    else{
        recordItemStyle = "QLabel{padding:6px; color:black;  border-top-left-radius:10px; font:13px;  border-top-right-radius: 10px; border-bottom-right-radius:10px; background:#EAEAEA;}";
        chatRecordItem->addWidget(recordItem);
        chatRecordItem->addStretch();
    }
    recordItem->setStyleSheet(recordItemStyle);
    return chatRecordItem;
}


void CharInterfase::setFriendLayout1()  //设置添加好友界面的布局
{
    int itemCount = addFriendPageMainLayout2->count();
    for(int i = (itemCount - 1); i >= 0; i--){
        QLayoutItem *item = addFriendPageMainLayout2->layout()->takeAt(i);
        if(item != nullptr)
            addFriendPageMainLayout2->removeWidget(item->widget());
    }
    addFriendPageMainLayout2->addStretch(124);
    addFriendPageMainLayout2->addWidget(middleWidget);
    addFriendPageMainLayout2->addStretch(220);
}

void CharInterfase::setFriendLayout2()  //设置添加好友界面的布局
{
    int itemCount = addFriendPageMainLayout2->count();
    for(int i = (itemCount - 1); i >= 0; i--){
        QLayoutItem *item = addFriendPageMainLayout2->layout()->takeAt(i);
        if(item != nullptr)
            addFriendPageMainLayout2->removeWidget(item->widget());
    }
    addFriendPageMainLayout2->addStretch(124);
    addFriendPageMainLayout2->addWidget(middleWidget);
    addFriendPageMainLayout2->addWidget(findFriendWidget);
    addFriendPageMainLayout2->addStretch(150);
}

void CharInterfase::setNotFriendLayout1()
{
    int itemCount = middleFindResultLayout->count();
    for(int i = (itemCount - 1); i >= 0; i--){
        QLayoutItem *item = middleFindResultLayout->layout()->takeAt(i);
        if(item != nullptr)
            middleFindResultLayout->removeWidget(item->widget());
    }
    middleFindResultLayout->addStretch(3);
    middleFindResultLayout->addWidget(newFriendIcon);
    middleFindResultLayout->addStretch(1);
    middleFindResultLayout->addWidget(newFriendInfo);
    middleFindResultLayout->addStretch(15);
    middleFindResultLayout->addWidget(newFriendAddBtn);
    middleFindResultLayout->addStretch(2);

    notFindInfo->hide();
    newFriendIcon->show();
    newFriendInfo->show();
    newFriendAddBtn->show();
}

void CharInterfase::setNotFriendLayout2()
{
    int itemCount = middleFindResultLayout->count();
    for(int i = (itemCount - 1); i >= 0; i--){
        QLayoutItem *item = middleFindResultLayout->layout()->takeAt(i);
        if(item != nullptr)
            middleFindResultLayout->removeWidget(item->widget());
    }
    middleFindResultLayout->addStretch(1);
    middleFindResultLayout->addWidget(notFindInfo);
    middleFindResultLayout->addStretch(1);

    newFriendIcon->hide();
    newFriendInfo->hide();
    newFriendAddBtn->hide();
    notFindInfo->show();
}

//数据库相关函数
bool CharInterfase::findUserDB(QString id) //查询用户是否存在
{
    bool result = false;
    QSqlQuery sq;
    QString sql = "select * from user where id = '" + id + "'";
    sq.exec(sql);
    if(sq.size() >= 1){
        result = true;
    }
    return result;
}

void CharInterfase::connectDB()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("chatdb");
    db.setUserName("root");
    db.setPassword("ingottkx");
    db.open();
}

void CharInterfase::friendDB()
{
    QVector<AddFri> vu;
    QSqlQuery sq;
    QString sql = "select * from friend where id = '" + user->id + "' or " + "aimsId = '" + user->id + "'";
    sq.exec(sql);
    while(sq.next()){
        AddFri af;
        af.id = sq.value(0).toString();
        af.aimsId = sq.value(1).toString();
        vu.push_back(af);
    }
    for(int i = 0; i < vu.size(); ++i){
        if(vu[i].id == user->id)
            recentChatList->addItem(getFriendItemObj(vu[i].aimsId));
        else
            recentChatList->addItem(getFriendItemObj(vu[i].id));
    }
}

void CharInterfase::messInfoDB()
{

}

void CharInterfase::addChatInfo(QVector<QPair<QString,bool>> messInfo)
{
    int len = messInfo.size() < 8 ? messInfo.size() : 8;
    for(int i = 0; i < chatMessCount; ++i){
        int index = chatMessLayoutFlag[i] ? 1: 0;
        chatRecordLayout->removeItem(chatMessLayoutArr[i]);
        QLayoutItem *it = chatMessLayoutArr[i]->layout()->itemAt(index);
        QLabel *label = qobject_cast<QLabel *>(it->widget());
        label->hide();
        delete chatMessLayoutArr[i];
    }
    for(int i = 0; i < len; ++i){
        QHBoxLayout *chatRecordItem = createChatInfo2(messInfo[messInfo.size() - len + i].first, messInfo[messInfo.size() - len + i].second);
        chatRecordLayout->addLayout(chatRecordItem);
        chatMessLayoutArr[i] = chatRecordItem;
        chatMessLayoutFlag[i] = messInfo[messInfo.size() - len + i].second;
    }
    chatRecord->update();
    chatMessCount = len;
    chatMessRmIndex = 0;
}

AddFri *CharInterfase::friendInfoHandle(QString str)
{
    AddFri *result = new AddFri;
    QString id = str.left(10);
    QString aimsId = str.left(20).right(10);
    result->id = trim(id);
    result->aimsId = trim(aimsId);
    return result;
}

void CharInterfase::justLoginAfter()
{
    QLayoutItem *item = chatInterfaseMainLayout->layout()->takeAt(1);
    if(item != nullptr)
        chatInterfaseMainLayout->removeItem(item);
   chatInterfaseMainLayout->addLayout(chatWinLayout);
}

QVector<QPair<QString,bool>> CharInterfase::resultMessDB(QString aimsId)
{
    QVector<QPair<QString,bool>> result;
    QPair<QString,bool> psb;
    QString sql = "select * from mess where (id = '" + user->id + "' and aimsId = '" + aimsId + "') "
                                             "or (id = '" + aimsId + "' and aimsId = '" + user->id + "')";
    QSqlQuery sq;
    sq.exec(sql);
    while(sq.next()){
        QString id = sq.value(0).toString();
        QString mess = sq.value(2).toString();
        if(id == user->id){
            psb.second = true;
        }
        else{
            psb.second = false;
        }
        psb.first = mess;
        result.push_back(psb);
    }
    return result;
}

QString CharInterfase::strHandle(const Mess mess)
{
    QString id = mess.id;
    QString aimsId = mess.aimsId;
    QString message = mess.mess;
    id.resize(10, char(1));
    QString result = id + aimsId;
    result.resize(20, char(1));
    result = result + message;
    result.resize(50, char(1));
    result[49] = 'm';
    return QString::fromUtf8(result.toUtf8().data());
}

QString CharInterfase::strHandle(const AddFri fri)
{
    QString id = fri.id;
    QString aimsId = fri.aimsId;
    id.resize(10, char(1));
    QString result = id + aimsId;
    result.resize(50,char(1));
    result[49] = 'f';
    return result;
}

QListWidgetItem *CharInterfase::getFriendItemObj(QString id)
{
    QListWidgetItem *result = new QListWidgetItem(QPixmap("C:\\Users\\wjingbo\\Desktop\\GraduationProject\\img\\headIcon.png"), id);
    return result;
}


//------------------------MouseEvent------------------------
void CharInterfase::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        pressFlag = true;
        beginPos = event->pos();
    }
    QWidget::mousePressEvent(event);
}

void CharInterfase::mouseMoveEvent(QMouseEvent *event)
{
    if(pressFlag){
        QPoint relPos(QCursor::pos() - beginPos);
        move(relPos);
    }
    QWidget::mouseMoveEvent(event);
}

void CharInterfase::mouseReleaseEvent(QMouseEvent *event)
{
    pressFlag = false;
    QWidget::mouseReleaseEvent(event);
}

