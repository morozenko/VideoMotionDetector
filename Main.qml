import QtQuick
import QtMultimedia
import QtQuick.Window 2.2
import org.freedesktop.gstreamer.Qt6GLVideoItem 1.0

Window {
    id: currentWindowId
    width: 640
    height: 480
    visible: true
//    flags: Qt.Tool
    title: qsTr("VideoMotionDetector")

    GstGLQt6VideoItem {
            id: videoOutput
            objectName: "videoOutputItem" // Used by C++ to find the item
            anchors.fill: parent
        }
    // CaptureSession {
    //     id: captureSession

    //     screenCapture: ScreenCapture {
    //         id: screenCapture
    //         active: true
    //         screen: Screen

    //         Component.onCompleted: { console.log("width: ", width);
    //             console.log("height: ", height);
    //             console.log("screen: ", Screen);
    //             console.log("screen.width: ", Screen.width);
    //             console.log("screen height: ", Screen.height);
    //         }
    //     }

    //     videoOutput: VideoOutput {
    //         id: videoOutput
    //         //anchors.fill: parent
    //         width: currentWindowId.width
    //         height: currentWindowId.height

    //         Component.onCompleted: { console.log("width: ", width);
    //             console.log("height: ", height)}
    //     }
    // }
}
