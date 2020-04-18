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


void MainWindow::on_pushButton_clicked()
{
    try { // викликаємо функцію для побудови таблиці
        buildTable();
    } catch (...) { // повідомлення в разі помилки
        QMessageBox msgBox;
        msgBox.setText("Error");
        msgBox.setInformativeText("Something went wrong. Check the data you entered!");
        msgBox.exec();
    }
}


// функція для визначення результату булевих функцій
int MainWindow::calculate(int x, int y, QString action)
{
    if (action == '&')
    {
        if ( x == 1 && y == 1)
            return 1;
        else
            return 0;
    }
    else if(action == '|')
    {
        if (x == 1 && y == 1)
            return 0;
        else
            return 1;
    }
    else if (action == '*')
    {
        if (x == 0 && y == 0)
            return 0;
        else
            return 1;
    }
    else if (action == '+'){
        if(x!=y){
            return 1;
        }else return 0;
    }
    else if (action == '#'){
        if (x == 0 && y == 0)
            return 1;
        else
            return 0;
    }
    else if (action == '@'){
        if(x==0&&y==1){
            return 0;
        }else return 1;
    }
    else if(action == '='){
        if(x==y){
            return 1;
        }else return 0;
    }
    else if(action == '!'){
        return !x;
    }
}


// функція для побудови таблиці
void MainWindow::buildTable()
{
    // отримуємо всі можливі змінні
    QStringList variables = ui->lineEdit->text().split(QRegularExpression("\\W+"));
    variables.removeDuplicates();
    variables.removeAll("");

    // створюємо масив заголовків таблиці
    QStringList headers = variables + getOrder();

    // встановлюємо кількість стовбців та рядків
    ui->tableWidget->setColumnCount(headers.length());
    ui->tableWidget->setRowCount((int)pow(2,variables.length()));

    // заповнюємо значення змінних
    fillVariables((int)pow(2,variables.length()),0,2,variables.length());
    // встановлюємо заголовки
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // передивляємося кожен ствопець
    for  (int i = variables.length(); i < headers.length(); i++)
    {   // передивляємося кожен рядок
        for (int j = 0; j < (int)pow(2,variables.length()); j++)
        {
            // отримання нагального стовпчика
            QTableWidgetItem *item;
            QString header = headers[i];
            // якщо необхідно виконувати операцію заперечення
             if (headers[i][0] == "!")
             {
                 // знаходимо колонку з таким же елементом без заперечення
                 int columnb = -1;
                 for (int k = 0; k < i;k++)
                 {
                     if (header.contains(headers[k]))
                     {
                         columnb = k;
                     }
                 }
                 // видаляємо його
                 header.remove(headers[columnb]);

                 // отримуємо результат
                 item = new QTableWidgetItem(QString::number(calculate(ui->tableWidget->item(j,columnb)->text().toInt(),1,"!")));

                 // якщо необхідно заперечити цілий вираз
                 if (header != "!")
                 {   // знаходимо колонку з наступною чатсиною виразу
                     int columna = -1;

                     for (int k = 0; k < i;k++)
                     {
                         if (header.contains(headers[k]))
                         {
                             columna = k;
                         }
                     }

                     // видаляємо вираз
                     header.remove(headers[columna]);
                     // отримуємо результат
                     item = new QTableWidgetItem(QString::number(calculate(ui->tableWidget->item(j,columna)->text().toInt(),ui->tableWidget->item(j,columnb)->text().toInt(),header)));
                 }
             }
             else
             {
                 // отримуємо необхідні колонки з частинами наявного виразу(до знака операції і після)
                 int columnb = -1;
                 int columna = -1;
                 for (int k = 0; k < i;k++)
                 {
                     if (header.contains(headers[k]))
                     {
                         columnb = k;
                     }
                 }

                 header.remove(headers[columnb]);

                 for (int k = 0; k < i;k++)
                 {
                     if (header.contains(headers[k]))
                     {
                         columna = k;
                     }
                 }
                 header.remove(headers[columna]);

                 // отримуємо результат
                 item = new QTableWidgetItem(QString::number(calculate(ui->tableWidget->item(j,columna)->text().toInt(),ui->tableWidget->item(j,columnb)->text().toInt(),header)));
             }
             // встановлюємо елемент
             ui->tableWidget->setItem(j,i,item);
        }
    }
    // змінюємо розміри таблиці
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
        ui->tableWidget->setItem(i-1, column, item);
    }
    // виклик функції
    fillVariables(rows,column+1,multiplier*2, variables);
}


// функція для отримання порядку обчислень(заголовків)
QStringList MainWindow::getOrder()
{
    // отримуємо змінні і операції
    QStringList variables = ui->lineEdit->text().split(QRegularExpression("\\W+"));
    QStringList operations = ui->lineEdit->text().split(QRegularExpression("\\w{0,1}"));
    operations.removeAll("");
    variables.removeAll("");

    // вхідні данні для польської оберненої нотації
    QStringList order;
    std::stack <QString> queue;

    // лічильники
    int currentOperation = 0;
    int currentVariable = 0;

    // змінна для визначення, що іде далі
    bool operationsFirst = false;

    // результат
    QStringList decomposed;
    if (operations[0] == "!" || operations[0] == "(")
        operationsFirst = true;
    else
        operationsFirst = false;

    // проходимо по кожному елементу
    for (int i = 0; i < operations.length() + variables.length();i++)
    {   // якщо це скобка чи заперечення
        if (currentOperation < operations.length() && (operations[currentOperation] == "(" || operations[currentOperation] == "!"))
        {   // змінюємо значення
            if (operationsFirst)
                operationsFirst = !operationsFirst;
            // втсавляємо до результату
            decomposed.append(operations[currentOperation]);
            currentOperation++;
        }// якщо далі йде операція
        else if (currentOperation < operations.length() && operationsFirst)
        {// змінюємо значення
            if (operations[currentOperation] != ")")
                operationsFirst = !operationsFirst;
            // втсавляємо до результату
            decomposed.append(operations[currentOperation]);
            currentOperation++;
        }// якщо далі йде змінна
        else if(currentVariable < variables.length() && !operationsFirst)
        {// вставляємо змінну
            decomposed.append(variables[currentVariable]);
            currentVariable++;
            operationsFirst = !operationsFirst;
        }

    }
    // підготовка до оберненої польскої нотації завершена
    // обнуляємо змінні
    currentVariable = 0;
    currentOperation = 0;

    // ОБЕРНЕНА ПОЛЬСЬКА НОТАЦІЯ
    // продивляємося кожен елемент
    for (int i = 0; i < decomposed.length();i++)
    {// якщо це відкриваюча скобка або заперечення
        if (decomposed[i] == "(" || decomposed[i] == "!")
            queue.push(decomposed[i]); // додаємо у стек
        else if (decomposed[i] == ")") // якщо закриваюча скобка
        {// звільняємо стек, переносячи кожну операцію у масив виходу
            while(queue.top() != "(")
            {
                order.append(queue.top());
                queue.pop();
            }
            queue.pop();
        }// якщо це змінна
        else if (variables.indexOf(decomposed[i]) != -1)
            order.append(decomposed[i]);//  додаємо у масив виходу
        else
        {// в іншому випадку
            if (queue.empty())// якщо стек порожній
                queue.push(decomposed[i]);// додаємо операцію
            else if (moveToOutput(queue.top(),decomposed[i]))// якщо елемент зі стеку необхідно перенести у масив
            {// переносимо елемент зі стеку у масив, а новий елемент додаємо у стек
                order.append(queue.top());
                queue.pop();
                queue.push(decomposed[i]);
            }
            else// додаємо елемент у стек
                queue.push(decomposed[i]);
        }
    }
    // поки стек не порожній
    while (!queue.empty())
    {   // додаємо елемент у масив виходу і видаляємо елемент зі стеку
        order.append(queue.top());
        queue.pop();
    }
    // власне заголовки таблиці
    std::stack<QString> result;
    QStringList headers;
    // проходимо по кожному елементу
    for (int i = 0; i < order.length();i++)
    {// якщо це змінна
        if (variables.indexOf(order[i]) != -1)
        {// додаємо до стеку
            result.push(order[i]);
        }
        else
        {// отримємо елемент зі стеку, видаляємо його зі стеку
            QString a = result.top();
            result.pop();
            // якщо це заперечення
            if (order[i] == "!")
            {// додаємо у стек заперечення і елемент
                result.push(order[i] + a);
            }
            else
            {//  в іншому випадку отрмуємо другий елемент зі стеку і звільняємо місце
                QString b = result.top();
                result.pop();
                // додаємо у стек елементи з відповідною операцією
                result.push(a + order[i] + b);
            }// додаємо елемент зі стеку до зоголовку
            headers.append(result.top());
        }
    }
    //  повертаємо масив заголовків
    return headers;
}


// функція. яка визначає, як необхідно додавати операцію у стек
// за ОБЕРНЕНОЮ ПОЛЬСЬКОЮ НОТАЦІЄЮ
// якщо елемент з вищим пріорітетом ніж наявний у стеку,
// то елемент зі стеку переходить у масив виходу(тру)
// в іншому випадку, додається у стек
bool MainWindow::moveToOutput(QString stack, QString current)
{
    if (stack == "(")
        return false;
    if (stack == "!")
        return true;
    if (current == "*")
        return false;
    if (stack == "&" && current == "*")
        return false;
    if (stack == "@" && (current == "*" || current == "&"))
        return false;
    if (stack == "=" && (current == "*" || current == "&" || current == "@"))
        return false;
    else
        return true;
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+'+');
    ui->lineEdit->setFocus();
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+'&');
    ui->lineEdit->setFocus();
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+'*');
    ui->lineEdit->setFocus();
}

void MainWindow::on_pushButton_5_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+'!');
    ui->lineEdit->setFocus();
}

void MainWindow::on_pushButton_9_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+'@');
     ui->lineEdit->setFocus();
}

void MainWindow::on_pushButton_8_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+'=');
     ui->lineEdit->setFocus();
}

void MainWindow::on_pushButton_7_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+'#');
     ui->lineEdit->setFocus();
}

void MainWindow::on_pushButton_6_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+'|');
     ui->lineEdit->setFocus();
}
