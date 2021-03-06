/*
Launchy: Application Launcher
Copyright (C) 2007  Josh Karlin

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "UWPApp.h"

#ifdef Q_OS_WIN
#include <windows.h>

// #include <tchar.h>
// #include <VersionHelpers.h>

// #include <shlobj.h>
#include <Shobjidl.h>
#include <atlbase.h>
#include <propvarutil.h>

DEFINE_GUID(BHID_EnumItems, 0x94f60519, 0x2850, 0x4924, 0xaa, 0x5a, 0xd1, 0x5e, 0x84, 0x86, 0x80, 0x39);
DEFINE_GUID(BHID_PropertyStore, 0x0384e1a4, 0x1523, 0x439c, 0xa4, 0xc8, 0xab, 0x91, 0x10, 0x52, 0xf5, 0x86);

#endif

#include <QtGui>
#include <QFile>
#include <QDebug>

#include "LaunchyLib/PluginMsg.h"

#define PLUGIN_NAME "UWPApp"

UWPApp::UWPApp()
    : HASH_UWPAPP(qHash(QString(PLUGIN_NAME))) {

}

UWPApp::~UWPApp() {

}

int UWPApp::msg(int msgId, void* wParam, void* lParam) {
    int handled = 0;
    switch (msgId) {
    case MSG_INIT:
        init();
        handled = 1;
        break;
    case MSG_GET_LABELS:
        getLabels((QList<launchy::InputData>*) wParam);
        handled = 1;
        break;
    case MSG_GET_ID:
        getID((uint*)wParam);
        handled = 1;
        break;
    case MSG_GET_NAME:
        getName((QString*)wParam);
        handled = 1;
        break;
    case MSG_GET_RESULTS:
        getResults((QList<launchy::InputData>*) wParam, (QList<launchy::CatItem>*) lParam);
        handled = 1;
        break;
    case MSG_GET_CATALOG:
        getCatalog((QList<launchy::CatItem>*) wParam);
        handled = 1;
        break;
    case MSG_LAUNCH_ITEM:
        launchItem((QList<launchy::InputData>*) wParam, (launchy::CatItem*)lParam);
        handled = 1;
        break;
    //case MSG_EXTRACT_ICON:
        // extractIcon((launchy::CatItem*)wParam, (QIcon*)lParam);
        // handled = 1;
        //break;
    case MSG_HAS_DIALOG:
        // Set to true if you provide a gui
        // handled = 1;
        break;
    case MSG_DO_DIALOG:
        // This isn't called unless you return true to MSG_HAS_DIALOG
        // doDialog((QWidget*)wParam, (QWidget**)lParam);
        break;
    case MSG_END_DIALOG:
        // This isn't called unless you return true to MSG_HAS_DIALOG
        // endDialog((bool)wParam);
        break;

    default:
        break;
    }

    return handled;
}

void UWPApp::init() {

}

void UWPApp::getID(uint* id) {
    *id = HASH_UWPAPP;
}

void UWPApp::getName(QString* str) {
    *str = PLUGIN_NAME;
}

void UWPApp::setPath(const QString* path) {

}

void UWPApp::getCatalog(QList<launchy::CatItem>* items) {

    qDebug() << "UWPAppPlugin::getCatalog";

    CoInitialize(NULL);

    do {
        CComPtr<IShellItem> appFolder;
        if (FAILED(SHCreateItemFromParsingName(L"shell:AppsFolder",
                                               nullptr,
                                               IID_PPV_ARGS(&appFolder)))) {
            qWarning() << "fail to open shell:AppsFolder";
            break;
        }

        qDebug() << "succeed to open shell::AppsFolder";

        CComPtr<IEnumShellItems> enumShellItems;
        if (FAILED(appFolder->BindToHandler(nullptr,
                                            BHID_EnumItems,
                                            IID_PPV_ARGS(&enumShellItems)))) {
            qWarning() << "fail to bind to handler";
            break;
        }

        qDebug() << "succeed to bind to handler";

        PROPERTYKEY pkLauncherAppState;
        PSGetPropertyKeyFromName(L"System.Launcher.AppState", &pkLauncherAppState);
        PROPERTYKEY pkSmallLogoPath;
        PSGetPropertyKeyFromName(L"System.Tile.SmallLogoPath", &pkSmallLogoPath);
        PROPERTYKEY pkAppUserModelID;
        PSGetPropertyKeyFromName(L"System.AppUserModel.ID", &pkAppUserModelID);
        PROPERTYKEY pkInstallPath;
        PSGetPropertyKeyFromName(L"System.AppUserModel.PackageInstallPath", &pkInstallPath);

        const size_t pvslen = 512;
        CComHeapPtr<wchar_t> pvs;
        pvs.Allocate(pvslen);

        IShellItem* shellItemNext = nullptr;
        while (enumShellItems->Next(1, &shellItemNext, nullptr) == S_OK) {
            CComPtr<IShellItem> shellItem = shellItemNext;

            CComPtr<IPropertyStore> propertyStore;
            if (FAILED(shellItem->BindToHandler(NULL,
                                                BHID_PropertyStore,
                                                IID_PPV_ARGS(&propertyStore)))) {
                continue;
            }

            PROPVARIANT pv;
            PropVariantInit(&pv);

            // UWP app always has valid "Launcher.AppState"
            if (FAILED(propertyStore->GetValue(pkLauncherAppState, &pv))) {
                continue;
            }
            else {
                memset(pvs, 0, sizeof(wchar_t) * pvslen);
                PropVariantToString(pv, pvs, pvslen);
                if (std::wcslen(pvs) == 0) {
                    continue;
                }
            }

            QString shortName;
            QString fullPath;
            QString installPath;
            QString iconPath;

            CComHeapPtr<wchar_t> name;
            if (SUCCEEDED(shellItem->GetDisplayName(SIGDN_NORMALDISPLAY, &name))) {
                shortName = QString::fromWCharArray(name);
                qDebug() << "name: " << shortName;
            }

            PropVariantClear(&pv);
            if (SUCCEEDED(propertyStore->GetValue(pkAppUserModelID, &pv))) {
                memset(pvs, 0, sizeof(wchar_t) * pvslen);
                PropVariantToString(pv, pvs, pvslen);
                fullPath = QString::fromWCharArray(static_cast<wchar_t*>(pvs));
                qDebug() << " id: " << fullPath;
            }

            PropVariantClear(&pv);
            if (SUCCEEDED(propertyStore->GetValue(pkInstallPath, &pv))) {
                memset(pvs, 0, sizeof(wchar_t) * pvslen);
                PropVariantToString(pv, pvs, pvslen);
                installPath = QString::fromWCharArray(pvs);
                qDebug() << " install: " << installPath;
            }

            PropVariantClear(&pv);
            if (SUCCEEDED(propertyStore->GetValue(pkSmallLogoPath, &pv))) {
                memset(pvs, 0, sizeof(wchar_t) * pvslen);
                PropVariantToString(pv, pvs, pvslen);
                iconPath = QString::fromWCharArray(pvs);
                qDebug() << " logo: " << iconPath;
                iconPath = installPath + QDir::separator() + iconPath;
                iconPath = validateIconPath(iconPath);
                qDebug() << " logo(valiate): " << iconPath;
            }

            items->push_back(launchy::CatItem(fullPath,
                                              shortName,
                                              HASH_UWPAPP,
                                              iconPath));
        }

    } while (0);

    CoUninitialize();
}

void UWPApp::getLabels(QList<launchy::InputData>* inputData) {

}

void UWPApp::getResults(QList<launchy::InputData>* inputData, QList<launchy::CatItem>* results) {

}

void UWPApp::launchItem(QList<launchy::InputData>* id, launchy::CatItem* item) {
    // Specify the appropriate COM threading model
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

    IApplicationActivationManager* pAAM = nullptr;
    HRESULT hr = CoCreateInstance(CLSID_ApplicationActivationManager,
                                  nullptr,
                                  CLSCTX_INPROC_SERVER,
                                  IID_PPV_ARGS(&pAAM));
    if (FAILED(hr)) {
        qWarning() << "UWPAppPlugin::launchItem, Error creating CoCreateInstance & HR is" << hr;
        return;
    }

    DWORD pid = 0;
    hr = pAAM->ActivateApplication(item->fullPath.toStdWString().c_str(), L"", AO_NONE, &pid);
    if (FAILED(hr)) {
        qWarning() << "UWPAppPlugin::launchItem, Error in ActivateApplication call & HR is " << hr;
        return;
    }

    if (hr == 0) {
        qDebug() << "UWPAppPlugin::launchItem, Activated " << item->fullPath << " with pid " << pid;
    }

    CoUninitialize();
}

void UWPApp::extractIcon(launchy::CatItem* item, QIcon* icon) {
    QColor background;
    QStringList iconPaths = item->iconPath.split('\t');
    QString backgroundColor;

    if (iconPaths.size() > 1) {
        backgroundColor = iconPaths[1];
        qDebug() << "background color: " << backgroundColor;
    }

    if (backgroundColor.isEmpty() || backgroundColor == "transparent") {
        // Get theme color for background.
        DWORD colorizationColor;
        DWORD size = sizeof(DWORD);

        RegGetValue(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\DWM", L"ColorizationColor",
                    RRF_RT_REG_DWORD, 0, &colorizationColor, &size);

        BYTE r = (colorizationColor >> 16) & 0xFF;
        BYTE g = (colorizationColor >> 8) & 0xFF;
        BYTE b = colorizationColor & 0xFF;
        background.setRgb(r, g, b);
    }
    else if (backgroundColor.startsWith("#")) {
        background.setNamedColor(backgroundColor);
    }

    // Create image with background color
    QString path = iconPaths[0];
    QPixmap iconPixmap = QPixmap(path);
    // Compose proper image with icon alpha channel
    QImage iconImage = QImage(iconPixmap.size(), QImage::Format_ARGB32);
    iconImage.fill(background);
    QPainter painter(&iconImage);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawPixmap(0, 0, iconPixmap);
    icon->addPixmap(QPixmap::fromImage(iconImage));
}

void UWPApp::doDialog(QWidget* parent, QWidget** dialog) {

}

void UWPApp::endDialog(bool accept) {

}

QString UWPApp::validateIconPath(const QString& iconPath) {

    static const char* scales[] = {".scale-200.", ".scale-100.", ".scale-300.", ".scale-400."};

    QString iconPathBase = iconPath.section('.', 0, -2);
    QString iconPathExt = iconPath.section('.', -1);

    for (int i = 0; i < sizeof(scales)/sizeof(scales[0]); ++i) {
        QString path = iconPathBase + scales[i] + iconPathExt;
        if (QFile::exists(path)) {
            return path;
        }
    }

    return iconPath;
}
