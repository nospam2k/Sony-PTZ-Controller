#ifndef LISTITEM_H
#define LISTITEM_H

#include <QWidget>

namespace Ui {
class ListItem;
}

class ListItem : public QWidget
{
    Q_OBJECT

public:
    explicit ListItem(QWidget *parent = 0);
    ~ListItem();

private:
    Ui::ListItem *ui;
};

#endif // LISTITEM_H
