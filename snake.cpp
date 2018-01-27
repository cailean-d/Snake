#include <QPainter>
#include <QTime>
#include "snake.h"


Snake::Snake(QWidget *parent) : QWidget(parent) {

    setStyleSheet("QWidget {background-image: url(:/img/floor.jpg) }");
    leftDirection = false;
    rightDirection = true;
    upDirection = false;
    downDirection = false;
    inGame = true;

    resize(B_WIDTH, B_HEIGHT);
    setFixedSize(B_WIDTH,B_HEIGHT);
    loadImages();
    initGame();
}

void Snake::loadImages() {   //загрузка текстур

    QImage img1;    img1.load(":/img/g_dot.png");
    QImage img2;    img2.load(":/img/head.png");
    QImage img3;    img3.load(":/img/apple_1.png");
    QImage img4;    img4.load(":/img/b_wall.png");

    //уменьшение
    dot = img1.scaled(DOT_SIZE,DOT_SIZE);
    head = img2.scaled(DOT_SIZE,DOT_SIZE);
    apple = img3.scaled(DOT_SIZE,DOT_SIZE);
    wall = img4.scaled(DOT_SIZE, DOT_SIZE);
}

void Snake::initGame() {

    dots = 3;

    for (int z = 0; z < dots; z++) {
        x[z] = (DOT_SIZE*1) - z * DOT_SIZE;
        y[z] = (DOT_SIZE*1);
    }

    locateApple();

    timerId = startTimer(DELAY);
}

void Snake::paintEvent(QPaintEvent *e) {

    Q_UNUSED(e);

    doDrawing();
    DrawingWalls();
    DrawingScore();
}

void Snake::doDrawing() {

    QPainter qp(this);

    if (inGame) {

        qp.drawImage(apple_x, apple_y, apple);

        for (int z = 0; z < dots; z++) {
            if (z == 0) {
                qp.drawImage(x[z], y[z], head);
            } else {
                qp.drawImage(x[z], y[z], dot);
            }
        }
    } else {

        gameOver(qp);
    }
}

void Snake::gameOver(QPainter &qp) {
    QString sc = QString::number(SCORE);
    QString message("Game over");
    QString message2("Your Score: "+sc);

    QFont font("Courier", 40, QFont::DemiBold);
    QFontMetrics fm(font);
    int textWidth = fm.width(message);
    qp.setFont(font);
    qp.setPen(Qt::green);
    int h = height();
    int w = width();

    qp.translate(QPoint(w/2, h/2));
    qp.drawText(-textWidth/2, -20, message);
    qp.drawText(-textWidth/2-60, 20, message2);

}

void Snake::checkApple() {    //сбор яблок

   if ((x[0] == apple_x) && (y[0] == apple_y)) {
        dots++;
        //увеличение получаемых очков
        if (dots <=7)
            SCORE+=2;
        else if (dots > 7 && dots <=12)
        {SCORE+=5;}
        else if (dots > 12 && dots <=20)
        {SCORE+=10;}
        else if (dots > 20 && dots <=27)
        {SCORE+=20;}
        else if (dots > 27)
        {SCORE+=40;}
  //=============================================
           //увеличении скорости
            if (dots ==7)
            { timerId = startTimer(250);}
            else if (dots ==12)
            {timerId = startTimer(250);}
            else if (dots ==20)
            {timerId = startTimer(250);}
            else if (dots ==27)
            { timerId = startTimer(250);}
            else if (dots ==35)
            { timerId = startTimer(250);}
        locateApple();
    }
}

void Snake::move() {   //движение змеи

    for (int z = dots; z > 0; z--) {
        x[z] = x[(z - 1)];
        y[z] = y[(z - 1)];
    }

    if (leftDirection) {
        x[0] -= DOT_SIZE;
    }

    if (rightDirection) {
        x[0] += DOT_SIZE;
    }

    if (upDirection) {
        y[0] -= DOT_SIZE;
    }

    if (downDirection) {
        y[0] += DOT_SIZE;
    }
}

void Snake::checkCollision() {
     //столкнованиа с хвостом
    for (int z = dots; z > 0; z--) {
        if ((z > 4) && (x[0] == x[z]) && (y[0] == y[z])) {
            inGame = false;
        }
    }
    //столкновение со стенами
    for (int i = 0; i < 30; i++){
        for (int j = 0; j < 30; j++){
            if (wall_xy[i][j] == 1){
             if ((x[0] == j*DOT_SIZE) && (y[0]==i*DOT_SIZE)) { inGame = false;}
            }
        }
    }
    //выход за рамки поля
    if (y[0] >= B_HEIGHT) {
        inGame = false;
    }

    if (y[0] < 0) {
        inGame = false;
    }

    if (x[0] >= B_WIDTH) {
        inGame = false;
    }

    if (x[0] < 0) {
        inGame = false;
    }
    // в этом случае остановить игру
    if(!inGame) {
        killTimer(timerId);
    }
}

void Snake::locateApple() {   //размещение яблок

    QTime time = QTime::currentTime();
    qsrand((uint) time.msec());

    int r = qrand() % RAND_POS;
    apple_x = (r * DOT_SIZE);

    r = qrand() % RAND_POS;
    apple_y = (r * DOT_SIZE);
}

void Snake::timerEvent(QTimerEvent *e) {   //рисование каждый кадр

    Q_UNUSED(e);

    if (inGame) {

        checkApple();
        checkCollision();
        move();
    }

    repaint();
}

void Snake::keyPressEvent(QKeyEvent *e) {   //события нажатия клавиш

    int key = e->key();

    if (((key == Qt::Key_Left) && (!rightDirection)) || ((key == Qt::Key_A) && (!rightDirection))) {
        leftDirection = true;
        upDirection = false;
        downDirection = false;
    }

    if(((key == Qt::Key_Right) && (!leftDirection)) || ((key == Qt::Key_D) && (!leftDirection))) {
        rightDirection = true;
        upDirection = false;
        downDirection = false;
    }

    if (((key == Qt::Key_Up) && (!downDirection))  ||  ((key == Qt::Key_W) && (!downDirection))) {
        upDirection = true;
        rightDirection = false;
        leftDirection = false;
    }

    if (((key == Qt::Key_Down) && (!upDirection))  || ((key == Qt::Key_S) && (!upDirection))){
        downDirection = true;
        rightDirection = false;
        leftDirection = false;
    }

    QWidget::keyPressEvent(e);
}


void Snake::DrawingWalls() {  //рисование препядствий
   QPainter qp(this);

  if (inGame){
    for (int i = 0; i < 30; i++){
        for (int j = 0; j < 30; j++){
            if (wall_xy[i][j] == 1){
            qp.drawImage(DOT_SIZE*j,DOT_SIZE*i, wall);
            }
        }
    }
  }
else {gameOver(qp);}
}


void Snake::DrawingScore() {  //отображение счета

    QPainter qp(this);

    if (inGame) {
      QString sc = QString::number(SCORE);  //перевод числа в строку
      QString dt = QString::number(dots);
      qp.setPen(Qt::green);  // цвет
      qp.drawText(10, 20, "Score: "+ sc);  //рисование переменных
      qp.drawText(10, 30, "Dots: "+ dt);

    } else {
       gameOver(qp);
    }
}

void Snake::Restart(){
     killTimer(timerId);
    setStyleSheet("QWidget {background-image: url(:/img/floor.jpg) }");
    leftDirection = false;
    rightDirection = true;
    upDirection = false;
    downDirection = false;
    inGame = true;

    resize(B_WIDTH, B_HEIGHT);
    setFixedSize(B_WIDTH,B_HEIGHT);
    loadImages();
    initGame();
}


