#include "camerasetupdlg.h"
#include "ui_camerasetupdlg.h"
#include "sonyptzapp.h"
CameraSetupDlg::CameraSetupDlg(QWidget *parent , SonyCam *cam) :
    QDialog(parent),
    ui(new Ui::CameraSetupDlg)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);
    camToEdit = cam;
    initUi();
}

CameraSetupDlg::~CameraSetupDlg()
{
    camToEdit = nullptr;
    delete ui;
}
void CameraSetupDlg::initUi()
{
    connect(ui->okBtn , SIGNAL(clicked(bool)) , this , SLOT(saveCamera()));
    connect(ui->cancelBtn , SIGNAL(clicked(bool)) , this , SLOT(cancelCamera()));
}
void CameraSetupDlg::saveCamera()
{
    if(camToEdit)
    {
        //edit
        camToEdit->setCameraIp(ui->ipEdit->text());
        camToEdit->setCameraName(ui->nameEdit->text());
    }
    else
    {
        //create
        camToEdit = new SonyCam(ui->ipEdit->text() , DEFAULT_TARGET_PORTNUM , ui->nameEdit->text());
        //add
        App()->getCameraManager()->addCamera(camToEdit);
    }
    accept();
}
void CameraSetupDlg::cancelCamera()
{
    close();
}
