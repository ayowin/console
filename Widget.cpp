#include "Widget.h"
#include "ui_Widget.h"

#include <QDebug>
#include <QDir>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , currentUserName(QDir::home().dirName())
{
    ui->setupUi(this);

    this->ui->lineEdit->setFocus();

    connect(this->ui->pushButton,SIGNAL(clicked()),this,SLOT(onExecute()));
    connect(this->ui->lineEdit,SIGNAL(returnPressed()),this,SLOT(onExecute()));
    connect(this->ui->pushButton_2,SIGNAL(clicked()),this,SLOT(onRestart()));

    connect(&process,SIGNAL(readyReadStandardOutput()),this,SLOT(onConsoleResponse()));
    connect(&process,SIGNAL(readyReadStandardError()),this,SLOT(onConsoleResponseException()));
    connect(&process,SIGNAL(stateChanged(QProcess::ProcessState)),this,SLOT(onConsoleStateChange(QProcess::ProcessState)));
    connect(&process,SIGNAL(errorOccurred(QProcess::ProcessError)),this,SLOT(onConsoleError(QProcess::ProcessError)));

#ifdef _WIN32
    process.setProgram("cmd");
#else
    process.setProgram("bash");
#endif

    process.start();
}

Widget::~Widget()
{
    process.close();

    delete ui;
}

void Widget::onExecute()
{
    QString command = this->ui->lineEdit->text().trimmed();
    QByteArray byteArray = command.toLocal8Bit();
    byteArray.append('\n');
    int count = process.write(byteArray);

    if(count >= byteArray.length())
    {
#ifndef _WIN32
    this->ui->textEdit->append(QString("[%1]: %2").arg(currentUserName).arg(command));
#endif
        this->ui->lineEdit->setText("");
    }
}

void Widget::onRestart()
{
    process.close();

    QString hint = QString("\n[Attention: %1]\n").arg("The last console has exited , the new console has started.");
    this->ui->textEdit->append(hint);

    process.start();
}

void Widget::onConsoleResponse()
{
    QByteArray byteArray = process.readAllStandardOutput();
    QString response = QString::fromLocal8Bit(byteArray);

    this->ui->textEdit->append(response);
}

void Widget::onConsoleResponseException()
{
    QByteArray byteArray = process.readAllStandardOutput();
    QString exception = QString::fromLocal8Bit(byteArray);

    qDebug() << exception << endl;
}

void Widget::onConsoleStateChange(QProcess::ProcessState newState)
{
    qDebug() << newState << endl;

    switch (newState)
    {
    case QProcess::ProcessState::Starting:
        break;
    case QProcess::ProcessState::Running:
        break;
    case QProcess::ProcessState::NotRunning:
        break;
    default:
        break;
    }
}

void Widget::onConsoleError(QProcess::ProcessError error)
{
    qDebug() << error <<endl;

    switch (error)
    {
    case QProcess::FailedToStart:
        break;
    case QProcess::Crashed:
        break;
    case QProcess::Timedout:
        break;
    case QProcess::WriteError:
        break;
    case QProcess::ReadError:
        break;
    case QProcess::UnknownError:
        break;
    default:
        break;
    }
}

