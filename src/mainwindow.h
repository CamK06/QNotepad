#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);

private:
	void fileExit() { exit(true); } // Just a workaround for stupid behaviour
	bool exit(bool fullExit);
	void textUpdated();
	void updateTitle();
	void newFile();
	void openFile();
	void saveFile();
	void saveAs();
	void reportBug();
	void aboutDialog();
	void undo();
	void redo();
	void cut();
	void copy();
	void paste();
	std::string saveAsDialog();
	void closeEvent(QCloseEvent *event);

	Ui::MainWindow *ui;
	std::string fileName = "Untitled";
	std::string filePath;
	bool saved = true; // This is true on default just to prevent an asterisk in the title
};