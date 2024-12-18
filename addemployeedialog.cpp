#include "addemployeedialog.h"
#include "ui_addemployeedialog.h"
#include <QDate>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QMessageBox>

AddEmployeeDialog::AddEmployeeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEmployeeDialog)
{
    ui->setupUi(this);
    setWindowTitle("Добавить Сотрудника");

    QRegularExpression numberRegex("\\d+");
    QRegularExpressionValidator *numberValidator = new QRegularExpressionValidator(numberRegex, this);

    QRegularExpression letterRegex("^[A-Za-zА-Яа-яЁё]+$");
    QRegularExpressionValidator *letterValidator = new QRegularExpressionValidator(letterRegex, this);

    ui->lineEditPassport->setValidator(numberValidator);
    ui->lineEditMedicalPolicy->setValidator(numberValidator);
    ui->lineEditInsurancePolicy->setValidator(numberValidator);

    ui->lineEditName->setValidator(letterValidator);
    ui->lineEditSurname->setValidator(letterValidator);
    ui->lineEditJobTitle->setValidator(letterValidator);
    ui->lineEditPatronymic->setValidator(letterValidator);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &AddEmployeeDialog::validateAndAccept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &AddEmployeeDialog::reject);
}

AddEmployeeDialog::~AddEmployeeDialog()
{
    delete ui;
}

void AddEmployeeDialog::validateAndAccept()
{
    if (ui->lineEditName->text().isEmpty() ||
        ui->lineEditSurname->text().isEmpty() ||
        ui->lineEditPassport->text().isEmpty() ||
        ui->lineEditJobTitle->text().isEmpty() ||
        ui->lineEditMedicalPolicy->text().isEmpty() ||
        ui->lineEditInsurancePolicy->text().isEmpty() ||
        ui->dateEditVacationStart->date() > ui->dateEditVacationEnd->date()) {
        QMessageBox::warning(this, "Ошибка ввода", "Пожалуйста, заполните все обязательные поля и убедитесь, что дата начала отпуска не позднее даты окончания!");
        return;
    }
    accept();
}

QString AddEmployeeDialog::name() const {
    return ui->lineEditName->text();
}

QString AddEmployeeDialog::surname() const {
    return ui->lineEditSurname->text();
}

QString AddEmployeeDialog::patronymic() const {
    return ui->lineEditPatronymic->text();
}

QString AddEmployeeDialog::passport() const {
    return ui->lineEditPassport->text();
}

QString AddEmployeeDialog::jobTitle() const {
    return ui->lineEditJobTitle->text();
}

QString AddEmployeeDialog::medicalPolicy() const {
    return ui->lineEditMedicalPolicy->text();
}

QString AddEmployeeDialog::insurancePolicy() const {
    return ui->lineEditInsurancePolicy->text();
}

QString AddEmployeeDialog::vacation() const {
    QDate startDate = ui->dateEditVacationStart->date();
    QDate endDate = ui->dateEditVacationEnd->date();
    return startDate.toString("dd.MM.yyyy") + " - " + endDate.toString("dd.MM.yyyy");
}
