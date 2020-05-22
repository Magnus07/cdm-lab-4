#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>

QList<QTableWidgetItem*> used;
QStringList hheaders = {"0","1"};
QStringList vheaders = {"0","1"};

using namespace std;

// 2 в н-ному степені
//int pow2(int n)
//{
//    int result=1;
//    while(n>0)
//        result*=2,n--;
//    return result;
//}
//int combination(int n,int r) // вираховуємо кількість комбінацій (n! / ((N-r)! * (R)!)
//{
//    int fm=1,fz=1;
//    for(int i=1;i<=r;i++,n--)
//    {
//        fm*=i;
//        fz*=n;
//    }
//    return fz/fm;
//}
//void d2b(int d,char* b,int n) // від десяткового до двійкового
//{
//    for(n--;n>=0;n--)
//    {
//        b[n]='0'+d%2;
//        d/=2;
//    }
//}
//int ispair(char* a1,char* a2,int n) // дивимось на наслідки утворення пар в одному напрямі
//{
//    int x,y=0;
//    for(int i=0;i<n;i++)
//        if(a1[i]!=a2[i])
//            x=i,y++;
//    if(y==1) return x;
//    else return-1;
//}
//bool issame(char* a1,char* a2,int n) // чи вони однакові
//{
//    for(int i=0;i<n;i++)
//        if(a1[i]!=a2[i])
//            return 0;
//    return 1;
//}
//int left1(char* a,int N) // починаючи з першого елемента масиву, знаходимо крайній лівий і повертаємо його зміщення
//{
//    for(int i=0;i<N;i++)
//        if(a[i]=='1')
//            return i;
//    return -1;
//}
//void copy(char* a1,char* a2,int n) // копіювання між масивами
//{
//    for(int i=0;i<n;i++)
//        a2[i]=a1[i];
//}
//bool isinside(int x,char* a,int n) // чи знаходиться десятковий Х в масиві
//{
//    for(n--;n>=0;n--)
//    {
//        if(a[n]!='x' && a[n]!=(char)('0'+x%2))
//            return 0;
//        x/=2;
//    }
//    return 1;
//}
//void output(stringstream& file,char* a,int n) // формуємо вивід
//{
//    for(int i=0;i<n;i++)
//    {
//        if(a[i]=='0') file<<(char)('a'+i)<<'\'';
//        if(a[i]=='1') file<<(char)('a'+i);
//    }
//    file<<'*';
//}
//int count(char* table,char* a,int n,int N) // Порахуємо, скільки найменших елементів задовольняє 1, зміщення якого включено до масиву a
//{
//    int counter=0;
//    for(int i=0;i<N;i++)
//    {
//        if(table[i]!='1') continue;
//        if(isinside(i,a,n)) counter++;
//    }
//    return counter;
//}
//void clean(char* table,char* a,int n,int N) // Вилучаємо 1, що міститься у найменшому термі, що вийшов, і встановимо його на x
//{
//    for(int j=0;j<N;j++)
//        if(isinside(j,a,n))
//            table[j]='x';
//}


//void MainWindow::getResult(int variables, int rows, int columns)
//{
//    //оголошуємо початкові змінні
//    int valNum = variables; //кількість змінних
//    int minTermLength=pow2(valNum); // мінімальна довжина терма
//    char* minTermExpression=new char[minTermLength]; // попередньо виділяємо память для зберігання найменого запису
//    int lineOff = pow2(ceil(double(valNum)/2));
//    // в нормальних умовах

//    int index = 0;
//    // передивляємося кожну комірку таблиці
//    for (int i =0; i < rows; i++)
//    {
//        for(int j = 0; j < columns;j++)
//        {
//            // записуємо дані
//            minTermExpression[index] = ui->tableWidget->item(i,j)->text()[0].toLatin1();
//            index++;
//        }
//    }

//    // Виділяємо пам’ять для вказівника на імплікацію
//    char*** implication=new char**[valNum]; // Трирівневий покажчик

//    int nonZeroNum=0;
//    for(int i=0;i<minTermLength;i++)
//        if(minTermExpression[i]!='0') // Цей крок обчислює загальну кількість елементів у масивах
//            nonZeroNum++;
//    for(int i=0;i<valNum;i++) // i-кубічний, наприклад, коли кількість змінних дорівнює 3, i = 0 відповідає точці в просторі, i = 1 відповідає ребру, i = 2 відповідає грані
//    {
//        if(pow2(i)>nonZeroNum)break;
//        int x=pow2(i-1)*combination(nonZeroNum,pow2(i));

//        implication[i]=new char*[x];
//        for(int j=0;j<x;j++)
//            implication[i][j]=new char[valNum];
//    }
//    // Дізнаємося, що означають умови
//    int* countNum=new int[valNum+1];
//    countNum[0]=0;
//    for(int i=0;i<minTermLength;i++)
//        if(minTermExpression[i]!='0') // знаходимо нульові значення
//        {
//            d2b(i,implication[0][countNum[0]],valNum); // Ненульові компенсації зберігаються в масиві імплікацій [0]
//            countNum[0]++;
//        }
//    int isOptimal=0;
//    while(countNum[isOptimal]>0) // Тільки коли є ненульове значення, воно знайде значення
//    {
//        countNum[isOptimal+1]=0;
//        for(int i=0;i<countNum[isOptimal]-1;i++)
//            for(int j=i+1;j<countNum[isOptimal];j++)
//            {
//                int x=ispair(implication[isOptimal][i],implication[isOptimal][j],valNum); // Шукаєш значення в одному вимірі
//                if(x==-1) continue;
//                copy(implication[isOptimal][i],implication[isOptimal+1][countNum[isOptimal+1]],valNum);// Оновлення наслідків наступного покоління
//                implication[isOptimal+1][countNum[isOptimal+1]][x]='x'; // Міститься одночасно у двох масивів, відповідний бінарний біт позначається як x, щоб він не був роздрукований
//                countNum[isOptimal+1]++;
//            }
//        for(int i=0;i<countNum[isOptimal+1]-1;i++)
//            for(int j=i+1;j<countNum[isOptimal+1];j++)
//                if(issame(implication[isOptimal+1][i],implication[isOptimal+1][j],valNum)) // чи це повторне значення
//                {
//                    for(int k=j;k<countNum[isOptimal+1]-1;k++)
//                        copy(implication[isOptimal+1][k+1],implication[isOptimal+1][k],valNum);
//                    countNum[isOptimal+1]--;
//                }
//        isOptimal++;
//    }
//    isOptimal--;

//    stringstream result;
//    result<<"F=";
//    while(left1(minTermExpression,minTermLength)>=0) // поки 1 існує в minTermExpression
//    {
//        bool flag=0; // біт прапора, логіка управління
//        for(int i=0;i<minTermLength&&flag==0;i++)
//        {
//            if(minTermExpression[i]!='1') continue;
//            int counter=0,recorder;
//            for(int j=0;j<countNum[isOptimal];j++)
//                if(isinside(i,implication[isOptimal][j],valNum))
//                    counter++,recorder=j;
//            if(counter!=1) continue;
//            output(result,implication[isOptimal][recorder],valNum);
//            clean(minTermExpression,implication[isOptimal][recorder],valNum,minTermLength);
//            flag=1;
//        }
//        if(flag==1) continue;
//        int termMaxInclude=0;
//        int recorder=0;
//        for(int i=0;i<countNum[isOptimal];i++) // Найкращий показник імплікації тут - включити найбільшу 1 в найменший набір як стандарт
//            if(count(minTermExpression,implication[isOptimal][i],valNum,minTermLength)>termMaxInclude)
//                termMaxInclude=count(minTermExpression,implication[isOptimal][i],valNum,minTermLength),recorder=i;
//        if(termMaxInclude==0) {isOptimal--; continue;}
//        output(result,implication[isOptimal][recorder],valNum);
//        clean(minTermExpression,implication[isOptimal][recorder],valNum,minTermLength);
//    }

//    QString out = QString::fromStdString(result.str());
//    out.replace('a','x');out.replace('b','y');out.replace('c','x');out.replace('d','t');
//    out.remove(out.size()-1,1);
//    ui->label->setText(out);
//}

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
//    getResult(ui->comboBox->currentIndex()+1,rows,columns);

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


QString MainWindow::getItemsCode(QTableWidgetItem * item)
{
    return hheaders[item->column()] + vheaders[item->row()];
}

QString MainWindow::calculate(QList<QString>selected)
{
    if (selected.length() == 2)
    {
        QString item = selected[0];
        QString item2 = selected[1];
        for (int i = 0; i < item.length();i++)
        {
            if (item[i] != item2[i])
            {
                item[i] = 'x';
                return item;
            }
        }
    }

    QList<QString> group1;
    QList<QString> group2;

    for (int i = 0; i < selected.length();i++)
    {
        if (i < selected.length()/2)
            group1.append(selected[i]);
        else
            group2.append(selected[i]);
    }

    QList<QString> group3;
    group3.append(calculate(group1));
    group3.append(calculate(group2));

    return calculate(group3);
}


//QString MainWindow::calculate(QList<QTableWidgetItem*>selected)
//{
//    if (selected.length() == 1)
//        return getItemsCode(selected[0]);

//    if (selected.length() == 2)
//    {
//        QString item = getItemsCode(selected[0]);
//        QString item2 = getItemsCode(selected[1]);
//        for (int i = 0; i < item.length();i++)
//        {
//            if (item[i] != item2[i])
//            {
//                item[i] = 'x';
//                return item;
//            }
//        }
//    }

//    QList<QTableWidgetItem*> group1;
//    QList<QTableWidgetItem*> group2;

//    for (int i = 0; i < selected.length();i++)
//    {
//        if (i < selected.length()/2)
//            group1.append(selected[i]);
//        else
//            group2.append(selected[i]);
//    }



//    else if (selected.length() == 4)
//    {
//        QList<QTableWidgetItem*> p1;
//        p1.append(selected[0]);
//        p1.append(selected[1]);

//        QList<QTableWidgetItem*> p2;
//        p2.append(selected[2]);
//        p2.append(selected[3]);

//        QString item = calculate(p1);
//        QString item2 = calculate(p2);

//        for (int i = 0; i < item.length();i++)
//        {
//            if (item[i] != item2[i])
//            {
//                item[i] = 'x';
//                return item;
//            }
//        }
//    }
//    else if (selected.length() == 8)
//    {
//        QList<QTableWidgetItem*> p1;
//        p1.append(selected[0]);
//        p1.append(selected[1]);
//        p1.append(selected[2]);
//        p1.append(selected[3]);

//        QList<QTableWidgetItem*> p2;
//        p2.append(selected[4]);
//        p2.append(selected[5]);
//        p2.append(selected[6]);
//        p2.append(selected[7]);

//        QString item = calculate(p1);
//        QString item2 = calculate(p2);

//        for (int i = 0; i < item.length();i++)
//        {
//            if (item[i] != item2[i])
//            {
//                item[i] = 'x';
//                return item;
//            }
//        }
//    }
//}

void MainWindow::optimize()
{
    QList<QString> container = ui->label->text().remove("F = ").split("&");

    if (container.length() > 1)
    {
        for (int i = 0; i < container.length();i++)
        {
            for (int j = i+1; j < container.length(); j++)
            {
                ushort index = 0;
                ushort differ = 0;
                for (int k = 0; k < container[i].length();k++)
                {
                    if (container[i][k] != container[j][k])
                    {
                        if (container[i][k] == 'x' || container[j][k] == 'x')
                            differ++;
                        else
                        {
                            differ++;
                            index = k;
                        }
                    }
                }
                if (differ == 1)
                {
                    container[i][index] = 'x';
                    container.removeAt(j);
                }
            }
        }
        ui->label->setText("F = ");

        for (int i = 0; i < container.length(); i++)
        {
            ui->label->setText(ui->label->text() + container[i] + "&");
        }

//        ui->label->setText(ui->label->text() + "(" + container[0] + ")");
    }
}


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

    // перевірка на вже використані елементи
//    for (int i = 0; i < selected.length();i++)
//    {
//        for (int j = 0; j < used.length(); j++)
//        {
//            if (selected[i] == used[j])
//            {
//                QMessageBox msgBox;
//                msgBox.setText("До області входять вже використані елементи.");
//                msgBox.exec();
//                return;
//            }
//        }
//    }

    used.append(selected);
    if (selected.length() > 1)
    {
        QList<QString> sel;

        for (int i = 0; i < selected.length();i++)
        {
            sel.append(getItemsCode(selected[i]));
        }

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

    optimize();
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->label->setText("F = ");
    used.clear();
}

void MainWindow::on_pushButton_3_clicked()
{
    optimize();
}
