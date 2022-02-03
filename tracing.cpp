#include "tracing.h"
#include "ui_tracing.h"

tracing::tracing(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tracing)
{
    ui->setupUi(this);
    ui->draw_widget->SetTrace(true);
}

tracing::~tracing()
{
    delete ui;
}
