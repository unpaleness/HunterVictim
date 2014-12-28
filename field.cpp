#include <ctime>
#include <cstdlib>
#include <fstream>

#include "field.h"
#include "widget.h"
#include "ui_field.h"

Field::Field(QGLWidget *parent) : QGLWidget(parent), _ui(new Ui::Field)
{
  srand(time(0));
  _isInitialized = false;
  _cyclePause = true;
  _color[0][0] = 1.0;
  _color[0][1] = 1.0;
  _color[0][2] = 1.0;
  _color[1][0] = 0.0;
  _color[1][1] = 0.0;
  _color[1][2] = 1.0;
  _color[2][0] = 1.0;
  _color[2][1] = 0.0;
  _color[2][2] = 0.0;
  _speed = new short [5];
  _r = new short *[5];
  _sums = new long long int [5];
  _ui->setupUi(this);

  _timer = new QTimer(this);
  connect(_timer, SIGNAL(timeout()), this, SLOT(oneIteration()));
  _timer->start(0.001);
  _timer2 = new QTimer(this);
  connect(_timer2, SIGNAL(timeout()), this, SLOT(updateField()));
  _timer2->start(20);
}

Field::~Field()
{
  delete _timer;
  delete _timer2;
  if(_isInitialized)
  {
    for(int i = 0; i < 5; i++)
      delete [] _r[i];
    delete [] _f;
  }
  delete [] _speed;
  delete [] _r;
  delete [] _sums;
  delete _ui;
}

void Field::setHeight(int height) { _height = height; }
void Field::setWidth(int width) { _width = width; }
void Field::setSpeed(int index, short value) { _speed[index] = value; }

void Field::initializeGL()
{
  qglClearColor(Qt::black);
}

void Field::resizeGL(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, _width, _height, 0, 1, 0);
  if(_isInitialized)
    _paintCells();
}

void Field::paintGL()
{
//  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//  for(GLshort j = 0; j < _height; j++)
//    for(GLshort i = 0; i < _height; i++)
//      _paintCell(j, i);
//  _paintCells();
}

void Field::showEvent(QShowEvent *)
{
  if(_isInitialized)
  {
    delete [] _f;
    for(int i = 0; i < 5; i++)
      delete [] _r[i];
    _isInitialized = true;
  }
  _f = new short [_height * _width];
  for(int i = 0; i < 5; i++)
    _r[i] = new short [_height * _width];
  _randomize();
  _initialize();
}

void Field::closeEvent(QCloseEvent *)
{
  _cyclePause = true;
  _widget->setEnabled(true);
}

void Field::keyPressEvent(QKeyEvent *e)
{
  if(e->key() == Qt::Key_P)
  {
    _cyclePause = !_cyclePause;
  }
}

void Field::setWidget(Widget *w) { _widget = w; }

/*
 * PRIVATE SLOTS
 */

void Field::oneIteration()
{
  if(!_cyclePause)
  {
//    _randomize();
    _oneIteration();
//    _paintCells();
//    updateGL();
  }
}

void Field::updateField()
{
  if(!_cyclePause)
  {
    _paintCells();
    updateGL();
  }
}

/*
 * PRIVATE
 */

int Field::_getNeighbourIndex(int dir, int index)
{
  switch(dir)
  {
    case 0: // top
      if(index / _width == 0) return (_height - 1) * _width + index;
      else return index - _width;
    case 1: // right
      if(index % _width == _width - 1) return index - _width + 1;
      else return index + 1;
    case 2: // buttom
      if(index / _width == _height - 1) return index % _width;
      else return index + _width;
    case 3: // left
      if(index % _width == 0) return index + _width - 1;
      else return index - 1;
  }
  return 0;
}

void Field::_setReactions(int index)
{
  short amountNeighbours[3];
  for(int k = 0; k < 3; k++)
    amountNeighbours[k] = 0;
  for(int k = 0; k < 5; k++)
    _r[k][index] = 0;
  for(int k = 0; k < 4; k++)
    amountNeighbours[_f[_getNeighbourIndex(k, index)]]++;
  switch(_f[index])
  {
    case 0: // if cell is empty
      // birth of victim
      _r[0][index] = amountNeighbours[1] * _speed[0];
      break;
    case 1: // if cell contains victim
      // death of victim
      _r[1][index] = _speed[1];
      // death of victim because of hunter
      _r[2][index] = amountNeighbours[2] * _speed[2];
      // birth of hunter because of victim
      _r[3][index] = amountNeighbours[2] * _speed[3];
      break;
    case 2: // if cell contains hunter
      // death of hunter
      _r[4][index] = _speed[4];
      break;
  }
  for(int k = 0; k < 5; k++)
    _sums[k] += _r[k][index];
}

void Field::_initialize()
{
  for(int i = 0; i < 5; i++)
    _sums[i] = 0;
  _sum = 0;
  for(int j = 0; j < _height; j++)
    for(int i = 0; i < _width; i++)
      _setReactions(j * _width + i);
  for(int i = 0; i < 5; i++)
    _sum += _sums[i];
}

void Field::_oneIteration()
{
  // drop a random value
  long long int ksi = rand() % _sum;
  // locate reaction
  long long int temp = 0;
  int reactionType;
  int cellIndex;
  for(reactionType = 0; reactionType < 5; reactionType++)
  {
    if(temp + _sums[reactionType] >= ksi)
      break;
    temp += _sums[reactionType];
  }
  for(cellIndex = 0; cellIndex < _height * _width; cellIndex++)
  {
    if(temp + _r[reactionType][cellIndex] >= ksi)
      break;
    temp += _r[reactionType][cellIndex];
  }
  // remove previous reactions
  for(int k = 0; k < 4; k++)
  {
    int neighbourIndex = _getNeighbourIndex(k, cellIndex);
    for(int i = 0; i < 5; i++)
      _sums[i] -= _r[i][neighbourIndex];
  }
  for(int i = 0; i < 5; i++)
    _sums[i] -= _r[i][cellIndex];
  // set current cell
  switch(reactionType)
  {
    case 0:
      _f[cellIndex] = 1;
      break;
    case 1:
      _f[cellIndex] = 0;
      break;
    case 2:
      _f[cellIndex] = 0;
      break;
    case 3:
      _f[cellIndex] = 2;
      break;
    case 4:
      _f[cellIndex] = 0;
      break;
  }
  // set new reactions
  _setReactions(cellIndex);
  for(int i = 0; i < 4; i++)
    _setReactions(_getNeighbourIndex(i, cellIndex));
  _sum = 0;
  for(int i = 0; i < 5; i++)
    _sum += _sums[i];
}

void Field::_randomize()
{
  for(int j = 0; j < _height; j++)
    for(int i = 0; i < _width; i++)
      _f[j * _width + i] = rand() % 3;
}

//void Field::_paintCell(GLshort y, GLshort x)
//{
//  glColor3dv(_color[_f[y * _height + x]]);
//  glBegin(GL_QUADS);
//  glVertex2s(x, y);
//  glVertex2s(x, y + 1);
//  glVertex2s(x + 1, y + 1);
//  glVertex2s(x + 1, y);
//  glEnd();
//}

void Field::_paintCells()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBegin(GL_QUADS);
  for(GLshort j = 0; j < _height; j++)
    for(GLshort i = 0; i < _width; i++)
    {
      glColor3dv(_color[_f[j * _width + i]]);
      glVertex2s(i, j);
      glVertex2s(i, j + 1);
      glVertex2s(i + 1, j + 1);
      glVertex2s(i + 1, j);
    }
  glEnd();
}
