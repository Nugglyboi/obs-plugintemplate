#pragma once
#include <QWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include "scoreboard-switcher.hpp"
#include <QColor>

namespace Ui {
class ScoreboardDockHor;
class ScoreboardDockVert;
}

class ScoreboardDock : public QWidget {
    Q_OBJECT

public:
    explicit ScoreboardDock(QWidget *parent = nullptr);
    ~ScoreboardDock();

    void switchLayout(bool vertical);

private:
    Ui::ScoreboardDockHor *uiHor = nullptr;
    Ui::ScoreboardDockVert *uiVert = nullptr;

    QWidget *page = nullptr;
    QVBoxLayout *mainLayout = nullptr;
    QColor tagColor = QColor("#FF0000");

    ScoreboardUI ui;

    bool isVertical = false;

    void bindUI();
    void connectButtons();
    void createPage();
    void changeScore(int playerIndex, int delta);
    void clearScore(int playerIndex);
    void updateScoreLabels();
    void applyToObs();
    void swapPlayers();
    void resetAll();
    void applyDefaultStyle();
    void chooseTagColor();
    void updateTagColorButton();

    int score1 = 0;
    int score2 = 0;
    int maxGames = 999;
};