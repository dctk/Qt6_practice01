#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum OPERATION{
        none,
        plus,
        minus,
        div,
        mul,
    };

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void inputDecimalPoint();
    void result();
    void clear();

signals:
    void inputValueChanged(const QString &value);
    void formulaDisplayChanged(const QString &value);

private:
    Ui::MainWindow *ui;

    bool m_resetValue;
    bool m_error;

    QString m_formulaDisplay;
    QString m_leftValue;
    QString m_inputValue;
    OPERATION m_operation;

private:
    QString convertOprationToString(OPERATION op);
    double calc(double left,double right,OPERATION op);
    void inputNumber(int num);
    void inputOperation(OPERATION op);

};
#endif // MAINWINDOW_H
