#include "player.hpp"

Player::Player(int x, int y):
  Character(25, 70, 30, 46, x, y,
    QPixmap(":/images/player_idleRight.png"),
    QPixmap(":/images/player_idleLeft.png"),
    QPixmap(":/images/player_walkRight1.png"),
    QPixmap(":images/player_walkRight2.png"),
    QPixmap(":images/player_walkLeft1.png"),
    QPixmap(":images/player_walkLeft2.png")),
  punchRightImg_(":images/player_punchRight.png"),
  punchLeftImg_(":images/player_punchLeft.png"),
  punching_(false),
  punchingLeft_(false)
{
  setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
  setPos(x, y);
}

void Player::punch()
{
  punching_ = true;
}
void Player::animationTick()
{
  if(punching_)
  {
    if(movingLeft_ || state_ == idleLeft)
    {
      punchingLeft_ = true;
      setPos(x() - 10, y());
      setPixmap(punchLeftImg_);
    }
    else
    {
      punchingLeft_ = false;
      setPixmap(punchRightImg_);
    }
    punching_ = false;
  }
  else
  {
    if(punchingLeft_)
    {
      setPos(x()+10, y());
      punchingLeft_ = false;
    }
    Character::animationTick();
  }
}
