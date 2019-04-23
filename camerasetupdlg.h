#ifndef CAMERASETUPDLG_H
#define CAMERASETUPDLG_H

#include <QDialog>

namespace Ui {
class CameraSetupDlg;
}

class CameraSetupDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CameraSetupDlg(QWidget *parent = 0);
    ~CameraSetupDlg();

private:
    Ui::CameraSetupDlg *ui;
};

#endif // CAMERASETUPDLG_H
