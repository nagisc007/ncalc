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
}

MainWindow::~MainWindow()
{
  if (core_) core_.reset();
  delete ui;
  qDebug() << "MainWindow: destruct";
}

auto MainWindow::OnBackSpace() -> void
{
  qDebug() << "MainWindow: bc";
}

auto MainWindow::OnClear() -> void
{
  qDebug() << "MainWindow: c";
}

auto MainWindow::OnDevide() -> void
{
  qDebug() << "MainWindow: /";
}

auto MainWindow::OnDot() -> void
{
  qDebug() << "MainWindow: .";
}

auto MainWindow::OnEqual() -> void
{
  qDebug() << "MainWindow: =";
}

auto MainWindow::OnMinus() -> void
{
  qDebug() << "MainWindow: -";
}

auto MainWindow::OnMultiply() -> void
{
  qDebug() << "MainWindow: *";
}

auto MainWindow::OnNum0() -> void
{
  qDebug() << "MainWindow: 0";
}

auto MainWindow::OnNum1() -> void
{
  qDebug() << "MainWindow: 1";
}

auto MainWindow::OnNum2() -> void
{
  qDebug() << "MainWindow: 2";
}

auto MainWindow::OnNum3() -> void
{
  qDebug() << "MainWindow: 3";
}

auto MainWindow::OnNum4() -> void
{
  qDebug() << "MainWindow: 4";
}

auto MainWindow::OnNum5() -> void
{
  qDebug() << "MainWindow: 5";
}

auto MainWindow::OnNum6() -> void
{
  qDebug() << "MainWindow: 6";
}

auto MainWindow::OnNum7() -> void
{
  qDebug() << "MainWindow: 7";
}

auto MainWindow::OnNum8() -> void
{
  qDebug() << "MainWindow: 8";
}

auto MainWindow::OnNum9() -> void
{
  qDebug() << "MainWindow: 9";
}

auto MainWindow::OnPlus() -> void
{
  qDebug() << "MainWindow: +";
}

auto MainWindow::OnTo2Bin() -> void
{
  qDebug() << "MainWindow: to 0b";
}

auto MainWindow::OnToHex() -> void
{
  qDebug() << "MainWindow: to 0x";
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
  OnTo2Bin();
}

void MainWindow::on_Button_ToHex_clicked()
{
  OnToHex();
}

void MainWindow::on_Button_BC_clicked()
{
  OnBackSpace();
}

void MainWindow::on_Button_C_clicked()
{
  OnClear();
}

void MainWindow::on_Button_Devide_clicked()
{
  OnDevide();
}

void MainWindow::on_Button_Multiply_clicked()
{
  OnMultiply();
}

void MainWindow::on_Button_Minus_clicked()
{
  OnMinus();
}

void MainWindow::on_Button_Plus_clicked()
{
  OnPlus();
}

void MainWindow::on_Button_Equal_clicked()
{
  OnEqual();
}

void MainWindow::on_Button_Dot_clicked()
{
  OnDot();
}

void MainWindow::on_Button_0_clicked()
{
  OnNum0();
}

void MainWindow::on_Button_1_clicked()
{
  OnNum1();
}

void MainWindow::on_Button_2_clicked()
{
  OnNum2();
}

void MainWindow::on_Button_3_clicked()
{
  OnNum3();
}

void MainWindow::on_Button_4_clicked()
{
  OnNum4();
}

void MainWindow::on_Button_5_clicked()
{
  OnNum5();
}

void MainWindow::on_Button_6_clicked()
{
  OnNum6();
}

void MainWindow::on_Button_7_clicked()
{
  OnNum7();
}

void MainWindow::on_Button_8_clicked()
{
  OnNum8();
}

void MainWindow::on_Button_9_clicked()
{
  OnNum9();
}
