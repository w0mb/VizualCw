#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "filterdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addEmployee();
    void deleteEmployee();
    void prevPage();
    void nextPage();
    void editEmployee(int row, int column);
    void on_actionFilter_triggered();
    void applyFilter(const QString &filterField);
    void on_pushButtonSearch_clicked();
    void openSettingsDialog();
    void applySettings();

private:
    Ui::MainWindow *ui;
    int m_currentPage;
    int m_rowsPerPage;
    int totalRecords = 0;
    void loadEmployeeData();
    void updatePaginationLabels();
    bool connectDatabase();
    void performSearch(const QString &searchText);
    QString currentFilterField;
};

#endif // MAINWINDOW_H
