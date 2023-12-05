#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    TCPSocket = new QTcpSocket();
    TCPSocket->connectToHost("127.0.0.1",1100);
    connect(TCPSocket,SIGNAL(readyRead()),this,SLOT(readDataFromServer()));
    TCPSocket->open(QIODevice::ReadWrite);
    if (TCPSocket->isOpen())
    {

    }

    tiles[0] = ui->tileA8;
    tiles[8] = ui->tileA7;
    tiles[16] = ui->tileA6;
    tiles[24] = ui->tileA5;
    tiles[32] = ui->tileA4;
    tiles[40] = ui->tileA3;
    tiles[48] = ui->tileA2;
    tiles[56] = ui->tileA1;

    tiles[1] = ui->tileB8;
    tiles[9] = ui->tileB7;
    tiles[17] = ui->tileB6;
    tiles[25] = ui->tileB5;
    tiles[33] = ui->tileB4;
    tiles[41] = ui->tileB3;
    tiles[49] = ui->tileB2;
    tiles[57] = ui->tileB1;

    tiles[2] = ui->tileC8;
    tiles[10] = ui->tileC7;
    tiles[18] = ui->tileC6;
    tiles[26] = ui->tileC5;
    tiles[34] = ui->tileC4;
    tiles[42] = ui->tileC3;
    tiles[50] = ui->tileC2;
    tiles[58] = ui->tileC1;

    tiles[3] = ui->tileD8;
    tiles[11] = ui->tileD7;
    tiles[19] = ui->tileD6;
    tiles[27] = ui->tileD5;
    tiles[35] = ui->tileD4;
    tiles[43] = ui->tileD3;
    tiles[51] = ui->tileD2;
    tiles[59] = ui->tileD1;

    tiles[4] = ui->tileE8;
    tiles[12] = ui->tileE7;
    tiles[20] = ui->tileE6;
    tiles[28] = ui->tileE5;
    tiles[36] = ui->tileE4;
    tiles[44] = ui->tileE3;
    tiles[52] = ui->tileE2;
    tiles[60] = ui->tileE1;

    tiles[5] = ui->tileF8;
    tiles[13] = ui->tileF7;
    tiles[21] = ui->tileF6;
    tiles[29] = ui->tileF5;
    tiles[37] = ui->tileF4;
    tiles[45] = ui->tileF3;
    tiles[53] = ui->tileF2;
    tiles[61] = ui->tileF1;

    tiles[6] = ui->tileG8;
    tiles[14] = ui->tileG7;
    tiles[22] = ui->tileG6;
    tiles[30] = ui->tileG5;
    tiles[38] = ui->tileG4;
    tiles[46] = ui->tileG3;
    tiles[54] = ui->tileG2;
    tiles[62] = ui->tileG1;

    tiles[7] = ui->tileH8;
    tiles[15] = ui->tileH7;
    tiles[23] = ui->tileH6;
    tiles[31] = ui->tileH5;
    tiles[39] = ui->tileH4;
    tiles[47] = ui->tileH3;
    tiles[55] = ui->tileH2;
    tiles[63] = ui->tileH1;


    for(int i=0;i<64;i++){
        connect(tiles[i],SIGNAL(toggled(bool)),this,SLOT (buttonToggled(bool)));
    }


    ui->gameModeRadio->setChecked(true);
    connect(ui->connectButton,SIGNAL(clicked(bool)),this,SLOT(connectButtonToggled(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::unToggleAll(){
    for(int i=0;i<64;i++){
        tiles[i]->setChecked(false);
    }
 }

void MainWindow::buttonToggled(bool checked){
    QPushButton* btnSender = qobject_cast<QPushButton*>(sender()); // retrieve the button you have clicked
    QString btnName = btnSender->objectName(); // retrive the button name from the button clicked


    if(checked)
        toggledButtonsCounter++;
    else
        toggledButtonsCounter--;

    // 0 wciśniętych, cofnięto wciśnięcie
    if(toggledButtonsCounter==0){
        choosenTile1[0] = 'x';
        choosenTile1[1] = 'x';
    }

    // 1 przycisk wciśnięty
    if(toggledButtonsCounter==1){
        choosenTile1[0] = btnName[4].toLatin1();
        choosenTile1[1] = btnName[5].toLatin1();
    }

    // 2 przyciski wciśnięte
    if(toggledButtonsCounter==2){
        choosenTile2[0] = btnName[4].toLatin1();
        choosenTile2[1] = btnName[5].toLatin1();
        //TODO: Wysłanie wiadomości do serwera
        char sendMsg[255];
        sendMsg[0] = choosenTile1[0];
        sendMsg[1] = choosenTile1[1];
        sendMsg[2] = choosenTile2[0];
        sendMsg[3] = choosenTile2[1];
        sendMsg[4] = '\0';

        TCPSocket->write(sendMsg,sizeof(sendMsg));

        MainWindow::unToggleAll();
        toggledButtonsCounter=0;
    }


}

bool MainWindow::connectButtonToggled(bool checked)
{
    QString nickname = ui->nicknameEdit->text();
    if(nickname.length()<3)
        return false;


    char gamemode[1];
    if(ui->gameModeRadio->isChecked())
        gamemode[0] = '0';
    else     if(ui->gameModeRadio_2->isChecked())
        gamemode[0] = '1';
    else     if(ui->gameModeRadio_3->isChecked())
        gamemode[0] = '2';


    char sendMsg[255];
    strcpy(sendMsg,nickname.toStdString().c_str());


    TCPSocket->write(sendMsg,sizeof(sendMsg));

    bzero(sendMsg,sizeof(sendMsg));
    strcpy(sendMsg,gamemode);
    TCPSocket->write(sendMsg,sizeof(sendMsg));

    if(gamemode[0]=='2'){
        QString friendNickname = ui->joinGameEdit->text();
        if(nickname.length()<3)
            return false;
        bzero(sendMsg,sizeof(sendMsg));
        strcpy(sendMsg,friendNickname.toStdString().c_str());
        TCPSocket->write(sendMsg,sizeof(sendMsg));
    }

    ui->groupBox->hide();

    return true;
}




/*
 * White
 * king - ♔
 * queen - ♕
 * rook - ♖
 * bishop -♗
 * knight -♘
 * pawn - ♙

Black
 *king - ♚
 * queen - ♛
 * rook - ♜
 * bishop -♝
 * knight -♞
 * pawn - ♟︎




*/
void MainWindow::setButtonsNames(QString board){
    //std::reverse(board.begin(),board.end());

    for(int i =63;i>-1;i--){
        switch(board[i].toLatin1()){
            case 'K':{
                tiles[i]->setText(QString::fromUtf8("♔"));
                break;
            }
            case 'Q':{
                tiles[i]->setText(QString::fromUtf8("♕"));
                break;
            }
            case 'R':{
                tiles[i]->setText(QString::fromUtf8("♖"));
                break;
            }
            case 'B':{
                tiles[i]->setText(QString::fromUtf8("♗"));
                break;
            }
            case 'H':{
                tiles[i]->setText(QString::fromUtf8("♘"));
                break;
            }
            case 'P':{
                tiles[i]->setText(QString::fromUtf8("♙"));
                break;
            }
            case 'k':{
                tiles[i]->setText(QString::fromUtf8("♚"));
                break;
            }
            case 'q':{
                tiles[i]->setText(QString::fromUtf8("♛"));
                break;
            }
            case 'r':{
                tiles[i]->setText(QString::fromUtf8("♜"));
                break;
            }
            case 'b':{
                tiles[i]->setText(QString::fromUtf8("♝"));
                break;
            }
            case 'h':{
                tiles[i]->setText(QString::fromUtf8("♞"));
                break;
            }
            case 'p':{
                tiles[i]->setText(QString::fromUtf8("♟︎"));
                break;
            }
            default:{
                tiles[i]->setText(QString::fromUtf8(" "));
            }
        }
    }
}

void MainWindow::readDataFromServer()
{
    if(TCPSocket){
        if(TCPSocket->isOpen()){
            QByteArray recvMessage = TCPSocket->readAll();
                //String message = recvMessage.toStdString;
                setButtonsNames(recvMessage);

        }
    }
}



