#ifndef MASKPOPUP_H
#define MASKPOPUP_H

#include <QWidget>
#include <QScreen>

namespace Ui {
class MaskPopUp;
}

class MaskPopUp : public QWidget
{
    Q_OBJECT

public:
    explicit MaskPopUp(QWidget *parent = nullptr);
    ~MaskPopUp();


private:
    Ui::MaskPopUp *ui;
};

#endif // MASKPOPUP_H
