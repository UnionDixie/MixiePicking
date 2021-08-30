#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class QListWidget;
class QSlider;
class GLWidget;
class MainWindow;

class Window : public QWidget
{
    Q_OBJECT
public:
    explicit Window(MainWindow *mw = nullptr);
protected:
    void keyPressEvent(QKeyEvent *event) override;
public slots:
    void openNewFile();
    void addItemOnList(const QString& newItem);
signals:
    void closeWin();
    void openFile(const QString& pathToFile);
private:
     QSlider *createSlider();
private:
    MainWindow *mainWindow = nullptr;
    GLWidget   *glWidget   = nullptr;
    QSlider    *xSlider    = nullptr;
    QSlider    *ySlider    = nullptr;
    QSlider    *zSlider    = nullptr;
    QSlider    *sSlider    = nullptr;
    QSlider    *moveXSlider    = nullptr;
    QSlider    *moveYSlider    = nullptr;
    QListWidget* listOfObjects = nullptr;
};

#endif //WINDOW_H
