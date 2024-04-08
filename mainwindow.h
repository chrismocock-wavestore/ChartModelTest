#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QMainWindow>
#include <QQuickView>

#include "modelcontainer.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class ChartWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void requestData();

private slots:
    void quickViewStatusChanged(QQuickView::Status);
    void sceneGraphError(QQuickWindow::SceneGraphError error, const QString &message);

private:
    void handleDataRetrieved();

private:
    Ui::MainWindow *ui;
    QQuickView *quick_view_;
    ModelContainer models_{};
    ChartWidget* chart_widget_{};
};
#endif // MAINWINDOW_H
