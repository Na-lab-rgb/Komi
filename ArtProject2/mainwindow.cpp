#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myglwidget.h"
#include <QPainter>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //widgets
    ent1=new QLineEdit;
    ent2=new QLineEdit;
    grid=new QGridLayout();
    openGLW=new myGLWidget(this);
    btn_calc=new QPushButton;
    for (int i=0;i<5;i++)
    {
        QPushButton* btn=new QPushButton;
        ui->verticalLayout->addWidget(btn);
        connect(btn,&QPushButton::clicked,this,&MainWindow::Fswitch);
        G->addButton(btn,i);
    }


    G->button(0)->setText("Вершина");
    G->button(1)->setText("-->");
    G->button(2)->setText("<->");
    G->button(3)->setText("Передвинуть");
    G->button(4)->setText("Удалить");
    btn_calc->setText("рассчитать");
    ui->MainLayout->addWidget(openGLW);
    ui->verticalLayout->addWidget(G->button(0));
    ui->verticalLayout->addWidget(G->button(3));
    ui->verticalLayout_4->addWidget(G->button(4));
    ui->verticalLayout_4->addWidget(btn_calc);
    ui->verticalLayout_2->addWidget(ent1);
    ui->verticalLayout_2->addWidget(ent2);
    ui->verticalLayout_3 ->addWidget(G->button(1));
    ui->verticalLayout_3->addWidget(G->button(2));
    connect(btn_calc,&QPushButton::clicked,this,&MainWindow::Fcheck);
}
void MainWindow::Fswitch()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    int buttonID = G->id(button);

    extern QString status;
    extern QString text_for_arrows;
    extern QString statusBook[7];
    if (buttonID==1) text_for_arrows=ent1->text();
    if (buttonID==2) text_for_arrows=ent2->text();
    status=statusBook[buttonID];
}
void MainWindow::Fcheck()
{
    try
    {
        F();
    }
    catch(...)
    {
        QMessageBox::warning(this, "Внимание","невозможно вычислить");
    }
}
void MainWindow::F()
{
    extern int len;
    extern QVector <QVector<int>> matrix;
    QVector <int> result_arr_1;
    QVector <int> result_arr_2;
    int sum=0;
    bool flag=true;
    QVector <QVector<int>> data=matrix;
    for (int i=0;i<len;i++) for (int k=0;k<len;k++) if (i==k || data[i][k]==0) data[i][k]=INT_MAX;
    while (flag)
    {
        QVector <int> row(len);
        row.fill(INT_MAX);
        QVector <int> column(len);
        column.fill(INT_MAX);
        for (int i=0;i<len;i++) for (int k=0;k<len;k++) row[i]=qMin(row[i],data[i][k]);
        for (int i=0;i<len;i++) for (int k=0;k<len;k++) if (row[i]!=INT_MAX && data[i][k]!=INT_MAX) data[i][k]-=row[i];
        for (int i=0;i<len;i++) for (int k=0;k<len;k++) column[k]=qMin(column[k],data[i][k]);
        for (int i=0;i<len;i++) for (int k=0;k<len;k++) if (column[k]!=INT_MAX && data[i][k]!=INT_MAX) data[i][k]-=column[k];
        int max_mark_x;
        int max_mark_y;
        int max_mark=-1;
        for (int i=0;i<len;i++) for (int k=0;k<len;k++) if (data[i][k]==0)
        {
            int min1=INT_MAX;
            int min2=INT_MAX;
            for (int j=0;j<len;j++) if (j!=k) min1=qMin(min1,data[i][j]);
            for (int j=0;j<len;j++) if (j!=i) min2=qMin(min2,data[j][k]);
            if (min1==INT_MAX || min2==INT_MAX || min1+min2>max_mark)
            {
                max_mark=min1+min2;
                max_mark_x=i;
                max_mark_y=k;
            }
        }
        sum+=matrix[max_mark_x][max_mark_y];
        data[max_mark_y][max_mark_x]=INT_MAX;
        for (int i=0;i<len;i++)
        {
            data[max_mark_x][i]=INT_MAX;
            data[i][max_mark_y]=INT_MAX;
        }
        result_arr_1.push_back(max_mark_x+1);
        result_arr_2.push_back(max_mark_y+1);
        flag=false;
        for (int i=0;i<len;i++) for (int k=0;k<len;k++) if (data[i][k]!=INT_MAX) flag=true;
    }
    QString result="->"+QString::number(result_arr_1[0])+"->"+QString::number(result_arr_2[0]);
    int x=result_arr_2[0];
    result_arr_1.erase(result_arr_1.begin());
    result_arr_2.erase(result_arr_2.begin());
    while (result_arr_1.size()!=0)
    {
        for (int i=0;i<result_arr_1.size();i++) if (result_arr_1[i]==x)
        {
            result+="->"+QString::number(result_arr_2[i]);
            x=result_arr_2[i];
            result_arr_1.erase(result_arr_1.begin()+i);
            result_arr_2.erase(result_arr_2.begin()+i);
            break;
        }
    }
    result[0]=' ';
    result[1]=' ';
    result+="|"+QString::number(sum);
    QMessageBox::information(this, "Результат",result);
}
MainWindow::~MainWindow()
{
    delete ui;
}

