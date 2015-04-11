#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <QApplication>

class Application : public QApplication
  {
    Q_OBJECT

  public:

    Application(int & argc, char ** argv);

  };

#endif