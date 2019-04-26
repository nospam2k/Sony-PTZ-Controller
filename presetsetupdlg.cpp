#include "presetsetupdlg.h"
#include "ui_presetsetupdlg.h"

PresetSetupDlg::PresetSetupDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PresetSetupDlg)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);
    initUi();
}

PresetSetupDlg::~PresetSetupDlg()
{
    delete ui;
}
void PresetSetupDlg::initUi()
{

}
