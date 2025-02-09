#ifndef MASKPOPUP_H
#define MASKPOPUP_H

#include <QWidget>
#include <QScreen>
#include <stdio.h>
#include "mask.h"

namespace Ui {
class MaskPopUp;
}

class MaskPopUp : public QWidget
{
    Q_OBJECT

public:
    explicit MaskPopUp(QWidget *parent = nullptr);
    ~MaskPopUp();

signals:
    void maskSet(std::vector<float>);

private:
    Ui::MaskPopUp *ui;
    void setMask();
    QString edit_line_settings = "QLineEdit {background-color: white; border: black; border-radius: 10px; width: 40px; height: 40px}";
};

#endif // MASKPOPUP_H
