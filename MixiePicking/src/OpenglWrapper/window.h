#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class QSlider;
class GlWidget;
class MainWindow;

class Window : public QWidget
{
    Q_OBJECT
public:
    explicit Window(MainWindow *mw = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;
private:
     QSlider *createSlider();
private:
    MainWindow *mainWindow = nullptr;
    GlWidget *glWidget = nullptr;
    QSlider *xSlider = nullptr;
    QSlider *ySlider = nullptr;
    QSlider *zSlider = nullptr;
};

#endif // WINDOW_H
