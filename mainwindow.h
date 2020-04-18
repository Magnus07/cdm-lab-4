#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <math.h>
#include <stack>

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

private:
    Ui::MainWindow *ui;

    void fillVariables(int rows, int column, int multiplier, int variables);

    void buildKarnoTable();
};

#endif // MAINWINDOW_H
