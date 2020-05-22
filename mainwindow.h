#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <math.h>
#include <vector>
#include <string>
#include <QTableWidgetItem>

using namespace std;

// Store index values as pairs
struct Pair{
    int i,j;

    Pair(int i, int j)
    {
        this->i = i;
        this->j = j;
    }
};


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

private:
    Ui::MainWindow *ui;

    void fillVariables(int rows, int column, int multiplier, int variables);

    void buildKarnoTable();

    QString getItemsCode(QTableWidgetItem * item);

    QString calculate(QList<QString>selected);

    QString translate(QStringList content);

    void optimize();
};

#endif // MAINWINDOW_H
