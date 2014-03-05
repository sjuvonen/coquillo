import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

MainWindow {
  id: mainwindow
  visible: true
  width: 1000
  height: 600

  SplitView {
    id: splitter

    anchors {
      fill: parent
    }

    TabView {
      id: tools
      Layout.minimumWidth: 200

      anchors {
        margins: 3
      }

      Tab {
        title: "Directories"
        FileBrowser {
          id: browser
        }
      }
    }

    MetaDataView {
      id: mainView
      Layout.minimumWidth: 200
      Layout.fillWidth: true

      model: metaData
    }

    TabView {
      id: editor
      Layout.minimumWidth: 280

      Tab {
        title: "Tags"

        BasicTags {
          id: editor
        }
      }
    }
  }
}
