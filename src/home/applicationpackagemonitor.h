/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of mhome.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#ifndef APPLICATIONPACKAGEMONITOR_H
#define APPLICATIONPACKAGEMONITOR_H

#include <QObject>
#include <QDBusConnection>

/*!
 * ApplicationPackageMonitor listens dbus signals from Package Manager to be used
 * updating Launcher Button statuses.
 */
class ApplicationPackageMonitor : public QObject
{
    Q_OBJECT
public:
    /*!
     * Initializes ApplicationPackageMonitor object. Connects dbus signals to slots.
     */
    ApplicationPackageMonitor();
    /*!
     * Destroys ApplicationPackageMonitor.
     */
    virtual ~ApplicationPackageMonitor();

signals:
    /*!
     * Status of download progress of package being installed.
     *
     *\param desktopEntryName is name of desktop entry file.
     *\param bytesLoaded is current amount of bytes downloaded.
     *\param bytesTotal is size of downloading package in bytes.
     */
    void downloadProgress(const QString &packageName, const QString &packageExtraPath, int bytesLoaded, int bytesTotal);
    /*!
     * Status of install progress of package being installed.
     *
     *\param desktopEntryName is name of desktop entry file.
     *\param percentage is install completion level.
     */
    void installProgress(const QString &packageName, const QString &packageExtraPath, int percentage);
    /*!
     * Notifies about success in installing and downloading package.
     *
     *\param desktopEntryName is name of desktop entry file.
     */
    void operationSuccess(const QString &packageName, const QString &packageExtraPath);
    /*!
     * Notifies about error in installing and downloading package.
     *
     *\param desktopEntryName is name of desktop entry file.
     *\param error is string format description of error occured.
     */
    void operationError(const QString &packageName, const QString &packageExtraPath, const QString& error);

private slots:
    /*!
     * Slot to handle PackageManagers download_progress signal.
     */
    void packageDownloadProgress(const QString& operation, const QString& packageName, const QString& packageVersion, int already, int total);
    /*!
     * Slot to handle PackageManagers operation_started signal.
     */
    void packageOperationStarted(const QString& operation, const QString& packageName, const QString& packageVersion);
    /*!
     * Slot to handle PackageManagers operation_progress signal.
     */
    void packageOperationProgress(const QString& operation, const QString& packageame, const QString& packageVersion, int percentage);
    /*!
     * Slot to handle PackageManagers operation_complete signal.
     */
    void packageOperationComplete(const QString& operation, const QString& packageName, const QString& packageVersion, const QString& error, bool need_reboot);

private:

    //! Stores package properties
    class PackageProperties {
    public:
        PackageProperties() :
            desktopEntryName(QString()),
            installing(false) {};

        QString desktopEntryName;
        bool installing;
    };

    /*!
     * Returns PackageProperties for package name and sets desktop entry path if one is found.
     *
     *\param name of package being installed
     *\return PackageProperties object.
     */
    PackageProperties & activePackageProperties(const QString packageName);

    /*!
     * Checks that package has desktop entry name property and operation is install or upgrade.
     *
     *\param properties of package being installed.
     *\param type of the current operation.
     */
    bool isValidOperation(const PackageProperties &properties, const QString &operation);

    /*!
     * Returns desktop entry path from APPLICATIONS_DIR/installer-extra/ or if it doesn't
     * exist empty QString.
     *
     *\param name of the package being installed
     *\return path to desktop file in installer-extra directory.
     */
    QString desktopEntryName(const QString &packageName);

    //! Mapping of installing package names and installing phase properties
    QMap<QString, PackageProperties> activePackages;

    //! DBus connection to system bus
    QDBusConnection con;

#ifdef UNIT_TEST
    friend class Ut_ApplicationPackageMonitor;
#endif

};

#endif
