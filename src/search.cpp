#include <QWidget>
#include <spdlog/spdlog.h>

#include "search.h"
#include "./ui_searchdialog.h"

SearchDialog::SearchDialog(QWidget *parent)
    :QDialog(parent)
    , ui(new Ui::SearchDialog)
{
    ui->setupUi(this);

    connect(ui->cancelButton, &QPushButton::pressed, this, &SearchDialog::cancel);
    connect(ui->findNextButton, &QPushButton::pressed, this, &SearchDialog::findNext);
}

void SearchDialog::findNext()
{
    spdlog::info("Find next");
}

void SearchDialog::cancel()
{
    close();
}