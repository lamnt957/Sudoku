#include <thread>
#include "Sudoku.h"
//#include "leaderBoard.h"
#include <chrono>
#include <thread>
#include <time.h>
#include <iostream>
#include <QMessageBox>
#include <QInputDialog>
#include <QTimer>
#include <QCoreApplication>
#include <QThread>
#include <QFile>
#include <QTextStream>

int NumSolutionFound = 0;
const int EMPTY = 0;
#define N 9
int grid[N][N] = {
   {0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0}
};

int previousgrid[N][N] = {
   {0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0}
};

bool customize = false;

int min = 0;
int sec0 = 0;
int sec1 = 0;


Sudoku::Sudoku(QWidget* parent)
    : QMainWindow(parent),
    ui(new Ui::sudokuClass)
{
    ui->setupUi(this);
    ui->SolveBox->setVisible(false);
    ui->PlayAgain->setVisible(false);

    connect(timer, &QTimer::timeout, this, &Sudoku::updateTimer);
    
}

Sudoku::~Sudoku()
{
    delete ui;
}

void Sudoku::on_Clear_clicked()
{
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            ui->Table->setItem(i, j, new QTableWidgetItem);
            grid[i][j] = EMPTY;
        }
    }
    ui->TimerLbl->setText("");
    ui->Table->setEnabled(false);
    ui->Stop_Recussion->setEnabled(false);
    ui->Undo_Solve->setEnabled(false);
    ui->SolveBox->setVisible(false);
    ui->PlayBox->setVisible(true);
    ui->Custom->setEnabled(true);
    ui->DifficultySlider->setEnabled(true);
    ui->PlayAgain->setVisible(false);

    timer->stop();

    customize = false;
    min = 0;
    sec0 = 0;
    sec1 = 0;
}


void Sudoku::on_SolveSlow_clicked()
{


    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            grid[i][j] = EMPTY;
        }
    }

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            QTableWidgetItem* cell = ui->Table->item(i, j);
            if (cell) {
                if (cell->text().toInt() > 0)
                {
                    if (cell->text().toInt() >= 10)
                    {
                        QMessageBox msgBox;
                        msgBox.setWindowTitle("Cell greater than 9!");
                        msgBox.setIcon(QMessageBox::Warning);
                        msgBox.setText("Please re-edit the board");
                        msgBox.setStandardButtons(QMessageBox::Ok);
                        msgBox.setDefaultButton(QMessageBox::Ok);
                        int ret = msgBox.exec();
                        return;
                    }
                    if (isValidPlace(i, j, cell->text().toInt()) == false)
                    {
                        QMessageBox msgBox;
                        msgBox.setWindowTitle("Incorrect board!");
                        msgBox.setIcon(QMessageBox::Warning);
                        msgBox.setText("Please re-edit the board");
                        msgBox.setStandardButtons(QMessageBox::Ok);
                        msgBox.setDefaultButton(QMessageBox::Ok);
                        int ret = msgBox.exec();
                        return;
                    }
                }
                grid[i][j] = cell->text().toInt();
            }
            previousgrid[i][j] = grid[i][j];
        }
    }


    connect(this, &Sudoku::on_Stopped, &SolveThread, &Solve::Stop);
    connect(&SolveThread, &Solve::NewItem, this, &Sudoku::DisplaySingleItem);


    QtConcurrent::run(&Solve::start, &this->SolveThread, grid, ui->horizontalSlider->value());
    ui->Undo_Solve->setEnabled(true);
    DisplayGrid();
}
void Sudoku::on_Stop_Recursion_clicked()
{
    emit on_Stopped();

}


void Sudoku::on_Solve_clicked()
{
    
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            grid[i][j] = EMPTY;
        }
    }


    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            QTableWidgetItem* cell = ui->Table->item(i, j);
            if (cell) {
                if (cell->text().toInt() > 0)
                {
                    if (cell->text().toInt() >= 10)
                    {
                        QMessageBox msgBox;
                        msgBox.setWindowTitle("Cell greater than 9!");
                        msgBox.setIcon(QMessageBox::Warning);
                        msgBox.setText("Please re-edit the board");
                        msgBox.setStandardButtons(QMessageBox::Ok);
                        msgBox.setDefaultButton(QMessageBox::Ok);
                        int ret = msgBox.exec();
                        return;
                    }
                    if (isValidPlace(i, j, cell->text().toInt()) == false)
                    {
                        QMessageBox msgBox;
                        msgBox.setWindowTitle("Incorrect Board!");
                        msgBox.setIcon(QMessageBox::Warning);
                        msgBox.setText("Please re-edit the board");
                        msgBox.setStandardButtons(QMessageBox::Ok);
                        msgBox.setDefaultButton(QMessageBox::Ok);
                        int ret = msgBox.exec();
                        return;
                    }
                }
                grid[i][j] = cell->text().toInt();
            }
            previousgrid[i][j] = grid[i][j];
        }
    }
    qDebug() << "Solving";
    bool test = solveSudoku();
    qDebug() << "Solved";

    NumSolutionFound = 0;
    bool test1 = solveSudokuNumber();
    ui->Undo_Solve->setEnabled(true);
    DisplayGrid();

}

void Sudoku::on_Undo_Solve_clicked() {
    ui->Undo_Solve->setEnabled(false);
    ui->UndoCheck->setEnabled(false);

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            grid[i][j] = previousgrid[i][j];
            ui->Table->setItem(i, j, new QTableWidgetItem);
            if (grid[i][j] > 0) {
                QString str = QString::number(grid[i][j]);
                ui->Table->item(i, j)->setText(str);
                lockCells();
            }

        }
    }

}



void Sudoku::DisplayGrid()
{
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            ui->Table->setItem(i, j, new QTableWidgetItem);
            if (grid[i][j] > 0) {
                QString str = QString::number(grid[i][j]);
                ui->Table->item(i, j)->setText(str);
                if (previousgrid[i][j] > 0)
                {
                    ui->Table->item(i, j)->setFlags(ui->Table->item(i, j)->flags() & ~Qt::ItemIsEditable);
                    ui->Table->item(i, j)->setBackground(Qt::lightGray);
                }
            }
        }
    }
    update();
}

void Sudoku::GreyCells()
{
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            ui->Table->setItem(i, j, new QTableWidgetItem);
            if (grid[i][j] > 0) {
                QString str = QString::number(grid[i][j]);
                ui->Table->item(i, j)->setText(str);
                ui->Table->item(i, j)->setBackground(Qt::lightGray);
            }
        }
    }
    update();
}

void Sudoku::DisplaySingleItem(int i, int j, int n)
{
    ui->Table->setItem(i, j, new QTableWidgetItem);
    QString str = QString::number(n);
    ui->Table->item(i, j)->setText(str);
}


bool Sudoku::isPresentInCol(int col, int num) { //check whether num is present in col or not
    for (int row = 0; row < N; row++)
        if (grid[row][col] == num)
            return true;
    return false;
}
bool Sudoku::isPresentInRow(int row, int num) { //check whether num is present in row or not
    for (int col = 0; col < N; col++)
        if (grid[row][col] == num)
            return true;
    return false;
}
bool Sudoku::isPresentInBox(int boxStartRow, int boxStartCol, int num) {
    //check whether num is present in 3x3 box or not
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
            if (grid[row + boxStartRow][col + boxStartCol] == num)
                return true;
    return false;
}
bool Sudoku::findEmptyPlace(int& row, int& col) { //get empty location and update row and column
    for (row = 0; row < N; row++)
        for (col = 0; col < N; col++)
            if (grid[row][col] == 0) //marked with 0 is empty
                return true;
    return false;
}
bool Sudoku::isValidPlace(int row, int col, int num) {
    //when item not found in col, row and current 3x3 box
    return !isPresentInRow(row, num) && !isPresentInCol(col, num) && !isPresentInBox(row - row % 3,
        col - col % 3, num);
}
bool Sudoku::solveSudoku() {
    int row, col;
    if (!findEmptyPlace(row, col))
        return true; //when all places are filled
    for (int num = 1; num <= 9; num++) { //valid numbers are 1 - 9
        if (isValidPlace(row, col, num)) { //check validation, if yes, put the number in the grid
            grid[row][col] = num;
            if (solveSudoku()) //recursively go for other rooms in the grid
                return true;
            grid[row][col] = 0; //turn to unassigned space when conditions are not satisfied
        }
    }
    return false;
}

bool Sudoku::isDuplicatedInCol(int col, int num) { //check whether num is present in col or not
    int t = 0;
    for (int row = 0; row < N; row++)
        if (grid[row][col] == num)
            t += 1;
    if (t > 1)
        return true;
    return false;
}
bool Sudoku::isDuplicatedInRow(int row, int num) { //check whether num is present in row or not
    int t = 0;
    for (int col = 0; col < N; col++)
        if (grid[row][col] == num)
            t += 1;
    if (t > 1)
        return true;
    return false;
}
bool Sudoku::isDuplicatedInBox(int boxStartRow, int boxStartCol, int num) {
    //check whether num is present in 3x3 box or not
    int t = 0;
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
            if (grid[row + boxStartRow][col + boxStartCol] == num)
                t += 1;
    if (t > 1)
        return true;
    return false;
}

bool Sudoku::isValidNum(int row, int col, int num) {
    //when item not found in col, row and current 3x3 box
    return !isDuplicatedInRow(row, num) && !isDuplicatedInCol(col, num) && !isDuplicatedInBox(row - row % 3,
        col - col % 3, num);
}
int Sudoku::checkSudoku() {
    int t = 0;
    for (int row = 0; row < N; row++)
        for (int col = 0; col < N; col++)
        {
            int num = grid[row][col];
            if (num == EMPTY) {
                ui->Table->item(row, col)->setBackground(Qt::red);
                t += 1;
            }
            else if (!isValidNum(row, col, num) && (ui->Table->item(row, col)->background().color() != Qt::lightGray)) {
                ui->Table->item(row, col)->setBackground(Qt::red);
                t += 1;
            }
        }
    return t;
}

bool Sudoku::solveSudokuNumber() {
    int row, col;
    if (!findEmptyPlace(row, col))
        NumSolutionFound++;
    qDebug() << "Number solutions found is " << NumSolutionFound;
    return false; //when all places are filled
    for (int num = 1; num <= 9; num++) { //valid numbers are 1 - 9
        if (isValidPlace(row, col, num)) { //check validation, if yes, put the number in the grid
            grid[row][col] = num;
            if (solveSudoku()) //recursively go for other rooms in the grid
                return true;
            grid[row][col] = 0; //turn to unassigned space when conditions are not satisfied
        }
    }
    return false;
}
/// <summary>
/// //////////////////////////////////////////////////
/// </summary>
void Sudoku::on_Play_clicked() {

    if (isCustomize()) {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                grid[i][j] = EMPTY;
            }
        }


        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                QTableWidgetItem* cell = ui->Table->item(i, j);
                if (cell) {
                    if (cell->text().toInt() > 0)
                    {
                        if (cell->text().toInt() >= 10)
                        {
                            QMessageBox msgBox;
                            msgBox.setWindowTitle("Cell greater than 9!");
                            msgBox.setIcon(QMessageBox::Warning);
                            msgBox.setText("Please re-edit the board");
                            msgBox.setStandardButtons(QMessageBox::Ok);
                            msgBox.setDefaultButton(QMessageBox::Ok);
                            int ret = msgBox.exec();
                            return;
                        }
                        if (isValidPlace(i, j, cell->text().toInt()) == false)
                        {
                            QMessageBox msgBox;
                            msgBox.setWindowTitle("Incorrect board!");
                            msgBox.setIcon(QMessageBox::Warning);
                            msgBox.setText("Please re-edit the board");
                            msgBox.setStandardButtons(QMessageBox::Ok);
                            msgBox.setDefaultButton(QMessageBox::Ok);
                            int ret = msgBox.exec();
                            return;
                        }
                    }
                    grid[i][j] = cell->text().toInt();
                }
                previousgrid[i][j] = grid[i][j];
            }
        }
        lockCells();
    }
    else playRnd();
    timer->start(1000);
    ui->SolveBox->setVisible(true);
    ui->PlayBox->setVisible(false);
    ui->TimerLbl->setText("0:00");
}

void Sudoku::on_Check_clicked() {
    ui->UndoCheck->setEnabled(true);
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            QTableWidgetItem* cell = ui->Table->item(i, j);
            if (cell) {
                grid[i][j] = cell->text().toInt();
            }
        }
    }
    int numErr = checkSudoku();
    if (numErr == 0) {
        timer->stop();
        for (int row = 0; row < N; row++)
            for (int col = 0; col < N; col++)
                if (ui->Table->item(row, col)->background().color() != Qt::lightGray)
                    ui->Table->item(row, col)->setBackground(Qt::green);
        QMessageBox msgBox;
        msgBox.setWindowTitle("Congrats!");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText("You have completed the board!");
        msgBox.setInformativeText("Your record: " + QString::number(min) + ":" + QString::number(sec0) + QString::number(sec1)); /*\
            +"\nDo you wanna save your record?");*/
        /*msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);*/
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();
        /*
        if (ret == QMessageBox::Yes) {
            bool ok;
            QString name = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                tr("Type your name:"), QLineEdit::Normal,
                QDir::home().dirName(), &ok);
            if (ok && !name.isEmpty())
                saveRecord(name);
        }*/
        ui->PlayAgain->setVisible(true);
        ui->SolveBox->setVisible(false);
    }
}


void Sudoku::on_UndoCheck_clicked() {
    ui->UndoCheck->setEnabled(false);
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (ui->Table->item(i, j)->background().color() != Qt::lightGray) {
                ui->Table->item(i, j)->setBackground(Qt::white);
            }
        }
    }
}
void Sudoku::generate_Sudoku() {
    // Sử dụng thư viện ngẫu nhiên để tạo số ngẫu nhiên
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 9);
    int n = 1;

    for (int i = 0; i < 5; ++i) {
        int row = dis(gen);
        int col = dis(gen);
        int num = dis(gen);
        while (!isValidPlace(row, col, num)) {
            num = dis(gen);
        }
        grid[row][col] = num;
    }
    solveSudoku();
}
void Sudoku::removeRandomCells(int numCellsToRemove) {
    // Sử dụng thư viện ngẫu nhiên để tạo số ngẫu nhiên
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, N - 1);
    int removedCells = 0;
    while (removedCells < numCellsToRemove) {
        int row = dis(gen);
        int col = dis(gen);
        if (grid[row][col] != EMPTY) {
            grid[row][col] = EMPTY;
            removedCells++;
        }
    }
}
void Sudoku::lockCells() {
    // Chặn sửa đổi trên các ô không phải là EMPTY
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            QTableWidgetItem* item = ui->Table->item(i, j);
            if (item && grid[i][j] != EMPTY) {
                item->setFlags(item->flags() & ~Qt::ItemIsEditable); // Chặn sửa đổi
                item->setBackground(Qt::lightGray); // Có thể thay đổi màu để chỉ định ô đã khoá
            }
        }
    }
}
void  Sudoku::playRnd() {
    ui->Table->setEnabled(true);
    generate_Sudoku();
    removeRandomCells(ui->DifficultySlider->value());
    DisplayGrid();
    lockCells();
}

bool Sudoku::isCustomize() {
    return customize;
}

void Sudoku::on_Custom_clicked() {
    ui->Table->setEnabled(true);
    ui->Custom->setEnabled(false);
    ui->DifficultySlider->setEnabled(false);

    customize = true;
}

void Sudoku::updateTimer() {
    sec1++;
    if (sec1 == 10) {
        sec0++;
        sec1 = 0;
    }
    if (sec0 == 6) {
        sec0 = 0;
        sec1 = 0;
        min++;
    }
    QString time = QString::number(min) + ":" + QString::number(sec0) + QString::number(sec1);
    ui->TimerLbl->setText(time);
}

void Sudoku::saveRecord(QString name){
    QString filename = "records.txt";
    QFile file(filename);
    if (file.open(QIODevice::ReadWrite)) {
        QString data = "";
        while (!file.atEnd()) 
            data += file.readLine();
        QTextStream stream(&file);
        stream << name + ", " + QString::number(min) + ":" + QString::number(sec0) + QString::number(sec1) + "\n";
        file.close();
    }
}

void Sudoku::on_PlayAgain_clicked() {
    on_Clear_clicked();
}

//void Sudoku::on_leaderBoard_clicked() {
//    Leaderboard *leaderboard = new Leaderboard(this, Qt::Window);
//    leaderboard.show();
//}