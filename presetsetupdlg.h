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
    void initUi();
private slots:
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void moveHome();
    void stopMoving();
    void zoomInOnce();
    void zoomOutOnce();
    void stopZooming();
    void focusInOnce();
    void focusOutOnce();
    void stopFocusing();
    void changeFocusMode();
    void setPreset();
    void callPreset();

    void savePresetSetup();
    void cancelPresetSetup();

};

#endif // PRESETSETUPDLG_H
