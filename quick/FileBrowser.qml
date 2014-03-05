import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

Rectangle {
  id: browser
  width: 140

  ColumnLayout {
    id: layout

    anchors {
      fill: parent
      margins: 5
    }

    TextField {
      id: path
      Layout.fillWidth: true
    }

    TableView {
      id: directories
      Layout.fillWidth: true
      Layout.fillHeight: true
    }

    CheckBox {
      text: "Scan subdirectories"
      Layout.fillWidth: true
    }
  }
}
