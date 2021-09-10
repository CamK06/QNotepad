#include "search.h"
#include "./ui_searchdialog.h"

#include <QWidget>

SearchDialog::SearchDialog(QWidget *parent)
    :QDialog(parent)
{
    ui.setupUi(this);
}