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
  // connects
  connect(core_.data(), &NCALC::Core::failed, this, &MainWindow::OnCatchFailure);
  core_->Reset();
}

MainWindow::~MainWindow()
{
  if (core_) core_.reset();
  delete ui;
  qDebug() << "MainWindow: destruct";
}

auto MainWindow::InitWidgets() -> void
{
  if (!core_->SetDisplay(ui->Display, ui->Display_Sub) ||
      !core_->SetModeLabel(ui->ModeLabel)) {
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
void MainWindow::OnCatchFailure(const QString& msg)
{
  qWarning() << "MainWindow: catch error: " << msg;
  close();
}

/*
 * slots: menus
 */
void MainWindow::on_actionQuit_triggered()
{
  Quit();
}

void MainWindow::on_Button_To2Bin_clicked()
{
  core_->ChangeMode(NCALC::DispMode::BIN);
}

void MainWindow::on_Button_ToHex_clicked()
{
  core_->ChangeMode(NCALC::DispMode::HEX);
}

void MainWindow::on_Button_BC_clicked()
{
  core_->OnBackSpace();
}

void MainWindow::on_Button_C_clicked()
{
  core_->Reset();
}

void MainWindow::on_Button_Devide_clicked()
{
  core_->OnOperate(NCALC::OpCode::DEVIDE);
}

void MainWindow::on_Button_Multiply_clicked()
{
  core_->OnOperate(NCALC::OpCode::MULTIPLY);
}

void MainWindow::on_Button_Minus_clicked()
{
  core_->OnOperate(NCALC::OpCode::SUBTRACT);
}

void MainWindow::on_Button_Plus_clicked()
{
  core_->OnOperate(NCALC::OpCode::ADD);
}

void MainWindow::on_Button_Equal_clicked()
{
  core_->OnOperate(NCALC::OpCode::NOP);
}

void MainWindow::on_Button_Dot_clicked()
{
  core_->AppendPoint();
}

void MainWindow::on_Button_0_clicked()
{
  core_->OnNumber(0);
}

void MainWindow::on_Button_1_clicked()
{
  core_->OnNumber(1);
}

void MainWindow::on_Button_2_clicked()
{
  core_->OnNumber(2);
}

void MainWindow::on_Button_3_clicked()
{
  core_->OnNumber(3);
}

void MainWindow::on_Button_4_clicked()
{
  core_->OnNumber(4);
}

void MainWindow::on_Button_5_clicked()
{
  core_->OnNumber(5);
}

void MainWindow::on_Button_6_clicked()
{
  core_->OnNumber(6);
}

void MainWindow::on_Button_7_clicked()
{
  core_->OnNumber(7);
}

void MainWindow::on_Button_8_clicked()
{
  core_->OnNumber(8);
}

void MainWindow::on_Button_9_clicked()
{
  core_->OnNumber(9);
}

void MainWindow::on_Button_ToDecimal_clicked()
{
  core_->ChangeMode(NCALC::DispMode::DECIMAL);
}

void MainWindow::on_Button_And_clicked()
{
  core_->OnOperate(NCALC::OpCode::L_AND);
}

void MainWindow::on_Button_Or_clicked()
{
  core_->OnOperate(NCALC::OpCode::L_OR);
}

void MainWindow::on_Button_Not_clicked()
{
  core_->OnOperate(NCALC::L_NOT);
}

void MainWindow::on_Button_Xor_clicked()
{
  core_->OnOperate(NCALC::OpCode::L_XOR);
}
