#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::buildKarnoTable()
{
    int rows = 2;
    int columns = 2;
    QStringList hheaders = {"0","1"};
    QStringList vheaders = {"0","1"};

    if (ui->comboBox->currentIndex() >= 2) {columns = 4; hheaders[0] = "00"; hheaders[1] ="01";hheaders.append("11");hheaders.append("10");}
    if (ui->comboBox->currentIndex() == 3) {rows = 4;vheaders[0] = "00"; vheaders[1] ="01";vheaders.append("11");vheaders.append("10");}

    ui->tableWidget->setHorizontalHeaderLabels(hheaders);
    ui->tableWidget->setVerticalHeaderLabels(vheaders);

    ui->tableWidget->setRowCount(rows);
    ui->tableWidget->setColumnCount(columns);

    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->resizeColumnsToContents();
}


void MainWindow::fillVariables(int rows, int column, int multiplier, int variables)
{   // умова виходу з рекурсії
    if (variables == column)
        return;

    // ознака нуликів і одиничок
    bool isIt = false;
    for (int i = 1; i <= rows; i++)
    {
        QTableWidgetItem * item;
        if (!isIt)
            item = new QTableWidgetItem("0");
        else
            item = new QTableWidgetItem("1");
        if (i % (rows/(multiplier)) == 0) // ознака зміни нуликів на одинички чи навпаки
            isIt = !isIt;
        // встановлюємо елемент
        ui->tableWidget_2->setItem(i-1, column, item);
    }
    // виклик функції
    fillVariables(rows,column+1,multiplier*2, variables);
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    if (index == 0)
        return;
    int rows = 4;
    QStringList headers = {"x","y"};
    if (index == 2){rows = 8;headers.append("z");}
    if (index == 3){rows = 16;headers.append("z");headers.append("t");}

    headers.append("f");

    ui->tableWidget_2->setRowCount(rows);
    ui->tableWidget_2->setColumnCount(index + 2);

    ui->tableWidget_2->setHorizontalHeaderLabels(headers);

    fillVariables(rows,0,2,index+1);

    ui->tableWidget_2->resizeRowsToContents();
    ui->tableWidget_2->resizeColumnsToContents();
}


void MainWindow::on_tableWidget_2_cellChanged(int row, int column)
{
    QTableWidgetItem * item;

    for (int i = 0; i < ui->tableWidget_2->rowCount();i++)
    {
        for (int j = 0; j < ui->tableWidget_2->columnCount(); j++)
        {
            item = ui->tableWidget_2->item(i,j);

            if (item == NULL || item->text() != '0' && item->text() != '1')
                return;
        }
    }

    buildKarnoTable();
}
