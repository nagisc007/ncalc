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
  ui(new Ui::MainWindow),
  core_(new NCALC::Core())
{
  ui->setupUi(this);
  qDebug() << "MainWindow: construct";
  InitWidgets();
}

MainWindow::~MainWindow()
{
  if (core_) core_.reset();
  delete ui;
  qDebug() << "MainWindow: destruct";
}

auto MainWindow::InitWidgets() -> void
{
  if (!core_->SetDisplay(ui->Display)) {
    qWarning() << "MainWindow: cannot initialize widgets!";
  }
  setWindowTitle("Ncalc");
  qDebug() << "MainWindow: init";
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

void MainWindow::on_Button_To2Bin_clicked()
{
  core_->OnTo2Bin();
}

void MainWindow::on_Button_ToHex_clicked()
{
  core_->OnToHex();
}

void MainWindow::on_Button_BC_clicked()
{
  core_->OnBackSpace();
}

void MainWindow::on_Button_C_clicked()
{
  core_->OnClear();
}

void MainWindow::on_Button_Devide_clicked()
{
  core_->OnDevide();
}

void MainWindow::on_Button_Multiply_clicked()
{
  core_->OnMultiply();
}

void MainWindow::on_Button_Minus_clicked()
{
  core_->OnMinus();
}

void MainWindow::on_Button_Plus_clicked()
{
  core_->OnPlus();
}

void MainWindow::on_Button_Equal_clicked()
{
  core_->OnEqual();
}

void MainWindow::on_Button_Dot_clicked()
{
  core_->OnDot();
}

void MainWindow::on_Button_0_clicked()
{
  core_->OnNum0();
}

void MainWindow::on_Button_1_clicked()
{
  core_->OnNum1();
}

void MainWindow::on_Button_2_clicked()
{
  core_->OnNum2();
}

void MainWindow::on_Button_3_clicked()
{
  core_->OnNum3();
}

void MainWindow::on_Button_4_clicked()
{
  core_->OnNum4();
}

void MainWindow::on_Button_5_clicked()
{
  core_->OnNum5();
}

void MainWindow::on_Button_6_clicked()
{
  core_->OnNum6();
}

void MainWindow::on_Button_7_clicked()
{
  core_->OnNum7();
}

void MainWindow::on_Button_8_clicked()
{
  core_->OnNum8();
}

void MainWindow::on_Button_9_clicked()
{
  core_->OnNum9();
}
