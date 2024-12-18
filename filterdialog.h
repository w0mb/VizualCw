#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>

namespace Ui {
class FilterDialog;
}

class FilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilterDialog(QWidget *parent = nullptr);
    ~FilterDialog();

signals:
    void filterSelected(const QString &filterField);

private slots:
    void on_pushButtonName_clicked();
    void on_pushButtonSurname_clicked();
    void on_pushButtonPatronymic_clicked();
    void on_pushButtonPassport_clicked();
    void on_pushButtonJobTitle_clicked();
    void on_pushButtonMedicalPolicy_clicked();
    void on_pushButtonInsurancePolicy_clicked();
    void on_pushButtonCancel_clicked();

private:
    Ui::FilterDialog *ui;
};

#endif // FILTERDIALOG_H
