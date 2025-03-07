#include "leaderBoard.h"
#include <QVBoxLayout>
#include <QTableWidgetItem>
#include <QFile>
#include <QTextStream>

Leaderboard::Leaderboard(QWidget* parent)
    : QMainWindow(parent),
    ui(new Ui::LeaderBoardClass)
{
    ui->setupUi(this);
}

void Leaderboard::addEntry(const QString& name, const QString& time)
{
    // Insert a new row
    int row = ui->leaderTable->rowCount();
    ui->leaderTable->insertRow(row);

    // Set the name and score in the new row
    ui->leaderTable->setItem(row, 0, new QTableWidgetItem(name));
    ui->leaderTable->setItem(row, 1, new QTableWidgetItem(time));
}

void Leaderboard::loadLeaderboardFromFile(const QString& filename)
{
    QFile file(filename);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);

        while (!in.atEnd())
        {
            QString line = in.readLine();
            QStringList parts = line.split(' ');

            if (parts.length() == 2)
            {
                QString name = parts[0];
                QString time = parts[1];
                addEntry(name, time);
            }
        }

        file.close();
    }
}
