#ifndef GAMECONTROLLER_HPP
#define GAMECONTROLLER_HPP

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QTimer>
#include <QObject>
#include <QRectF>
#include <list>
#include "player.hpp"
#include "zombie.hpp"
#include "blockgrid.hpp"

class Player;
class GameController: public QObject
{
  Q_OBJECT
public:
  GameController();
  virtual ~GameController();
  void start();

  QGraphicsScene *getScene() const;
  Player *getPlayer() const;

public slots:
  void tick();

  void keyPress(QKeyEvent *event);
  void keyRelease(QKeyEvent *event);
  void LeftClick(const QPointF &location);
  void RightClick(const QPointF &location);

  void playerMoved(Character *);
  void enemyMoved(Character *character);
  void enemyStuckCheck();

signals:
  void playerPositionChanged();

private:
  QGraphicsScene *scene;
  BlockGrid *grid;

  Player *player;
  std::list<Zombie*> enemies;

  QTimer *frameTimer, *enemyStuckTimer;

  void handleCharacterMovement(Character *);
  void hitEnemies();
};

#endif // GAMECONTROLLER_HPP
