#pragma once


#include "ui_Sudoku.h"
#include "Solve.h"
#include <QThread>
#include <QTimer>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>


class Sudoku : public QMainWindow
{
    Q_OBJECT

public:
    explicit Sudoku(QWidget* parent = Q_NULLPTR);
    ~Sudoku();

    void DisplayGrid();

private:
    Ui::sudokuClass* ui;
    Solve SolveThread;
    QTimer* timer = new QTimer(this);
    bool isPresentInCol(int col, int num);
    bool isPresentInRow(int row, int num);
    bool isPresentInBox(int boxStartRow, int boxStartCol, int num);
    bool isDuplicatedInCol(int col, int num);
    bool isDuplicatedInRow(int row, int num);
    bool isDuplicatedInBox(int boxStartRow, int boxStartCol, int num);
    bool findEmptyPlace(int& row, int& col);
    bool isValidPlace(int row, int col, int num);
    bool isValidNum(int row, int col, int num);
    bool solveSudoku();
    int checkSudoku();
    bool solveSudokuNumber();
    void GreyCells();
    void generate_Sudoku();
    void removeRandomCells(int numCellsToRemove);
    void lockCells();
    bool isCustomize();
    void playRnd();
    void updateTimer();
    void saveRecord(QString name);

signals:
    void on_Stopped();

private slots:
    void on_Clear_clicked();
    void on_SolveSlow_clicked();
    void on_Solve_clicked();
    void on_Undo_Solve_clicked();
    void on_Stop_Recursion_clicked();
    void on_Play_clicked();
    void on_Check_clicked();
    void on_UndoCheck_clicked();
    void on_Custom_clicked();
    void on_PlayAgain_clicked();
    //void on_leaderBoard_clicked();

public slots:
    void DisplaySingleItem(int i, int j, int n);
};


