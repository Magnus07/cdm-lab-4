#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>


using namespace std;
//子函数定义
int pow2(int n) // 求以2为底数的幂，2^n
{
    int result=1;
    while(n>0)
        result*=2,n--;
    return result;
}
int combination(int n,int r) // 计算组合数C_n^r，即 n! / ((n-r)!*(r)!)
{
    int fm=1,fz=1;
    for(int i=1;i<=r;i++,n--)
    {
        fm*=i;
        fz*=n;
    }
    return fz/fm;
}
void d2b(int d,char* b,int n) // 十进制转二进制，将十进制数字d以二进制的形式存在数组b中
{
    for(n--;n>=0;n--)
    {
        b[n]='0'+d%2;
        d/=2;
    }
}
int ispair(char* a1,char* a2,int n) // 寻找卡诺图中单个方向上成对的蕴涵
{
    int x,y=0;
    for(int i=0;i<n;i++)
        if(a1[i]!=a2[i])
            x=i,y++;
    if(y==1) return x;
    else return-1;
}
bool issame(char* a1,char* a2,int n) // 是否相等
{
    for(int i=0;i<n;i++)
        if(a1[i]!=a2[i])
            return 0;
    return 1;
}
int left1(char* a,int N) // 从数组的第一个元素开始，寻找最左端的1，返回其偏移量
{
    for(int i=0;i<N;i++)
        if(a[i]=='1')
            return i;
    return -1;
}
void copy(char* a1,char* a2,int n) // 数组间的复制
{
    for(int i=0;i<n;i++)
        a2[i]=a1[i];
}
bool isinside(int x,char* a,int n) // 十进制x是否在二进制数组a中
{
    for(n--;n>=0;n--)
    {
        if(a[n]!='x' && a[n]!=(char)('0'+x%2))
            return 0;
        x/=2;
    }
    return 1;
}
void output(stringstream& file,char* a,int n) // 将数组a转化为变量符号并输出
{
    for(int i=0;i<n;i++)
    {
        if(a[i]=='0') file<<(char)('a'+i)<<'\'';
        if(a[i]=='1') file<<(char)('a'+i);
    }
    file<<'+';
}
int count(char* table,char* a,int n,int N) // 计数，有多少最小项中的1满足其偏移量包含在数组a中
{
    int counter=0;
    for(int i=0;i<N;i++)
    {
        if(table[i]!='1') continue;
        if(isinside(i,a,n)) counter++;
    }
    return counter;
}
void clean(char* table,char* a,int n,int N) // 清除已经输出的蕴涵在最小项中所包含的1，并置为x
{
    for(int j=0;j<N;j++)
        if(isinside(j,a,n))
            table[j]='x';
}


void MainWindow::getResult(int variables, int rows, int columns)
{
    //数据读取
    int valNum = variables; //变量数
    int minTermLength=pow2(valNum); // 输入的最小项的值共有2^valNum个
    char* minTermExpression=new char[minTermLength]; // 为输入最小项的存储预先分配内存
    int lineOff = pow2(ceil(double(valNum)/2));
    // 读取正常情况下

    int index = 0;

    for (int i =0; i < rows; i++)
    {
        for(int j = 0; j < columns;j++)
        {
            minTermExpression[index] = ui->tableWidget->item(i,j)->text()[0].toLatin1();
            index++;
        }
    }

    // 为implication蕴涵指针分配内存
    char*** implication=new char**[valNum]; // 三级指针

    int nonZeroNum=0;
    for(int i=0;i<minTermLength;i++)
        if(minTermExpression[i]!='0') // 这一步计算ON阵列和DC阵列中元素个数总和
            nonZeroNum++;
    for(int i=0;i<valNum;i++) // i-立方，例如当变量数为3时，i=0对应空间中的点,i=1对应边，i=2对应面
    {
        if(pow2(i)>nonZeroNum)break;
        int x=pow2(i-1)*combination(nonZeroNum,pow2(i));

        implication[i]=new char*[x];
        for(int j=0;j<x;j++)
            implication[i][j]=new char[valNum];
    }
    //找出符合条件的蕴涵
    int* countNum=new int[valNum+1];
    countNum[0]=0;
    for(int i=0;i<minTermLength;i++)
        if(minTermExpression[i]!='0') // 找到非0值
        {
            d2b(i,implication[0][countNum[0]],valNum); // 非零值的偏移量存储在implication[0]数组中
            countNum[0]++;
        }
    int isOptimal=0;
    while(countNum[isOptimal]>0) // 有非零值才会寻找蕴涵
    {
        countNum[isOptimal+1]=0;
        for(int i=0;i<countNum[isOptimal]-1;i++)
            for(int j=i+1;j<countNum[isOptimal];j++)
            {
                int x=ispair(implication[isOptimal][i],implication[isOptimal][j],valNum); // 寻找一个维度上的蕴涵
                if(x==-1) continue;
                copy(implication[isOptimal][i],implication[isOptimal+1][countNum[isOptimal+1]],valNum);// 下一代implication更新
                implication[isOptimal+1][countNum[isOptimal+1]][x]='x'; // 同时被两个蕴涵包含的，对应的二进制位标为x，这样就不会将其打印出来
                countNum[isOptimal+1]++;
            }
        for(int i=0;i<countNum[isOptimal+1]-1;i++)
            for(int j=i+1;j<countNum[isOptimal+1];j++)
                if(issame(implication[isOptimal+1][i],implication[isOptimal+1][j],valNum)) // 蕴涵是否重复
                {
                    for(int k=j;k<countNum[isOptimal+1]-1;k++)
                        copy(implication[isOptimal+1][k+1],implication[isOptimal+1][k],valNum);
                    countNum[isOptimal+1]--;
                }
        isOptimal++;
    }
    isOptimal--;

    stringstream result;
    result<<"F=";
    while(left1(minTermExpression,minTermLength)>=0) //当minTermExpression中存在1
    {
        bool flag=0; // 标志位，控制逻辑
        for(int i=0;i<minTermLength&&flag==0;i++)
        {
            if(minTermExpression[i]!='1') continue;
            int counter=0,recorder;
            for(int j=0;j<countNum[isOptimal];j++)
                if(isinside(i,implication[isOptimal][j],valNum))
                    counter++,recorder=j;
            if(counter!=1) continue;
            output(result,implication[isOptimal][recorder],valNum);
            clean(minTermExpression,implication[isOptimal][recorder],valNum,minTermLength);
            flag=1;
        }
        if(flag==1) continue;
        int termMaxInclude=0;
        int recorder=0;
        for(int i=0;i<countNum[isOptimal];i++) // 这里的最佳蕴涵的指标，是包含了最小项中最多的1为标准，将其筛选出来
            if(count(minTermExpression,implication[isOptimal][i],valNum,minTermLength)>termMaxInclude)
                termMaxInclude=count(minTermExpression,implication[isOptimal][i],valNum,minTermLength),recorder=i;
        if(termMaxInclude==0) {isOptimal--; continue;}
        output(result,implication[isOptimal][recorder],valNum);
        clean(minTermExpression,implication[isOptimal][recorder],valNum,minTermLength);
    }
    char finalExpression[201];

    int termMaxInclude=0;
    for(;finalExpression[termMaxInclude]!='\0';termMaxInclude++);
        finalExpression[termMaxInclude-1]='\0';

    QString out = QString::fromStdString(result.str());
    out.remove(out.size()-1,1);
    ui->label->setText(out);
}

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

//    simplifySet(simplifySet(set));
//    getSimplifiedExpression(vheaders, hheaders);

    getResult(ui->comboBox->currentIndex()+1,rows,columns);

    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->resizeColumnsToContents();
}


//QString MainWindow::formGroups(QStringList vocabulary, int totalVariablesCount)
//{
//    QString solution;
//    vector <Pair> toBeIncluded;
////    char vocabulary[totalVariablesCount];
//    bool flag = false;
//    if(totalVariablesCount==2){	// 2 VARIABLE CASE
//        outer:
//        //checking for square of 4 cells
//        for(int i=0;i<ui->tableWidget->rowCount();i++){
//            for(int j=0;j<ui->tableWidget->columnCount();j++){
//                if(!ui->tableWidget->item(i,j)){
//                    flag = false;
//                    break;
//                }
//            }
//        }
//        if(flag){
//            solution.append("1");
//            return solution;
//        }
//        // checking all combinations of groups of 2
//        //HORIZONTAL
//        for(int i=0;i<2;i++){
//            if(ui->tableWidget->item(i,0) && ui->tableWidget->item(i,1)  && !ui->tableWidget->item(i,0) && !ui->tableWidget->item(i,1)){
//                toBeIncluded.push_back(Pair(i,0));
//                toBeIncluded.push_back(Pair(i,1));
//                solution.append((vocabulary[0]));
//                if(i==0)
//                    solution.append("!");
//                solution.append(",");
//            }
//        }
//        //VERTICAL
//        for(int i=0;i<2;i++){
//            if(ui->tableWidget->item(0,i) && ui->tableWidget->item(1,i)  && !included[0][i] && !included[1][i]){
//                toBeIncluded.push_back(Pair(0,i));
//                toBeIncluded.push_back(Pair(1,i));
//                solution.append(vocabulary[1]);
//                if(i==0)
//                    solution.append("!");
//                solution.append(",");
//            }
//        }
//        markIncluded();
//        // checking if single points are left out
//        for(int i=0;i<map.length;i++){
//            for(int j=0;j<map[i].length;j++){
//                if(map[i][j] && !included[i][j]){
//                    if(map[(i+1)%2][j]){
//                        solution.append(new Character(vocabulary[1]).toString());
//                        if(j==0)
//                            solution.append("!");
//                        toBeIncluded.add(new Pair(i,j));
//                        solution.append(",");
//                    }else if(map[i][(j+1)%2]){
//                        solution.append(new Character(vocabulary[0]).toString());
//                        if(i==0)
//                            solution.append("!");
//                        toBeIncluded.add(new Pair(i,j));
//                        solution.append(",");
//                    }

//                }
//            }
//        }
//        markIncluded();
//        // checking for diagonal points
//        for(int i=0;i<map.length;i++){
//            for(int j=0;j<map[i].length;j++){
//                if(map[i][j] && !included[i][j]){
//                    solution.append(new Character(vocabulary[0]).toString());
//                    if(i==0)
//                        solution.append("!");
//                    solution.append(new Character(vocabulary[1]).toString());
//                    if(j==0)
//                        solution.append("!");
//                    toBeIncluded.add(new Pair(i,j));
//                    solution.append(",");
//                }
//            }
//        }
//        markIncluded();
//}



//void MainWindow::pushUnsortedGroups(vector<vector <Pairs>> &groups,int i, int j){
//    vector<Pairs> group;
//    Pairs temp;
//    bool foundGroups = false;
//    int d[4][2] = {{0,1},{1,0},{0,-1},{-1,0}};

//    temp.i = i; temp.j = j;
//    group.push_back(temp);

//    // Search for pairs in four cardinal directions
//    for(int k = 0; k < 4; k++){
//        // Check for pairs
//        if(ui->tableWidget->item((i+d[k][0]+4)%4,(j+d[k][1]+4)%4)->text() == "1"){
//            temp.i = (i+d[k][0]+4)%4; temp.j = (j+d[k][1]+4)%4;
//            group.push_back(temp);

//            // Check for long quad
//            if(ui->tableWidget->item((i+2*d[k][0]+4)%4,(j+2*d[k][1]+4)%4)->text() == "1" &&
//                ui->tableWidget->item((i+3*d[k][0+4])%4,(j+3*d[k][1]+4)%4)->text() == "1"){
//                    temp.i = (i+2*d[k][0]+4)%4; temp.j =(j+2*d[k][1]+4)%4;
//                    group.push_back(temp);

//                    temp.i = (i+3*d[k][0]+4)%4; temp.j =(j+3*d[k][1]+4)%4;
//                    group.push_back(temp);

//                    // Check for Octant
//                    if(ui->tableWidget->item((i+0*d[k][0]+d[(k+1)%4][0]+4)%4,(j+0*d[k][1]+d[(k+1)%4][1]+4)%4)->text() == "1" &&
//                        ui->tableWidget->item((i+1*d[k][0]+d[(k+1)%4][0]+4)%4,(j+1*d[k][1]+d[(k+1)%4][1]+4)%4)->text() == "1" &&
//                        ui->tableWidget->item((i+2*d[k][0]+d[(k+1)%4][0]+4)%4,(j+2*d[k][1]+d[(k+1)%4][1]+4)%4)->text() == "1" &&
//                        ui->tableWidget->item((i+3*d[k][0]+d[(k+1)%4][0]+4)%4,(j+3*d[k][1]+d[(k+1)%4][1]+4)%4)->text() == "1"){

//                            temp.i = (i+0*d[k][0]+d[(k+1)%4][0]+4)%4; temp.j =(j+0*d[k][1]+d[(k+1)%4][1]+4)%4;
//                            group.push_back(temp);

//                            temp.i = (i+1*d[k][0]+d[(k+1)%4][0]+4)%4; temp.j =(j+1*d[k][1]+d[(k+1)%4][1]+4)%4;
//                            group.push_back(temp);

//                            temp.i = (i+2*d[k][0]+d[(k+1)%4][0]+4)%4; temp.j =(j+2*d[k][1]+d[(k+1)%4][1]+4)%4;
//                            group.push_back(temp);

//                            temp.i = (i+3*d[k][0]+d[(k+1)%4][0]+4)%4; temp.j =(j+3*d[k][1]+d[(k+1)%4][1]+4)%4;
//                            group.push_back(temp);
//                    }

//                    groups.push_back(group);
//                    group.clear();
//                    foundGroups = true;
//                    break;
//            }

//            // Check for square quad
//            if(ui->tableWidget->item((i+0*d[k][0]+d[(k+1)%4][0]+4)%4,(j+0*d[k][1]+d[(k+1)%4][1]+4)%4)->text() == 1 &&
//                ui->tableWidget->item((i+1*d[k][0]+d[(k+1)%4][0]+4)%4,(j+1*d[k][1]+d[(k+1)%4][1]+4)%4)->text() == 1){
//                    temp.i = (i+0*d[k][0]+d[(k+1)%4][0]+4)%4; temp.j =(j+0*d[k][1]+d[(k+1)%4][1]+4)%4;
//                    group.push_back(temp);

//                    temp.i = (i+1*d[k][0]+d[(k+1)%4][0]+4)%4; temp.j =(j+1*d[k][1]+d[(k+1)%4][1]+4)%4;
//                    group.push_back(temp);
//            }
//            groups.push_back(group);
//            group.clear();
//            temp.i = i; temp.j = j;
//            group.push_back(temp);
//            foundGroups = true;
//        }
//    }
//    // If no pairs, quads or octants were found, push the unit group
//    if(!foundGroups)
//        groups.push_back(group);
//}


//string MainWindow::generateStringFromGroup(vector <Pairs> &group, QStringList vheaders, QStringList hheaders){
//    string result;
//    string rows[vheaders.length()];
//    string columns[hheaders.length()];


//    for (int i = 0; i < rows->length();i++)
//    {
//        rows[i] = vheaders[i].toStdString();
//    }

//    for (int i = 0; i < columns->length();i++)
//    {
//        columns[i] = hheaders[i].toStdString();
//    }


//    string temp2 = columns[group[0].j] + rows[group[0].i];

//    // If the group is a unit group, no processing is necessary
//    if(group.size() == 1){
//        result = temp2;
//    } else {
//        // Store only the common terms
//        for(int i = 1; i < group.size(); i++){
//            result = "";
//            string temp1 = columns[group[i].j] + rows[group[i].i];

//            for(int j = 0; j < temp2.size(); j++){
//                if(temp1.find(temp2[j]) != string::npos){
//                    result += temp2[j];
//                }
//            }
//            temp2 = result;
//        }
//    }

////    // If POS then use suitable connectives
////    if(!map->getIsSop()){
////        temp2 = result;
////        result = "";
////        for(int i = 0; i < temp2.length(); i++){
////            result += (i?" + ":"");
////            result.push_back(toggleCase(temp2[i]));
////        }
////    }

//    return result;
//}

//string MainWindow::getSimplifiedExpression(QStringList vheaders, QStringList hheaders){
////    map->getui->tableWidget->item((ui->tableWidget->item();
//    string result = "";

//    // A linked list where all the groups will be stored
//    vector<vector <Pairs>> groups;

//    for(int i = 0; i < 4; i++){
//        for(int j = 0; j < 4; j++){
//            if(ui->tableWidget->item(i, j)->text() == "1")
//                pushUnsortedGroups(groups,i,j);
//        }
//    }
////    for(int i = 0; i < groups.size(); i++)
////        sortUnsortedGroup(groups[i]);

////    removeDuplicateGroups(groups);

////    // Insert appropriate connectives between groups of expressions
//////    for(int i = 0; i < groups.size(); i++)
//////        result += (i?(map-> getIsSop()?" + ":")("): "") + generateStringFromGroup(groups[i]);
////    for(int i = 0; i < groups.size(); i++)
////        result += " " + generateStringFromGroup(groups[i], vheaders, hheaders);

////    if(!map-> getIsSop()) result = "(" + result + ")";
//    return result;
//}


//void MainWindow::sortUnsortedGroup(vector <Pairs> &group){

//    // Bubble sort
//    for(int i = 0; i < group.size(); i++){
//        for(int j = 0; j < group.size()-i-1; j++){
//            if(group[j].i*4+group[j].j > group[j+1].i*4+group[j+1].j){
//                Pairs temp = group[j];
//                group[j] = group[j+1];
//                group[j+1] = temp;
//            }
//        }
//    }

//}


//bool hasCommonPair(Pairs pair,vector <vector <Pairs>> &groups, int skip){
//    for(int i = 0; i < groups.size(); i++){
//        // Do not compare with self
//        if(i == skip) continue;
//        for(int j = 0; j < groups[i].size(); j++){
//            if(groups[i][j].i*4 + groups[i][j].j == pair.i * 4 + pair.j)
//                return true;
//        }
//    }
//    return false;
//}


////void MainWindow::removeDuplicateGroups(vector<vector <Pairs>> &groups){
////    vector<string> temp;

////    // Generate a string list from the groups
////    for(int i = 0; i < groups.size(); i++){
////        temp.push_back("");
////        for(int j = 0; j < groups[i].size(); j++){
////            temp[i] += to_string(groups[i][j].i*4 + groups[i][j].j) + " ";
////        }
////    }

////    // Remove duplicate entries
////    for(int i = 0; i < temp.size(); i++){
////        for(int j = i+1; j < temp.size(); j++){
////            if (temp[i].compare(temp[j]) == 0) {
////                temp.erase(temp.begin()+j);
////                groups.erase(groups.begin()+j);
////            }
////        }
////    }

////    // Remove subset entries
////    for(int i = 0; i < groups.size(); i++){
////        vector<Pairs> group = groups[i];
////        for(int j = 0; j < groups.size(); j++){
////            if(i == j) continue;
////            if(isSubsetOf(groups[j],group)){
////                groups.erase(groups.begin()+j);
////                j--;
////            }
////        }
////    }
////    // Remove redundant entries
////    bool allCommon;
////    for(int i = 0; i < groups.size(); i++){
////        vector<Pairs> group = groups[i];
////        allCommon = true;
////        for(int j = 0; j < group.size(); j++){
////            if(!hasCommonPair(group[j],groups,i)){
////                allCommon = false;
////                break;
////            }
////        }
////        if(allCommon){
////            groups.erase(groups.begin() + i);
////            i--;
////        }
////    }
////}


//bool MainWindow::isSubsetOf(vector <Pairs> &group1,vector <Pairs> &group2){
//    bool found;

//    // Check if all members of group1 are in group2
//    for(int i = 0; i < group1.size(); i++){
//        found = false;
//        for(int j = 0; j < group2.size(); j++){
//            if(group1[i].i*4 + group1[i].j == group2[j].i*4 + group2[j].j){
//                found = true;
//                break;
//            }
//        }
//        if(!found) return false;
//    }
//    return true;
//}


//QStringList MainWindow::simplifySet(QStringList set)
//{
//    for (int i = 0; i < set.length()-1; i++)
//    {
//        std::vector<int> indexes;
//        int max = 0;
//        int length = 0;
//        for (int j = i+1; j < set.length();j++)
//        {
//            int current = 0;
//            length = 0;
//            for (int k = 0; k < set[i].length(); k++)
//            {
//                if (set[i][k] != " " && set[j][k] != " " && set[i][k] == set[j][k])
//                    current++;
//                if (set[i][k] != " ")
//                    length++;
//            }
//            if (current > max)
//            {
//                indexes.clear();
//                indexes.push_back(j);
//                max = current;
//            }
//            else if (current == max)
//            {
//                indexes.push_back(j);
//            }
//        }
//        if (max == length - 1)
//        {
//            for (int j = 0;j < set[i].length(); j++)
//            {
//                if (set[indexes[0]][j] != set[i][j])
//                {
//                    set[i].replace(j,1," ");
//                    set[indexes[0]].replace(j,1," ");
//                }
//            }
//            set.removeAt(indexes[0]);
//        }
//    }
//    return set;
//}


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
