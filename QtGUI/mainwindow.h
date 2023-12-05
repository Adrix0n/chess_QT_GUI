#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QtNetwork/QTcpSocket>
#include <QString>
#include <QByteArray>
#include <QGroupBox>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QPushButton *tiles[64];

    char choosenTile1[3]={' ',' ','\0'};
    char choosenTile2[3]={' ',' ','\0'};
    int toggledButtonsCounter=0;



public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void unToggleAll();
    void buttonToggled(bool checked);
    bool connectButtonToggled(bool checked);
    void newGameButtonClicked();
    void setButtonsNames(QString board);
    void readDataFromServer();
private:
    Ui::MainWindow *ui;
    QTcpSocket *TCPSocket;
};
#endif // MAINWINDOW_H
