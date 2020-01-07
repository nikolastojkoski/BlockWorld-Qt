#include "block.hpp"
#include <QBrush>

Block::Block(int x, int y, const BlockType &type, const QPixmap &img, QGraphicsItem *parent):
  QGraphicsPixmapItem(parent)
{
  type_ = type;
  setPixmap(img);
  setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
  setPos(QPointF(x,y));
}
void Block::setType(const Block::BlockType &type)
{
  type_ = type;
}
void Block::setImage(const QPixmap &img)
{
  setPixmap(img);
}
