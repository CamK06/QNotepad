#pragma once

#include <QMainWindow>
#include <QPrintDialog>
#include <QLabel>
#include <QPrinter>
#include <thread>
#include "search.h"
#include "editor.h"
#include "./ui_searchdialog.h"

#if ADVANCED
#include <discord_rpc.h>
#define DISCORD_CLIENT_ID "819293814404677653"
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();
	void withFile(std::string fileName);
	void loadFile(std::string fileName);
	bool exit(bool fullExit);
	Editor *editor();

private:
#if ADVANCED
	void updatePresence();
	void discordWorker();
#endif

	// File menu
	void newFile();
	void openFile();
	void saveFile();
	void saveAs();
	void print();
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
	SearchDialog *searchDialog;
	QPrintDialog *printDialog = nullptr;
	QPrinter *printer = nullptr;
	std::string fileName = "Untitled";
	std::string filePath;
	QLabel statusBarLabel;
	bool saved = true; // This is true on default just to prevent an asterisk in the title
#if ADVANCED
	std::thread discordThread;
	DiscordEventHandlers discordHandlers;
	DiscordRichPresence discordPresence;
	bool discordInitialized = false;
	bool discordThreadRunning = false;
	int discordCounter = 0;
#endif
};