#ifndef LIBREGRID_H
#define LIBREGRID_H

#include <QMainWindow>

namespace Ui {
class libreGrid;
}

class libreGrid : public QMainWindow
{
    Q_OBJECT

public:
    explicit libreGrid(QWidget *parent = 0);
    ~libreGrid();

private:
    Ui::libreGrid *ui;
};

#endif // LIBREGRID_H
