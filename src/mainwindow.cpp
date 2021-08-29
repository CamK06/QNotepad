#include <QCloseEvent>
#include <QMessageBox>
#include <QStandardPaths>
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>
#include <spdlog/spdlog.h>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Untitled - QNotepad");

    // Signal handling
    connect(ui->fileExit, &QAction::triggered, this, &MainWindow::exit);
    connect(ui->fileNew, &QAction::triggered, this, &MainWindow::newFile);
    connect(ui->fileOpen, &QAction::triggered, this, &MainWindow::openFile);
    connect(ui->fileSave, &QAction::triggered, this, &MainWindow::saveFile);
    connect(ui->fileSaveAs, &QAction::triggered, this, &MainWindow::saveAs);
    connect(ui->text, &QTextEdit::textChanged, this, &MainWindow::textUpdated);
    connect(ui->helpBugReport, &QAction::triggered, this, &MainWindow::reportBug);
    connect(ui->helpAbout, &QAction::triggered, this, &MainWindow::aboutDialog);

    spdlog::info("Initialized main window");
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
            saveFile();
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
    delete newTitle;
}

void MainWindow::textUpdated() 
{
    // Update the title if the save state changes
    if(saved) {
        saved = false;
        updateTitle(); // Only update the title when changing state
    }
}

// File functions

void MainWindow::openFile()
{
    // Use exit for the save prompt
    if(!exit(true))
        return;

    // Get the file from the user
    QString fileName = QFileDialog::getOpenFileName(this,
    tr("Open"), QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), tr("Text Files (*.txt *.text);;All Files (*)"));
    
    // This should only trigger if the user exits the dialog
    if(!std::filesystem::exists(fileName.toStdString())) {
        spdlog::error("Invalid or no file");
        return;
    }

    // Read the file into a string
    std::ifstream file(fileName.toStdString());
    std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    
    // Load the file to GUI
    this->fileName = std::filesystem::path(fileName.toStdString()).filename();
    this->filePath = fileName.toStdString();
    ui->text->clear();
    ui->text->setText(text.c_str());
    this->saved = true;
    updateTitle();
    file.close();
}

void MainWindow::newFile()
{
    // We use exit here simply for the save dialog
    if(exit(true)) {
        ui->text->clear();
        saved = true;
        this->fileName = "Untitled";
        this->filePath.clear();
        updateTitle();
        spdlog::info("Created new file");
    }
}

void MainWindow::saveFile()
{
    // If there is no file path, use a "save as" dialog to get one
    if(filePath.empty()) {
        filePath = saveAsDialog();
        if(filePath.empty())
            return;
    }

    // Write the file
    std::ofstream file(filePath);
    file << ui->text->toPlainText().toStdString();
    file.close();

    // Change GUI and internal file vars
    this->saved = true;
    this->fileName = std::filesystem::path(filePath).filename();
    this->filePath = filePath;
    this->updateTitle();
}

void MainWindow::saveAs()
{
    // Get a file path from the user
    this->filePath = saveAsDialog();
    if(filePath.empty())
        return;

    // Save the file; using the other function 'cause it's already there
    saveFile();
}

std::string MainWindow::saveAsDialog()
{
    // Get the location from the user
    QString fileName = QFileDialog::getSaveFileName(this,
    tr("Save As..."), QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), tr("Text Files (*.txt *.text);;All Files (*)"));

    // TODO: Add path verification

    return fileName.toStdString();
}

// Help functions

void MainWindow::reportBug() { QDesktopServices::openUrl(QUrl("https://github.com/Starman0620/QNotepad")); }

void MainWindow::aboutDialog(){ QMessageBox::about(this, "About QNotepad", "QNotepad 0.01\n\nWritten by Cam K.\nLicensed under the BSD 2-Clause license"); }