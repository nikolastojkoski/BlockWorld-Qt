#include "gamecontroller.hpp"
#include <algorithm>
#include <memory>
#include "player.hpp"
#include "block.hpp"
#include "blockgrid.hpp"

GameController::GameController()
{
  scene = new QGraphicsScene(0,0, 5000, 1500);

  frameTimer =new QTimer(this);
  frameTimer->setInterval(1000/60);
  frameTimer->start();
  QObject::connect(frameTimer, SIGNAL(timeout()), this, SLOT(tick()));
}

void GameController::start()
{
  grid = new BlockGrid(scene);
  grid->generate();

  player = new Player(300,300);
  scene->addItem(player);

  QObject::connect(frameTimer, SIGNAL(timeout()), player, SLOT(tick()));
  QObject::connect(player, SIGNAL(moved(Character*)), this, SLOT(playerMoved(Character*)));

  for(int i=1; i<5; i++)
  {
    enemies.push_back(new Zombie(i*1000, 300));
    QObject::connect(frameTimer, SIGNAL(timeout()), enemies.back(), SLOT(tick()));
    QObject::connect(enemies.back(), SIGNAL(moved(Character*)), this, SLOT(enemyMoved(Character*)));
    scene->addItem(enemies.back());
  }

  enemyStuckTimer = new QTimer(this);
  enemyStuckTimer->setInterval(150);
  enemyStuckTimer->start();
  QObject::connect(enemyStuckTimer, SIGNAL(timeout()), this, SLOT(enemyStuckCheck()));

  handleCharacterMovement(player);

  for(auto enemy: enemies)
  {
    handleCharacterMovement(enemy);
  }
}

QGraphicsScene *GameController::getScene() const
{
  return scene;
}

Player *GameController::getPlayer() const
{
  return player;
}
void GameController::keyPress(QKeyEvent *event)
{
  if(event->isAutoRepeat())
  {
    event->accept();
    return;
  }
  switch(event->key())
  {
  case Qt::Key_Right:
  case Qt::Key_D:
    player->startMovingRight();
    break;
  case Qt::Key_Left:
  case Qt::Key_A:
    player->startMovingLeft();
    break;
  case Qt::Key_Up:
  case Qt::Key_W:
    player->jump();
    break;
  case::Qt::Key_Space:
    player->punch();
    hitEnemies();
    break;
  default:
    break;
  }
  event->accept();
}
void GameController::keyRelease(QKeyEvent *event)
{
  if(event->isAutoRepeat())
  {
    event->accept();
    return;
  }
  switch(event->key())
  {
  case Qt::Key_Right:
  case Qt::Key_D:
    player->stopMovingRight();
    break;
  case Qt::Key_Left:
  case Qt::Key_A:
    player->stopMovingLeft();
    break;
  default:
    break;
  }
  event->accept();
}
void GameController::LeftClick(const QPointF &location)
{
  grid->removeBlock(location.x(), location.y());
  handleCharacterMovement(player);
}
void GameController::RightClick(const QPointF &location)
{
  std::unique_ptr<QGraphicsRectItem> dummy(grid->getDummyBlock(location.x(), location.y()));

  if(!player->getBody()->collidesWithItem(dummy.get()) &&
     !player->getHead()->collidesWithItem(dummy.get()))
  {
    grid->placeBlock(location.x(), location.y(), Block::BlockType::dirt);
  }
}

void GameController::handleCharacterMovement(Character *character)
{
  if(!grid->collidesWith(character->getFeet()))
  {
    if(!character->isMovingVertical())
    {
      character->fall();
    }
  }
  else
  {
    character->stopJump();

    /* refine Y position */
    int pbot = character->y() + character->getHeight();
    pbot = (pbot/Block::height) * Block::height;
    int newYpos = pbot - character->getHeight();

    character->setPos(character->x(), qreal(newYpos));
  }

  if(grid->collidesWith(character->getBody()))
  {
    character->gotoPrevXPos();
  }

  if(grid->collidesWith(character->getHead()))
  {
    character->gotoPrevYPos();
    character->fall();
  }
}
void GameController::playerMoved(Character *)
{
  handleCharacterMovement(player);

  emit playerPositionChanged();
}
void GameController::enemyMoved(Character *character)
{
  handleCharacterMovement(character);
}
void GameController::tick()
{
  for(auto zombie: enemies)
  {
    if(std::abs(zombie->x() - player->x()) > 500)
    {
      continue;
    }

    if(std::abs(player->x() - zombie->x()) < 1)
    {
      zombie->stopMovingLeft();
      zombie->stopMovingRight();
    }
    else if(player->x() < zombie->x())
    {
      if(!zombie->isMovingLeft())
      {
        zombie->startMovingLeft();
      }
    }
    else if(player->x() > zombie->x())
    {
      if(!zombie->isMovingRight())
      {
        zombie->startMovingRight();
      }
    }
  }
}
void GameController::enemyStuckCheck()
{
  for(auto zombie: enemies)
  {
    int distanceToPlayer = std::abs(zombie->x() - player->x());
    if((distanceToPlayer > 500) || (distanceToPlayer < 40))
    {
      continue;
    }
    if(std::abs(zombie->x() - zombie->getLastCheckedX()) < 1)
    {
      zombie->jump();
    }
    zombie->setLastCheckedX(zombie->x());
  }
}
void GameController::hitEnemies()
{
  auto it = enemies.begin();
  while(it != enemies.end())
  {
    if(player->collidesWithItem(*it))
    {
      scene->removeItem(*it);
      delete *it;
      it = enemies.erase(it);
    }
    else
    {
      it++;
    }
  }
}
GameController::~GameController()
{
  delete scene;
  delete grid;
  delete player;
  for(auto i: enemies)
  {
    delete i;
  }
  delete frameTimer;
  delete enemyStuckTimer;
}
