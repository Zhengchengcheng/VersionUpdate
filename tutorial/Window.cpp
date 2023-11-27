/*
 * Copyright (c) 2014-2016 Alex Spataru <alex_spataru@outlook.com>
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#include "Window.h"
#include "ui_Window.h"

#include <QDebug>
#include <QSimpleUpdater.h>

//==============================================================================
// Define the URL of the Update Definitions file
//==============================================================================

static const QString DEFS_URL = "https://raw.githubusercontent.com/"
                                "alex-spataru/QSimpleUpdater/master/tutorial/"
                                "definitions/updates.json";

static const QString DEFS_URL2 = "https://raw.githubusercontent.com/"
                                 "OrangeCiiCii/VersionUpdate/main/"
                                 "definitions/updates2.json";

//==============================================================================
// Window::Window
//==============================================================================

Window::Window(QWidget *parent)
    : QMainWindow(parent)
{
    m_ui = new Ui::Window;
    m_ui->setupUi(this);
    //   m_ui->label_4->setText("当前的版本号: "+QCoreApplication::applicationVersion());
    setWindowTitle("在线升级窗口");
    setWindowIcon(QIcon(":/source/update.png"));

    /* QSimpleUpdater is single-instance */
    m_updater = QSimpleUpdater::getInstance();

    /* Check for updates when the "Check For Updates" button is clicked */
    connect(m_updater, SIGNAL(checkingFinished(QString)), this, SLOT(updateChangelog(QString)));
    connect(m_updater, SIGNAL(appcastDownloaded(QString, QByteArray)), this, SLOT(displayAppcast(QString, QByteArray)));

    /* React to button clicks */
    connect(m_ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(m_ui->checkButton, SIGNAL(clicked()), this, SLOT(checkForUpdates()));

    /* Resize the dialog to fit */
    setMinimumSize(minimumSizeHint());
    resize(minimumSizeHint());

}

//==============================================================================
// Window::~Window
//==============================================================================

Window::~Window()
{
    delete m_ui;
}

//==============================================================================
// Window::checkForUpdates
//==============================================================================

void Window::checkForUpdates()
{
    /* Get settings from the UI */
    //   QString version = m_ui->installedVersion->text();

    bool notifyOnFinish = m_ui->showAllNotifcations->isChecked();
    bool notifyOnUpdate = m_ui->showUpdateNotifications->isChecked();
    bool downloaderEnabled = m_ui->enableDownloader->isChecked();
    bool customAppcast = m_ui->customAppcast->isChecked();
    bool mandatoryUpdate = m_ui->mandatoryUpdate->isChecked();

    /* Apply the settings */
    //   m_updater->setModuleVersion(DEFS_URL, version);
    m_updater->setNotifyOnFinish(DEFS_URL, notifyOnFinish);
    m_updater->setNotifyOnUpdate(DEFS_URL, notifyOnUpdate);
    m_updater->setDownloaderEnabled(DEFS_URL, downloaderEnabled);
    m_updater->setUseCustomAppcast(DEFS_URL, customAppcast);
    m_updater->setMandatoryUpdate(DEFS_URL, mandatoryUpdate);

    /* Check for updates */
    m_updater->checkForUpdates(DEFS_URL);
}

//==============================================================================
// Window::updateChangelog
//==============================================================================

void Window::updateChangelog(const QString &url)
{
    if (url == DEFS_URL)
        m_ui->changelogText->setText(m_updater->getChangelog(url));
}

//==============================================================================
// Window::displayAppcast
//==============================================================================

void Window::displayAppcast(const QString &url, const QByteArray &reply)
{
    if (url == DEFS_URL)
    {
        QString text = "This is the downloaded appcast: <p><pre>" + QString::fromUtf8(reply)
                + "</pre></p><p> If you need to store more information on the "
                  "appcast (or use another format), just use the "
                  "<b>QSimpleUpdater::setCustomAppcast()</b> function. "
                  "It allows your application to interpret the appcast "
                  "using your code and not QSU's code.</p>";

        m_ui->changelogText->setText(text);
    }
}
