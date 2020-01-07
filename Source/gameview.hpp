#ifndef GAMEVIEW_HPP
#define GAMEVIEW_HPP

#include <QGraphicsView>
#include <QObject>
#include <QMouseEvent>
#include <QImage>

#include "gamecontroller.hpp"

class GameView: public QGraphicsView
{
  Q_OBJECT
public:
  GameView();
  virtual ~GameView() = default;

public slots:
  void playerPositionChanged();

signals:
  void LeftClick(const QPointF &location);
  void RightClick(const QPointF &location);
  void keyPress(QKeyEvent *event);
  void keyRelease(QKeyEvent *event);

protected:
  void mousePressEvent(QMouseEvent *event);
  void mouseDoubleClickEvent(QMouseEvent *event);
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);

private:
  GameController *controller_;

};


#endif // GAMEVIEW_HPP
