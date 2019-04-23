#ifndef PRESETSETUPDLG_H
#define PRESETSETUPDLG_H

#include <QDialog>

namespace Ui {
class PresetSetupDlg;
}

class PresetSetupDlg : public QDialog
{
    Q_OBJECT

public:
    explicit PresetSetupDlg(QWidget *parent = 0);
    ~PresetSetupDlg();

private:
    Ui::PresetSetupDlg *ui;
};

#endif // PRESETSETUPDLG_H
