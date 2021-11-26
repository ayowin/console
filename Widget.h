#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QProcess>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void onExecute();
    void onRestart();

    void onConsoleResponse();
    void onConsoleResponseException();
    void onConsoleStateChange(QProcess::ProcessState newState);
    void onConsoleError(QProcess::ProcessError error);

private:
    Ui::Widget *ui;
    QString currentUserName;
    QProcess process;

};
#endif // WIDGET_H
