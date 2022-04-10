#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);    

    connect(ui->ui_num0,&QPushButton::clicked,this,[this](){ inputNumber(0); });
    connect(ui->ui_num1,&QPushButton::clicked,this,[this](){ inputNumber(1); });
    connect(ui->ui_num2,&QPushButton::clicked,this,[this](){ inputNumber(2); });
    connect(ui->ui_num3,&QPushButton::clicked,this,[this](){ inputNumber(3); });
    connect(ui->ui_num4,&QPushButton::clicked,this,[this](){ inputNumber(4); });
    connect(ui->ui_num5,&QPushButton::clicked,this,[this](){ inputNumber(5); });
    connect(ui->ui_num6,&QPushButton::clicked,this,[this](){ inputNumber(6); });
    connect(ui->ui_num7,&QPushButton::clicked,this,[this](){ inputNumber(7); });
    connect(ui->ui_num8,&QPushButton::clicked,this,[this](){ inputNumber(8); });
    connect(ui->ui_num9,&QPushButton::clicked,this,[this](){ inputNumber(9); });

    connect(ui->ui_plus,&QPushButton::clicked,this,[this](){ inputOperation(OPERATION::plus); });
    connect(ui->ui_minus,&QPushButton::clicked,this,[this](){ inputOperation(OPERATION::minus); });
    connect(ui->ui_div,&QPushButton::clicked,this,[this](){ inputOperation(OPERATION::div); });
    connect(ui->ui_mul,&QPushButton::clicked,this,[this](){ inputOperation(OPERATION::mul); });

    connect(ui->ui_clear,&QPushButton::clicked,this,&MainWindow::clear);
    connect(ui->ui_decimal_point,&QPushButton::clicked,this,&MainWindow::inputDecimalPoint);
    connect(ui->ui_result,&QPushButton::clicked,this,&MainWindow::result);

    connect(this,&MainWindow::inputValueChanged,ui->ui_input,&QLabel::setText);
    connect(this,&MainWindow::formulaDisplayChanged,ui->ui_formula,&QLabel::setText);

    clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clear()
{
    m_resetValue = true;
    m_error = false;
    m_leftValue = "0";
    m_operation = OPERATION::none;

    m_inputValue = "0";
    emit formulaDisplayChanged("");
    emit inputValueChanged(m_inputValue);
}

void MainWindow::inputNumber(int num)
{
    qDebug() << Q_FUNC_INFO << ":" << num;

    if (m_error)
    {
        clear();
    }

    QString value = m_inputValue;
    if (value == "0")
    {
        if (num != 0)
        {
            value = QString::number(num);
            m_resetValue = false;
        }
    }
    else {
        if (m_resetValue)
        {
            value = QString::number(num);
            m_resetValue = false;
        }
        else
        {
            if (value.length() >= 10)
            {
                return;
            }
            value += QString::number(num);
        }
    }
    m_inputValue = value;
    emit inputValueChanged(m_inputValue);
}

void MainWindow::inputDecimalPoint()
{
    qDebug() << Q_FUNC_INFO;

    QString value = m_inputValue;
    if (m_error)
    {
        clear();
    }

    if (value.length() >= 8)
    {
        return;
    }

    if (!value.contains(".")){
        value += ".";
    }

    m_resetValue = false;
    m_inputValue = value;
    emit inputValueChanged(m_inputValue);
}

void MainWindow::inputOperation(OPERATION op)
{
    qDebug() << Q_FUNC_INFO << ":" << op;

    if (m_error)
    {
        clear();
        return;
    }

    if (m_operation == OPERATION::none)
    {
        m_operation = op;
        m_leftValue = m_inputValue;
        m_resetValue = true;
        emit formulaDisplayChanged(m_leftValue + " " + convertOprationToString(op));
        return;
    }

    double left = m_leftValue.toDouble();
    double right = m_inputValue.toDouble();

    if (m_operation == OPERATION::mul && right == 0.0)
    {
        clear();
        m_error = true;
        m_inputValue = "0で割ることはできません";
        emit inputValueChanged(m_inputValue);
        return;
    }

    double result = calc(left,right,m_operation);
    m_operation = op;

    m_leftValue = QString::number(result);
    m_resetValue = true;
    emit formulaDisplayChanged(m_leftValue + " " + convertOprationToString(m_operation));
}

void MainWindow::result()
{
    qDebug() << Q_FUNC_INFO;

    if (m_error)
    {
        clear();
        return;
    }

    if (m_operation == OPERATION::none)
    {
        m_resetValue = true;
        if (m_error)
        {
            emit formulaDisplayChanged("");
        }
        else
        {
            emit formulaDisplayChanged(m_inputValue + " =");
        }
        return;
    }

    double left = m_leftValue.toDouble();
    double right = m_inputValue.toDouble();

    if (m_operation == OPERATION::mul && right == 0.0)
    {
        clear();
        m_error = true;
        m_inputValue = "0で割ることはできません";
        emit inputValueChanged(m_inputValue);
        return;
    }

    QString formula = m_leftValue + " " + convertOprationToString(m_operation) + " " + m_inputValue + " =";
    double result = calc(left,right,m_operation);

    m_operation = OPERATION::none;
    m_resetValue = true;
    emit formulaDisplayChanged(formula);

    m_inputValue = QString::number(result);
    emit inputValueChanged(m_inputValue);
}

double MainWindow::calc(double left,double right,OPERATION op)
{
    qDebug() << Q_FUNC_INFO << ":" << "left=" << left << " right=" << right << " operation=" << op;

    double calc = 0;
    switch(op)
    {
    case OPERATION::none:
        break;
    case OPERATION::plus:
        calc = left + right;
        break;
    case OPERATION::minus:
        calc = left - right;
        break;
    case OPERATION::div:
        calc = left * right;
        break;
    case OPERATION::mul:
        calc = left / right;
        break;
    }
    return calc;
}


QString MainWindow::convertOprationToString(OPERATION op)
{
    switch(op)
    {
    case OPERATION::none:
        break;
    case OPERATION::plus:
        return "+";
    case OPERATION::minus:
        return "-";
    case OPERATION::div:
        return "*";
    case OPERATION::mul:
        return "/";
    }
    return "?";
}

