#ifndef LISTITEM_H
#define LISTITEM_H

#include <QWidget>
#include "sonycam.h"
#include "presetsetupdlg.h"
namespace Ui {
class ListItem;
}

class ListItem : public QWidget
{
    Q_OBJECT

public:
    explicit ListItem(QWidget *parent = 0 , int presetIndex = -1);
    ~ListItem();

private:
    Ui::ListItem *ui;
    int presetIndex;
private slots:
    void onEditBtnClicked();
};

#endif // LISTITEM_H
