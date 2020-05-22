#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>

// зберігання використаних областей
QList<QList<QTableWidgetItem*>> used;
QStringList hheaders = {"0","1"};
QStringList vheaders = {"0","1"};

using namespace std;

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
    if (ui->comboBox->currentIndex() == 1){vheaders[0]="0";vheaders[1]="1";hheaders[0]="0";hheaders[1]="1";}
    if (ui->comboBox->currentIndex() >= 2) {columns = 4; hheaders[0] = "00"; hheaders[1] ="01";hheaders.append("11");hheaders.append("10");vheaders[0]="0";vheaders[1]="1";}
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
        QString tmp;
        int columnIndex = 0;

        if (ui->comboBox->currentIndex() >= 2)
        {
            columnIndex = hheaders.indexOf(item.left(2));
            if(ui->comboBox->currentIndex() ==3)tmp=item.right(2);
            if(ui->comboBox->currentIndex() ==2)tmp=item.right(1);

        }
        else
        {   columnIndex = hheaders.indexOf(item.left(1));
            if(ui->comboBox->currentIndex()==1) tmp=item.right(1);

        }

        int rowIndex = vheaders.indexOf(tmp);

        QTableWidgetItem *tableItem = new QTableWidgetItem("1");
        ui->tableWidget->setItem(rowIndex,columnIndex,tableItem);

    }

    // Заповнюємо вільні місця нулями
    for (int i = 0; i < ui->tableWidget->rowCount();i++)
    {
        for (int j = 0; j < ui->tableWidget->columnCount(); j++)
        {
            QTableWidgetItem * item = ui->tableWidget->item(i,j);
            if (item == NULL){

                QTableWidgetItem * zeroItem = new QTableWidgetItem("0");
                ui->tableWidget->setItem(i,j,zeroItem);}
        }
    }

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

// отримання двійкових кодів з карти Карно
QString MainWindow::getItemsCode(QTableWidgetItem * item)
{
    return hheaders[item->column()] + vheaders[item->row()];
}


// функція для мінімізації термів
QString MainWindow::calculate(QList<QString>selected)
{   // якщо передано два елементи
    if (selected.length() == 2)
    {   // дістаємо елементи
        QString item = selected[0];
        QString item2 = selected[1];
        // порівнюємо змінні
        for (int i = 0; i < item.length();i++)
        {   // якщо змінні не співпадають
            if (item[i] != item2[i])
            {
                item[i] = 'x';
                return item;
            }
        }
    }

    // створюємо дві групи
    QList<QString> group1;
    QList<QString> group2;

    // розподіляємо усі елементи на дві групи
    for (int i = 0; i < selected.length();i++)
    {
        if (i < selected.length()/2)
            group1.append(selected[i]);
        else
            group2.append(selected[i]);
    }
    // група з результатом
    QList<QString> group3;
    group3.append(calculate(group1));
    group3.append(calculate(group2));
    // повертаємо результат
    return calculate(group3);
}


// оптимізація реузльтату
void MainWindow::optimize()
{   // отримуємо усі терми
    QList<QString> container = ui->label->text().remove("F = ").split("&");
    // якщо їх більше одного
    if (container.length() > 1)
    {   // співставляємо кожен з наступним
        for (int i = 0; i < container.length();i++)
        {
            for (int j = i+1; j < container.length(); j++)
            {   // індекс місця, де змінні не співпадають, та кількість неспівпадінь
                ushort index = 0;
                ushort differ = 0;
                // продивляємо кожен елемент, співставляємо змінні
                for (int k = 0; k < container[i].length();k++)
                {
                    if (container[i][k] != container[j][k])
                    {   // якщо десь порожнє місце, то виходимо з циклу
                        if (container[i][k] == 'x' || container[j][k] == 'x')
                            break;
                        else
                        {
                            differ++;
                            index = k;
                        }
                    }
                } // якщо відрізняється одна змінна
                if (differ == 1)
                {   // видаляємо її і повторюваний елемент
                    container[i][index] = 'x';
                    container.removeAt(j);
                }
            }
        }

        // втсановлюємо новий результат
        ui->label->setText("F = ");

        for (int i = 0; i < container.length(); i++)
        {
            if (ui->label->text() == ("F = "))
                ui->label->setText(ui->label->text() + container[i]);
            else
                ui->label->setText(ui->label->text() + "&" + container[i]);
        }
    }
}


//QString MainWindow::translate(QStringList content)
//{
//    ui->label->setText("F = ");

//    for (int i = 0; i < content.length(); i++)
//    {
//        for (int j = 0; j < content[i].length();j++)
//        {
//            if (content[i][j] != 'x')
//            {
//                if (j == 0)
//                {
//                    if (content[i][j] == '0')
//                        ui->label->setText(ui->label->text() + "!x");
//                    else
//                        ui->label->setText(ui->label->text() + "x");
//                }
//                if (j == 1)
//                {
//                    if (content[i][j] == '0')
//                        ui->label->setText(ui->label->text() + QString("!y"));
//                    else
//                        ui->label->setText(ui->label->text() + "y");
//                }
//                if (j == 2)
//                {
//                    if (content[i][j] == '0')
//                        ui->label->setText(ui->label->text() + "!z");
//                    else
//                        ui->label->setText(ui->label->text() + "z");
//                }
//                if (j == 3)
//                {
//                    if (content[i][j] == '0')
//                        ui->label->setText(ui->label->text() + "!t");
//                    else
//                        ui->label->setText(ui->label->text() + "t");
//                }
//            }
//        }
////        if (i != content.length()-1)
////            ui->label->setText(ui->label->text() + "&");
//    }
//}


void MainWindow::on_pushButton_clicked()
{
    QList<int> accepted = {1,2,4,8,16};
    QList<QTableWidgetItem*> selected = ui->tableWidget->selectedItems();

    // перевірка на кількість елементів
    if (!accepted.contains(selected.length()))
    {
        QMessageBox msgBox;
        msgBox.setText("Оберіть область, яка складається з 1, 2, 4, 8 або 16 елементів");
        msgBox.exec();
        return;
    }

    // перевірка на одиниці
    for (int i = 0; i < selected.length();i++)
    {
        if (selected[i]->text() == "0")
        {
            QMessageBox msgBox;
            msgBox.setText("Область містить нулі!");
            msgBox.exec();
            return;
        }
    }

    // перевірка на вже використані області
    for (int i = 0; i < selected.length();i++)
    {
        if (used.contains(selected))
        {
            QMessageBox msgBox;
            msgBox.setText("Дана область вже використана :(");
            msgBox.exec();
            return;
        }
    }
    // додаємо до використаних
    used.append(selected);
    // формуємо вивід
    if (selected.length() > 1)
    {
        QList<QString> sel;
        // переводимо у двійковий код
        for (int i = 0; i < selected.length();i++)
            sel.append(getItemsCode(selected[i]));

        if (ui->label->text() == ("F = "))
            ui->label->setText(ui->label->text() + calculate(sel));
        else
            ui->label->setText(ui->label->text() + '&' + calculate(sel));
    }
    else
    {
        if (ui->label->text() == ("F = "))
            ui->label->setText(ui->label->text() + getItemsCode(selected[0]));
        else
            ui->label->setText(ui->label->text() + '&' + getItemsCode(selected[0]));
    }
    // оптимізуємо виведення
    optimize();

    //    translate(ui->label->text().remove("F = ").split("&"));
}

void MainWindow::on_pushButton_2_clicked()
{   // очищення
    ui->label->setText("F = ");
    used.clear();
}
