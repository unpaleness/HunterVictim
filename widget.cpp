#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), _ui(new Ui::Widget)
{
  _ui->setupUi(this);
}

Widget::~Widget()
{
  delete _ui;
}
