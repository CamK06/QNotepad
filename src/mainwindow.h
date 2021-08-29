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
	bool exit(bool full);
	void textUpdated();
	void updateTitle();
	void newFile();
	void openFile();
	void saveFile();
	void saveAs();
	void reportBug();
	void aboutDialog();
	std::string saveAsDialog();
	void closeEvent(QCloseEvent *event);

	Ui::MainWindow *ui;
	std::string fileName = "Untitled";
	std::string filePath;
	bool saved = true; // This is true on default just to prevent an asterisk in the title
};