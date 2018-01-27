#include <QApplication>
#include <QtWidgets>
#include <QtGui>
#include "snake.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QPixmap icon(":/img/snake.png");
   app.setWindowIcon(QIcon(icon));
   QMenuBar mnuBar;
 // QWidget mainWindow;
//  QLabel ooo("Score");
  QHBoxLayout mainLayout;
 // QVBoxLayout sidebox;
  Snake window;
 // ooo.setFixedWidth(50);
 mainLayout.addWidget(&window);
 // mainLayout.addWidget(&ooo);
  mnuBar.setLayout(&mainLayout);


  mainLayout.setContentsMargins(1,20,1,10);




      QMenu*   pmnu   = new QMenu("&Menu");


      pmnu->addSeparator();
      pmnu->addAction("&Exit", &app, SLOT(quit()));

      mnuBar.addMenu(pmnu);
   //   mnuBar.show();



 // window.setWindowTitle("Snake");
  // window.show();
  window.setFocus();
  mnuBar.setStyleSheet("background-color:grey;");
  mnuBar.setFixedSize(window.width() + 25 ,window.height() + 25);
  mnuBar.setWindowTitle("Snake");
  mnuBar.show();



  return app.exec();
}
