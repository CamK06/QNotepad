#include <QTextEdit>
#include <QDropEvent>
#include <QMimeData>
#include <spdlog/spdlog.h>

#include "mainwindow.h"
#include "editor.h"

Editor::Editor(QWidget *parent)
    : QTextEdit(parent) {}

void Editor::dropEvent(QDropEvent *event)
{
    MainWindow* mainWindow = ((MainWindow*)parent()->parent());

    // Use exit for the save prompt
    if(!mainWindow->exit(false))
        return;

    // This is probably a very messy way of loading the file, but for now it works
    if(event->mimeData()->hasText()) {
        spdlog::info("Loading dropped file...");
        mainWindow->loadFile(event->mimeData()->text().remove("file://").toStdString());
    }

    // Not sure if this is actually necessary, but it's here anyways
    event->accept();
}