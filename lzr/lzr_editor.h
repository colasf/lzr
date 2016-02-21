#ifndef FRAMEEDITOR_H
#define FRAMEEDITOR_H

#include <QtWidgets>
#include "frameview.h"
#include "colordock.h"


class LZREditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit LZREditor();
    ~LZREditor();

private:
    void setupUi();

    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    FrameView* frame;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    ColorDock* color;
};

#endif // FRAMEEDITOR_H