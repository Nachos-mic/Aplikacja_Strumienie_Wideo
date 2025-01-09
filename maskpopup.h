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
};

#endif // MASKPOPUP_H
