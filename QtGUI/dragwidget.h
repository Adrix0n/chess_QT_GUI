#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H
#include <QLabel>
#include <QByteArray>
#include <QMimeData>
#include <QDrag>
#include <QMouseEvent>

class DragWidget : public QFrame{
public:
    explicit DragWidget(QWidget *parent = nullptr);
    virtual ~DragWidget();
protected:
    //void dragEnterEvent(QDragEnterEvent *event) override;
    //void dragMoveEvent(QDragMoveEvent *event) override;
    //void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};



#endif // DRAGWIDGET_H
