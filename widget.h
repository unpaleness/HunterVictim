#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui { class Widget; }

class Field;

class Widget : public QWidget
{
  Q_OBJECT

public:
  explicit Widget(QWidget *parent = 0);
  ~Widget();

  void setField(Field *);

private slots:
  void on_pushButtonStart_clicked();

private:
  Ui::Widget *_ui;
  Field *_field;
};

#endif // WIDGET_H
