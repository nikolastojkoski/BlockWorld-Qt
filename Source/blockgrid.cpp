#include "blockgrid.hpp"
#include <queue>
#include "block.hpp"

BlockGrid::BlockGrid(QGraphicsScene *scene):
  mainScene_(scene),
  width_(scene->width()),
  height_(scene->height()),
  dirtImg(QPixmap(":/images/dirtBlock.jpg")),
  stoneImg(QPixmap(":/images/stoneBlock.jpg"))
{
  maxVerticalBlocks_ = height_/Block::height;
  maxHorizontalBlocks_ = width_/Block::width;

  grid_.resize(maxHorizontalBlocks_);
  for(size_t i=0;i<grid_.size();i++)
  {
    grid_[i].resize(maxVerticalBlocks_);
    std::fill(grid_[i].begin(), grid_[i].end(), nullptr);
  }

}
void BlockGrid::generate()
{
  const size_t middleY = maxVerticalBlocks_ - maxVerticalBlocks_ / 3;

  bool risingRidge = true;

  int curHeight = middleY;
  int ridgeLength = 0;
  int finalRidgeLength = qrand() % 5;

  for(int i=0;i<maxHorizontalBlocks_; i++)
  {
    if(ridgeLength >= finalRidgeLength)
    {
      risingRidge = !risingRidge;
      ridgeLength = 0;
      finalRidgeLength = qrand() % 5;
    }

    if(qrand() % 10 == 0 && std::abs(curHeight - middleY) < 10)
    {
      finalRidgeLength += qrand() % 5;
    }

    if(curHeight > maxVerticalBlocks_ - (maxVerticalBlocks_ / 5))
    {
      curHeight -= 4;
      risingRidge = true;
      ridgeLength = 0;
      finalRidgeLength = qrand() % 10;
    }
    else if(curHeight < maxVerticalBlocks_ / 3)
    {
      curHeight += 4;
      risingRidge = false;
      ridgeLength = 0;
      finalRidgeLength = qrand() % 10;
    }
    ridgeLength++;

    if(risingRidge)
    {
      curHeight = curHeight - std::abs(qrand() % 2 - qrand() % 2 + qrand() % 2 - qrand() % 2);

    }
    else
    {
      curHeight = curHeight + std::abs(qrand() % 2 - qrand() % 2 + qrand() % 2 - qrand() % 2);
    }


    for(int j=curHeight;j<maxVerticalBlocks_;j++)
    {
      placeBlock(i,j,Block::BlockType::dirt, true);
    }
  }

  /* BLOCK COLORING */

  std::vector<std::vector<bool> > visited;
  visited.resize(maxHorizontalBlocks_);
  for(auto it = visited.begin(); it!=visited.end(); it++)
  {
    (*it).resize(maxVerticalBlocks_);
    std::fill((*it).begin(), (*it).end(), false);
  }

  struct QueueNode
  {
    int x, y, radius;
  };

  std::queue<QueueNode> q;
  for(int i=0; i<maxHorizontalBlocks_; i += 20)
  {
    for(int j = maxVerticalBlocks_/3; j<maxVerticalBlocks_; j+= 20)
    {
      if(grid_[i][j] != nullptr)
      {
        q.push(QueueNode{i,j,qrand()%7});
      }
    }
  }

  while(!q.empty())
  {
    QueueNode cur = q.front();
    q.pop();

    if(inBounds(cur.x, cur.y) && cur.radius > 0 && !visited[cur.x][cur.y])
    {
      visited[cur.x][cur.y] = true;

      if(grid_[cur.x][cur.y] != nullptr)
      {
        grid_[cur.x][cur.y]->setType(Block::BlockType::stone);
        grid_[cur.x][cur.y]->setImage(stoneImg);
      }

      q.push(QueueNode{cur.x+1, cur.y, cur.radius-qrand()%3});
      q.push(QueueNode{cur.x-1, cur.y, cur.radius-qrand()%3});
      q.push(QueueNode{cur.x, cur.y+1, cur.radius-qrand()%3});
      q.push(QueueNode{cur.x, cur.y-1, cur.radius-qrand()%3});
    }
  }

}

bool BlockGrid::collidesWith(QGraphicsItem *item)
{
  QRectF rect = item->boundingRect();

  if(!inBounds(toGridX(rect.x()), toGridY(rect.y())))
  {
    return false;
  }
  if(!inBounds(toGridX(rect.x() + rect.width()), toGridY(rect.y() + rect.height())))
  {
    return false;
  }

  for(size_t x=toGridX(rect.x()); x <= toGridX(rect.x() + rect.width()); x++)
  {
    for(size_t y=toGridY(rect.y()); y <= toGridY(rect.y() + rect.height()); y++)
    {
      if(grid_[x][y] != nullptr && grid_[x][y]->collidesWithItem(item))
      {
        return true;
      }
    }
  }
  return false;
}

QGraphicsRectItem *BlockGrid::getDummyBlock(int x, int y)
{
  return new QGraphicsRectItem((x/Block::width)*Block::width, (y/Block::height)*Block::height,
                               Block::width, Block::height);
}

bool BlockGrid::placeBlock(int x, int y, const Block::BlockType &type, bool mappedToGrid)
{
  if(!mappedToGrid)
  {
    x = toGridX(x);
    y = toGridY(y);
  }

  if(x >= maxHorizontalBlocks_ || y >= maxVerticalBlocks_)
  {
    return false;
  }

  if(grid_[x][y] != nullptr)
  {
    return false;
  }

  Block *block;
  switch(type)
  {
  case Block::BlockType::dirt:
    block = new Block(x*Block::width, y*Block::height, type, dirtImg);
    break;
  case Block::BlockType::stone:
    block = new Block(x*Block::width, y*Block::height, type, stoneImg);
    break;
  }

  grid_[x][y] = block;

  mainScene_->addItem(block);

  return true;
}

bool BlockGrid::removeBlock(int x, int y, bool mappedToGrid)
{
  if(!mappedToGrid)
  {
    x = toGridX(x);
    y = toGridY(y);
  }

  if(!inBounds(x, y))
  {
    return false;
  }

  Block *(&blockRef) = grid_[x][y];
  if(blockRef != nullptr)
  {
    mainScene_->removeItem(blockRef);
    delete blockRef;
    blockRef = nullptr;
    return true;
  }
  else
  {
    return false;
  }
}
int BlockGrid::toGridX(int x)
{
  return static_cast<size_t>(x/Block::width);
}
int BlockGrid::toGridY(int y)
{
  return static_cast<size_t>(y/Block::height);
}

bool BlockGrid::inBounds(int x, int y) const
{
  if(x >= maxHorizontalBlocks_ || y >= maxVerticalBlocks_ || x<0 || y < 0)
  {
    return false;
  }
  else
  {
    return true;
  }
}
