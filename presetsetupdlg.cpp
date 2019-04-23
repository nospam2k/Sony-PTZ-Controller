#include "presetsetupdlg.h"
#include "ui_presetsetupdlg.h"

PresetSetupDlg::PresetSetupDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PresetSetupDlg)
{
    ui->setupUi(this);
}

PresetSetupDlg::~PresetSetupDlg()
{
    delete ui;
}
