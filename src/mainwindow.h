#pragma once

#include <QMainWindow>
#include <QLabel>
#include "search.h"
#include "editor.h"
#include "./ui_searchdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	void withFile(std::string fileName);
	void loadFile(std::string fileName);
	bool exit(bool fullExit);
	Editor *editor();

private:
	// File menu
	void newFile();
	void openFile();
	void saveFile();
	void saveAs();
	void fileExit() { exit(true); } // This is just a workaround for Qt being annoying

	// Edit menu
	void undo();
	void redo();
	void cut();
	void copy();
	void paste();
	void search();

	// Format menu
	void wordWrap();
	void selectFont();

	// View menu
	void statusBar();

	// Help menu
	void reportBug();
	void aboutDialog();

	// Internal stuff (not directly triggered by the user)
	std::string saveAsDialog();
	void closeEvent(QCloseEvent *event);
	void textUpdated();
	void cursorMoved();
	void updateTitle();

	Ui::MainWindow *ui;
	std::string fileName = "Untitled";
	std::string filePath;
	QLabel statusBarLabel;
	bool saved = true; // This is true on default just to prevent an asterisk in the title
};