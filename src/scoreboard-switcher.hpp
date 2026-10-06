#pragma once

#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>

class ScoreboardUI // Struct to hold pointers to the UI elements for easy access and manipulation
{
public:
    QPushButton* player1Plus = nullptr;
    QPushButton* player1Minus = nullptr;
    QPushButton* player1ClearScore = nullptr;

    QPushButton* player2Plus = nullptr;
    QPushButton* player2Minus = nullptr;
    QPushButton* player2ClearScore = nullptr;

    QPushButton* swapButton = nullptr;
    QPushButton* resetButton = nullptr;
    QPushButton* applyButton = nullptr;
    QPushButton* modeButton = nullptr;

    QLabel* player1Score = nullptr;
    QLabel* player2Score = nullptr;

    QLineEdit* player1Name = nullptr;
    QLineEdit* player2Name = nullptr;

    QLineEdit* player1Tag = nullptr;
    QLineEdit* player2Tag = nullptr;

    QTextEdit* matchName = nullptr;

    QPushButton* tagColorButton = nullptr;
};