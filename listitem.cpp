#include "listitem.h"
#include "ui_listitem.h"
#include "sonyptzapp.h"
ListItem::ListItem(QWidget *parent , int presetIndex) :
    QWidget(parent),
    ui(new Ui::ListItem)
{
    ui->setupUi(this);
    this->presetIndex = presetIndex;
    ui->presetName->setText(App()->getCameraManager()->getCurCam()->getPresetList().at(presetIndex).presetName);
    connect(ui->editBtn , SIGNAL(clicked(bool)) , this , SLOT(onEditBtnClicked()));
}

ListItem::~ListItem()
{
    delete ui;
}

void ListItem::onEditBtnClicked()
{
    PresetSetupDlg presetSetupDlg(nullptr, presetIndex);
    presetSetupDlg.exec();
    ui->presetName->setText(App()->getCameraManager()->getCurCam()->getPresetList().at(presetIndex).presetName);
}
