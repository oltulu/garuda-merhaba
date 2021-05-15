/**********************************************************************
 *  garudawelcome.h
 **********************************************************************
 * Copyright (C) 2020 Garuda Authors
 *          Garuda Linux <http://garudalinux.in>
 *
 * This file is part of garuda-welcome.
 *
 * garuda-welcome is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * garuda-welcome is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with garuda-welcome.  If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************/

#ifndef GarudaWELCOME_H
#define GarudaWELCOME_H

#include <QMessageBox>

namespace Ui {
class garudawelcome;
}

struct Result {
    int exitCode;
    QString output;
};

class garudawelcome : public QDialog {
    Q_OBJECT

public:
    explicit garudawelcome(QWidget* parent = 0);
    ~garudawelcome();

    Result runCmd(QString cmd, bool include_stderr = true);
    QString getVersion();
    QString getCodename();
    QString getDescription();

    QString version,codename,description;
    QString output;

    void setup();
    bool isFirstLaunch();

private slots:
    void on_buttonAbout_clicked();

    void on_checkBox_clicked(bool checked);

    void on_buttonForum_clicked();

    void on_buttonWebsite_clicked();

    void on_buttonGitlab_clicked();

    void on_buttonGarudaSettings_clicked();

    void on_buttonTimeshift_clicked();

    void on_buttonPartition_clicked();

    void on_buttonInstallGaruda_clicked();

    void on_buttonChroot_clicked();

    void on_buttonSystemCleaner_clicked();

    void on_buttonSoftware_clicked();

    void on_buttonBootOptions_clicked();

    void on_buttonNetworkAssistant_clicked();

    void on_buttonRepository_clicked();

    void on_buttonTelegram_clicked();

    void on_buttonTwitter_clicked();

    void on_buttonBitwarden_clicked();

    void on_buttonPrivatebin_clicked();

    void on_buttonCryptpad_clicked();

    void on_buttonBigbluebutton_clicked();

    void on_buttonNextcloud_clicked();

    void on_buttonStatping_clicked();

    void on_buttonWhoogle_clicked();

    void on_buttonSearx_clicked();

    void on_buttonWiki_clicked();

    void on_buttonElement_clicked();

    void on_buttonfaq_clicked();

    void on_buttonGarudaAssistant_clicked();

    void on_buttonGarudaGamer_clicked();

    void on_buttonBootRepair_clicked();

    void on_buttonQwikaccess_clicked();

    void on_pushButton_setupassistant_clicked();

private:
    Ui::garudawelcome* ui;
    bool checkAndInstall(QString packagename);
};

#endif // GarudaSNAPSHOT_H
