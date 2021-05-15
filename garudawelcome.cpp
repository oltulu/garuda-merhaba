/**********************************************************************
 *  garudawelcome.cpp
 **********************************************************************
 * Copyright (C) 2015 Garuda Authors
 *
 * Authors: Adrian
 *          Paul David Callahan
 *          Dolphin Oracle
 *          Garuda Linux <http://garudalinux.org>
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

#include "garudawelcome.h"
#include "ui_garudawelcome.h"

#include <QDebug>
#include <QFile>
#include <QProcess>
#include <QSettings>

garudawelcome::garudawelcome(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::garudawelcome)
{
    ui->setupUi(this);
    setup();
}

garudawelcome::~garudawelcome()
{
    delete ui;
}

bool garudawelcome::isFirstLaunch()
{
    QSettings settings;

    if (settings.value("garuda-welcome/firstRun").isNull()) {
        settings.setValue("garuda-welcome/firstRun", false);

        QProcess proc;
        proc.start("last", { "reboot" });
        proc.waitForFinished();
        QString out = proc.readAllStandardOutput();
        if (out.count("\n") <= 3)
            return true;
    }
    return false;
}

// do various setup tasks
void garudawelcome::setup()
{
    version = getVersion();
    codename = getCodename();
    description = getDescription();
    QFont f( "Noto Sans", 16, QFont::Bold);
    ui->labelTitle->setFont(f);
    ui->labelTitle->setText(description + " " + codename);
    this->setWindowTitle(tr("Garuda  Linux'a Hoşgeldiniz"));
    if (system("[ -f ~/.config/autostart/garuda-welcome.desktop ]") == 0) {
        ui->checkBox->setChecked(true);
    } else {
        ui->checkBox->setChecked(false);
    }
    // if running live
    QString test = runCmd("df -T / |tail -n1 |awk '{print $2}'").output;
    qDebug() << test;
    if (test == "aufs" || test == "overlay") {
        ui->checkBox->hide();
        ui->pushButton_setupassistant->hide();
    } else {
        ui->buttonInstallGaruda->hide();
        ui->buttonChroot->hide();
        ui->buttonBootRepair->hide();
        ui->buttonTimeshift->setText("Timeshift");

        if (isFirstLaunch()) {
            if (QFile::exists("/usr/bin/setup-assistant")) {

                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(nullptr, tr("Welcome to Garuda Linux!"), tr("Garuda Linux'a hoş geldiniz! Kurulumunuzu bitirmek için kurulum asistanını başlatmak ister misiniz?"),
                    QMessageBox::Yes | QMessageBox::No);
                if (reply == QMessageBox::Yes) {
                    system("setup-assistant");
                }
            }
        }
    }



    // We want to sync the widths of every button in tab_3
    // Otherwise the alignment between the 2 groupboxes is non existent and it makes dr460nf1r3 and I sad
    int width = 0;
    auto buttons = this->ui->tab_3->findChildren<QToolButton*>();
    for (auto element : qAsConst(buttons)) {
        // Let QT calculate the correct width for us.
        int b_width = element->sizeHint().width();
        if (b_width > width)
            width = b_width;
    }

    for (auto button : qAsConst(buttons)) {
        // Now that we know the size, we can force it.
        button->setMinimumWidth(width);
        // We need to force the maximum height here, otherwise weird things happen if you resize the window.
        // This would not be necessary if we used "fixed" size qtoolbuttons, but in that case the qtoolbuttons would be a different height sometimes
        button->setMaximumHeight(button->sizeHint().height());
    }

    this->adjustSize();
}

// Util function for getting bash command output and error code
Result garudawelcome::runCmd(QString cmd, bool include_stderr)
{
    QProcess proc(this);
    if (include_stderr)
    proc.setProcessChannelMode(QProcess::MergedChannels);
    proc.start("/bin/bash", QStringList() << "-c" << cmd);
    // Wait forever
    proc.waitForFinished(-1);
    Result result = { proc.exitCode(), proc.readAll().trimmed() };
    return result;
}

// Get version of the program
QString garudawelcome::getVersion()
{
    QString cmd = QString("grep -oP '(?<=DISTRIB_RELEASE=).*' /etc/lsb-release");
    return runCmd(cmd).output;
}

// Get distribution name and codename of the program
QString garudawelcome::getCodename()
{
    QString cmd = QString("grep -oP '(?<=DISTRIB_CODENAME=).*' /etc/lsb-release");
    return runCmd(cmd).output;
}

QString garudawelcome::getDescription()
{
    QString cmd = QString("grep -oP '(?<=DISTRIB_DESCRIPTION=).*' /etc/lsb-release");
    return runCmd(cmd).output;
}

// About button clicked
void garudawelcome::on_buttonAbout_clicked()
{
    this->hide();
    QMessageBox msgBox(QMessageBox::NoIcon,
        tr("Garuda Welcome Hakkında"), "<p align=\"center\"><b><h2>" + tr("Garuda Welcome") + "</h2></b></p><p align=\"center\">" + tr("Version: ") + version + "</p><p align=\"center\"><h3>" + tr("Garuda Linux'ta bir karşılama ekranı görüntülemek için program") + "</h3></p><p align=\"center\"><a href=\"http://www.garudalinux.org/\">http://www.garudalinux.org/</a><br /></p><p align=\"center\">" + tr("Copyright (c) Garuda Linux") + "<br /><br /></p>", 0, this);
    msgBox.addButton(tr("License"), QMessageBox::AcceptRole);
    msgBox.addButton(tr("Cancel"), QMessageBox::NoRole);
    if (msgBox.exec() == QMessageBox::AcceptRole) {
        QString cmd = QString("xdg-open https://www.gnu.org/licenses/gpl-3.0.html");
        system(cmd.toUtf8());
    }
    this->show();
}

// Add/remove autostart at login
void garudawelcome::on_checkBox_clicked(bool checked)
{
    if (checked) {
        system("cp /usr/share/applications/garuda-welcome.desktop ~/.config/autostart/garuda-welcome.desktop");
    } else {
        system("rm ~/.config/autostart/garuda-welcome.desktop >/dev/null 2>&1");
    }
}

// Launch Forum in browser
void garudawelcome::on_buttonForum_clicked()
{
    this->hide();
    system("xdg-open https://forum.aylinux.org/");
    this->show();
}

void garudawelcome::on_buttonWebsite_clicked()
{
    this->hide();
    system("xdg-open https://www.aylinux.org/");
    this->show();
}

void garudawelcome::on_buttonGitlab_clicked()
{
    this->hide();
    system("xdg-open https://notabug.org/aylinux");
    this->show();
}

void garudawelcome::on_buttonRepository_clicked()
{
    this->hide();
    system("xdg-open https://aur.chaotic.cx");
    this->show();
}

void garudawelcome::on_buttonTelegram_clicked()
{
    this->hide();
    system("xdg-open https://telegram.me/aylinuxorg");
    this->show();
}

void garudawelcome::on_buttonTwitter_clicked()
{
    this->hide();
    system("xdg-open https://twitter.com/aylinuxorg");
    this->show();
}

void garudawelcome::on_buttonBitwarden_clicked()
{
    this->hide();
    system("xdg-open https://bitwarden.aylinuxorg.org/");
    this->show();
}

void garudawelcome::on_buttonPrivatebin_clicked()
{
    this->hide();
    system("xdg-open https://bin.aylinuxorg.org/");
    this->show();
}

void garudawelcome::on_buttonCryptpad_clicked()
{
    this->hide();
    system("xdg-open https://pad.aylinuxorg.org/");
    this->show();
}

void garudawelcome::on_buttonBigbluebutton_clicked()
{
    this->hide();
    system("xdg-open https://meet.garudalinux.org/");
    this->show();
}

void garudawelcome::on_buttonNextcloud_clicked()
{
    this->hide();
    system("xdg-open https://cloud.aylinuxorg.org/");
    this->show();
}

void garudawelcome::on_buttonStatping_clicked()
{
    this->hide();
    system("xdg-open https://stats.uptimerobot.com/RwpoGIrmnJ");
    this->show();
}

void garudawelcome::on_buttonWhoogle_clicked()
{
    this->hide();
    system("xdg-open https://search.aylinuxorg.org/");
    this->show();
}

void garudawelcome::on_buttonSearx_clicked()
{
    this->hide();
    system("xdg-open https://searx.aylinuxorg.org/");
    this->show();
}

void garudawelcome::on_buttonWiki_clicked()
{
    this->hide();
    system("xdg-open https://wiki.aylinuxorg.org/");
    this->show();
}

void garudawelcome::on_buttonElement_clicked()
{
    this->hide();
    system("xdg-open https://element.aylinuxorg.org/");
    this->show();
}

void garudawelcome::on_buttonfaq_clicked()
{
    this->hide();
    system("xdg-open https://forum.aylinuxorg.org/");
    this->show();
}

void garudawelcome::on_buttonGarudaSettings_clicked()
{
    if (!checkAndInstall("garuda-settings-manager"))
        return;
    this->hide();
    system("garuda-settings-manager");
    this->show();
}

void garudawelcome::on_buttonGarudaAssistant_clicked()
{
    if (!checkAndInstall("garuda-assistant"))
        return;
    this->hide();
    system("garuda-assistant");
    this->show();
}

void garudawelcome::on_buttonGarudaGamer_clicked()
{
    if (!checkAndInstall("garuda-gamer"))
        return;
    this->hide();
    system("garuda-gamer");
    this->show();
}

void garudawelcome::on_buttonBootOptions_clicked()
{
    if (!checkAndInstall("garuda-boot-options"))
        return;
    this->hide();
    system("pkexec env DISPLAY=$DISPLAY XAUTHORITY=$XAUTHORITY HOME=$HOME garuda-boot-options -style kvantum");
    this->show();
}

void garudawelcome::on_buttonBootRepair_clicked()
{
    if (!checkAndInstall("garuda-boot-repair"))
        return;
    this->hide();
    system("pkexec env DISPLAY=$DISPLAY XAUTHORITY=$XAUTHORITY HOME=$HOME garuda-boot-repair -style kvantum");
    this->show();
}

void garudawelcome::on_buttonNetworkAssistant_clicked()
{
    if (!checkAndInstall("garuda-network-assistant"))
        return;
    this->hide();
    system("pkexec env DISPLAY=$DISPLAY XAUTHORITY=$XAUTHORITY HOME=$HOME garuda-network-assistant -style kvantum");
    this->show();
}

void garudawelcome::on_buttonTimeshift_clicked()
{
    if (!checkAndInstall("timeshift"))
        return;
    this->hide();
    system("timeshift-launcher");
    this->show();
}

void garudawelcome::on_buttonPartition_clicked()
{
    this->hide();
    if (QFile::exists("/usr/bin/gnome-disk-utility")) {
        system("gnome-disk-utility");
    } else if (QFile::exists("/usr/bin/partitionmanager")) {
        system("partitionmanager");
    } else if (QFile::exists("/usr/bin/gparted")) {
        system("gparted");
    } else {
        if (checkAndInstall("gparted"))
            system("gparted");
    }

    this->show();
}

void garudawelcome::on_buttonSystemCleaner_clicked()
{
    this->hide();
    if (QFile::exists("/usr/bin/stacer")) {
        system("stacer");
    } else if (QFile::exists("/usr/bin/bleachbit")) {
        system("bleachbit");
    } else if (QFile::exists("/usr/bin/sweeper")) {
        system("sweeper");
    } else {
        if (checkAndInstall("stacer"))
            system("stacer");
    }

    this->show();
}

void garudawelcome::on_buttonSoftware_clicked()
{
    this->hide();
    if (QFile::exists("/usr/bin/mps_ui.py)) {
        system("aysu mps_ui.py");
    } else if (QFile::exists("/usr/bin/bauh")) {
        system("bauh");
    }

    this->show();
}

void garudawelcome::on_buttonInstallGaruda_clicked()
{
    this->hide();
    system("python3 /opt/Aylinux-Yukleyici/kur.py");
    this->show();
}

void garudawelcome::on_buttonChroot_clicked()
{
    this->hide();
    system("alacritty --command pkexec bash -c 'aylinux-chroot -a && read p'");
    this->show();
}

void garudawelcome::on_buttonQwikaccess_clicked()
{
    if (!checkAndInstall("qwikaccess-git"))
        return;
    this->hide();
    system("qwikaccess");
    this->show();
}

void garudawelcome::on_pushButton_setupassistant_clicked()
{
    if (!checkAndInstall("garuda-setup-assistant"))
        return;
    this->hide();
    system("setup-assistant");
    this->show();
}

bool garudawelcome::checkAndInstall(QString package)
{
    // Not async, don't really care either tho, not my problem
    // Assumption: package is a safe string
    auto output = runCmd("paket sor" + package, false);

    // If it's already installed, we are good to go
    if (output.output == package || output.output == package + "-git")
        return true;

    if (!QFile::exists("/usr/bin/pamac-installer")) {
        QMessageBox::warning(this, tr("Başlarken hata ") + package, tr("The package \"%package%\" is not installed and pamac-installer is missing.").replace("%package%", package));
        return false;
    }

    this->hide();
    runCmd("aysu paket kur " + package);

    // Checking pamac-installer's exit code alone is not enough. For some reason it decides to return 0 even tho it failed sometimes
    output = runCmd("paket sor " + package,  false);
    if (output.output == package)
        return true;
    else {
        this->show();
        return false;
    }
}
