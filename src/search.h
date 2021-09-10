#pragma once

#include <QDialog>
#include <QWidget>

#include "./ui_searchdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SearchDialog; }
QT_END_NAMESPACE

class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    SearchDialog(QWidget *parent = nullptr);
    Ui::SearchDialog *ui;

private:
    void findNext();
    void cancel();
};