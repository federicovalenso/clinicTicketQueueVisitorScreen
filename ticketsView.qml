import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Window {
    id: ticketsWindow
    visible: true
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight
    title: qsTr("Талоны")
    color: "black"
    visibility: Window.FullScreen
    onActiveChanged: {
        ticketsWindow.visibility = activeFocusItem ? Window.FullScreen : Window.Minimized;
    }

    Transition {
        id: addTransition
        function getStartColor() {
            return ViewTransition.index % 2 == 0 ? lwTickets.evenColor : lwTickets.oddColor
        }
        function getEndColor() {
            return Qt.lighter(getStartColor(), 2.0)
        }
        SequentialAnimation {
            NumberAnimation { property: "x"; from: 2000; to:0 ; duration: 1000; easing.type: Easing.Linear }
            SequentialAnimation {
                PropertyAnimation {
                    property: "color"
                    from: addTransition.getStartColor()
                    to: addTransition.getEndColor()
                    duration: 300
                }
                PropertyAnimation {
                    property: "color"
                    from: addTransition.getEndColor()
                    to: addTransition.getStartColor()
                    duration: 300
                }
                loops: 5
            }
        }
    }

    Transition {
        id: removeTransition
        NumberAnimation {
            properties: "x";
            from: 0
            to: 2000;
            duration: 1000
        }
    }

    Transition {
        id: displacedTransition
        function getStartColor() {
            return ViewTransition.index % 2 == 0 ? lwTickets.oddColor : lwTickets.evenColor
        }
        function getEndColor() {
            return ViewTransition.index % 2 == 0 ? lwTickets.evenColor : lwTickets.oddColor
        }
        ParallelAnimation {
            NumberAnimation { properties: "y"; duration: 1000 }
            PropertyAnimation {
                property: "color"
                from: displacedTransition.getStartColor()
                to: displacedTransition.getEndColor()
                duration: 1000
            }
        }
    }

    Component {
        id: lwHeader
        Row {
            width: parent.width
            height: ticketsWindow.height / 5
            Rectangle {
                width: parent.width / 2
                height: parent.height
                color: "black"
                Image {
                    y: parent.height * 0.05
                    height: parent.height * 0.9
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:///img/mdcLogo.svg"
                }
                Text {
                    anchors.centerIn: parent;
                    color: "Orange";
                    text: qsTr("Талон");
                    font.bold: true;
                    font.pointSize: 72
                }
            }
            Rectangle {
                width: parent.width / 2
                height: parent.height
                color: "black"
                Text {
                    anchors.centerIn: parent;
                    color: "Orange";
                    text: qsTr("Окно");
                    font.bold: true;
                    font.pointSize: 72
                }
            }
        }
    }

    Component {
        id: lwDelegate
        Rectangle {
            width: parent.width
            height: ticketsWindow.height / 5
            color: {
                return (index % 2) == 0 ? "#34ace0" : "#33d9b2"
            }
            Rectangle {
                id: numberRect
                width: parent.width / 2
                height: parent.height
                color: parent.color
                anchors.left: parent.left
                Text {
                    text: number
                    font.bold: true
                    font.pointSize: 72.0
                    anchors.centerIn: parent
                }
            }
            Rectangle {
                width: parent.width / 2
                height: parent.height
                color: parent.color
                anchors.left: numberRect.right
                Text {
                    text: window
                    font.bold: true
                    font.pointSize: 72.0
                    anchors.centerIn: parent
                }
            }
        }
    }

    ListView {
        id: lwTickets
        width: ticketsWindow.width
        height: ticketsWindow.height
        flickableDirection: Flickable.AutoFlickDirection
        header: lwHeader
        delegate: lwDelegate
        model: ticketsModel
        add: addTransition
        remove: removeTransition
        removeDisplaced: displacedTransition
        readonly property color oddColor: "#33d9b2"
        readonly property color evenColor: "#34ace0"
    }
}

