#include "widget.h"
#include "field.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), _ui(new Ui::Widget)
{
  _ui->setupUi(this);
}

Widget::~Widget()
{
  delete _ui;
}

void Widget::setField(Field *f) { _field = f; }

void Widget::on_pushButtonStart_clicked()
{
  this->setDisabled(true);
  _field->setHeight(_ui->lineEditY->text().toShort());
  _field->setWidth(_ui->lineEditX->text().toShort());
  _field->setSpeed(0, _ui->lineEdit1->text().toShort());
  _field->setSpeed(1, _ui->lineEdit2->text().toShort());
  _field->setSpeed(2, _ui->lineEdit3->text().toShort());
  _field->setSpeed(3, _ui->lineEdit4->text().toShort());
  _field->setSpeed(4, _ui->lineEdit5->text().toShort());
  _field->show();
}
