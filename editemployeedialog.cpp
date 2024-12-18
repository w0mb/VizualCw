#include "EditEmployeeDialog.h"
#include "ui_EditEmployeeDialog.h"
#include <QMessageBox>
#include <QDialogButtonBox>

EditEmployeeDialog::EditEmployeeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditEmployeeDialog)
{
    ui->setupUi(this);
    setWindowTitle("Редактировать Сотрудника");

    QRegularExpression numberRegex("\\d+");
    QRegularExpressionValidator *numberValidator = new QRegularExpressionValidator(numberRegex, this);

    QRegularExpression letterRegex("^[A-Za-zА-Яа-яЁё]+$");
    QRegularExpressionValidator *letterValidator = new QRegularExpressionValidator(letterRegex, this);

    ui->lineEditPassport->setValidator(numberValidator);
    ui->lineEditMedicalPolicy->setValidator(numberValidator);
    ui->lineEditInsurancePolicy->setValidator(numberValidator);

    ui->lineEditName->setValidator(letterValidator);
    ui->lineEditSurname->setValidator(letterValidator);
    ui->lineEditPatronymic->setValidator(letterValidator);
    ui->lineEditJobTitle->setValidator(letterValidator);

    connect(ui->buttonBox, &QPushButton::clicked, this, &EditEmployeeDialog::onSaveClicked);
    connect(ui->buttonCancel, &QPushButton::clicked, this, &EditEmployeeDialog::onCancelClicked);
}


EditEmployeeDialog::~EditEmployeeDialog()
{
    delete ui;
}

void EditEmployeeDialog::setEmployeeData(const QString &name,
                                         const QString &surname,
                                         const QString &patronymic,
                                         const QString &passport,
                                         const QString &jobTitle,
                                         const QString &medicalPolicy,
                                         const QString &insurancePolicy,
                                         const QDate &vacationStart,
                                         const QDate &vacationEnd)
{
    setName(name);
    setSurname(surname);
    setPatronymic(patronymic);
    setPassport(passport);
    setJobTitle(jobTitle);
    setMedicalPolicy(medicalPolicy);
    setInsurancePolicy(insurancePolicy);
    setVacation(vacationStart, vacationEnd);
}

void EditEmployeeDialog::setName(const QString &name)
{
    ui->lineEditName->setText(name);
}

void EditEmployeeDialog::setSurname(const QString &surname)
{
    ui->lineEditSurname->setText(surname);
}

void EditEmployeeDialog::setPatronymic(const QString &patronymic)
{
    ui->lineEditPatronymic->setText(patronymic);
}

void EditEmployeeDialog::setPassport(const QString &passport)
{
    ui->lineEditPassport->setText(passport);
}

void EditEmployeeDialog::setJobTitle(const QString &jobTitle)
{
    ui->lineEditJobTitle->setText(jobTitle);
}

void EditEmployeeDialog::setMedicalPolicy(const QString &medicalPolicy)
{
    ui->lineEditMedicalPolicy->setText(medicalPolicy);
}

void EditEmployeeDialog::setInsurancePolicy(const QString &insurancePolicy)
{
    ui->lineEditInsurancePolicy->setText(insurancePolicy);
}

void EditEmployeeDialog::setVacation(const QDate &vacationStart, const QDate &vacationEnd)
{
    ui->dateEditVacationStart->setDate(vacationStart);
    ui->dateEditVacationEnd->setDate(vacationEnd);
}

QString EditEmployeeDialog::name() const
{
    return ui->lineEditName->text();
}

QString EditEmployeeDialog::surname() const
{
    return ui->lineEditSurname->text();
}

QString EditEmployeeDialog::patronymic() const
{
    return ui->lineEditPatronymic->text();
}

QString EditEmployeeDialog::passport() const
{
    return ui->lineEditPassport->text();
}

QString EditEmployeeDialog::jobTitle() const
{
    return ui->lineEditJobTitle->text();
}

QString EditEmployeeDialog::medicalPolicy() const
{
    return ui->lineEditMedicalPolicy->text();
}

QString EditEmployeeDialog::insurancePolicy() const
{
    return ui->lineEditInsurancePolicy->text();
}

QDate EditEmployeeDialog::vacationStart() const
{
    return ui->dateEditVacationStart->date();
}

QDate EditEmployeeDialog::vacationEnd() const
{
    return ui->dateEditVacationEnd->date();
}

void EditEmployeeDialog::onSaveClicked()
{
    if (ui->lineEditName->text().isEmpty() ||
        ui->lineEditSurname->text().isEmpty() ||
        ui->lineEditPassport->text().isEmpty() ||
        ui->lineEditJobTitle->text().isEmpty() ||
        ui->lineEditMedicalPolicy->text().isEmpty() ||
        ui->lineEditInsurancePolicy->text().isEmpty()) {
        QMessageBox::warning(this, "Ошибка ввода", "Пожалуйста, заполните все обязательные поля!");
        return;
    }

    if (ui->dateEditVacationStart->date() > ui->dateEditVacationEnd->date()) {
        QMessageBox::warning(this, "Ошибка ввода", "Дата начала отпуска не может быть позже даты окончания!");
        return;
    }

    accept();
}


void EditEmployeeDialog::onCancelClicked()
{
    reject();
}
