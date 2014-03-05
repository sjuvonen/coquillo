import QtQuick 2.2
import QtQuick.Controls 1.1

TableView {
  id: metadataview

  TableViewColumn {
    title: "Title"
    role: "title"
  }

  TableViewColumn {
    title: "Filename"
    role: "filename"
  }
}
