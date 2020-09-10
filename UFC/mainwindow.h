#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QSqlQuery>
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void onResultEvents(QNetworkReply *reply);
    void onResultFighters(QNetworkReply *reply);
    int PrintTable(QSqlQuery p_query, int index);

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *networkManagerF;
    QNetworkAccessManager *networkManagerT;
};

#endif // MAINWINDOW_H
