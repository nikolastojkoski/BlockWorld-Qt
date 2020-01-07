#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <QPixmap>
#include <QTimer>
#include "character.hpp"

class Player: public Character
{
  Q_OBJECT
public:
  Player(int x, int y);
  void punch();
public slots:
  void animationTick() override;
private:
  QPixmap punchRightImg_, punchLeftImg_;
  bool punching_, punchingLeft_;
};

#endif // PLAYER_HPP
