/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>

// utilities
namespace {

T_bits _bitsCombined(T_bits bus, T_bits btn)
{
  return (bus << 8) | btn;
}

}  // ns inner global

// class: MainWindow
MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  cpu_th(new QThread()),
  gpu_th(new QThread()),
  cpu(new CPU::Core()),
  gpu(new GPU::Core())
{
  ui->setupUi(this);
  if (!InitConnections()) {
    qWarning() << "Cannot initialize connections!";
    exit(EXIT_FAILURE);
  }
  qDebug() << "MainWindow: constructed";

  cpu_th->start();
  gpu_th->start();
  qDebug() << "MainWindow: cpu and gpu threads started";

  PushCmdButton(CPU::CmdButton::CLEAR_ALL);
}

MainWindow::~MainWindow()
{
  if (cpu_th) {
    cpu_th->quit();
    cpu_th->wait();
    cpu_th.reset();
  }
  if (gpu_th) {
    gpu_th->quit();
    gpu_th->wait();
    gpu_th.reset();
  }
  delete ui;
  qDebug() << "MainWindow: destruct";
}

// slots
void MainWindow::FromGpu(T_dev_addr addr, T_str val)
{
  switch (addr) {
  case DEV::Addr::RESULT:
    UpdateResult(val);
    break;
  case DEV::Addr::INPUT:
    UpdateInput(val);
    break;
  case DEV::Addr::DIGIT:
    UpdateDigit(val);
    break;
  case DEV::Addr::CMD:
    UpdateCmd(val);
    break;
  default:
    break;
  }
}

// methods
auto MainWindow::InitConnections() -> bool
{
  if (cpu_th.isNull() || gpu_th.isNull() ||
      cpu.isNull() || gpu.isNull()) return false;

  // move to thread
  cpu->moveToThread(cpu_th.data());
  gpu->moveToThread(gpu_th.data());

  // registered
  qRegisterMetaType<T_bits>("T_bits");
  qRegisterMetaType<T_base_num>("T_base_num");
  qRegisterMetaType<T_dev_addr>("T_dev_addr");
  qRegisterMetaType<T_str>("T_str");

  // input to cpu
  connect(this, &MainWindow::ToCpu, cpu.data(), &CPU::Core::FromInput);
  // cpu to gpu
  connect(cpu.data(), &CPU::Core::ToGpu, gpu.data(), &GPU::Core::FromCpu);
  // gpu to output
  connect(gpu.data(), &GPU::Core::ToDisplay, this, &MainWindow::FromGpu);

  return true;
}

auto MainWindow::UpdateResult(const QString& val) -> void
{
  ui->Display0->setText(val);
}

auto MainWindow::UpdateInput(const QString& val) -> void
{
  ui->Display1->setText(val);
}

auto MainWindow::UpdateDigit(const QString& val) -> void
{
  ui->Button_Digits->setText(val);
}

auto MainWindow::UpdateCmd(const QString& val) -> void
{
  ui->Cmdlabel->setText(val);
}

auto MainWindow::PushCmdButton(T_cmd_btn c) -> void
{
  emit ToCpu(_bitsCombined(static_cast<T_bits>(CPU::Addr::CMD),
                           static_cast<T_bits>(c)));
}

auto MainWindow::PushNumButton(T_num_btn n) -> void
{
  emit ToCpu(_bitsCombined(static_cast<T_bits>(CPU::Addr::NUM),
                           static_cast<T_bits>(n)));
}

// slots: menus
void MainWindow::on_actQuit_triggered()
{
  close();
}

void MainWindow::on_actAboutQt_triggered()
{
  QMessageBox::aboutQt(this);
}

void MainWindow::on_actAboutApp_triggered()
{
  auto title = QString("About %1")
      .arg(APP::VALUE::NAME);
  auto msg = QString("<h3>About %1 %2</h3><p>%3<br>Licensed by %4.<br>%5</p><p>%6</p>")
      .arg(APP::VALUE::NAME)
      .arg(APP::VALUE::VERSION)
      .arg(APP::VALUE::DESCRIPTION)
      .arg(APP::VALUE::LICENSE)
      .arg(APP::VALUE::COPYRIGHT)
      .arg(APP::VALUE::AUTHORS);
  QMessageBox::about(this, title, msg);
}

// slots: command buttons
void MainWindow::on_Button_Undo_clicked()
{
  PushCmdButton(CPU::CmdButton::UNDO);
}

void MainWindow::on_Button_Redo_clicked()
{
  PushCmdButton(CPU::CmdButton::REDO);
}

void MainWindow::on_Button_Return_clicked()
{
  PushCmdButton(CPU::CmdButton::NOP);
}

void MainWindow::on_Button_Digits_clicked()
{
  PushCmdButton(CPU::CmdButton::CHANGE_DIGIT);
}

void MainWindow::on_Button_CL_clicked()
{
  PushCmdButton(CPU::CmdButton::CLEAR_INPUT);
}

void MainWindow::on_Button_AC_clicked()
{
  PushCmdButton(CPU::CmdButton::CLEAR_ALL);
}

void MainWindow::on_Button_AND_clicked()
{
  PushCmdButton(CPU::CmdButton::AND);
}

void MainWindow::on_Button_OR_clicked()
{
  PushCmdButton(CPU::CmdButton::OR);
}

void MainWindow::on_Button_XOR_clicked()
{
  PushCmdButton(CPU::CmdButton::XOR);
}

void MainWindow::on_Button_NOT_clicked()
{
  PushCmdButton(CPU::CmdButton::NOT);
}

void MainWindow::on_Button_Add_clicked()
{
  PushCmdButton(CPU::CmdButton::ADD);
}

void MainWindow::on_Button_Subtract_clicked()
{
  PushCmdButton(CPU::CmdButton::SUBTRACT);
}

void MainWindow::on_Button_Multiply_clicked()
{
  PushCmdButton(CPU::CmdButton::MULTIPLY);
}

void MainWindow::on_Button_Devide_clicked()
{
  PushCmdButton(CPU::CmdButton::DIVIDE);
}

// slots: number buttons
void MainWindow::on_Button_NUM_0_clicked()
{
  PushNumButton(CPU::NumButton::NUM_0);
}

void MainWindow::on_Button_NUM_1_clicked()
{
  PushNumButton(CPU::NumButton::NUM_1);
}

void MainWindow::on_Button_NUM_2_clicked()
{
  PushNumButton(CPU::NumButton::NUM_2);
}

void MainWindow::on_Button_NUM_3_clicked()
{
  PushNumButton(CPU::NumButton::NUM_3);
}

void MainWindow::on_Button_NUM_4_clicked()
{
  PushNumButton(CPU::NumButton::NUM_4);
}

void MainWindow::on_Button_NUM_5_clicked()
{
  PushNumButton(CPU::NumButton::NUM_5);
}

void MainWindow::on_Button_NUM_6_clicked()
{
  PushNumButton(CPU::NumButton::NUM_6);
}

void MainWindow::on_Button_NUM_7_clicked()
{
  PushNumButton(CPU::NumButton::NUM_7);
}

void MainWindow::on_Button_NUM_8_clicked()
{
  PushNumButton(CPU::NumButton::NUM_8);
}

void MainWindow::on_Button_NUM_9_clicked()
{
  PushNumButton(CPU::NumButton::NUM_9);
}

void MainWindow::on_Button_NUM_A_clicked()
{
  PushNumButton(CPU::NumButton::NUM_A);
}

void MainWindow::on_Button_NUM_B_clicked()
{
  PushNumButton(CPU::NumButton::NUM_B);
}

void MainWindow::on_Button_NUM_C_clicked()
{
  PushNumButton(CPU::NumButton::NUM_C);
}

void MainWindow::on_Button_NUM_D_clicked()
{
  PushNumButton(CPU::NumButton::NUM_D);
}

void MainWindow::on_Button_NUM_E_clicked()
{
  PushNumButton(CPU::NumButton::NUM_E);
}

void MainWindow::on_Button_NUM_F_clicked()
{
  PushNumButton(CPU::NumButton::NUM_F);
}

void MainWindow::on_Button_NUM_DOT_clicked()
{
  PushNumButton(CPU::NumButton::NUM_DOT);
}
