#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQuery>
#include <QTableWidgetItem>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include "addemployeedialog.h"
#include <QDialog>
#include <QtDebug>
#include <QSqlRecord>
#include "editemployeedialog.h"
#include <QDate>
#include <QSettings>
#include <settingsdialog.h>
#include "filterdialog.h"
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDebug>
#include <QHeaderView>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(1760, 830);
    ui->lineEditSearch->setPlaceholderText("Введите текст для поиска...");
    ui->tableWidget->setSortingEnabled(true);

    ui->tableWidget->setColumnCount(8);
    ui->tableWidget->setHorizontalHeaderLabels({"Имя", "Фамилия", "Отчество", "Паспорт", "Должность", "Мед.Полис", "Страховой Полис", "Отпуск"});
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

    m_currentPage = 1;
    m_rowsPerPage = 12;
    updatePaginationLabels();

    QFont font = ui->tableWidget->font();
    font.setPointSize(12);
    ui->tableWidget->setFont(font);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
            QTableWidgetItem *item = ui->tableWidget->item(row, col);
            if (!item) {
                item = new QTableWidgetItem();
                ui->tableWidget->setItem(row, col, item);
            }
            item->setFont(font);
            item->setTextAlignment(Qt::AlignCenter);
        }
    }
    connect(ui->pushButtonAdd, &QPushButton::clicked, this, &MainWindow::addEmployee);
    connect(ui->pushButtonDelete, &QPushButton::clicked, this, &MainWindow::deleteEmployee);
    connect(ui->pushButtonPrevPage, &QPushButton::clicked, this, &MainWindow::prevPage);
    connect(ui->pushButtonNextPage, &QPushButton::clicked, this, &MainWindow::nextPage);
    connect(ui->tableWidget, &QTableWidget::cellDoubleClicked, this, &MainWindow::editEmployee);
    connect(ui->pushButtonFilter, &QPushButton::clicked, this, &MainWindow::on_actionFilter_triggered);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &MainWindow::on_pushButtonSearch_clicked);
    connect(ui->pushButtonSettings, &QPushButton::clicked, this, &MainWindow::openSettingsDialog);

    loadEmployeeData();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::connectDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.db");

    if (!db.open()) {
        qDebug() << "Ошибка открытия БД:" << db.lastError().text();
        return false;
    }
    return true;
}

void MainWindow::on_actionFilter_triggered()
{
    FilterDialog filterDialog(this);

    connect(&filterDialog, &FilterDialog::filterSelected, this, [=](const QString &selectedField) {
        applyFilter(selectedField);
    });

    filterDialog.exec();
}

void MainWindow::applyFilter(const QString &filterField)
{
    currentFilterField = filterField;
    ui->statusBar->showMessage("Текущий фильтр: " + filterField);
    ui->lineEditSearch->setPlaceholderText("Поиск по: " + filterField);
    ui->lineEditSearch->clear();
}

void MainWindow::on_pushButtonSearch_clicked()
{
    QString searchText = ui->lineEditSearch->text();
    performSearch(searchText);
}

void MainWindow::performSearch(const QString &searchText)
{
    if (currentFilterField.isEmpty()) {
        ui->statusBar->showMessage("Выберите поле для фильтрации через меню!");
        return;
    }

    int visibleRowCount = 0;

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        QString cellText;
        if (currentFilterField == "Имя") cellText = ui->tableWidget->item(row, 0)->text();
        else if (currentFilterField == "Фамилия")  cellText = ui->tableWidget->item(row, 1)->text();
        else if (currentFilterField == "Отчество") cellText = ui->tableWidget->item(row, 2)->text();
        else if (currentFilterField == "Паспорт")  cellText = ui->tableWidget->item(row, 3)->text();
        else if (currentFilterField == "Должность")cellText = ui->tableWidget->item(row, 4)->text();
        else if (currentFilterField == "Мед.Полис")cellText = ui->tableWidget->item(row, 5)->text();
        else if (currentFilterField == "Страховой Полис") cellText = ui->tableWidget->item(row, 6)->text();
        else if (currentFilterField == "Отпуск")   cellText = ui->tableWidget->item(row, 7)->text();

        bool match = cellText.contains(searchText, Qt::CaseInsensitive);
        ui->tableWidget->setRowHidden(row, !match);
        if (match) visibleRowCount++;
    }

    ui->statusBar->showMessage(QString("Найдено результатов: %1").arg(visibleRowCount));
}

void MainWindow::addEmployee()
{
    AddEmployeeDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        QString name = dialog.name();
        QString surname = dialog.surname();
        QString patronymic = dialog.patronymic();
        QString passport = dialog.passport();
        QString jobTitle = dialog.jobTitle();
        QString medicalPolicy = dialog.medicalPolicy();
        QString insurancePolicy = dialog.insurancePolicy();
        QString vacation = dialog.vacation();

        if (!connectDatabase()) {
            qDebug() << "Ошибка подключения к базе данных";
            return;
        }

        QSqlQuery query;
        query.prepare("INSERT INTO Employees (name, surname, patronymic, passport, job_title, medical_policy, insurance_policy, vacation) "
                      "VALUES (:name, :surname, :patronymic, :passport, :job_title, :medical_policy, :insurance_policy, :vacation)");
        query.bindValue(":name", name);
        query.bindValue(":surname", surname);
        query.bindValue(":patronymic", patronymic);
        query.bindValue(":passport", passport);
        query.bindValue(":job_title", jobTitle);
        query.bindValue(":medical_policy", medicalPolicy);
        query.bindValue(":insurance_policy", insurancePolicy);
        query.bindValue(":vacation", vacation);

        if (!query.exec()) {
            qDebug() << "Ошибка вставки данных:" << query.lastError().text();
        } else {
            qDebug() << "Сотрудник успешно добавлен в базу данных.";
        }

        int currentRowCount = (m_currentPage - 1) * m_rowsPerPage + ui->tableWidget->rowCount();
        if (currentRowCount % m_rowsPerPage == 0) {
            m_currentPage++;
        }

        loadEmployeeData();
    } else {
        qDebug() << "Добавление сотрудника отменено.";
    }
}

void MainWindow::deleteEmployee()
{
    int currentRow = ui->tableWidget->currentRow();

    if (currentRow == -1) {
        QMessageBox::warning(this, "Удаление сотрудника", "Пожалуйста, выберите строку для удаления.");
        return;
    }

    QString firstCellText = ui->tableWidget->item(currentRow, 0)->text();
    if (firstCellText.isEmpty()) {
        QMessageBox::warning(this, "Удаление сотрудника", "Выбранная строка пуста. Нечего удалять.");
        return;
    }

    QString passport = ui->tableWidget->item(currentRow, 3)->text();

    if (!connectDatabase()) {
        qDebug() << "Ошибка подключения к базе данных";
        return;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM Employees WHERE passport = :passport");
    query.bindValue(":passport", passport);

    if (!query.exec()) {
        qDebug() << "Ошибка удаления данных:" << query.lastError().text();
        QMessageBox::critical(this, "Ошибка", "Не удалось удалить сотрудника из базы данных.");
        return;
    }

    qDebug() << "Сотрудник с паспортом " << passport << " успешно удалён.";

    for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
        ui->tableWidget->item(currentRow, col)->setText("");
    }

    loadEmployeeData();

    QMessageBox::information(this, "Удаление", "Сотрудник успешно удалён.");
}

void MainWindow::prevPage()
{
    if (m_currentPage > 1) {
        m_currentPage--;
        loadEmployeeData();
        updatePaginationLabels();
    }
}

void MainWindow::nextPage()
{
    m_currentPage++;
    loadEmployeeData();
    updatePaginationLabels();
}

void MainWindow::loadEmployeeData()
{
    if (!connectDatabase()) {
        qDebug() << "РћС€РёР±РєР° РїРѕРґРєР»СЋС‡РµРЅРёСЏ Рє Р±Р°Р·Рµ РґР°РЅРЅС‹С…";
        return;
    }

    ui->tableWidget->setSortingEnabled(false);

    QSqlQuery countQuery("SELECT COUNT(*) FROM Employees");
    if (!countQuery.exec()) {
        qDebug() << "РћС€РёР±РєР° РїРѕРґСЃС‡РµС‚Р° Р·Р°РїРёСЃРµР№:" << countQuery.lastError().text();
        return;
    }

    if (countQuery.next()) {
        totalRecords = countQuery.value(0).toInt();
    }

    int totalPages = (totalRecords + m_rowsPerPage - 1) / m_rowsPerPage;
    m_currentPage = qMax(1, qMin(m_currentPage, totalPages));

    QSqlQuery query;
    query.prepare("SELECT surname, name, patronymic, passport, job_title, medical_policy, insurance_policy, vacation "
                  "FROM Employees LIMIT :limit OFFSET :offset");
    query.bindValue(":limit", m_rowsPerPage);
    query.bindValue(":offset", (m_currentPage - 1) * m_rowsPerPage);

    if (!query.exec()) {
        qDebug() << "РћС€РёР±РєР° Р·Р°РіСЂСѓР·РєРё РґР°РЅРЅС‹С… СЃРѕС‚СЂСѓРґРЅРёРєРѕРІ:" << query.lastError().text();
        return;
    }

    ui->tableWidget->setRowCount(0);

    while (query.next()) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < query.record().count(); ++col) {
            ui->tableWidget->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
    }

    while (ui->tableWidget->rowCount() < m_rowsPerPage) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
            ui->tableWidget->setItem(row, col, new QTableWidgetItem(""));
        }
    }

    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        ui->tableWidget->setRowHeight(i, 55);
    }



    updatePaginationLabels();
    ui->pushButtonPrevPage->setEnabled(m_currentPage > 1);
    ui->pushButtonNextPage->setEnabled(m_currentPage < totalPages);

    ui->tableWidget->setSortingEnabled(true);

    int sortColumn = ui->tableWidget->horizontalHeader()->sortIndicatorSection();
    Qt::SortOrder sortOrder = ui->tableWidget->horizontalHeader()->sortIndicatorOrder();
    ui->tableWidget->sortItems(sortColumn, sortOrder);
}

void MainWindow::editEmployee(int row, int column)
{
    Q_UNUSED(column);

    if (ui->tableWidget->item(row, 0)->text().isEmpty()) {
        return;
    }

    QString surname = ui->tableWidget->item(row, 0)->text();
    QString name = ui->tableWidget->item(row, 1)->text();
    QString patronymic = ui->tableWidget->item(row, 2)->text();
    QString passport = ui->tableWidget->item(row, 3)->text();
    QString jobTitle = ui->tableWidget->item(row, 4)->text();
    QString medicalPolicy = ui->tableWidget->item(row, 5)->text();
    QString insurancePolicy = ui->tableWidget->item(row, 6)->text();
    QString vacation = ui->tableWidget->item(row, 7)->text();

    EditEmployeeDialog dialog(this);

    dialog.setSurname(surname);
    dialog.setName(name);
    dialog.setPatronymic(patronymic);
    dialog.setPassport(passport);
    dialog.setJobTitle(jobTitle);
    dialog.setMedicalPolicy(medicalPolicy);
    dialog.setInsurancePolicy(insurancePolicy);

    QStringList vacationDates = vacation.split(" - ");
    QDate vacationStart = QDate::fromString(vacationDates.value(0), "dd.MM.yyyy");
    QDate vacationEnd = vacationDates.size() > 1 ? QDate::fromString(vacationDates.value(1), "dd.MM.yyyy") : vacationStart;

    dialog.setVacation(vacationStart, vacationEnd);

    if (dialog.exec() == QDialog::Accepted) {
        QString newSurname = dialog.surname();
        QString newName = dialog.name();
        QString newPatronymic = dialog.patronymic();
        QString newPassport = dialog.passport();
        QString newJobTitle = dialog.jobTitle();
        QString newMedicalPolicy = dialog.medicalPolicy();
        QString newInsurancePolicy = dialog.insurancePolicy();
        QString newVacationStart = dialog.vacationStart().toString("dd.MM.yyyy");
        QString newVacationEnd = dialog.vacationEnd().toString("dd.MM.yyyy");
        QString newVacation = newVacationStart + " - " + newVacationEnd;

        if (connectDatabase()) {
            QSqlQuery query;
            query.prepare("UPDATE Employees SET surname = :surname, name = :name, patronymic = :patronymic, passport = :newPassport, "
                          "job_title = :jobTitle, medical_policy = :medicalPolicy, "
                          "insurance_policy = :insurancePolicy, vacation = :vacation "
                          "WHERE passport = :oldPassport");
            query.bindValue(":surname", newSurname);
            query.bindValue(":name", newName);
            query.bindValue(":patronymic", newPatronymic);
            query.bindValue(":newPassport", newPassport);
            query.bindValue(":jobTitle", newJobTitle);
            query.bindValue(":medicalPolicy", newMedicalPolicy);
            query.bindValue(":insurancePolicy", newInsurancePolicy);
            query.bindValue(":vacation", newVacation);
            query.bindValue(":oldPassport", passport);

            if (query.exec()) {
                qDebug() << "Данные сотрудника обновлены";

                ui->tableWidget->item(row, 0)->setText(newSurname);
                ui->tableWidget->item(row, 1)->setText(newName);
                ui->tableWidget->item(row, 2)->setText(newPatronymic);
                ui->tableWidget->item(row, 3)->setText(newPassport);
                ui->tableWidget->item(row, 4)->setText(newJobTitle);
                ui->tableWidget->item(row, 5)->setText(newMedicalPolicy);
                ui->tableWidget->item(row, 6)->setText(newInsurancePolicy);
                ui->tableWidget->item(row, 7)->setText(newVacation);
            } else {
                qDebug() << "Ошибка обновления данных:" << query.lastError().text();
            }
        }
    }
}

void MainWindow::updatePaginationLabels()
{
    int totalPages = (totalRecords + m_rowsPerPage - 1) / m_rowsPerPage;
    ui->labelCurrentPage->setText(QString("Страница: %1 из %2").arg(m_currentPage).arg(totalPages));
    ui->labelRowCount->setText(QString("Всего записей: %1").arg(totalRecords));
}

void MainWindow::openSettingsDialog()
{
    SettingsDialog settingsDialog(this);
    if (settingsDialog.exec() == QDialog::Accepted) {
        applySettings();
    }
}

void MainWindow::applySettings()
{
    QSettings settings("YourCompanyName", "EmployeeManagementApp");

    QColor buttonColor = settings.value("buttonColor", QColor(Qt::blue)).value<QColor>();
    QColor buttonTextColor = settings.value("buttonTextColor", QColor(Qt::white)).value<QColor>();
    QColor buttonHoverColor = buttonColor.lighter(110);

    QString buttonStyleSheet = QString(
        "QPushButton { background-color: %1; color: %2; border: none; padding: 5px; border-radius: 5px; }"
        "QPushButton:hover { background-color: %3; }"
    ).arg(buttonColor.name(), buttonTextColor.name(), buttonHoverColor.name());

    ui->pushButtonAdd->setStyleSheet(buttonStyleSheet);
    ui->pushButtonDelete->setStyleSheet(buttonStyleSheet);
    ui->pushButtonSearch->setStyleSheet(buttonStyleSheet);
    ui->pushButtonFilter->setStyleSheet(buttonStyleSheet);
    ui->pushButtonPrevPage->setStyleSheet(buttonStyleSheet);
    ui->pushButtonNextPage->setStyleSheet(buttonStyleSheet);
    ui->pushButtonSettings->setStyleSheet(buttonStyleSheet);

    QColor tableColor = settings.value("tableColor", QColor(Qt::white)).value<QColor>();
    QColor tableAlternateColor = tableColor.lighter(105);

    QString tableStyleSheet = QString(
        "QTableWidget { background-color: %1; alternate-background-color: %2; }"
    ).arg(tableColor.name(), tableAlternateColor.name());

    ui->tableWidget->setStyleSheet(tableStyleSheet);
    ui->tableWidget->setAlternatingRowColors(true);
}









