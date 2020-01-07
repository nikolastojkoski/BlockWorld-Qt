#ifndef BLOCKGRID_HPP
#define BLOCKGRID_HPP

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QRectF>
#include <QPixmap>
#include <memory>
#include "block.hpp"

class BlockGrid: public QObject
{
  Q_OBJECT
public:
  BlockGrid(QGraphicsScene *scene);
  virtual ~BlockGrid() = default;

  bool placeBlock(int x, int y, const Block::BlockType &type, bool mappedToGrid = false);
  bool removeBlock(int x, int y, bool mappedToGrid = false);
  void generate();

  bool collidesWith(QGraphicsItem *item);

  static QGraphicsRectItem *getDummyBlock(int x, int y);

private:
  QGraphicsScene *mainScene_;
  std::vector<std::vector<Block*>> grid_;

  int width_, height_;
  size_t maxVerticalBlocks_, maxHorizontalBlocks_;

  QPixmap dirtImg, stoneImg;

  static int toGridX(int x);
  static int toGridY(int y);
  bool inBounds(int x, int y) const;
};

#endif // BLOCKGRID_HPP
