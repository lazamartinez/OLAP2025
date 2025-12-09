import QtQuick
import QtQuick3D
import QtQuick3D.Helpers

Item {
    id: root
    width: 800
    height: 600

    property var voxelData: [] 

    // Background Professional Style (Clean White)
    Rectangle {
        anchors.fill: parent
        color: "#F4F6F9"
        
        Text {
            anchors.centerIn: parent
            text: "Inicializando Motor de Visualización 3D..."
            color: "#7F8C8D"
            font.family: "Segoe UI"
            font.pixelSize: 18
            visible: voxelsNode.children.length === 0
        }
    }

    View3D {
        id: view3d
        anchors.fill: parent
        opacity: 1.0

        environment: SceneEnvironment {
            clearColor: "#F4F6F9"
            backgroundMode: SceneEnvironment.Color
            antialiasingMode: SceneEnvironment.MSAA
            antialiasingQuality: SceneEnvironment.High
            enableVerticalSync: true
        }

        Node {
            id: sceneRoot

            DirectionalLight {
                eulerRotation.x: -45
                eulerRotation.y: -45
                color: "#FFFFFF"
                ambientColor: "#B0B0B0" 
                brightness: 1.2
            }
            
            // Standard Grid for context
            Model {
                source: "#Rectangle"
                scale: Qt.vector3d(100, 100, 1)
                eulerRotation.x: -90
                y: -10
                materials: DefaultMaterial {
                    diffuseColor: "#E0E0E0"
                }
                opacity: 0.5
            }

            // Camera
            PerspectiveCamera {
                id: camera
                position: Qt.vector3d(0, 150, 400)
                eulerRotation.x: -25
            }
        }
        
        // Orbit Controls
        OrbitCameraController {
            origin: sceneRoot
            camera: camera
            xSpeed: 0.5
            ySpeed: 0.5
        }

        // Voxel Node
        Node {
             id: voxelsNode
        }
        
        Component {
            id: voxelComponent
            Model {
                source: "#Cube"
                scale: Qt.vector3d(0.08, 0.08, 0.08)
                materials: DefaultMaterial {
                    diffuseColor: "#3498DB" // Corporate Blue
                    specularAmount: 0.2
                    roughness: 0.6
                }
            }
        }
        
        function updateVoxels(dataList) {
            // Limpiar anteriores
            for(var i = voxelsNode.children.length; i > 0; i--) {
                voxelsNode.children[i-1].destroy();
            }
            
            for (var j = 0; j < dataList.length; j++) {
                var d = dataList[j];
                // Color Logic: High Sales = Orange/Red, Normal = Blue
                var colorHex = (d.val > 5000) ? "#E67E22" : "#3498DB";
                
                var obj = voxelComponent.createObject(voxelsNode, {
                    "position": Qt.vector3d(d.x, d.y, d.z),
                    "materials": [
                        Qt.createQmlObject('import QtQuick3D; DefaultMaterial { diffuseColor: "' + colorHex + '"; }', voxelsNode)
                    ]
                });
            }
        }
    }
    
    function setData(data) {
        view3d.updateVoxels(data);
    }
}

