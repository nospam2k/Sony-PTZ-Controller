#ifndef CAMERASETUPDLG_H
#define CAMERASETUPDLG_H

#include <QDialog>
#include "sonycam.h"
namespace Ui {
class CameraSetupDlg;
}

class CameraSetupDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CameraSetupDlg(QWidget *parent = 0 , SonyCam *cam = nullptr);
    ~CameraSetupDlg();

private:
    Ui::CameraSetupDlg *ui;
    void initUi();
    SonyCam *camToEdit;
private slots:
    void saveCamera();
    void cancelCamera();
};

#endif // CAMERASETUPDLG_H
