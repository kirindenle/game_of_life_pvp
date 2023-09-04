#include "GameOfLifeWidget.h"
#include <QPainter>
#include <QTimer>
#include <QMouseEvent>
#include <random>
// #include <QBrush>

void field::resize(size_t x, size_t y) {
  field_.resize(x * y);
  old_field_.resize(x * y);
  n_cells_x = x;
  n_cells_y = y;
}

void field::randomize(float life)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> rand;
  std::uniform_int_distribution<> choose(0, 1);
  for (auto& c : field_) {
    c = rand(gen) < life ? choose(gen) ? state::ALIVE1 : state::ALIVE2 : state::DEAD;
  }
}

void field::kill_all()
{
  for (auto &x : field_) {
    x = state::DEAD;
  }
}

void field::count_neighbours(int x, int y, int *pl1, int *pl2) {
  *pl1 = 0;
  *pl2 = 0;
  for (int i = x - 1; i <= x + 1; ++i) {
    for (int j = y - 1; j <= y + 1; ++j) {
      state c = cell((i + n_cells_x) % n_cells_x, (j + n_cells_y) % n_cells_y);
      (i == x && j == y) /*|| i < 0 || j < 0 || i >= n_cells_x || j >= n_cells_y*/ || c == state::DEAD ? 0 : (c == state::ALIVE1 ? (++*pl1) : (++*pl2));
      // cnt += ((i == x && j == y) || i < 0 || j < 0 || i >= n_cells_x || j >= n_cells_y) ? 0 : cell(i, j) == state::ALIVE;
    }
  }
}

void field::step() {
  for (int y = 0; y < n_cells_y; ++y) {
    for (int x = 0; x < n_cells_x; ++x) {
      int nei1;
      int nei2;
      count_neighbours(x, y, &nei1, &nei2);
      // set_tmp(x, y, neighbours == 3 || (cell(x, y) == state::ALIVE && neighbours == 2) ? state::ALIVE : state::DEAD);
      state c = cell(x, y);
      set_tmp(x, y,
        c == state::DEAD
        ? nei1 + nei2 == 3 ? nei1 > nei2 ? state::ALIVE1 : state::ALIVE2 : state::DEAD
        : nei1 + nei2 == 2 || nei1 + nei2 == 3 ? c : state::DEAD);
    }
  }
  std::swap(old_field_, field_);
}

field::state field::cell(int x, int y) {
  return field_[y * n_cells_x + x];
}

void field::set_tmp(int x, int y, state s) {
  old_field_[y * n_cells_x + x] = s;
}

void field::set_cell(int x, int y, state s) {
  field_[y * n_cells_x + x] = s;
}

GameOfLifeWidget::GameOfLifeWidget()
{
  setMinimumWidth(512);
  setMinimumHeight(512);

  setMouseTracking(true);

  cells.resize(80, 80);
  cells.randomize(0.3);

  repaint();
  /*
  auto* timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &GameOfLifeWidget::Step);
  timer->start(100);
  */
}

void GameOfLifeWidget::step() {
  cells.step();
  repaint();
}

void GameOfLifeWidget::clear() {
  cells.kill_all();
  repaint();
}

void GameOfLifeWidget::randomize()
{
  cells.randomize(0.3);
  repaint();
}

void GameOfLifeWidget::paintEvent(QPaintEvent* event)
{
  QWidget::paintEvent(event);

  QPainter painter(this);


  const int cell_width = width() / cells.n_cells_x;
  const int cell_heigth = height() / cells.n_cells_y;

  for (size_t x = 0; x < cells.n_cells_x; ++x) {
    for (size_t y = 0; y < cells.n_cells_y; ++y) {
      if (cells.cell(x, y) == field::state::ALIVE1) {
        painter.setBrush(QBrush(Qt::green));
      }
      else if (cells.cell(x, y) == field::state::ALIVE2) {
        painter.setBrush(QBrush(Qt::red));
      }
      if (cells.cell(x, y) != field::state::DEAD) {
        painter.drawRect(x * cell_width, y * cell_heigth, cell_width, cell_heigth);
      }
    }
  }
  painter.setPen(QPen(QBrush(Qt::black), 4));
  painter.drawLine(0, cells.n_cells_y / 2 * cell_heigth, width(), cells.n_cells_y / 2 * cell_heigth);
}

void GameOfLifeWidget::mouseReleaseEvent(QMouseEvent* event)
{
  const int cell_width = width() / cells.n_cells_x;
  const int cell_heigth = height() / cells.n_cells_y;

  int x = event->pos().x() / cell_width;
  int y = event->pos().y() / cell_heigth;

  if (x < cells.n_cells_x && y < cells.n_cells_y && x >= 0 && y >= 0) {
    if (event->button() == Qt::MiddleButton
      && cells.cell(x, y) != field::state::DEAD) {
      cells.set_cell(x, y, field::state::DEAD);
      repaint();
    }
    else if (event->button() == Qt::LeftButton
      && cells.cell(x, y) != field::state::ALIVE1) {
      cells.set_cell(x, y, field::state::ALIVE1);
      repaint();
    }
    else if (event->button() == Qt::RightButton
      && cells.cell(x, y) != field::state::ALIVE2) {
      cells.set_cell(x, y, field::state::ALIVE2);
      repaint();
    }
  }
}

void GameOfLifeWidget::mouseMoveEvent(QMouseEvent* event)
{
  const int cell_width = width() / cells.n_cells_x;
  const int cell_heigth = height() / cells.n_cells_y;

  int x = event->pos().x() / cell_width;
  int y = event->pos().y() / cell_heigth;

  if (x < cells.n_cells_x && y < cells.n_cells_y && x >= 0 && y >= 0) {
    if (event->buttons() == Qt::MiddleButton
      && cells.cell(x, y) != field::state::DEAD) {
      cells.set_cell(x, y, field::state::DEAD);
      repaint();
    }
    else if (event->buttons() == Qt::LeftButton
      && cells.cell(x, y) != field::state::ALIVE1) {
      cells.set_cell(x, y, field::state::ALIVE1);
      repaint();
    }
    else if (event->buttons() == Qt::RightButton
      && cells.cell(x, y) != field::state::ALIVE2) {
      cells.set_cell(x, y, field::state::ALIVE2);
      repaint();
    }
  }
}
