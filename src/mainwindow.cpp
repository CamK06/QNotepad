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
#include <fmt/core.h>

#include "mainwindow.h"
#include "version.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    updateTitle();

    // Signal handling
    connect(ui->fileExit, &QAction::triggered, this, &MainWindow::fileExit);
    connect(ui->fileNew, &QAction::triggered, this, &MainWindow::newFile);
    connect(ui->fileOpen, &QAction::triggered, this, &MainWindow::openFile);
    connect(ui->fileSave, &QAction::triggered, this, &MainWindow::saveFile);
    connect(ui->fileSaveAs, &QAction::triggered, this, &MainWindow::saveAs);
    connect(ui->text, &QTextEdit::textChanged, this, &MainWindow::textUpdated);
    connect(ui->helpBugReport, &QAction::triggered, this, &MainWindow::reportBug);
    connect(ui->helpAbout, &QAction::triggered, this, &MainWindow::aboutDialog);

    spdlog::info("Initialized main window");
}

bool MainWindow::exit(bool fullExit = true) 
{ 
    if(!saved) {
        // Create a messagebox
        std::string questionText = fmt::format("File '{}' has been modified.\n\nWould you like to save the changes?", fileName);
        auto answer = QMessageBox::question(this, "QNotepad", questionText.c_str(), 
                                            QMessageBox::Save | QMessageBox::StandardButton::Discard | QMessageBox::Cancel);

        // Handle the output of the messagebox
        if(answer == QMessageBox::Save) {
            saveFile();
            return true;
        }
        else if(answer == QMessageBox::Discard && !fullExit)
            return true;
        else if(answer == QMessageBox::Cancel && !fullExit)
            return false;
        else if(!fullExit)
            return false;
    }

    // If we are exiting the whole program
    if(fullExit) {
        spdlog::info("Shutting down...");
        std::exit(0);
    }

    return true; // Just to make the compiler happy
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // Call exit() to get save dialogs if needed
    if(exit(false)) {
        spdlog::info("Shutting down...");
        event->accept();
    }
    else
        event->ignore();
}

void MainWindow::updateTitle()
{
    // Set a new title string based on the save state
    std::string newTitle = fmt::format("{}{} - QNotepad {}", saved ? "" : "*", fileName.c_str(), VERSION);
    this->setWindowTitle(newTitle.c_str());
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
    if(!exit(false))
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
    if(exit(false)) {
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

void MainWindow::aboutDialog() 
{
    std::string text = fmt::format("QNotepad {}\n\nWritten by Cam K.\nLicensed under the BSD 2-Clause license", VERSION);
    QMessageBox::about(this, "About QNotepad", text.c_str());
}