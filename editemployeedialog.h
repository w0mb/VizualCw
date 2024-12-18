#ifndef EDITEMPLOYEEDIALOG_H
#define EDITEMPLOYEEDIALOG_H

#include <QDialog>
#include <QDate>

namespace Ui {
class EditEmployeeDialog;
}

class EditEmployeeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditEmployeeDialog(QWidget *parent = nullptr);
    ~EditEmployeeDialog();

    void setEmployeeData(const QString &name,
                         const QString &surname,
                         const QString &patronymic,
                         const QString &passport,
                         const QString &jobTitle,
                         const QString &medicalPolicy,
                         const QString &insurancePolicy,
                         const QDate &vacationStart,
                         const QDate &vacationEnd);

    void setName(const QString &name);
    void setSurname(const QString &surname);
    void setPatronymic(const QString &patronymic);
    void setPassport(const QString &passport);
    void setJobTitle(const QString &jobTitle);
    void setMedicalPolicy(const QString &medicalPolicy);
    void setInsurancePolicy(const QString &insurancePolicy);
    void setVacation(const QDate &vacationStart, const QDate &vacationEnd);

    QString name() const;
    QString surname() const;
    QString patronymic() const;
    QString passport() const;
    QString jobTitle() const;
    QString medicalPolicy() const;
    QString insurancePolicy() const;
    QDate vacationStart() const;
    QDate vacationEnd() const;

private slots:
    void onSaveClicked();
    void onCancelClicked();

private:
    Ui::EditEmployeeDialog *ui;
};

#endif // EDITEMPLOYEEDIALOG_H
