#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <QGraphicsRectItem>
#include <QGraphicsItem>

class Block: public QGraphicsPixmapItem
{
public:

  enum BlockType
  {
    dirt,
    stone
  };

  Block(int x, int y, const BlockType &type,
        const QPixmap &img, QGraphicsItem *parent = 0);

  void setType(const BlockType &type);
  void setImage(const QPixmap &img);
  virtual ~Block() = default;
  static const int height = 40;
  static const int width = 40;

private:
  BlockType type_;
};


#endif // BLOCK_HPP
