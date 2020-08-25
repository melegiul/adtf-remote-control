#include <QSettings>
#include <QWidget>
#include <QFileDialog>
#include <QtDebug>

#include <QDir>
#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <iostream>

#include "dialog_log_analyzer.h"
#include "mainwindow.h"

LogAnalyzerDialog::LogAnalyzerDialog(QWidget *parent, LogModel *parentModel) : QDialog(parent),
                                                                               parentModel(parentModel) {
    this->setupUi(this);

//    QFileDialog dialog(this);
    connect(this->loadButton, SIGNAL(clicked()), this, SLOT(handleLoadButtonClicked()));
    connect(this->directoryButton, SIGNAL(clicked()), this, SLOT(switchSource()));
    connect(this->liveLogButton, SIGNAL(clicked()), this, SLOT(switchSource()));
    connect(this->applyButton, SIGNAL(clicked()), this, SLOT(handleApplyButtonClicked()));
//    connect(this->clearLog,SIGNAL(clicked()),this,SLOT(on_clearButton_clicked()));
    //connect(this->load_car_config_button, SIGNAL(clicked()), this, SLOT(loadPreferences()));
    sourceModel = new LogModel(this);
    proxyModel = new ProxyModel(this);
    proxyModel->setSourceModel(parentModel);
    proxyModel->setDynamicSortFilter(true);
    this->tableView->setModel(proxyModel);
    this->tableView->setColumnWidth(0, 160);
    this->tableView->setColumnWidth(1, 40);
    this->tableView->setColumnWidth(2, 120);
    this->tableView->setColumnWidth(3, 80);
    this->tableView->horizontalHeader()->setStretchLastSection(true);


}

void LogAnalyzerDialog::loadLog() {
    //open dialog
//    fileNameLog = QFileDialog::getOpenFileName(this, "Choose LogSerialization file");
//    qDebug() << "LogSerialization file" << fileNameLog << "selected for opening";
//    if (fileNameLog.isNull()) return;
}

void LogAnalyzerDialog::saveLog() {

}

void LogAnalyzerDialog::handleLoadButtonClicked() {
    QStringList fileNames;
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Json (*.json)"));
    dialog.setDirectory(QDir::homePath());
    if (dialog.exec()) {
        fileNames = dialog.selectedFiles();
    }
    QString string = fileNames.first();
    removeEntries();
    addEntries(log.loadLog(string));
    QStringList history = dialog.history();
    history.append(string);
    dialog.setHistory(history);
    this->historyBox->addItems(history);


}

void LogAnalyzerDialog::handleApplyButtonClicked() {
    proxyModel->setFilter(getFilterList(logLevelListWidget), getFilterList(sourceListWidget),
                          getFilterList(contextListWidget));

}

void LogAnalyzerDialog::addEntries(QList<QStringList> logList) {
    for (QStringList logValues: logList) {
        sourceModel->insertRows(0, 1, QModelIndex());
        for (int i = 0; i < logValues.size(); i++) {
            QModelIndex index = sourceModel->index(0, i, QModelIndex());
            sourceModel->setData(index, logValues.value(i), Qt::DisplayRole);
        }
    }
}

void LogAnalyzerDialog::removeEntries() {
    sourceModel->removeRows(0, sourceModel->rowCount(), QModelIndex());
}

void LogAnalyzerDialog::switchSource() {
    if (this->liveLogButton->isChecked()) {
        proxyModel->setSourceModel(parentModel);
    } else {
        proxyModel->setSourceModel(sourceModel);
    }
}

//void LogAnalyzerDialog::on_clearButton_clicked(){
//    this->tableWidget->setPlainText(nullptr);
//}

QStringList LogAnalyzerDialog::getFilterList(QListWidget *filterList) {
    QStringList entries;
    for (int i = 0; i < filterList->count(); i++) {
        if (filterList->item(i)->checkState() == Qt::Checked) {
            entries.append(filterList->item(i)->text());
        }
    }
    return entries;

}

