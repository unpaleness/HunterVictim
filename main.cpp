#include "widget.h"
#include "field.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  Widget w;
  Field f;
  w.setField(&f);
  f.setWidget(&w);
  w.show();

  return a.exec();
}
