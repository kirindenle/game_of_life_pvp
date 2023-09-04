#pragma once

#include <QWidget>

struct field {
  enum class state { DEAD, ALIVE1, ALIVE2 };
  void resize(size_t x, size_t y);
  void randomize(float life);
  void kill_all();
  void count_neighbours(int x, int y, int* pl1, int* pl2);
  void step();
  state cell(int x, int y);
  void set_tmp(int x, int y, state s);
  void set_cell(int x, int y, state s);
  size_t n_cells_x;
  size_t n_cells_y;
  std::vector<state> field_;
  std::vector<state> old_field_;
};

class GameOfLifeWidget : public QWidget
{
    Q_OBJECT

public:
    GameOfLifeWidget();

    void paintEvent(QPaintEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

public slots:
    void step();
    void clear();
    void randomize();

private:
    field cells;
};
