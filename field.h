#ifndef FIELD_H
#define FIELD_H

#include <QGLWidget>
#include <QKeyEvent>
#include <QTimer>

namespace Ui { class Field; }

class Widget;

class Field : public QGLWidget
{
  Q_OBJECT

public:
  explicit Field(QGLWidget *parent = 0);
  ~Field();

  void setHeight(int);
  void setWidth(int);
  void setSpeed(int, short);

  void initializeGL();
  void resizeGL(int, int);
  void paintGL();
  void showEvent(QShowEvent *);
  void closeEvent(QCloseEvent *);
  void keyPressEvent(QKeyEvent *);

  void setWidget(Widget *);

private slots:
  void oneIteration();
  void updateField();

private:
  Ui::Field *_ui;
  Widget *_widget;

  QTimer *_timer;
  QTimer *_timer2;

  bool _isInitialized;
  bool _cyclePause;
  GLshort _width;
  GLshort _height;
  short *_speed; // speed of each reaction
  short *_f; // field: 0 - empty, 1 - victim, 2 - hunter
  short **_r; // reactions: 0 - no reaction in this cell, n - total summa
  long long int *_sums; // summas of speeds of each reaction type
  long long int _sum; // total summa of speeds
  GLdouble _color[3][3];

  int _getNeighbourIndex(int, int);
  void _setReactions(int);
  void _initialize();
  void _oneIteration();
  void _randomize();
//  void _paintCell(GLshort, GLshort);
  void _paintCells();
};

#endif // FIELD_H
