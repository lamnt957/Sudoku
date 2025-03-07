#include <QWidget>
#include <QTableWidget>
#include "ui_leaderBoard.h"


class Leaderboard : public QMainWindow
{
    Q_OBJECT

public:
    explicit Leaderboard(QWidget* parent = nullptr);
    void loadLeaderboardFromFile(const QString& filename);

public slots:
    void addEntry(const QString& name, const QString& time);

private:
    Ui::LeaderBoardClass* ui;
};