#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QPointF>
#include <QTimer>
#include <memory>

class Character: public QObject, public QGraphicsPixmapItem
{
  Q_OBJECT
public:
  Character(int speedX, int speedY, int width,
            int height, qreal prevX, qreal prevY,
            QPixmap idleRightImg, QPixmap idleLeftImg,
            QPixmap walkRight1Img, QPixmap walkRight2Img,
            QPixmap walkLeft1Img, QPixmap walkLeft2Img);

  virtual ~Character() = default;

  void startMovingLeft();
  void startMovingRight();
  void stopMovingLeft();
  void stopMovingRight();
  void jump();
  void fall();

  void stopJump();
  void gotoPrevXPos();
  void gotoPrevYPos();
  void moveBy(qreal dx, qreal dy);
  void setPos(qreal x, qreal y);
  void setPos(const QPointF &pos);
  void update();

  bool isMovingVertical() const;
  bool isMovingLeft() const;
  bool isMovingRight() const;

  int getHeight() const;
  int getWidth() const;

  QGraphicsRectItem *getHead() const;
  QGraphicsRectItem *getBody() const;
  QGraphicsRectItem *getFeet() const;

public slots:
  void tick();
  virtual void animationTick();
signals:
  void moved(Character*);

protected:
  const int speedX_, speedY_;
  const int width_, height_;
  qreal prevX_, prevY_;

  double vx_, vy_, v0y_, ty_, y0_;
  bool movingVertical_, movingRight_, movingLeft_;

  std::unique_ptr<QGraphicsRectItem> head_;
  std::unique_ptr<QGraphicsRectItem> body_;
  std::unique_ptr<QGraphicsRectItem> feet_;

  enum AnimationState
  {
    idleLeft,
    idleRight,
    walkRight1,
    walkRight2,
    walkLeft1,
    walkLeft2,
  };

  const QPixmap idleRightImg_, idleLeftImg_;
  const QPixmap walkRight1Img_, walkRight2Img_;
  const QPixmap walkLeft1Img_, walkLeft2Img_;

  AnimationState state_;

  std::unique_ptr<QTimer> movementTimer_;

};


#endif // CHARACTER_HPP
