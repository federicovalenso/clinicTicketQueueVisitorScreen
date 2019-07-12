import QtGraphicalEffects 1.12
import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Window 2.11

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
            id: headerRow
            width: parent.width
            height: lwTickets.delegateHeight
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
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    height: parent.height
                    width: parent.width
                    color: "Orange";
                    text: qsTr("Талон");
                    font.bold: true
                    minimumPointSize: 24.0
                    font.pointSize: 120.0
                    fontSizeMode: Text.VerticalFit
                }
            }
            Rectangle {
                width: parent.width / 2
                height: parent.height
                color: "black"
                Text {
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    height: parent.height
                    width: parent.width
                    color: "Orange";
                    text: qsTr("Окно");
                    font.bold: true
                    minimumPointSize: 24.0
                    font.pointSize: 120.0
                    fontSizeMode: Text.VerticalFit
                }
            }
        }
    }

    Component {
        id: lwDelegate
        Row {
            spacing: 0
            width: parent.width
            height: lwTickets.delegateHeight
            property color color: {
                return (index % 2) == 0 ? "#34ace0" : "#33d9b2"
            }
            Rectangle {
                id: numberRect
                width: parent.width / 2
                height: parent.height
                color: parent.color
                Text {
                    text: number
                    verticalAlignment: Text.AlignVCenter
                    height: parent.height
                    font.bold: true
                    minimumPointSize: 24.0
                    font.pointSize: 120.0
                    fontSizeMode: Text.Fit
                    anchors.centerIn: parent
                }
            }
            Rectangle {
                width: parent.width / 2
                height: parent.height
                color: parent.color
                Text {
                    text: window
                    verticalAlignment: Text.AlignVCenter
                    height: parent.height
                    font.bold: true
                    minimumPointSize: 24.0
                    font.pointSize: 120.0
                    fontSizeMode: Text.Fit
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

        function calcHeightFromCount(delegatesCount) {
            return height / (parseFloat(delegatesCount) + 1)
        }
        property double delegateHeight: calcHeightFromCount(ticketsModel.getMaxTicketsCount())

        Connections {
            target: ticketsModel
            onMaxTicketChanged: {
                lwTickets.delegateHeight = lwTickets.calcHeightFromCount(number)
            }
        }
    }

    Rectangle {
        id: menuField
        color: "transparent"
        width: menu.width
        height: menu.height
        anchors.right: parent.right
        focus:  true
        activeFocusOnTab: true
        KeyNavigation.down: serverName
        KeyNavigation.tab: serverName

        Image {
            id: menu
            source: "qrc:///img/menu.png"
            anchors {
                right: parent.right
                top: parent.top
            }
        }
        MouseArea {
            anchors.fill: parent
            onClicked: settings.open()
        }

        ColorOverlay {
            anchors.fill: menu
            source: menu
            color: menuField.activeFocus ? "orange" : "transparent"
        }
        Keys.onReturnPressed: settings.open()
        Keys.onEnterPressed: settings.open()
    }

    Dialog {
        id: settings
        anchors.centerIn: parent
        modal: true
        title: qsTr("Редактирование настроек")
        palette.base: "#eff4ff"
        palette.text: "black"
        palette.window: "#603cba"
        palette.button: "#00aba9"
        font.pointSize: 12
        footer: settingButtons
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        KeyNavigation.down: serverName;

        Component.onCompleted: {
            serverName.text = appSettings.getServerName()
            port.text = appSettings.getPort()
            userName.text = appSettings.getUserName()
            password.text = appSettings.getPassword()
            maxRows.text = appSettings.getMaxRows()
        }


        function setSettings() {
            appSettings.setSettings(
                        serverName.text,
                        port.text,
                        userName.text,
                        password.text,
                        maxRows.text
                        )
            menuField.focus = true
        }

        onAccepted: setSettings()

        onRejected: menuField.focus = true

        ColumnLayout {
            width: parent.width
            height: parent.height

            GridLayout {
                columns: 2;
                width: parent.width
                columnSpacing: 10
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                Label {
                    text: qsTr("Имя сервера")
                    color: "#eff4ff"
                    font.bold: true
                }
                TextField {
                    id: serverName;
                    KeyNavigation.up: cancelBtn;
                    KeyNavigation.down: port;
                }
                Label {
                    text: qsTr("Порт")
                    color: "#eff4ff"
                    font.bold: true
                }
                TextField {
                    id: port
                    KeyNavigation.down: userName;
                    validator : RegExpValidator { regExp : /[0-9]+\.[0-9]+/ }
                }
                Label {
                    text: qsTr("Пользователь")
                    color: "#eff4ff"
                    font.bold: true
                }
                TextField {
                    id: userName
                    KeyNavigation.down: password;
                }
                Label {
                    text: qsTr("Пароль")
                    color: "#eff4ff"
                    font.bold: true
                }
                TextField {
                    id: password
                    KeyNavigation.down: maxRows
                    echoMode: TextInput.Password
                }
                Label {
                    text: qsTr("Талонов на экране (макс.)")
                    color: "#eff4ff"
                    font.bold: true
                }
                TextField {
                    id: maxRows
                    KeyNavigation.down: saveBtn;
                    validator : RegExpValidator { regExp : /[1-9]/ }
                }
            }

            DialogButtonBox {
                id: settingButtons
                Button {
                    id: saveBtn
                    text: qsTr("Сохранить")
                    DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
                    KeyNavigation.right: cancelBtn
                    KeyNavigation.down: serverName
                    Keys.onPressed: {
                        if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
                            settings.setSettings()
                            settings.close()
                            menuField.focus = true
                        }
                    }
                }

                Button {
                    id: cancelBtn
                    text: qsTr("Отмена")
                    DialogButtonBox.buttonRole: DialogButtonBox.RejectRole
                    KeyNavigation.up: maxRows
                    KeyNavigation.down: serverName
                    Keys.onPressed: {
                        if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
                            settings.close()
                            menuField.focus = true
                        }
                    }
                }
            }
        }
    }
}

