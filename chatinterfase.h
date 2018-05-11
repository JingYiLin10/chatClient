#ifndef CHARINTERFASE_H
#define CHARINTERFASE_H

#include <QWidget>
#include <QPoint>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStackedLayout>
#include <QPainter>
#include <QLabel>
#include <QDebug>
#include <QToolBox>
#include <QLineEdit>
#include <QToolButton>
#include <QGroupBox>
#include <QPalette>
#include <QTextEdit>
#include <QListWidget>
#include <QScrollBar>
#include <QFontMetrics>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QVector>
#include <QPair>
struct User{
    QString id;
    QString pwd;
};

struct Mess{
    QString id;
    QString aimsId;
    QString mess;
};

struct AddFri{
    QString id;
    QString aimsId;
};

class CharInterfase : public QWidget
{
    Q_OBJECT
public:
    explicit CharInterfase(User,QWidget *parent = 0);
    ~CharInterfase();

signals:

public slots:
    void windowMinimize();
    void chat_add_SwitchPage();
    void addFriend();
    void createChatInfo1();
    void findFriendSlot();
    void setChatObj(QListWidgetItem*);
    void tcpClose();
    void readMessHandle();
private:
    void createMainObj();
    void setWindowControlsAttributes(Qt::WindowFlags, int, int);
    void setWindowControlsLayout();
    void setStackedLayoutAttributes();

    void createChatInterfaseObj();
    void setChatInterfaseAttributes();
    void setChatInterfaseLayout();

    void createAddFriObj();
    void setAddFriAttributes();
    void setAddFriLayout();

//----------------------------------------
    void connectDB();
    bool findUserDB(QString);
    void friendDB();
    void messInfoDB();  //未用
    QVector<QPair<QString,bool>> resultMessDB(QString);
    QString trim(QString);
    void setUser(User);
    QString strHandle(const Mess);
    QString strHandle(const AddFri);

    User *user;
    QSqlDatabase db;
    QTcpSocket * tcpSocket;
    QWidget *chatNav;
    QPushButton *addFriendInfoBtn;
    QPushButton *chatInterfaseBtn;
    QPushButton *miniBtn;
    QPushButton *maxBtn;
    QPushButton *closeBtn;

    QHBoxLayout *navLayout;
    QStackedLayout *chat_add_StackLayout;
    QVBoxLayout *mainLayout;

//----------------------------------------
    QHBoxLayout *createChatInfo2(QString, bool);
    void addChatInfo(QVector<QPair<QString,bool>>);
    AddFri *friendInfoHandle(QString);
    QPushButton *lookWorldImg;
    bool justLogin;
    QHBoxLayout *lookWorldLayout;
    void justLoginAfter();

    QWidget *chatInterfasePage;
    int chatMessCount, chatMessRmIndex;
    QListWidget *recentChatList;
    QLabel *chatObjName;
    QWidget *chatRecord;
    QTextEdit *chatText;
    QPushButton *sendBtn;
    QHBoxLayout *chatMessLayoutArr[8];
    bool chatMessLayoutFlag[8];

    QHBoxLayout *chatWinTitleLayout;
    QVBoxLayout *chatRecordLayout;
    QHBoxLayout *chatWinSendLayout;
    QVBoxLayout *chatWinLayout;
    QHBoxLayout *chatInterfaseMainLayout;

//----------------------------------------
    void setFriendLayout1();
    void setFriendLayout2();
    void setNotFriendLayout1();
    void setNotFriendLayout2();
    QListWidgetItem *getFriendItemObj(QString);

    QWidget *addFriendPage;
    QWidget *middleWidget;
    QLabel *middleTopBg;
    QLabel *middleTitleInfo;
    QLineEdit *middleFindLine;
    QPushButton *middleFindBtn;
    QWidget *findFriendWidget;
    QPushButton *newFriendIcon;
    QPushButton *newFriendAddBtn;
    QLabel *newFriendInfo;
    QLabel *notFindInfo;

    QHBoxLayout *middleFindResultLayout;
    QHBoxLayout *middleTitleInfoLayout;
    QHBoxLayout *middleFindLayout;
    QVBoxLayout *middleLayout;
    QHBoxLayout *addFriendPageMainLayout1;
    QVBoxLayout *addFriendPageMainLayout2;

//----------------------------------------
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    bool pressFlag;
    QPoint beginPos;

};

#endif // CHARINTERFASE_H
