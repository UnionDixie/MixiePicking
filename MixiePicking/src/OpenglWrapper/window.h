#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class QListWidget;
class QSlider;
class GLWidget;
class MainWindow;
class QSpinBox;

class Window : public QWidget
{
    Q_OBJECT
public:
    explicit Window(MainWindow *mw = nullptr);
protected:
    void keyPressEvent(QKeyEvent *event) override;
public slots:
    void openNewFile();
    void saveScene();
    void addItemOnList(const QString& newItem);
    void setActiveItem(const QString& name);
    void hideGUI();
signals:
    void closeWin();
    void openFile(const QString& pathToFile);
    void signalSaveScene();
private:
     QSlider *createSlider();
private:
    bool isHidegGui = false;
    MainWindow *mainWindow = nullptr;
    GLWidget   *glWidget   = nullptr;
    QSlider    *xSlider    = nullptr;
    QSlider    *ySlider    = nullptr;
    QSlider    *zSlider    = nullptr;
    QSlider    *sSlider    = nullptr;
    QSlider    *moveXSlider    = nullptr;
    QSlider    *moveYSlider    = nullptr;
    QListWidget* listOfObjects = nullptr;
    QSpinBox* translateXSpinBox = nullptr;
};

#endif //WINDOW_H
