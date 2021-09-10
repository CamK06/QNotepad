#include <QCloseEvent>
#include <QMessageBox>
#include <QScreen>
#include <QStandardPaths>
#include <QFileDialog>
#include <QFontDialog>
#include <QDesktopServices>
#include <QGuiApplication>
#include <QUrl>
#include <spdlog/spdlog.h>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <fmt/core.h>

#include "mainwindow.h"
#include "version.h"
#include "editor.h"
#include "search.h"
#include "./ui_mainwindow.h"
#include "./ui_searchdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    spdlog::info(PROGRAM " " VERSION);
    ui->setupUi(this);
    ui->helpAbout->setText(fmt::format("About {}", PROGRAM).c_str());
    move(pos() + (QGuiApplication::primaryScreen()->geometry().center() - geometry().center()));
    updateTitle();

    // StatusBar
    this->statusBarLabel.setAlignment(Qt::AlignRight);
    cursorMoved();
    ui->statusBar->addWidget(&statusBarLabel, 1);

    // Signal handling
    connect(ui->fileExit, &QAction::triggered, this, &MainWindow::fileExit);
    connect(ui->fileNew, &QAction::triggered, this, &MainWindow::newFile);
    connect(ui->fileOpen, &QAction::triggered, this, &MainWindow::openFile);
    connect(ui->fileSave, &QAction::triggered, this, &MainWindow::saveFile);
    connect(ui->fileSaveAs, &QAction::triggered, this, &MainWindow::saveAs);
    connect(ui->text, &QTextEdit::textChanged, this, &MainWindow::textUpdated);
    connect(ui->helpBugReport, &QAction::triggered, this, &MainWindow::reportBug);
    connect(ui->helpAbout, &QAction::triggered, this, &MainWindow::aboutDialog);
    connect(ui->editUndo, &QAction::triggered, this, &MainWindow::undo);
    connect(ui->editRedo, &QAction::triggered, this, &MainWindow::redo);
    connect(ui->editCut, &QAction::triggered, this, &MainWindow::cut);
    connect(ui->editCopy, &QAction::triggered, this, &MainWindow::copy);
    connect(ui->editPaste, &QAction::triggered, this, &MainWindow::paste);
    connect(ui->formatWordWrap, &QAction::triggered, this, &MainWindow::wordWrap);
    connect(ui->viewStatusBar, &QAction::triggered, this, &MainWindow::statusBar);
    connect(ui->text, &QTextEdit::cursorPositionChanged, this, &MainWindow::cursorMoved);
    connect(ui->formatFont, &QAction::triggered, this, &MainWindow::selectFont);
    connect(ui->editFind, &QAction::triggered, this, &MainWindow::search);

    spdlog::info("Initialized main window");
}

void MainWindow::withFile(std::string fileName)
{
    // Load the file if it exists
    if(std::filesystem::exists(fileName))
        loadFile(fileName);
    // Otherwise make a new file
    else {
        this->fileName = fileName;
        this->filePath = fileName;
        updateTitle();
    }
}

// General functions

bool MainWindow::exit(bool fullExit = true) 
{ 
    if(!saved) {
        // Create a messagebox
        auto answer = QMessageBox::question(this, PROGRAM, fmt::format("File '{}' has been modified.\n\nWould you like to save the changes?", fileName).c_str(), 
                                            QMessageBox::Save | QMessageBox::StandardButton::Discard | QMessageBox::Cancel);

        // Handle the output of the messagebox
        if(answer == QMessageBox::Save) {
            saveFile();
            if(!fullExit)
                return true;
        }
        else if(answer == QMessageBox::Discard && !fullExit)
            return true;
        else if(answer == QMessageBox::Cancel)
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

void MainWindow::textUpdated() 
{
    // Update the title if the save state changes
    if(saved) {
        saved = false;
        updateTitle(); // Only update the title when changing state
    }
}

void MainWindow::cursorMoved()
{
    // Update the statusbar
    int row = ui->text->textCursor().blockNumber()+1;
    int col = ui->text->textCursor().positionInBlock()+1;
    this->statusBarLabel.setText(fmt::format("Ln {}, Col {}", row, col).c_str());
}

void MainWindow::updateTitle() { this->setWindowTitle(fmt::format("{}{} - {}", saved ? "" : "*", fileName.c_str(), PROGRAM).c_str()); }

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

    // Load the file
    loadFile(fileName.toStdString());
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
    this->fileName = std::filesystem::path(filePath).filename().u8string();
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

void MainWindow::loadFile(std::string fileName)
{
    // Read the file into a string
    std::ifstream file(fileName);
    std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    
    // Load the file to GUI
    this->fileName = std::filesystem::path(fileName).filename().u8string();
    this->filePath = fileName;
    ui->text->setText(text.c_str());
    ui->text->moveCursor(QTextCursor::MoveOperation::End);
    this->saved = true;
    updateTitle();
    file.close();
}

std::string MainWindow::saveAsDialog()
{
    // Get the location from the user
    QString fileName = QFileDialog::getSaveFileName(this,
    tr("Save As..."), QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), tr("Text Files (*.txt *.text);;All Files (*)"));

    return fileName.toStdString();
}

// Edit functions

void MainWindow::undo() { ui->text->undo(); }
void MainWindow::redo() { ui->text->redo(); }
void MainWindow::cut() { ui->text->cut(); }
void MainWindow::copy() { ui->text->copy(); }
void MainWindow::paste() { ui->text->paste(); }

void MainWindow::search()
{
    // Show the search dialog
    SearchDialog dialog;
    dialog.exec();
}

// Format functions

void MainWindow::wordWrap()
{
    // Set word wrapping in the textbox
    if(ui->formatWordWrap->isChecked())
        ui->text->setLineWrapMode(QTextEdit::LineWrapMode::WidgetWidth);
    else
        ui->text->setLineWrapMode(QTextEdit::LineWrapMode::NoWrap);
}

void MainWindow::selectFont()
{
    // Get the font input from the user
    bool setFont;
    QFont font = QFontDialog::getFont(&setFont, ui->text->font(), this);

    // Set the font if the user pressed OK
    if(setFont) {
        ui->text->setFont(font);
        spdlog::info("Changed font");
    }
}

// View functions

void MainWindow::statusBar()
{
    // Toggle the status bar
    if(ui->viewStatusBar->isChecked())
        ui->statusBar->show();
    else
        ui->statusBar->hide();
}

// Help functions

void MainWindow::reportBug() { QDesktopServices::openUrl(QUrl("https://github.com/Starman0620/QNotepad/issues/new")); }
void MainWindow::aboutDialog() { QMessageBox::about(this, fmt::format("About {}", PROGRAM).c_str(), fmt::format("{} {}\n\nWritten by Cam K.\nLicensed under the BSD 2-Clause license", PROGRAM, VERSION).c_str()); }