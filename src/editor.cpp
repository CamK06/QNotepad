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
    // This is probably a very messy way of loading the file, but for now it works
    if(event->mimeData()->hasText()) {
        spdlog::info("Loading dropped file...");
        ((MainWindow*)parent()->parent())->loadFile(event->mimeData()->text().remove("file://").toStdString());
    }

    // Not sure if this is actually necessary, but it's here anyways
    event->accept();
}