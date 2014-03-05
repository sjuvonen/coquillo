import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

ApplicationWindow {
  id: mainwindow

  menuBar: MenuBar {
    Menu {
      title: "&Coquillo"

      MenuItem {
        action: Action {
          text: "&Quit"
          shortcut: "Ctrl+Q"
          onTriggered: Qt.quit()
        }
      }
    }
  }
}
