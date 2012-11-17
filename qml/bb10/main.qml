import QtQuick 1.0

Rectangle {
    id: appWindow
    width: 800
    height: 1280
    color: "#000000"
    // Export the settings, so the SettingsPage can access its properties
    property alias settings: mainPage.settings

    MainPage {
        id: mainPage
        x: 150
        y: 200
        width: 570
        height: 1280
        rotation: 0
    }


}
