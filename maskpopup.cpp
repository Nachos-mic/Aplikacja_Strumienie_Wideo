#include "maskpopup.h"
#include "ui_maskpopup.h"

MaskPopUp::MaskPopUp(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MaskPopUp)
{

    ui->setupUi(this);

    setWindowFlags(Qt::Popup);
    connect(ui->CancelButton, &QPushButton::clicked, this, &MaskPopUp::close);
    connect(ui->AcceptButton, &QPushButton::clicked, this, &MaskPopUp::close);

    QRect geometry = QApplication::primaryScreen()->geometry();

    int x = (geometry.width() - width()) / 2;
    int y = (geometry.height() - height()) / 2;
    move(x, y);

}

MaskPopUp::~MaskPopUp()
{
    delete ui;
}


