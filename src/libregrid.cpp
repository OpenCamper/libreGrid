#include "libregrid.h"
#include "ui_libregrid.h"

libreGrid::libreGrid(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::libreGrid)
{
    ui->setupUi(this);
}

libreGrid::~libreGrid()
{
    delete ui;
}
