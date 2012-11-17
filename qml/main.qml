import QtQuick 1.1
import com.nokia.meego 1.0

PageStackWindow {
    id: appWindow
    showStatusBar: !settings.fullScreen || pageStack.currentPage != mainPage
    showToolBar: !settings.fullScreen || pageStack.currentPage != mainPage

    // Export the settings, so the SettingsPage can access its properties
    property alias settings: mainPage.settings

    initialPage: mainPage

    MainPage {
        id: mainPage
    }
}
