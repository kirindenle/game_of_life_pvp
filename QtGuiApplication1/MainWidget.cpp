#include "MainWidget.h"
#include "GameOfLifeWidget.h"

#include <QLayout>
#include <QPushButton>
#include <QTimer>


MainWidget::MainWidget()
{
  game_of_life = new GameOfLifeWidget;
  pause = new QPushButton("start");
  running = new QTimer(this);

  auto* main_ver_layout = new QVBoxLayout;
  {
    auto* buttons_layout = new QHBoxLayout;
    {
      buttons_layout->addWidget(pause);
      pause->setAutoDefault(true);
      paused = true;
      connect(pause, &QPushButton::clicked, this, &MainWidget::on_pause_clicked);

      auto* clear = new QPushButton("clear");
      buttons_layout->addWidget(clear);
      connect(clear, &QPushButton::clicked, game_of_life, &GameOfLifeWidget::clear);


      auto* randomize = new QPushButton("randomize");
      buttons_layout->addWidget(randomize);
      connect(randomize, &QPushButton::clicked, game_of_life, &GameOfLifeWidget::randomize);


      auto* step = new QPushButton("step");
      buttons_layout->addWidget(step);
      connect(step, &QPushButton::clicked, game_of_life, &GameOfLifeWidget::step);

      buttons_layout->addStretch(2);
    }
    main_ver_layout->addLayout(buttons_layout);
    main_ver_layout->addWidget(game_of_life, 2);
  }

  setLayout(main_ver_layout);

  setWindowTitle(tr("Game Of Life PvP"));
  resize(800, 800);

  connect(running, &QTimer::timeout, game_of_life, &GameOfLifeWidget::step);
}

void MainWidget::on_pause_clicked() {
  if (paused) {
    paused = false;
    pause->setText("pause");
    running->start(10);
  } else {
    paused = true;
    pause->setText("start");
    running->stop();
  }
}
