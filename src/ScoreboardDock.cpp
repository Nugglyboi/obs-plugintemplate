#include "ScoreboardDock.hpp"

#include "ui_ScoreboardDockHor.h"
#include "ui_ScoreboardDockVert.h"

#include "ObsTextWriter.hpp"
#include <algorithm> // std::clamp
#include <utility>   // std::swap
#include <QColorDialog> // color
#include <QSettings> //settings
#include <QDebug>

ScoreboardDock::ScoreboardDock(QWidget *parent) : QWidget(parent)
{
    uiHor  = new Ui::ScoreboardDockHor;
    uiVert = new Ui::ScoreboardDockVert;

    mainLayout = new QVBoxLayout(this);

    QSettings settings("obs-scoreboard", "obs-scoreboard");
    tagColor = settings.value("tagColor", QColor("#FF0000")).value<QColor>();

    isVertical = false;
    createPage();
    applyDefaultStyle();
    updateScoreLabels();
    updateTagColorButton();
}

void ScoreboardDock::createPage()
{
    if (page) {
        mainLayout->removeWidget(page);
        delete page;
        page = nullptr;
    }

    page = new QWidget(this);

    if (isVertical)
        uiVert->setupUi(page);
    else
        uiHor->setupUi(page);

    mainLayout->addWidget(page);

    bindUI();            // re-point ui.* to current layout widgets
    connectButtons();    // connect signals to the current widgets
}

ScoreboardDock::~ScoreboardDock()
{
  delete uiHor;
  delete uiVert;
}

void ScoreboardDock::changeScore(int playerIndex, int delta)
{
  //Gets a reference to the score variable for the specified player 
  //Score1 for player 1, Score2 for player 2

  //Clamps the new score to be within the range of 0 to maxGames (999) 
  //Updates the score labels in the UI.
  int &s = (playerIndex == 1) ? score1 : score2;
  s = std::clamp(s + delta, 0, maxGames);
  updateScoreLabels();
}

void ScoreboardDock::clearScore(int playerIndex)
{
  //Gets a reference to the score variable for the specified player 
  //Score1 for player 1, Score2 for player 2

  //Clamps the new score to be within the range of 0 to maxGames (999) 
  //Updates the score labels in the UI.

  int &s = (playerIndex == 1) ? score1 : score2;
  s = 0;
  if (playerIndex == 1) {
    ui.player1Name->clear();
    ui.player1Tag->clear();
  } else {
    ui.player2Name->clear();
    ui.player2Tag->clear();
  }
  updateScoreLabels();
}

void ScoreboardDock::updateScoreLabels() // Updates the score labels in the UI to reflect the current scores of both players
{
  ui.player1Score->setText(QString::number(score1));
  ui.player2Score->setText(QString::number(score2));
}

void ScoreboardDock::applyToObs()
{
    ObsTextWriter::setInt("ScoreText1", score1);
    ObsTextWriter::setInt("ScoreText2", score2);

    ObsTextWriter::setText("RoundText", ui.matchName->toPlainText());

    // Player 1
    QString player1Tag = ui.player1Tag->text().toHtmlEscaped();
    QString player1Name = ui.player1Name->text().toHtmlEscaped();

    QString player1Text;

    if (!player1Tag.isEmpty()) {
        player1Text =
            "<span foreground=\"" +
            tagColor.name() +
            "\">" +
            player1Tag +
            "</span> " +
            player1Name;
    } else {
        player1Text = player1Name;
    }

    ObsTextWriter::setText("PlayerText1", player1Text);


    // Player 2
    QString player2Tag = ui.player2Tag->text().toHtmlEscaped();
    QString player2Name = ui.player2Name->text().toHtmlEscaped();

    QString player2Text;

    if (!player2Tag.isEmpty()) {
        player2Text =
            "<span foreground=\"" +
            tagColor.name() +
            "\">" +
            player2Tag +
            "</span> " +
            player2Name;
    } else {
        player2Text = player2Name;
    }

    ObsTextWriter::setText("PlayerText2", player2Text);
}

void ScoreboardDock::swapPlayers() // Swaps the names and scores of the two players in the UI
{
  const QString p1 = ui.player1Name->text();
  ui.player1Name->setText(ui.player2Name->text());
  ui.player2Name->setText(p1);

  const QString p1Tag = ui.player1Tag->text();

  ui.player1Tag->setText(ui.player2Tag->text());
  ui.player2Tag->setText(p1Tag);

  std::swap(score1, score2);
  updateScoreLabels();
}

void ScoreboardDock::resetAll() // Resets the scores and names of both players in the UI to their default values
{
  ui.player1Name->clear();
  ui.player2Name->clear();
  ui.player1Tag->clear();
  ui.player2Tag->clear();
  ui.matchName->clear();
  score1 = 0;
  score2 = 0;
  updateScoreLabels();
}

void ScoreboardDock::applyDefaultStyle() // Applies a default style to all QPushButton widgets in the UI
{
    QString style =
        "font-size: 9pt;";

    for (QPushButton* button : findChildren<QPushButton*>())
    {
        button->setStyleSheet(style);
    }
}

void ScoreboardDock::switchLayout(bool vertical) // Switches the layout of the scoreboard between horizontal and vertical orientations
{
    //Cache from current page
    const QString p1 = ui.player1Name ? ui.player1Name->text() : QString();
    const QString p1Tag = ui.player1Tag ? ui.player1Tag->text() : QString();

    const QString p2 = ui.player2Name ? ui.player2Name->text() : QString();
    const QString p2Tag = ui.player2Tag ? ui.player2Tag->text() : QString();
    
    const QString matchText = ui.matchName ? ui.matchName->toPlainText() : QString();

    //Change mode + rebuild the page so new widgets exist
    isVertical = vertical;
    createPage();

    //ui.* points to the NEW widgets -> restore into the new page
    ui.player1Name->setText(p1);
    ui.player1Tag->setText(p1Tag);
    ui.player2Name->setText(p2);
    ui.player2Tag->setText(p2Tag);
    ui.matchName->setPlainText(matchText);

    //Refresh score labels on the new page
    updateScoreLabels();
    updateTagColorButton();
}

void ScoreboardDock::connectButtons() // Connects the clicked signals to each button
{
    connect(ui.player1Plus, &QPushButton::clicked,
        this, [this]{ changeScore(1, +1); });

    connect(ui.player1Minus, &QPushButton::clicked,
        this, [this]{ changeScore(1, -1); });

    connect(ui.player1ClearScore, &QPushButton::clicked,
        this, [this]{ clearScore(1); });

    connect(ui.player2Plus, &QPushButton::clicked,
        this, [this]{ changeScore(2, +1); });

    connect(ui.player2Minus, &QPushButton::clicked,
        this, [this]{ changeScore(2, -1); });

    connect(ui.player2ClearScore, &QPushButton::clicked,
        this, [this]{ clearScore(2); });

    connect(ui.swapButton, &QPushButton::clicked,
        this, [this]{ swapPlayers(); });

    connect(ui.resetButton, &QPushButton::clicked,
        this, [this]{ resetAll(); });

    connect(ui.applyButton, &QPushButton::clicked,
        this, [this]{ applyToObs(); });
        
    connect(ui.modeButton, &QPushButton::clicked,
        this, [this]{ switchLayout(!isVertical); });

    connect(ui.tagColorButton, &QPushButton::clicked,
        this, [this]{ chooseTagColor(); });
}

void ScoreboardDock::chooseTagColor()
{
    QColor color = QColorDialog::getColor(
        tagColor,
        this,
        "Choose Tag Color"
    );

    if (!color.isValid())
        return;

    tagColor = color;

    QSettings settings("obs-scoreboard", "obs-scoreboard");
    settings.setValue("tagColor", tagColor);
    settings.sync();

    qDebug() << "Saved tag color:" << tagColor;
    qDebug() << "QSettings location:" << settings.fileName();

    updateTagColorButton();
}

void ScoreboardDock::updateTagColorButton()
{
    if (!ui.tagColorButton)
        return;

    ui.tagColorButton->setStyleSheet(
        QString(
            "background-color: %1;"
            "font-size: 9pt;"
        ).arg(tagColor.name())
    );
}

void ScoreboardDock::bindUI() // Re-points the ui.* pointers to the current layout's widgets
{
    if (isVertical)
    {
        ui.player1Plus = uiVert->player1Plus;
        ui.player1Minus = uiVert->player1Minus;
        ui.player1ClearScore = uiVert->player1ClearScore;

        ui.player2Plus = uiVert->player2Plus;
        ui.player2Minus = uiVert->player2Minus;
        ui.player2ClearScore = uiVert->player2ClearScore;

        ui.swapButton = uiVert->swapButton;
        ui.resetButton = uiVert->resetButton;
        ui.applyButton = uiVert->applyButton;
        ui.modeButton = uiVert->modeButton;

        ui.player1Score = uiVert->player1Score;
        ui.player2Score = uiVert->player2Score;

        ui.player1Tag = uiVert->player1Tag;
        ui.player2Tag = uiVert->player2Tag;

        ui.player1Name = uiVert->player1Name;
        ui.player2Name = uiVert->player2Name;

        ui.matchName = uiVert->matchName;
        ui.tagColorButton = uiVert->tagColorButton;
    }
    else
    {
        ui.player1Plus = uiHor->player1Plus;
        ui.player1Minus = uiHor->player1Minus;
        ui.player1ClearScore = uiHor->player1ClearScore;

        ui.player2Plus = uiHor->player2Plus;
        ui.player2Minus = uiHor->player2Minus;
        ui.player2ClearScore = uiHor->player2ClearScore;

        ui.swapButton = uiHor->swapButton;
        ui.resetButton = uiHor->resetButton;
        ui.applyButton = uiHor->applyButton;
        ui.modeButton = uiHor->modeButton;

        ui.player1Score = uiHor->player1Score;
        ui.player2Score = uiHor->player2Score;

        ui.player1Name = uiHor->player1Name;
        ui.player2Name = uiHor->player2Name;

        ui.player1Tag = uiHor->player1Tag;
        ui.player2Tag = uiHor->player2Tag;

        ui.matchName = uiHor->matchName;
        ui.tagColorButton = uiHor->tagColorButton;
    }

    applyDefaultStyle();

    // The Score labels need to be larger
    ui.player1Score->setStyleSheet(
        "font-size: 32pt; font-weight: bold;"
    );

    ui.player2Score->setStyleSheet(
        "font-size: 32pt; font-weight: bold;"
    );
}