#include "listitem.h"
#include "ui_listitem.h"
#include "sonyptzapp.h"
ListItem::ListItem(QWidget *parent , int presetIndex) :
    QWidget(parent),
    ui(new Ui::ListItem)
{
    ui->setupUi(this);
    this->parent = (QListWidget*)parent;
    this->presetIndex = presetIndex;
    ui->presetName->setText(App()->getCameraManager()->getCurCam()->getPresetList().at(presetIndex).presetName);
    connect(ui->editBtn , SIGNAL(clicked(bool)) , this , SLOT(onEditBtnClicked()));
    connect(ui->delBtn , SIGNAL(clicked(bool)) , this , SLOT(onDelBtnClicked()));
}

ListItem::~ListItem()
{
    delete ui;
}

void ListItem::onEditBtnClicked()
{
    SonyCam *cam = App()->getCameraManager()->getCurCam();
    if(cam == nullptr)
        return;
    PresetSetupDlg presetSetupDlg(nullptr, presetIndex);
    presetSetupDlg.exec();
    ui->presetName->setText(App()->getCameraManager()->getCurCam()->getPresetList().at(presetIndex).presetName);
}
void ListItem::onDelBtnClicked()
{
    SonyCam *cam = App()->getCameraManager()->getCurCam();
    if(cam == nullptr)
        return;

    //remove from list widget
    ((QListWidget*)parent)->takeItem(presetIndex);
    //remove from curcam
    cam->removePreset(presetIndex);

    //reindexing
    for(int i = presetIndex; i < cam->getPresetList().length() ; i ++)
    {
        ((ListItem*)(((QListWidget*)parent)->itemWidget(((QListWidget*)parent)->item(i))))->setPresetIndex(i);
    }


    //remove from the file
    //TODO
}
void ListItem::setPresetIndex(int presetIndex)
{
    this->presetIndex = presetIndex;
}
