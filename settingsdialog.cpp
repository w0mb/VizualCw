#include "settingsdialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QColorDialog>
#include <QDialogButtonBox>

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Настройки");
    QSettings settings("YourCompanyName", "EmployeeManagementApp");
    m_buttonColor = settings.value("buttonColor", QColor(Qt::blue)).value<QColor>();
    m_tableColor = settings.value("tableColor", QColor(Qt::white)).value<QColor>();
    m_buttonTextColor = settings.value("buttonTextColor", QColor(Qt::white)).value<QColor>();
    QVBoxLayout *layout = new QVBoxLayout(this);

    QHBoxLayout *buttonColorLayout = new QHBoxLayout();
    QLabel *buttonColorLabel = new QLabel("Цвет кнопок:");
    m_buttonColorButton = new QPushButton("Выбрать цвет");
    m_buttonColorButton->setStyleSheet(QString("background-color: %1").arg(m_buttonColor.name()));
    buttonColorLayout->addWidget(buttonColorLabel);
    buttonColorLayout->addWidget(m_buttonColorButton);
    layout->addLayout(buttonColorLayout);

    QHBoxLayout *buttonTextColorLayout = new QHBoxLayout();
    QLabel *buttonTextColorLabel = new QLabel("Цвет текста кнопок:");
    m_buttonTextColorButton = new QPushButton("Выбрать цвет");
    m_buttonTextColorButton->setStyleSheet(QString("color: %1").arg(m_buttonTextColor.name()));
    buttonTextColorLayout->addWidget(buttonTextColorLabel);
    buttonTextColorLayout->addWidget(m_buttonTextColorButton);
    layout->addLayout(buttonTextColorLayout);

    QHBoxLayout *tableColorLayout = new QHBoxLayout();
    QLabel *tableColorLabel = new QLabel("Цвет таблицы:");
    m_tableColorButton = new QPushButton("Выбрать цвет");
    m_tableColorButton->setStyleSheet(QString("background-color: %1").arg(m_tableColor.name()));
    tableColorLayout->addWidget(tableColorLabel);
    tableColorLayout->addWidget(m_tableColorButton);
    layout->addLayout(tableColorLayout);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

    connect(m_buttonColorButton, &QPushButton::clicked, this, &SettingsDialog::chooseButtonColor);
    connect(m_buttonTextColorButton, &QPushButton::clicked, this, &SettingsDialog::chooseButtonTextColor);
    connect(m_tableColorButton, &QPushButton::clicked, this, &SettingsDialog::chooseTableColor);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::saveSettings);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void SettingsDialog::chooseButtonColor()
{
    QColor color = QColorDialog::getColor(m_buttonColor, this, "Выберите цвет кнопок");
    if (color.isValid()) {
        m_buttonColor = color;
        m_buttonColorButton->setStyleSheet(QString("background-color: %1").arg(color.name()));
    }
}

void SettingsDialog::chooseButtonTextColor()
{
    QColor color = QColorDialog::getColor(m_buttonTextColor, this, "Выберите цвет текста кнопок");
    if (color.isValid()) {
        m_buttonTextColor = color;
        m_buttonTextColorButton->setStyleSheet(QString("background-color: %1").arg(color.name()));
    }
}


void SettingsDialog::chooseTableColor()
{
    QColor color = QColorDialog::getColor(m_tableColor, this, "Выберите цвет таблицы");
    if (color.isValid()) {
        m_tableColor = color;
        m_tableColorButton->setStyleSheet(QString("background-color: %1").arg(color.name()));
    }
}

void SettingsDialog::saveSettings()
{
    QSettings settings("YourCompanyName", "EmployeeManagementApp");
    settings.setValue("buttonColor", m_buttonColor);
    settings.setValue("buttonTextColor", m_buttonTextColor);
    settings.setValue("tableColor", m_tableColor);
    accept();
}
