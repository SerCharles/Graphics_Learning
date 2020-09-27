#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Cliping.h"

class Cliping : public QMainWindow
{
    Q_OBJECT

public:
    Cliping(QWidget *parent = Q_NULLPTR);

private:
    Ui::ClipingClass ui;
};
