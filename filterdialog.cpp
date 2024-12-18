#include "filterdialog.h"
#include "ui_filterdialog.h"

FilterDialog::FilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterDialog)
{
    ui->setupUi(this);
    setWindowTitle("Фильтрация данных");
    setModal(true);
}

FilterDialog::~FilterDialog()
{
    delete ui;
}

void FilterDialog::on_pushButtonName_clicked()
{
    emit filterSelected("Имя");
    accept();
}

void FilterDialog::on_pushButtonSurname_clicked()
{
    emit filterSelected("Фамилия");
    accept();
}

void FilterDialog::on_pushButtonPatronymic_clicked()
{
    emit filterSelected("Отчество");
    accept();
}

void FilterDialog::on_pushButtonPassport_clicked()
{
    emit filterSelected("Паспорт");
    accept();
}

void FilterDialog::on_pushButtonJobTitle_clicked()
{
    emit filterSelected("Должность");
    accept();
}

void FilterDialog::on_pushButtonMedicalPolicy_clicked()
{
    emit filterSelected("Медицинский полис");
    accept();
}

void FilterDialog::on_pushButtonInsurancePolicy_clicked()
{
    emit filterSelected("Страховой полис");
    accept();
}

void FilterDialog::on_pushButtonCancel_clicked()
{
    reject();
}
