#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QColor>
#include <QSettings>

class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = nullptr);

private:
    QColor m_buttonColor;
    QColor m_tableColor;
    QColor m_buttonTextColor;
    QPushButton *m_buttonColorButton;
    QPushButton *m_tableColorButton;
    QPushButton *m_buttonTextColorButton;


private slots:
    void chooseButtonColor();
    void chooseTableColor();
    void chooseButtonTextColor();
    void saveSettings();

};

#endif // SETTINGSDIALOG_H
