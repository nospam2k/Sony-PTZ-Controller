#ifndef LISTITEM_H
#define LISTITEM_H

#include <QWidget>
#include <QListWidget>
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
    void setPresetIndex(int presetIndex);

private:
    Ui::ListItem *ui;
    int presetIndex;
    QListWidget *parent;
private slots:
    void onEditBtnClicked();
    void onDelBtnClicked();
};

#endif // LISTITEM_H
