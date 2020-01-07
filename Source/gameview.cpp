#include "gameview.hpp"
#include <QPointF>
#include <QImage>

GameView::GameView():
  controller_(new GameController())
{
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  QImage mybackground;
  mybackground.load(":/images/background_hills.png");
  setBackgroundBrush(mybackground);

  controller_->start();

  setScene(controller_->getScene());
  centerOn(controller_->getPlayer());

  QObject::connect(controller_, SIGNAL(playerPositionChanged()), this, SLOT(playerPositionChanged()));
  QObject::connect(this, SIGNAL(keyPress(QKeyEvent*)), controller_, SLOT(keyPress(QKeyEvent*)));
  QObject::connect(this, SIGNAL(keyRelease(QKeyEvent*)), controller_, SLOT(keyRelease(QKeyEvent*)));
  QObject::connect(this, SIGNAL(LeftClick(QPointF)), controller_, SLOT(LeftClick(QPointF)));
  QObject::connect(this, SIGNAL(RightClick(QPointF)), controller_, SLOT(RightClick(QPointF)));
}

void GameView::playerPositionChanged()
{
  centerOn(controller_->getPlayer());
  update(50,50,100,100);
}
void GameView::mousePressEvent(QMouseEvent *event)
{
  switch(event->button())
  {
  case Qt::LeftButton:
    emit LeftClick(mapToScene(event->pos()));
    break;
  case Qt::RightButton:
    emit RightClick(mapToScene(event->pos()));
    break;
  default:
    break;
  }
}

void GameView::mouseDoubleClickEvent(QMouseEvent *event)
{
  mousePressEvent(event);
  mousePressEvent(event);
}
void GameView::keyPressEvent(QKeyEvent *event)
{
  emit keyPress(event);
}
void GameView::keyReleaseEvent(QKeyEvent *event)
{
  emit keyRelease(event);
}
