#include "zombie.hpp"

Zombie::Zombie(int x, int y):
  Character(10, 70, 30, 46, x, y,
    QPixmap(":/images/zombie_idleRight.png"),
    QPixmap(":/images/zombie_idleLeft.png"),
    QPixmap(":/images/zombie_walkRight1.png"),
    QPixmap(":images/zombie_walkRight2.png"),
    QPixmap(":images/zombie_walkLeft1.png"),
    QPixmap(":images/zombie_walkLeft2.png"))
{
  setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
  setPos(x, y);
}

int Zombie::getLastCheckedX() const
{
  return lastCheckedX_;
}
void Zombie::setLastCheckedX(int x)
{
  lastCheckedX_ = x;
}
