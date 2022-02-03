#ifndef TRACING_H
#define TRACING_H

#include <QWidget>
#include "drawwidget.h"

namespace Ui {
class tracing;
}

class tracing : public QWidget
{
    Q_OBJECT

public:
    explicit tracing(QWidget *parent = nullptr);
    ~tracing();

private:
    Ui::tracing *ui;
};

#endif // TRACING_H
