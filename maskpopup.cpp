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
    connect(ui->AcceptButton, &QPushButton::clicked, this, &MaskPopUp::setMask);

    ui->w1Edit->setStyleSheet(edit_line_settings);
    ui->w2Edit->setStyleSheet(edit_line_settings);
    ui->w3Edit->setStyleSheet(edit_line_settings);
    ui->w4Edit->setStyleSheet(edit_line_settings);
    ui->w5Edit->setStyleSheet(edit_line_settings);
    ui->w6Edit->setStyleSheet(edit_line_settings);
    ui->w7Edit->setStyleSheet(edit_line_settings);
    ui->w8Edit->setStyleSheet(edit_line_settings);
    ui->w9Edit->setStyleSheet(edit_line_settings);

    QRect geometry = QApplication::primaryScreen()->geometry();

    int x = (geometry.width() - width()) / 2;
    int y = (geometry.height() - height()) / 2;
    move(x, y);

}

MaskPopUp::~MaskPopUp()
{
    delete ui;
}

void MaskPopUp::setMask(){

    std::vector mask = {ui->w1Edit->text().toFloat() , ui->w2Edit->text().toFloat() , ui->w3Edit->text().toFloat()
                        ,ui->w4Edit->text().toFloat() , ui->w5Edit->text().toFloat() , ui->w6Edit->text().toFloat()
                        ,ui->w7Edit->text().toFloat() , ui->w8Edit->text().toFloat() ,ui->w9Edit->text().toFloat()};

    setCustomFilterMask(mask);
}



