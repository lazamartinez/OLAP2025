import QtQuick
import QtQuick3D

Rectangle {
    id: root
    width: 800
    height: 600
    color: "#F4F6F9"

    Text {
        anchors.centerIn: parent
        text: "Visor 3D OLAP - Cargando..."
        color: "#2C3E50"
        font.pixelSize: 18
        font.bold: true
        visible: !view3d.visible
    }

    View3D {
        id: view3d
        anchors.fill: parent
        visible: true

        environment: SceneEnvironment {
            clearColor: "#F4F6F9"
            backgroundMode: SceneEnvironment.Color
        }

        PerspectiveCamera {
            id: camera
            position: Qt.vector3d(0, 80, 250)
            eulerRotation.x: -15
        }

        DirectionalLight {
            eulerRotation.x: -30
            eulerRotation.y: 30
            brightness: 1.2
        }

        Node {
            id: voxelsNode
        }

        Component {
            id: voxelComp
            Model {
                source: "#Cube"
                scale: Qt.vector3d(0.7, 0.7, 0.7)
                materials: DefaultMaterial {
                    diffuseColor: "#3498DB"
                }
            }
        }
    }

    function setData(dataList) {
        console.log("=== QML setData llamado, items:", dataList.length);
        
        // Limpiar
        for(var i = voxelsNode.children.length - 1; i >= 0; i--) {
            voxelsNode.children[i].destroy();
        }
        
        // Crear voxels
        for(var j = 0; j < dataList.length; j++) {
            var item = dataList[j];
            var color = (item.val > 5000) ? "#E67E22" : "#3498DB";
            
            voxelComp.createObject(voxelsNode, {
                "position": Qt.vector3d(item.x, item.y, item.z),
                "materials": [Qt.createQmlObject(
                    'import QtQuick3D; DefaultMaterial { diffuseColor: "' + color + '" }',
                    voxelsNode
                )]
            });
        }
        
        console.log("=== Voxels creados:", voxelsNode.children.length);
    }

    MouseArea {
        anchors.fill: parent
        property real prevX: 0
        property real prevY: 0
        
        onPressed: {
            prevX = mouse.x;
            prevY = mouse.y;
        }
        
        onPositionChanged: {
            if (pressed) {
                camera.eulerRotation.y += (mouse.x - prevX) * 0.3;
                camera.eulerRotation.x += (mouse.y - prevY) * 0.3;
                prevX = mouse.x;
                prevY = mouse.y;
            }
        }
        
        onWheel: {
            camera.position.z += wheel.angleDelta.y * 0.05;
        }
    }
}
