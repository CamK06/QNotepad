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
	bool exit(bool closeEvent);
	void textUpdated();
	void updateTitle();
	void closeEvent(QCloseEvent *event);

	Ui::MainWindow *ui;
	std::string fileName = "Untitled";
	std::string oldTitle;
	bool saved = true; // This is true on default just to prevent an asterisk in the title
};