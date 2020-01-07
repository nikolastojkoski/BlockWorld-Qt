#include <QApplication>
#include "gameview.hpp"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  GameView view;
  view.show();

  return a.exec();
}
