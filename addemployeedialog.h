#ifndef ADDEMPLOYEEDIALOG_H
#define ADDEMPLOYEEDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class AddEmployeeDialog;
}

class AddEmployeeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddEmployeeDialog(QWidget *parent = nullptr);
    ~AddEmployeeDialog();

    QString name() const;
    QString surname() const;
    QString patronymic() const;
    QString passport() const;
    QString jobTitle() const;
    QString medicalPolicy() const;
    QString insurancePolicy() const;
    QString vacation() const;

private slots:
    void validateAndAccept();

private:
    Ui::AddEmployeeDialog *ui;
};

#endif // ADDEMPLOYEEDIALOG_H
