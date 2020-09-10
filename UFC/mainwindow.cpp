#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QString>
#include <QtSql/QSqlRecord>
#include <string>
#include <qpixmap.h>
#include <QHttpPart>
#include <QDate>
#include <QLabel>
#include <QSqlError>
#include <QTableWidget>

QSqlDatabase DB;
     int MaxID = 700000;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    networkManagerF = new QNetworkAccessManager();
    networkManagerT = new QNetworkAccessManager();

    QNetworkRequest RequestFighters;
    QNetworkRequest RequestEvents;
    RequestFighters.setUrl(QUrl("http://ufc-data-api.ufc.com/api/v3/iphone/fighters"));
    RequestEvents.setUrl(QUrl("http://ufc-data-api.ufc.com/api/v3/iphone/events"));
    networkManagerF->get(RequestFighters);

    connect(networkManagerF, &QNetworkAccessManager::finished, this, &MainWindow::onResultFighters);

    networkManagerT->get(RequestEvents);

    connect(networkManagerT, &QNetworkAccessManager::finished, this, &MainWindow::onResultEvents);

    DB = QSqlDatabase::addDatabase("QSQLITE");
    DB.setDatabaseName("postgres");
    DB.setHostName("127.0.0.1");
    DB.setPort(5432);
    DB.setPassword("1234");
    DB.setUserName("user");
    DB.open();

    QSqlQuery query;

}

MainWindow::~MainWindow()
{
    QSqlQuery query;




    delete ui;
}

void MainWindow::onResultFighters(QNetworkReply *reply)
{
   QSqlQuery query;

   QString queryStr;

   if(!reply->error())
   {
       QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

       QJsonArray root = document.array();

       query.exec("DROP TABLE If exists Fighters");
       queryStr = "CREATE TABLE Fighters(id integer PRIMARY KEY, first_name varchar(100), last_name varchar(100), weight_class varchar(100), wins int, losses int, draws int, fighter_status varchar(100))";
       query.exec(queryStr);

       int id;
       for(int i = 0; i < root.count() - 600; i++)
       {
           QJsonObject people = root.at(i).toObject();

               id = people.value("id").toInt();

               QString FirstName = people.value("first_name").toString();
               QString LastName = people.value("last_name").toString();

               QString WeightClass = people.value("weight_class").toString();

               int Wins;
               int Losses;
               int Draws;

               Wins = people.value("wins").toInt();
               Losses = people.value("losses").toInt();
               Draws = people.value("draws").toInt();

               QString FighterStatus = people.value("fighter_status").toString();

                   queryStr = "INSERT INTO Fighters VALUES(" + QString::number(id) + ",'" + FirstName + "','" + LastName + "','" + WeightClass +
                                   "'," + QString::number(Wins) + "," + QString::number(Losses) + "," + QString::number(Draws) + ",'" + FighterStatus + "')";

               if(!query.exec(queryStr))
               {
                   ui->textEdit->append(queryStr);
               }
       }
   }

   query.exec("SELECT * FROM Fighters");

//ORDER BY Fixtures.id DESC

   MainWindow::PrintTable(query, 0);

   reply->deleteLater();
}

void MainWindow::onResultEvents(QNetworkReply *reply)
{
    QSqlQuery query;

    QString queryStr;
    if(!reply->error())
    {

        query.exec("DROP TABLE If exists Events");
        queryStr = "CREATE TABLE Events(Event_date varchar(100), Event_name varchar(100), Event_status varchar(100), fighter1_id int, fighter2_id int)";
        query.exec(queryStr);

        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

        QJsonArray root = document.array();

        for(int i = 0; i < root.count() - 300; i++)
        {
            QJsonObject event = root.at(i).toObject();

                QString Event_date = event.value("event_date").toString();
                QString Event_name = event.value("base_title").toString();
                QString Event_status = event.value("event_status").toString();

                int Fighter1id;
                int Fighter2id;

                Fighter1id = event.value("main_event_fighter1_id").toInt();
                Fighter2id = event.value("main_event_fighter2_id").toInt();

                queryStr = "INSERT INTO Events VALUES('" + Event_date + "','" + Event_name + "','" + Event_status +
                                    "'," + QString::number(Fighter1id) + "," + QString::number(Fighter2id) + ")";

                if(!query.exec(queryStr))
                {
                    ui->textEdit->append(queryStr);
                }
        }

        query.exec("DROP VIEW FirstFighter");
        query.exec("DROP VIEW SecondFighter");

        if(!query.exec("CREATE VIEW FirstFighter AS SELECT * FROM Events join Fighters on Fighter1_id = id")) //left
            ui->textEdit->append(query.lastError().text());
        if(!query.exec("CREATE VIEW SecondFighter AS SELECT * FROM Events join Fighters on Fighter2_id = id")) //left
            ui->textEdit->append(query.lastError().text());
        if(!query.exec("SELECT FirstFighter.Event_date, FirstFighter.Event_name, FirstFighter.first_name, FirstFighter.last_name, FirstFighter.Event_status, SecondFighter.first_name, SecondFighter.last_name FROM FirstFighter JOIN SecondFighter ON  FirstFighter.Fighter1_id = SecondFighter.Fighter1_id AND FirstFighter.Fighter2_id = SecondFighter.Fighter2_id"))
            ui->textEdit->append(query.lastError().text());



    }
   // query.exec("SELECT * FROM Events");
    MainWindow::PrintTable(query, 1);

    query.exec("DROP VIEW FirstFighter");
    query.exec("DROP VIEW SecondFighter");


    reply->deleteLater();
}

int  MainWindow::PrintTable(QSqlQuery p_query, int index)
{
    QTableWidget *table = new QTableWidget();

    if(index == 0)
        table = ui->tableWidget;
    if(index == 1)
        table = ui->tableWidget_2;

    QSqlRecord rec = p_query.record();

    if (rec.isEmpty())
        return 1;

    table->clear();

    p_query.last();
    int Size = p_query.at() + 1;
    p_query.first();
    table->setRowCount(Size);


    if (index == 0)
    {
        table->setColumnCount(8);
        table->setColumnWidth(0, 50);
        table->setColumnWidth(1, 90);
        table->setColumnWidth(2, 90);
        table->setColumnWidth(3, 150);
        table->setColumnWidth(4, 70);
        table->setColumnWidth(5, 70);
        table->setColumnWidth(6, 70);
        table->setColumnWidth(7, 90);
    }
    if (index == 1)
    {
        table->setColumnCount(7);
        table->setColumnWidth(0, 75);
        table->setColumnWidth(1, 150);
        table->setColumnWidth(2, 90);
        table->setColumnWidth(3, 90);
        table->setColumnWidth(4, 90);
        table->setColumnWidth(5, 90);
        table->setColumnWidth(6, 90);
    }

    QStringList FieldsNames;

    for (int i = 0; i < 8; i++)
    {
        FieldsNames.append(qPrintable(rec.fieldName(i)));
    }

    ui->tableWidget->setHorizontalHeaderLabels(FieldsNames);

    QTableWidgetItem* Item;
    int j = 0;

    do
    {
        rec = p_query.record();

        Item = new QTableWidgetItem(rec.value(0).toString());
        table->setItem(j, 0, Item);

        Item = new QTableWidgetItem(rec.value(1).toString());
        table->setItem(j, 1, Item);

        Item = new QTableWidgetItem(rec.value(2).toString());
        table->setItem(j, 2, Item);

        Item = new QTableWidgetItem(rec.value(3).toString());
        table->setItem(j, 3, Item);

        Item = new QTableWidgetItem(rec.value(4).toString());
        table->setItem(j, 4, Item);

        Item = new QTableWidgetItem(rec.value(5).toString());
        table->setItem(j, 5, Item);

        Item = new QTableWidgetItem(rec.value(6).toString());
        table->setItem(j, 6, Item);

        if (index == 0)
        {
            Item = new QTableWidgetItem(rec.value(7).toString());
            table->setItem(j, 7, Item);
        }

        j++;

    } while (p_query.next());

    return 0;
}

void MainWindow::on_pushButton_clicked()
{
    QSqlQuery query;
    if(!query.exec("INSERT INTO Fighters VALUES (" + QString::number(MaxID) + ",'" + ui->textEdit_2->toPlainText() + "','" + ui->textEdit_3->toPlainText() + "','" + ui->textEdit_7->toPlainText() + "','" +
                   ui->textEdit_4->toPlainText() + "','" + ui->textEdit_5->toPlainText() + "','" + ui->textEdit_6->toPlainText() + "','Active')"))
        ui->textEdit->append(query.lastError().text());
    MaxID++;
    query.exec("SELECT * FROM Fighters");
    MainWindow::PrintTable(query, 0);
}

void MainWindow::on_pushButton_2_clicked()
{
    QSqlQuery query;
    if(!query.exec("DELETE FROM Fighters WHERE id=" +  QString::number(ui->textEdit_8->toPlainText().toInt())))
        ui->textEdit->append(query.lastError().text());

    query.exec("SELECT * FROM Fighters");
    MainWindow::PrintTable(query, 0);

    query.exec("DROP VIEW FirstFighter");
    query.exec("DROP VIEW SecondFighter");

    if(!query.exec("CREATE VIEW FirstFighter AS SELECT * FROM Events join Fighters on Fighter1_id = id")) //left
        ui->textEdit->append(query.lastError().text());
    if(!query.exec("CREATE VIEW SecondFighter AS SELECT * FROM Events join Fighters on Fighter2_id = id")) //left
        ui->textEdit->append(query.lastError().text());
    if(!query.exec("SELECT FirstFighter.Event_date, FirstFighter.Event_name, FirstFighter.first_name, FirstFighter.last_name, FirstFighter.Event_status, SecondFighter.first_name, SecondFighter.last_name FROM FirstFighter JOIN SecondFighter ON  FirstFighter.Fighter1_id = SecondFighter.Fighter1_id AND FirstFighter.Fighter2_id = SecondFighter.Fighter2_id"))
        ui->textEdit->append(query.lastError().text());
    MainWindow::PrintTable(query, 1);
}
