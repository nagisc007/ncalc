/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv2 or any later version                             *
 *                                                                         *
 ***************************************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  qDebug() << "MainWindow: construct";
}

MainWindow::~MainWindow()
{
  delete ui;
  qDebug() << "MainWindow: destruct";
}

auto MainWindow::Quit() -> void
{
  close();
}

/*
 * slots
 */
void MainWindow::on_actionQuit_triggered()
{
  Quit();
}
