#pragma once

#include <string>
#include <QTextEdit>
#include <QDropEvent>

class Editor : public QTextEdit
{
    Q_OBJECT

public:
    Editor(QWidget *parent = nullptr);
    void loadFile(std::string fileName);

private:
    void dropEvent(QDropEvent *event);
};