import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

Rectangle {
  id: basictags

  GridLayout {
    id: layout
    columns: 3
    rowSpacing: 0

    anchors {
      top: parent.top
      left: parent.left
      right: parent.right
      margins: 5
    }

    /*
     * Title
     */

    TagLabel {
      text: "Title"
      horizontalAlignment: Text.AlignRight
    }

    TextField {
      Layout.fillWidth: true
    }

    TagButton {
      anchors {
        right: parent.right
      }
    }

    /*
     * Artist
     */

    TagLabel {
      text: "Artist"
    }

    TextField {
      id: tagArtist
      Layout.fillWidth: true
    }

    TagButton {

    }

    /*
     * Album
     */

    TagLabel {
      text: "Album"
    }

    TextField {
      Layout.fillWidth: true
    }

    TagButton {

    }

    /*
     * Year and disc number combined
     */
    TagLabel {
      text: "Year"
    }

    RowLayout {
      Layout.fillWidth: true

      SpinBox {
        minimumValue: 0
        maximumValue: 9999
      }

      TagButton {

      }

      Label {
        text: "Disc"
        Layout.fillWidth: true
      }

      SpinBox {
        minimumValue: 0
        maximumValue: 9999
      }
    }

    TagButton {

    }

    /*
     * Track number and total combined
     */

    TagLabel {
      text: "Number"
    }

    RowLayout {
      Layout.fillWidth: true

      SpinBox {
        minimumValue: 0
        maximumValue: 9999
      }

      TagLabel {
        text: " / "
        width: contentWidth
      }

      SpinBox {
        minimumValue: 0
        maximumValue: 9999
      }

      ToolButton {
        iconName: "view-sort-ascending"
      }
    }

    TagButton {

    }

    /*
     * Genre
     */

    TagLabel {
      text: "Genre"
    }

    TextField {
      Layout.fillWidth: true
    }

    TagButton {

    }

    /*
     * Comment
     */

    TagLabel {
      text: "Comment"
    }

    TextField {
      Layout.fillWidth: true
    }

    TagButton {

    }

    /*
     * URL
     */

    TagLabel {
      text: "URL"
    }

    TextField {
      Layout.fillWidth: true
    }

    TagButton {

    }

    /*
     * Original artist
     */

    TagLabel {
      text: "Orig. artist"
    }

    TextField {
      Layout.fillWidth: true
    }

    TagButton {

    }

    /*
     * Album artist
     */

    TagLabel {
      text: "Album artist"
    }

    TextField {
      Layout.fillWidth: true
    }

    TagButton {

    }

    /*
     * Composer
     */

    TagLabel {
      text: "Composer"
    }

    TextField {
      Layout.fillWidth: true
    }

    TagButton {

    }

    /*
     * Encoder
     */

    TagLabel {
      text: "Encoder"
    }

    TextField {
      Layout.fillWidth: true
    }

    TagButton {

    }


  }
}
