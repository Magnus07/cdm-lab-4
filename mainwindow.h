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

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;

    void fillVariables(int rows, int column, int multiplier, int variables);

    void buildKarnoTable();

    QString getItemsCode(QTableWidgetItem * item);

    QString calculate(QList<QString>selected);

    void optimize();

//    bool hasCommonPair(Pairs pair,vector <vector <Pairs>> &groups, int skip);

//    QStringList simplifySet(QStringList set);

//    string getSimplifiedExpression(QStringList vheaders, QStringList hheaders);

//    void pushUnsortedGroups(vector<vector <Pairs>> &groups,int i, int j);

//    void removeDuplicateGroups(vector<vector <Pairs>> &groups);

//    bool isSubsetOf(vector <Pairs> &group1,vector <Pairs> &group2);

//    void sortUnsortedGroup(vector <Pairs> &group);


//    string generateStringFromGroup(vector <Pairs> &group, QStringList vheaders, QStringList hheaders);

    void getResult(int variables, int rows, int columns);
};

#endif // MAINWINDOW_H
