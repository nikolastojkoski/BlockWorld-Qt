#include "character.hpp"

Character::Character(int speedX, int speedY, int width,
                     int height, qreal prevX, qreal prevY,
                     QPixmap idleRightImg, QPixmap idleLeftImg, QPixmap walkRight1Img, QPixmap walkRight2Img,
                     QPixmap walkLeft1Img, QPixmap walkLeft2Img):
  speedX_(speedX),
  speedY_(speedY),
  width_(width),
  height_(height),
  prevX_(prevX),
  prevY_(prevY),
  movingVertical_(false),
  movingRight_(false),
  movingLeft_(false),
  head_(new QGraphicsRectItem()),
  body_(new QGraphicsRectItem()),
  feet_(new QGraphicsRectItem()),
  idleRightImg_(idleRightImg),
  idleLeftImg_(idleLeftImg),
  walkRight1Img_(walkRight1Img),
  walkRight2Img_(walkRight2Img),
  walkLeft1Img_(walkLeft1Img),
  walkLeft2Img_(walkLeft2Img),
  state_(AnimationState::idleRight),
  movementTimer_(new QTimer(this))
{

  setPixmap(idleRightImg_);

  movementTimer_->setInterval(100);
  movementTimer_->start();
  QObject::connect(movementTimer_.get(), SIGNAL(timeout()), this, SLOT(animationTick()));

}

void Character::startMovingLeft()
{
  movingRight_ = false;
  setPixmap(walkLeft1Img_);
  state_ = AnimationState::walkLeft1;
  vx_ = -speedX_;
  movingLeft_ = true;
}
void Character::startMovingRight()
{
  movingLeft_ = false;
  setPixmap(walkRight1Img_);
  state_ = AnimationState::walkRight1;
  vx_ = speedX_;
  movingRight_ = true;
}
void Character::stopMovingLeft()
{
  if(!movingRight_)
  {
    setPixmap(idleLeftImg_);
    state_ = AnimationState::idleLeft;
  }

  movingLeft_ = false;
}
void Character::stopMovingRight()
{
  if(!movingLeft_)
  {
    setPixmap(idleRightImg_);
    state_ = AnimationState::idleRight;
  }
  movingRight_ = false;
}
void Character::jump()
{
  if(!movingVertical_)
  {
    movingVertical_ = true;
    v0y_ = speedY_;
    ty_ = 0.0;
    moveBy(0, -10);
    y0_ = y();
  }
}
void Character::fall()
{
  v0y_ = 0;
  ty_ = 0.0;
  y0_ = y();
  movingVertical_ = true;
}
void Character::stopJump()
{
  movingVertical_ = false;
}

void Character::gotoPrevXPos()
{
  setPos(prevX_, y());
}
void Character::gotoPrevYPos()
{
  setPos(x(), prevY_);
}

void Character::setPos(qreal x, qreal y)
{
  QGraphicsPixmapItem::setPos(x, y);
}

void Character::setPos(const QPointF &pos)
{
  QGraphicsPixmapItem::setPos(pos);
}

void Character::moveBy(qreal dx, qreal dy)
{
  QGraphicsPixmapItem::moveBy(dx, dy);
  emit moved(this);
}
void Character::update()
{
  prevX_ = x();
  prevY_ = y();
}
void Character::tick()
{
  static const double dt = 0.15;
  static const double g = -25; //9.81;

  if(movingRight_ || movingLeft_)
  {
    prevX_ = x();
    moveBy(dt*vx_, 0);
    emit moved(this);
  }

  if(movingVertical_)
  {

    prevY_ = y();
    qreal newYposition = y0_ - v0y_*ty_ - g*ty_*ty_/2;

    if(std::abs(newYposition - y()) > 40)
    {
      ty_ = 0;
    }
    else
    {
      setPos(x(), newYposition);

      ty_ += dt;
    }
    emit moved(this);
  }
}
void Character::animationTick()
{
  switch(state_)
  {
  case AnimationState::idleLeft:
    setPixmap(idleLeftImg_);
    break;
  case AnimationState::idleRight:
    setPixmap(idleRightImg_);
    break;
  case AnimationState::walkRight1:
    state_ = AnimationState::walkRight2;
    setPixmap(walkRight2Img_);
    break;
  case AnimationState::walkRight2:
    state_ = AnimationState::walkRight1;
    setPixmap(walkRight1Img_);
    break;
  case AnimationState::walkLeft1:
    state_ = AnimationState::walkLeft2;
    setPixmap(walkLeft2Img_);
    break;
  case AnimationState::walkLeft2:
    state_ = AnimationState::walkLeft1;
    setPixmap(walkLeft1Img_);
    break;
  default:
    break;
  }
}
QGraphicsRectItem *Character::getHead() const
{
  head_->setRect(x()+1, y(), width_-2, 1);
  return head_.get();
}
QGraphicsRectItem *Character::getBody() const
{
  body_->setRect(x(), y()+2, width_, height_-12);
  return body_.get();
}
QGraphicsRectItem *Character::getFeet() const
{
  feet_->setRect(x()+3, y()+height_-4, width_-6, 4);
  return feet_.get();
}
bool Character::isMovingVertical() const
{
  return movingVertical_;
}
int Character::getWidth() const
{
  return width_;
}
int Character::getHeight() const
{
  return height_;
}
bool Character::isMovingLeft() const
{
  return movingLeft_;
}
bool Character::isMovingRight() const
{
  return movingRight_;
}
