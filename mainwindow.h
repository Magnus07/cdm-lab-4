﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <math.h>
#include <vector>
#include <string>
#include <QTableWidgetItem>

using namespace std;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_tableWidget_2_cellChanged(int row, int column);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;

    void fillVariables(int rows, int column, int multiplier, int variables);

    void buildKarnoTable();

    QString getItemsCode(QTableWidgetItem * item);

    QString calculate(QList<QString>selected);

    void translate(QLabel* label);

//    void optimize(QLabel* label);
    void optimize(QLabel * label);

    void runMcCl(QLabel * label);

    void silentMode();
};

#endif // MAINWINDOW_H
