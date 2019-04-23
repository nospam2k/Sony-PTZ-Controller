#include "camerasetupdlg.h"
#include "ui_camerasetupdlg.h"

CameraSetupDlg::CameraSetupDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CameraSetupDlg)
{
    ui->setupUi(this);
}

CameraSetupDlg::~CameraSetupDlg()
{
    delete ui;
}
