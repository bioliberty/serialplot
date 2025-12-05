/*
  Copyright © 2025 Hasan Yavuz Özderya

  This file is part of serialplot.

  serialplot is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  serialplot is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with serialplot.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "defines.h"
#include "setting_defines.h"
#include "complexframedreadersettings.h"
#include "ui_complexframedreadersettings.h"

ComplexFramedReaderSettings::ComplexFramedReaderSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComplexFramedReaderSettings),
    fbGroup(this)
{
    ui->setupUi(this);

    ui->leSyncWord->setMode(false); // hex mode
    ui->leSyncWord->setText("AA BB");
    ui->spNumOfChannels->setMaximum(MAX_NUM_CHANNELS);

    connect(ui->cbChecksum, &QCheckBox::toggled,
            [this](bool enabled)
            {
                emit checksumChanged(enabled);
            });

    connect(ui->cbDebugMode, &QCheckBox::toggled,
            this, &ComplexFramedReaderSettings::debugModeChanged);

    {
        // add frame size selection buttons to the same fbGroup
        // fbGroup = new QButtonGroup(this);
        fbGroup.addButton(ui->rbFixedSize, (int) SizeFieldType::Fixed);
        fbGroup.addButton(ui->rbSize1Byte, (int) SizeFieldType::Field1Byte);
        fbGroup.addButton(ui->rbSize2Byte, (int) SizeFieldType::Field2Byte);

        connect(&fbGroup, &QButtonGroup::idToggled,
                [this](int id, bool enabled)
                {
                    if (!enabled) return;
                    if (id == static_cast<int>(SizeFieldType::Fixed))
                    {
                        emit sizeFieldChanged(SizeFieldType::Fixed, ui->spSize->value());
                    }
                    else
                    {
                        emit sizeFieldChanged(static_cast<SizeFieldType>(id), 0);
                    }
                });

        // Enable/disable size text field
        connect(ui->rbFixedSize, &QRadioButton::toggled,
                ui->spSize, &QWidget::setEnabled);

        connect(ui->spSize, &QSpinBox::valueChanged,
                [this](int value)
                {
                    if (ui->rbFixedSize->isChecked())
                        emit sizeFieldChanged(SizeFieldType::Fixed, value);
                });
    }

    connect(ui->spNumOfChannels, &QSpinBox::valueChanged,
            [this](int value)
            {
                emit numOfChannelsChanged(value);
            });

    connect(ui->leSyncWord, &QLineEdit::textChanged,
            this, &ComplexFramedReaderSettings::onSyncWordEdited);

    connect(ui->nfBox, SIGNAL(selectionChanged(NumberFormat)),
            this, SIGNAL(numberFormatChanged(NumberFormat)));

    updateSyncWordAscii(); // Initialize ASCII display
}

ComplexFramedReaderSettings::~ComplexFramedReaderSettings()
{
    delete ui;
}

void ComplexFramedReaderSettings::showMessage(QString message, bool error)
{
    ui->lMessage->setText(message);
    if (error)
    {
        ui->lMessage->setStyleSheet("color: red;");
    }
    else
    {
        ui->lMessage->setStyleSheet("");
    }
}

unsigned ComplexFramedReaderSettings::numOfChannels()
{
    return ui->spNumOfChannels->value();
}

NumberFormat ComplexFramedReaderSettings::numberFormat()
{
    return ui->nfBox->currentSelection();
}

Endianness ComplexFramedReaderSettings::endianness()
{
    return ui->endiBox->currentSelection();
}

QByteArray ComplexFramedReaderSettings::syncWord()
{
    QString text = ui->leSyncWord->text().remove(' ');

    // check if nibble is missing
    if (text.size() % 2 == 1)
    {
        return QByteArray();
    }
    else
    {
        return QByteArray::fromHex(text.toLatin1());
    }
}

void ComplexFramedReaderSettings::onSyncWordEdited()
{
    updateSyncWordAscii();
    // TODO: emit with a delay so that error message doesn't flash!
    emit syncWordChanged(syncWord());
}

void ComplexFramedReaderSettings::updateSyncWordAscii()
{
    QByteArray bytes = syncWord();
    QString asciiText;

    for (int i = 0; i < bytes.size(); ++i)
    {
        if (i > 0)
        {
            asciiText += ' ';  // Add space between characters
        }
        
        unsigned char byte = static_cast<unsigned char>(bytes[i]);
        // Display printable ASCII characters, otherwise show '.'
        if (byte >= 32 && byte <= 126)
        {
            asciiText += QChar(byte);
        }
        else
        {
            asciiText += '.';
        }
    }

    ui->lblSyncWordAscii->setText(asciiText);
}

ComplexFramedReaderSettings::SizeFieldType ComplexFramedReaderSettings::sizeFieldType() const
{
    return static_cast<SizeFieldType>(fbGroup.checkedId());
}

unsigned ComplexFramedReaderSettings::fixedFrameSize() const
{
    return ui->spSize->value();
}

bool ComplexFramedReaderSettings::isChecksumEnabled()
{
    return ui->cbChecksum->isChecked();
}

bool ComplexFramedReaderSettings::isDebugModeEnabled()
{
    return ui->cbDebugMode->isChecked();
}

void ComplexFramedReaderSettings::saveSettings(QSettings* settings)
{
    settings->beginGroup(SettingGroup_ComplexFrame);
    settings->setValue(SG_ComplexFrame_NumOfChannels, numOfChannels());
    settings->setValue(SG_ComplexFrame_NumberFormat, numberFormatToStr(numberFormat()));
    settings->setValue(SG_ComplexFrame_Endianness,
                       endianness() == LittleEndian ? "little" : "big");
    settings->setValue(SG_ComplexFrame_FrameStart, ui->leSyncWord->text());
    QString sizeFieldStr;
    if (sizeFieldType() == SizeFieldType::Field1Byte)
    {
        sizeFieldStr = "field1byte";
    }
    else if (sizeFieldType() == SizeFieldType::Field2Byte)
    {
        sizeFieldStr = "field2byte";
    }
    else
    {
        sizeFieldStr = "fixed";
    }
    settings->setValue(SG_ComplexFrame_SizeFieldType, sizeFieldStr);
    settings->setValue(SG_ComplexFrame_FixedFrameSize, fixedFrameSize());
    settings->setValue(SG_ComplexFrame_Checksum, ui->cbChecksum->isChecked());
    settings->setValue(SG_ComplexFrame_DebugMode, ui->cbDebugMode->isChecked());
    settings->endGroup();
}

void ComplexFramedReaderSettings::loadSettings(QSettings* settings)
{
    settings->beginGroup(SettingGroup_ComplexFrame);

    // load number of channels
    ui->spNumOfChannels->setValue(
        settings->value(SG_ComplexFrame_NumOfChannels, numOfChannels()).toInt());

    // load number format
    NumberFormat nfSetting =
        strToNumberFormat(settings->value(SG_ComplexFrame_NumberFormat,
                                          QString()).toString());
    if (nfSetting == NumberFormat_INVALID) nfSetting = numberFormat();
    ui->nfBox->setSelection(nfSetting);

    // load endianness
    QString endiannessSetting =
        settings->value(SG_ComplexFrame_Endianness, QString()).toString();
    if (endiannessSetting == "little")
    {
        ui->endiBox->setSelection(LittleEndian);
    }
    else if (endiannessSetting == "big")
    {
        ui->endiBox->setSelection(BigEndian);
    } // else don't change

    // load frame start
    QString frameStartSetting =
        settings->value(SG_ComplexFrame_FrameStart, ui->leSyncWord->text()).toString();
    auto validator = ui->leSyncWord->validator();
    validator->fixup(frameStartSetting);
    int pos = 0;
    if (validator->validate(frameStartSetting, pos) != QValidator::Invalid)
    {
        ui->leSyncWord->setText(frameStartSetting);
    }

    // load frame size type and fixed value
    ui->spSize->setValue(
        settings->value(SG_ComplexFrame_FixedFrameSize, ui->spSize->value()).toInt());

    QString sizeFieldStr = settings->value(SG_ComplexFrame_SizeFieldType, "").toString();
    if (sizeFieldStr == "fixed")
    {
        ui->rbFixedSize->setChecked(true);
    }
    else if (sizeFieldStr == "field1byte")
    {
        ui->rbSize1Byte->setChecked(true);
    }
    else if (sizeFieldStr == "field2byte")
    {
        ui->rbSize2Byte->setChecked(true);
    } // ignore invalid value

    // load checksum
    ui->cbChecksum->setChecked(
        settings->value(SG_ComplexFrame_Checksum, ui->cbChecksum->isChecked()).toBool());

    // load debug mode
    ui->cbDebugMode->setChecked(
        settings->value(SG_ComplexFrame_DebugMode, ui->cbDebugMode->isChecked()).toBool());

    settings->endGroup();
}
