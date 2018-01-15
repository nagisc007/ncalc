/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv2 or any later version                             *
 *                                                                         *
 ***************************************************************************/
#include "core.h"

#include <QDebug>

namespace NCALC {

/* Utils */
auto Core::NumConverter::operator()(const QString& txt, DispMode) const -> double
{
  return txt.toDouble();
}

auto Core::NumConverter::operator()(double num, DispMode mode) -> QString
{
  if (mode == DispMode::HEX) {
    auto tmp = static_cast<int>(num);
    return QString("%1").arg(tmp, tmp < 0xffff ? 4: 8, 16, QLatin1Char( '0' ));
  } else if (mode == DispMode::BIN) {
    auto tmp = static_cast<int>(num);
    return QString("%1").arg(tmp, tmp < 0xff ? 8: tmp < 0xffff ? 16: 32, 2, QLatin1Char( '0' ));
  }
  QString txt = QString::number(num);
  return txt;
}

/* Utils: operate */
auto Core::AddFnc::operator()(double acc, double imm) -> double
{
  return acc + imm;
}

auto Core::DevideFnc::operator()(double acc, double imm) -> double
{
  return acc / imm;
}

auto Core::MultiplyFnc::operator()(double acc, double imm) -> double
{
  return acc * imm;
}

auto Core::Nothing::operator()(double, double imm) -> double
{
  return imm;
}

auto Core::SubtractFnc::operator()(double acc, double imm) -> double
{
  return acc - imm;
}

auto Core::LogicalAndFnc::operator()(double acc, double imm) -> double
{
  return static_cast<int>(acc) & static_cast<int>(imm);
}

auto Core::LogicalNotFnc::operator()(double acc, double) -> double
{
  return ~static_cast<int>(acc);
}

auto Core::LogicalOrFnc::operator()(double acc, double imm) -> double
{
  return static_cast<int>(acc) | static_cast<int>(imm);
}

auto Core::LogicalXorFnc::operator()(double acc, double imm) -> double
{
  return static_cast<int>(acc) ^ static_cast<int>(imm);
}

auto Core::AppendNumToStr::operator()(double acc, int num) -> double
{
  QString txt = QString::number(acc);
  txt += QString::number(num);
  return txt.toDouble();
}

/* Core class */
Core::Core(QObject *parent) : QObject(parent),
  acc_(0.0),
  current_(0.0),
  mode_(DispMode::DECIMAL),
  table_(new QList<OpFnc>()),
  stack_(new QStack<OpFnc>()),
  display_(new QLineEdit()),
  display2_(new QLineEdit()),
  mode_label_(new QLabel())
{
  qDebug() << "Core: construct";
  if (!InitFncTable()) {
    qWarning() << "Core: cannot create func table!";
  }
}

Core::~Core()
{
  if (stack_) {
    stack_->clear();
    stack_.reset();
  }
  if (table_) {
    table_->clear();
    table_.reset();
  }
  if (display2_) display2_.reset();
  if (display_) display_.reset();
  if (mode_label_) mode_label_.reset();
  qDebug() << "Core: destruct";
}

auto Core::AppendNumber(int num) -> void
{
  auto tmp = QString::number(current_);

  tmp += QString::number(num);
  current_ = tmp.toDouble();
  UpdateDisplay(current_, acc_, mode_);
}

auto Core::AppendPoint() -> void
{
  auto tmp = QString::number(current_);

  if (!tmp.contains(".")) {
    tmp += ".";
    current_ = tmp.toDouble();
    UpdateDisplay(current_, acc_, mode_);
  }
}

auto Core::ChangeMode(DispMode mode) -> void
{
  switch (mode) {
  case DispMode::BIN:
    mode_label_->setText("B");
    break;
  case DispMode::DECIMAL:
    mode_label_->setText("D");
    break;
  case DispMode::HEX:
    mode_label_->setText("X");
    break;
  }
  mode_ = mode;
  UpdateDisplay(current_, acc_, mode_);
}

auto Core::ChopCurrent() -> void
{
  auto tmp = QString::number(current_);
  tmp.chop(1);
  if (tmp.isEmpty()) {
    tmp = "0";
  }
  current_ = tmp.toDouble();
  UpdateDisplay(current_, acc_, mode_);
}

auto Core::InitFncTable() -> bool
{
  table_->reserve(OpCodeInfo::SIZE);
  for (int i = 0; i < OpCodeInfo::SIZE; ++i) {
    table_->operator<<(Nothing());
  }
  table_->operator[](OpCode::ADD) = AddFnc();
  table_->operator[](OpCode::DEVIDE) = DevideFnc();
  table_->operator[](OpCode::L_AND) = LogicalAndFnc();
  table_->operator[](OpCode::L_NOT) = LogicalNotFnc();
  table_->operator[](OpCode::L_OR) = LogicalOrFnc();
  table_->operator[](OpCode::L_XOR) = LogicalXorFnc();
  table_->operator[](OpCode::MULTIPLY) = MultiplyFnc();
  table_->operator[](OpCode::SUBTRACT) = SubtractFnc();

  return true;
}

auto Core::OnBackSpace() -> void
{
  ChopCurrent();
}

auto Core::OnNumber(int num) -> void
{
  AppendNumber(num);
}

auto Core::OnOperate(OpCode code) -> void
{
  if (!stack_->isEmpty()) {
    auto opcode = stack_->pop();
    acc_ = opcode(acc_, current_);
    UpdateDisplay(current_, acc_, mode_);
  }
  current_ = 0.0;
  stack_->push(table_->at(code));
}

auto Core::Reset() -> void
{
  acc_ = 0.0;
  current_ = 0.0;
  stack_->clear();
  stack_->push(table_->at(OpCode::NOP));
  UpdateDisplay(current_, acc_, mode_);
}

auto Core::SetDisplay(QLineEdit* main, QLineEdit* sub) -> bool
{
  display_.reset(main);
  display2_.reset(sub);
  if (display_.isNull() || display2_.isNull()) return false;
  display_->setText("0");
  display2_->setText("0");
  return true;
}

auto Core::SetModeLabel(QLabel* label) -> bool
{
  mode_label_.reset(label);
  if (mode_label_.isNull()) return false;
  mode_label_->setText("D");
  mode_ = DispMode::DECIMAL;
  return true;
}

auto Core::UpdateDisplay(double current, double acc, DispMode mode) -> void
{
  display_->setText(NumConverter()(current, mode));
  display2_->setText(NumConverter()(acc, mode));
}

}  // namespace NCALC
