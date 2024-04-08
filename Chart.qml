import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts
import ChartModelTestQml

Item {
    id: root

    property int defaultPadding: 5;

    Column {
        id: column
        padding: defaultPadding

        Row {
            padding: defaultPadding
            height: statCombo.height

            TextInput {
                text: "Filter:"
                id: filterInput
                width: 200
                height: 20
            }

            ComboBox {
                id: statCombo
                objectName: "modelContainer"
                model: modelContainer
                textRole: "display"

                Component.onCompleted: currentIndex = 0

                onCurrentTextChanged: {
                    if (modelContainer) {
                        modelContainer.setCurrentModel(currentText)
                        modelMapper.model = modelContainer.getCurrentModel()
                    }
                }
            }
        }
    }

    ChartView {
        id: chart
        title: "Line Chart"
        antialiasing: true
        anchors.top: column.bottom
        anchors.bottom: root.bottom
        anchors.left: root.left
        anchors.right: root.right
        // animationOptions: ChartView.AllAnimations

        DateTimeAxis {
            id: axisX
            min: "2023-03-15T09:00:00.000Z"
            max: "2023-03-15T09:05:00.000Z"
        }

        ValueAxis {
            id: axisY
            min: modelContainer ? modelContainer.minValue : 0.0
            max: modelContainer ? modelContainer.maxValue : 100.0
        }

        LineSeries{
            id: line
            axisX: axisX
            axisY: axisY
        }

        VXYModelMapper {
            id: modelMapper
            model: modelContainer.getCurrentModel()
            series: line
            firstRow: 0
            xColumn: 0
            yColumn: 1
        }
    }
}
