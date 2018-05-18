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

/* class: MainWindow */
MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  m_mem(nullptr),
  m_core(nullptr),
  cpu_thread(nullptr)
{
  ui->setupUi(this);
  qDebug() << "MainWindow: construct";
  if (!InitMemory()) {
    qWarning() << "Cannot initialize memory!";
    exit(EXIT_FAILURE);
  }
  if (!InitCore()) {
    qWarning() << "Cannot initialize core!";
    exit(EXIT_FAILURE);
  }
  if (!InitConnections()) {
    qWarning() << "Cannot initialize connections!";
    exit(EXIT_FAILURE);
  }
  if (!InitThread()) {
    qWarning() << "Cannot initialize threads!";
    exit(EXIT_FAILURE);
  }
  emit ToCore(OpCode::Reset, QVariant(0));
}

MainWindow::~MainWindow()
{
  if (m_core) m_core.reset();
  if (cpu_thread) {
    cpu_thread->quit();
    cpu_thread->wait();
    cpu_thread.reset();
  }
  if (m_mem) m_mem.reset();
  delete ui;
  qDebug() << "MainWindow: destruct";
}

/* base */
auto MainWindow::InitMemory() -> bool
{
  m_mem.reset(new NCALC::Mem());

  return true;
}

auto MainWindow::InitCore() -> bool
{
  if (m_mem.isNull()) return false;

  m_core.reset(new NCALC::Core());
  m_core->SetMemoryRef(m_mem.data());

  return true;
}

auto MainWindow::InitConnections() -> bool
{
  if (m_mem.isNull() || m_core.isNull()) return false;

  qRegisterMetaType<T_opcode>("T_opcode");
  qRegisterMetaType<T_arg>("T_arg");

  connect(this, &MainWindow::ToCore, m_core.data(), &NCALC::Core::FromMWin);
  connect(m_core.data(), &NCALC::Core::ToMWin, this, &MainWindow::FromCore);

  return true;
}

auto MainWindow::InitThread() -> bool
{
  if (m_core.isNull()) return false;

  cpu_thread.reset(new QThread());
  m_core->moveToThread(cpu_thread.data());
  cpu_thread->start();

  return true;
}

/* methods */
auto MainWindow::UpdateGPU() -> void
{
  UpdateDisplays();
  UpdateSubDisplays();
}

auto MainWindow::UpdateDisplays() -> void
{
  auto acc = m_mem->ToRegRead(Reg::ACC);
  auto input = m_mem->ToRegRead(Reg::INPUT);
  auto type = static_cast<NumType>(m_mem->ToStateRead(State::NumType));
  auto digit = static_cast<Digit>(m_mem->ToStateRead(State::Digit));
  if (type == NumType::Int) {
    if (digit == Digit::DECIMAL) {
      ui->Display0->setText(QString("%1").arg(static_cast<qint64>(acc)));
      ui->Display1->setText(QString("%1").arg(static_cast<qint64>(input)));
    } else {
      auto acc_q64 = static_cast<qint64>(acc);
      auto input_q64 = static_cast<qint64>(input);
      ui->Display0->setText(QString("%1")
                            .arg(acc_q64, acc_q64 < 0xffff ? 4: 8, 16, QLatin1Char( '0' )));
      ui->Display1->setText(QString("%1")
                            .arg(input_q64, input_q64 < 0xffff ? 4: 8, 16, QLatin1Char( '0' )));
    }
  } else {
    ui->Display0->setText(QString("%1").arg(acc));
    ui->Display1->setText(QString("%1").arg(input));
  }
}

auto MainWindow::UpdateSubDisplays() -> void
{
  auto digit = m_mem->ToStateRead(State::Digit);
  if (ui->Button_Digits->text() == "DEC") {
    if (static_cast<Digit>(digit) == Digit::HEXADECIMAL) {
      ui->Button_Digits->setText("HEX");
    }
  } else {
    if (static_cast<Digit>(digit) == Digit::DECIMAL) {
      ui->Button_Digits->setText("DEC");
    }
  }
}

/* slots */
void MainWindow::FromCore()
{
  UpdateGPU();
}

/* slots: menus */
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

/*
 * slots: command buttons
 */
void MainWindow::on_Button_Undo_clicked()
{
  emit ToCore(OpCode::Undo, QVariant(0));
}

void MainWindow::on_Button_Redo_clicked()
{
  emit ToCore(OpCode::Redo, QVariant(0));
}

void MainWindow::on_Button_Return_clicked()
{
  // Nothing any more currently
}

void MainWindow::on_Button_Digits_clicked()
{
  emit ToCore(OpCode::ChangeDigit, QVariant(0));
}

void MainWindow::on_Button_CL_clicked()
{
  // Current Input Clear
  emit ToCore(OpCode::Clear, QVariant(0));
  ui->Cmdlabel->setText("");
}

void MainWindow::on_Button_AC_clicked()
{
  // All Clear
  emit ToCore(OpCode::Reset, QVariant(0));
  ui->Cmdlabel->setText("");
}

/*
 * slots: logical arithmetics
 */
void MainWindow::on_Button_AND_clicked()
{
  emit ToCore(OpCode::And, QVariant(0));
  ui->Cmdlabel->setText("&");
}

void MainWindow::on_Button_OR_clicked()
{
  emit ToCore(OpCode::Or, QVariant(0));
  ui->Cmdlabel->setText("|");
}

void MainWindow::on_Button_XOR_clicked()
{
  emit ToCore(OpCode::Xor, QVariant(0));
  ui->Cmdlabel->setText("^");
}

void MainWindow::on_Button_NOT_clicked()
{
  emit ToCore(OpCode::Not, QVariant(0));
  ui->Cmdlabel->setText("~");
}

/* arithmetics */
void MainWindow::on_Button_Add_clicked()
{
  emit ToCore(OpCode::Add, QVariant(0));
  ui->Cmdlabel->setText("+");
}

void MainWindow::on_Button_Subtract_clicked()
{
  emit ToCore(OpCode::Subtract, QVariant(0));
  ui->Cmdlabel->setText("-");
}

void MainWindow::on_Button_Multiply_clicked()
{
  emit ToCore(OpCode::Multiply, QVariant(0));
  ui->Cmdlabel->setText("*");
}

void MainWindow::on_Button_Devide_clicked()
{
  emit ToCore(OpCode::Devide, QVariant(0));
  ui->Cmdlabel->setText("/");
}

/*
 * slots: number buttons
 */
void MainWindow::on_Button_NUM_0_clicked()
{
  emit ToCore(OpCode::Store, QVariant(0));
}

void MainWindow::on_Button_NUM_1_clicked()
{
  emit ToCore(OpCode::Store, QVariant(1));
}

void MainWindow::on_Button_NUM_2_clicked()
{
  emit ToCore(OpCode::Store, QVariant(2));
}

void MainWindow::on_Button_NUM_3_clicked()
{
  emit ToCore(OpCode::Store, QVariant(3));
}

void MainWindow::on_Button_NUM_4_clicked()
{
  emit ToCore(OpCode::Store, QVariant(4));
}

void MainWindow::on_Button_NUM_5_clicked()
{
  emit ToCore(OpCode::Store, QVariant(5));
}

void MainWindow::on_Button_NUM_6_clicked()
{
  emit ToCore(OpCode::Store, QVariant(6));
}

void MainWindow::on_Button_NUM_7_clicked()
{
  emit ToCore(OpCode::Store, QVariant(7));
}

void MainWindow::on_Button_NUM_8_clicked()
{
  emit ToCore(OpCode::Store, QVariant(8));
}

void MainWindow::on_Button_NUM_9_clicked()
{
  emit ToCore(OpCode::Store, QVariant(9));
}

void MainWindow::on_Button_NUM_A_clicked()
{
  emit ToCore(OpCode::Store, QVariant(0xA));
}

void MainWindow::on_Button_NUM_B_clicked()
{
  emit ToCore(OpCode::Store, QVariant(0xB));
}

void MainWindow::on_Button_NUM_C_clicked()
{
  emit ToCore(OpCode::Store, QVariant(0xC));
}

void MainWindow::on_Button_NUM_D_clicked()
{
  emit ToCore(OpCode::Store, QVariant(0xD));
}

void MainWindow::on_Button_NUM_E_clicked()
{
  emit ToCore(OpCode::Store, QVariant(0xE));
}

void MainWindow::on_Button_NUM_F_clicked()
{
  emit ToCore(OpCode::Store, QVariant(0xF));
}

void MainWindow::on_Button_NUM_DOT_clicked()
{
  emit ToCore(OpCode::StoreDot, QVariant(0));
}
