import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    width: 300
    height: 400
    title: "QML updater example"

    Connections {
        target: updateWrapper
        onSigStepChanged: {
            var info = ""
            switch (step) {
            case 0:
                info = "TYPE_DOWNLOAD_INFO"
                break
            case 1:
                info = "TYPE_DOWNLOAD_UPDATE"
                break
            case 2:
                info = "TYPE_CHECK_UPDATE_VERSION"
                break
            case 3:
                info = "TYPE_CHECK_SIGNATURE"
                break
            case 4:
                info = "TYPE_RUN_INSTALLER"
                break
            default:
                info = step
                break
            }

            switch (result) {
            case 0:
                info += " : RESULT_PROCESSING"
                break
            case 1:
                info += " : RESULT_SUCCESS"
                break
            case 2:
                info += " : RESULT_FAILED"
                break
            case 3:
                info += " : RESULT_CANCELED"
                break
            }

            info += " : " + extraInfo

            updateLog.append(info)
        }
        onSigDownloadingProgress: {
            updateProgressBar.value = value
        }
    }

    TextArea {
        id: updateLog
        anchors.top: parent.top
        anchors.bottom: updateProgressBar.top
        anchors.left: parent.left
        anchors.right: parent.right
        readOnly: true
    }

    ProgressBar {
        id: updateProgressBar
        anchors.bottom: updateBtn.top
        anchors.bottomMargin: 3
        anchors.left: parent.left
        anchors.right: parent.right
        value: 0.0
    }

    Button {
        id: updateBtn
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        text: "Check and update"
        onClicked: {
            console.log("updateBtn onClicked")
            updateWrapper.update()
        }
    }
}
