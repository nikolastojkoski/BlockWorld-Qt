#ifndef ZOMBIE_HPP
#define ZOMBIE_HPP

#include "character.hpp"

class Zombie : public Character
{
public:
  Zombie(int x, int y);
  int getLastCheckedX() const;
  void setLastCheckedX(int x);
private:
  int lastCheckedX_;
};


#endif // ZOMBIE_HPP
