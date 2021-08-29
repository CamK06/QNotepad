#include <QCloseEvent>
#include <QMessageBox>
#include <spdlog/spdlog.h>
#include <cstring>

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "dialogs.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    spdlog::info("Initializing window...");
    ui->setupUi(this);
    this->setWindowTitle("Untitled - QNotepad");

    // Signal handling
    connect(ui->fileExit, &QAction::triggered, this, &MainWindow::exit);
    connect(ui->text, &QTextEdit::textChanged, this, &MainWindow::textUpdated); // Not sure why this needs to be different to the others, but it does
}

bool MainWindow::exit(bool now = false) 
{ 
    if(!saved) {
        // Create a messagebox
        char* questionText = new char[63+fileName.length()];
        sprintf(questionText, "File '%s' has been modified.\n\nWould you like to save the changes?", fileName.c_str());
        auto answer = QMessageBox::question(this, "QNotepad", questionText, 
                                            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        delete questionText;

        // Handle the output of the messagebox
        if(answer == QMessageBox::Yes) {
            // TODO: Add saving logic here
            return true;
        }
        else if(answer == QMessageBox::No)
            return true;
        else if(answer == QMessageBox::Cancel)
            return false;
        else 
            return false;
    }

    // If we aren't exiting *now* (just using the function for dialogs)
    if(!now) {
        spdlog::info("Shutting down...");
        std::exit(0);
    }

    return true; // Just to make the compiler happy
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // Call exit() to get save dialogs if needed
    if(exit(true)) {
        spdlog::info("Shutting down...");
        event->accept();
    }
    else
        event->ignore();
}

void MainWindow::updateTitle()
{
    // Create a new title string based on the save state
    char* newTitle = new char[10+fileName.length()+ !saved]; // This is a bit of a hacky way to determine length
    if(!saved) 
        sprintf(newTitle, "*%s - QNotepad", fileName.c_str());
    else
        sprintf(newTitle, "%s - QNotepad", fileName.c_str());

    // Set the title
    this->setWindowTitle(newTitle);
    oldTitle = newTitle;
    delete newTitle;
}

void MainWindow::textUpdated() 
{
    if(saved) {
        saved = false;
        updateTitle(); // Only update the title when changing state
    }
}