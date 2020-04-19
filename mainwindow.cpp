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
    // Конфігурація таблиці Карно
    int rows = 2;
    int columns = 2;
    QStringList hheaders = {"0","1"};
    QStringList vheaders = {"0","1"};

    if (ui->comboBox->currentIndex() >= 2) {columns = 4; hheaders[0] = "00"; hheaders[1] ="01";hheaders.append("11");hheaders.append("10");}
    if (ui->comboBox->currentIndex() == 3) {rows = 4;vheaders[0] = "00"; vheaders[1] ="01";vheaders.append("11");vheaders.append("10");}

    ui->tableWidget->setRowCount(rows);
    ui->tableWidget->setColumnCount(columns);

    ui->tableWidget->setHorizontalHeaderLabels(hheaders);
    ui->tableWidget->setVerticalHeaderLabels(vheaders);

    QStringList set;

    // Отримуємо набори значень, де функція дорвінює 1
    for (int i = 0; i < ui->tableWidget_2->rowCount();i++)
    {
        if (ui->tableWidget_2->item(i,ui->tableWidget_2->columnCount()-1)->text() == "1")
        {
            QString item;
            for (int j = 0;j < ui->tableWidget_2->columnCount()-1; j++)
            {
                item.append(ui->tableWidget_2->item(i,j)->text());
            }
            set.append(item);
        }
    }

    // Парсимо отримані набори і встановлюємо одиниці в таблицю Карно
    for (int i = 0; i < set.length();i++)
    {
        QString item = set[i];
        int columnIndex = 0;

        if (ui->comboBox->currentIndex() >= 2)
        {
            columnIndex = hheaders.indexOf(item.left(2));
            item.remove(0,2);
        }
        else
        {   columnIndex = hheaders.indexOf(item.left(1));
            item.remove(0,1);
        }

        int rowIndex = vheaders.indexOf(item);

        QTableWidgetItem *tableItem = new QTableWidgetItem("1");
        ui->tableWidget->setItem(rowIndex,columnIndex,tableItem);

    }

    // Заповнюємо вільні місця нулями
    for (int i = 0; i < ui->tableWidget->rowCount();i++)
    {
        for (int j = 0; j < ui->tableWidget->columnCount(); j++)
        {
            QTableWidgetItem * item = ui->tableWidget->item(i,j);
            QTableWidgetItem * zeroItem = new QTableWidgetItem("0");
            if (item == NULL)
                ui->tableWidget->setItem(i,j,zeroItem);
        }
    }

    simplifySet(simplifySet(set));

    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->resizeColumnsToContents();
}


QStringList MainWindow::simplifySet(QStringList set)
{
    for (int i = 0; i < set.length()-1; i++)
    {
        std::vector<int> indexes;
        int max = 0;
        int length = 0;
        for (int j = i+1; j < set.length();j++)
        {
            int current = 0;
            length = 0;
            for (int k = 0; k < set[i].length(); k++)
            {
                if (set[i][k] != " " && set[j][k] != " " && set[i][k] == set[j][k])
                    current++;
                if (set[i][k] != " ")
                    length++;
            }
            if (current > max)
            {
                indexes.clear();
                indexes.push_back(j);
                max = current;
            }
            else if (current == max)
            {
                indexes.push_back(j);
            }
        }
        if (max == length - 1)
        {
            for (int j = 0;j < set[i].length(); j++)
            {
                if (set[indexes[0]][j] != set[i][j])
                {
                    set[i].replace(j,1," ");
                    set[indexes[0]].replace(j,1," ");
                }
            }
            set.removeAt(indexes[0]);
        }
    }
    return set;
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
    // Конфігурація таблиці звичайної
    if (index == 0)
        return;
    int rows = 4;
    QStringList headers = {"x","y"};
    if (index == 2){rows = 8;headers.append("z");}
    if (index == 3){rows = 16;headers.append("z");headers.append("t");}

    headers.append("f");

    // очищаємо таблицю
    ui->tableWidget_2->clear();

    ui->tableWidget_2->setRowCount(rows);
    ui->tableWidget_2->setColumnCount(index + 2);

    ui->tableWidget_2->setHorizontalHeaderLabels(headers);

    // заповнюємо значення змінних
    fillVariables(rows,0,2,index+1);

    ui->tableWidget_2->resizeRowsToContents();
    ui->tableWidget_2->resizeColumnsToContents();
}


void MainWindow::on_tableWidget_2_cellChanged(int row, int column)
{
    QTableWidgetItem * item;

    // перевіряємо, чи вся таблиця заповнена
    for (int i = 0; i < ui->tableWidget_2->rowCount();i++)
    {
        for (int j = 0; j < ui->tableWidget_2->columnCount(); j++)
        {
            item = ui->tableWidget_2->item(i,j);

            if (item == NULL || item->text() != '0' && item->text() != '1')
                return;
        }
    }
    // очищаємо таблицю
    ui->tableWidget->clear();
    buildKarnoTable();
}
