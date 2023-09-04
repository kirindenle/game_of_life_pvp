#pragma once

#include <QWidget>

class GameOfLifeWidget;
class QPushButton;
class QTimer;

class MainWidget : public QWidget
{
public:
  MainWidget();

public slots:
  void on_pause_clicked();

private:
  GameOfLifeWidget* game_of_life;
  QPushButton* pause;
  QTimer* running;
  bool paused;
};

