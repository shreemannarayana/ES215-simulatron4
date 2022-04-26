#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}
MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QFile file("inputFile.s");

    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox :: warning(this,"title","file not open");
    }
    QTextStream out(&file);
    QString text = ui->Inbox->toPlainText ();

    out << text;
    file.flush();
    file.close();

    QString  command( "./assemble1" );
    QStringList params;

    if(ui->Hex->isChecked()){
        params = QStringList() << "inputFile.s" << "outputFile.o" << "hex";
    }
    else{
        params = QStringList() << "inputFile.s" << "outputFile.o" << "bin";
    }


    QProcess::startDetached( command, params );
    QFile file1("outputFile.o");
    if(!file1.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox :: warning(this,"Error","file not open");
    }
    QTextStream in(&file1);
    QString text1 = in.readAll();
    ui->Plaintext->setPlainText(text1);
    file1.close();
}


void MainWindow::on_pushButton_2_clicked()
{
    QFile file("DFile.o");


    if(!file.open(QFile::ReadWrite | QFile::Text)) {
        QMessageBox :: warning(this,"title","file not open");
    }
    QTextStream out(&file);
    QString text = ui->Plaintext->toPlainText ();

    out << text;
    file.flush();
    file.close();

    QString  command( "./assemble2" );
    QStringList params;
    params = QStringList() << "DFile.o";
    QProcess::startDetached( command, params );

    QFile file1("DFile.s");
    if(!file1.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox :: warning(this,"Error","file not open");
    }

    QTextStream in(&file1);
    QString text1 = in.readAll();
    ui->plainTextEdit->setPlainText(text1);
    file1.close();
}
