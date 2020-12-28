#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,tcpSocket(new QTcpSocket(this))
{
    stream = new QDataStream(tcpSocket);
    connect(tcpSocket, &QIODevice::readyRead, this, &MainWindow::readData);

    ui->setupUi(this);
    ui->gameWidget->setVisible(false);
    pits[0] = ui->pit0;
    pits[1] = ui->pit1;
    pits[2] = ui->pit2;
    pits[3] = ui->pit3;
    pits[4] = ui->pit4;
    pits[5] = ui->pit5;
    pits[6] = ui->pit6;
    pits[7] = ui->pit7;
    pits[8] = ui->pit8;
    pits[9] = ui->pit9;
    pits[10] = ui->pit10;
    pits[11] = ui->pit11;
    pits[12] = ui->pit12;
    pits[13] = ui->pit13;



    player = 0;
}

MainWindow::~MainWindow()
{
    tcpSocket->disconnectFromHost();
    delete ui;
}

void MainWindow::readData()
{
    qInfo()<<"Odebrano dane";
    qint32 temp;
    stream->startTransaction();
    *stream >> temp;
    if(temp == 2)//zwykły ruch
    {
        readBoard();
        qInfo() << "Obecny gracz "<<currentPlayer;
        if(player == currentPlayer)ui->gameFlowLabel1->setText(ui->lineEditName->text());
        else ui->gameFlowLabel1->setText(enemyName);
        currentPlayer = (currentPlayer+1)%2;

    }
    else if(temp == 1)//początkowe informacje
    {

        *stream >> player;
        *stream>> temp;
        enemyName = QByteArray(temp, Qt::Uninitialized);
        //tutaj prawdopdobnie czasami jest ruch do tytułu
        //serwerdwa razy wywołuje write i drugie pewnie nie zdąża czasami
        stream->readRawData( enemyName.data() , temp);
        qInfo() <<"nr gracza "<<player<<" " << enemyName<<" przeciwnik \n ";
        if(player)
        {
            ui->labelPlayer1->setText(ui->lineEditName->text());
            ui->labelPlayer0->setText(enemyName);

        }
        else
        {
            ui->labelPlayer0->setText(ui->lineEditName->text());
            ui->labelPlayer1->setText(enemyName);

        }
        for(int i=0;i<6;i++)
        {
            pits[i + 7*((player+1)%2)]->setEnabled(false);
        }


    }
    else if(temp == 3)//koniec gry
    {
        readBoard();
        ui->pushButtonBack->setVisible(true);
        ui->pushButtonBack->setEnabled(true);
        ui->pushButtonSurrender->setEnabled(false);
        ui->gameFlowLabel0->setText("Wygrał");
        *stream>> temp;
        if(temp == -1)
        {
           ui->gameFlowLabel0->setText("Remis");
           ui->gameFlowLabel1->setText("");
        }
        else if(player == temp)ui->gameFlowLabel1->setText(ui->lineEditName->text());
        else ui->gameFlowLabel1->setText(enemyName);

    }
    else if(temp == 4)//przeciwnik poddał się
    {
        ui->gameFlowLabel0->setText("Przeciwnik poddał się");
        ui->gameFlowLabel1->setText("");
        ui->pushButtonSurrender->setEnabled(false);
        ui->pushButtonBack->setVisible(true);
        ui->pushButtonBack->setEnabled(true);
    }
    stream->commitTransaction();
}


void MainWindow::on_pushButtonStart_clicked()
{
    ui->startingWidget->setVisible(false);
    ui->gameWidget->setVisible(true);
    ui->pushButtonBack->setVisible(false);


    tcpSocket->connectToHost("127.0.0.1", 1234);

    tcpSocket->write(ui->lineEditName->text().toUtf8());
    currentPlayer = 0;

    /*for(int i=0;i<6;i++)
    {
        pits[i + 7*player]->setEnabled(false);
    }*/
}




void MainWindow::sendMove(int move)
{

    int pom = 1;
    *stream <<pom<< move;

}

void MainWindow::readBoard()
{
    qint32 temp;
    for (int i=0;i<14 ;i++ ) {
        *stream >> temp;
        //qInfo() << temp<<"informacja \n ";
        pits[i]->setText(QString::number(temp));

    }
}



void MainWindow::on_pit7_clicked()
{
    sendMove(0);
}

void MainWindow::on_pit11_clicked()
{
    sendMove(4);
}

void MainWindow::on_pit0_clicked()
{
    sendMove(0);
}

void MainWindow::on_pit1_clicked()
{
    sendMove(1);
}

void MainWindow::on_pit2_clicked()
{
    sendMove(2);
}

void MainWindow::on_pit3_clicked()
{
    sendMove(3);
}

void MainWindow::on_pit4_clicked()
{
    sendMove(4);
}

void MainWindow::on_pit5_clicked()
{
    sendMove(5);
}

void MainWindow::on_pit8_clicked()
{
    sendMove(1);
}

void MainWindow::on_pit9_clicked()
{
    sendMove(2);
}

void MainWindow::on_pit10_clicked()
{
    sendMove(3);
}

void MainWindow::on_pit12_clicked()
{
    sendMove(5);
}

void MainWindow::on_pushButtonBack_clicked()
{
    back2menu();

}

void MainWindow::on_pushButtonSurrender_clicked()
{
    int pom = 2;
    *stream <<pom<< pom;
    back2menu();

}

void MainWindow::back2menu()
{
    tcpSocket->disconnectFromHost();
    ui->gameFlowLabel0->setText("Ruch gracza");
    ui->startingWidget->setVisible(true);
    ui->gameWidget->setVisible(false);
    ui->pushButtonBack->setEnabled(false);
    ui->pushButtonSurrender->setEnabled(true);
    for(int i=0;i<6;i++)
    {
        pits[i + 7*((player+1)%2)]->setEnabled(true);
    }
}


