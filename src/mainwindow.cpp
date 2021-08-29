#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Buttons
    connect(ui->fileExit, &QAction::triggered, this, &MainWindow::exit);
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::exit()
{
    std::exit(0);
}