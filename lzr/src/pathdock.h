
#pragma once
#include <QtWidgets>
#include <QDebug>


class PathDock : public QDockWidget
{
    Q_OBJECT

public:
    explicit PathDock(QWidget* parent = 0);
    void setModel(QAbstractItemModel* model);
    QItemSelectionModel* selectionModel();

private:
    QWidget* content;
    QVBoxLayout* layout;
    QListView* paths;
};
