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


Core::Core(QObject *parent) : QObject(parent),
  wait_for_operand_(false),
  num_cache_(0.0),
  factor_(OpFactor::NONE),
  mode_(OpMode::DECIMAL),
  display_(new QLineEdit()),
  mode_label_(new QLabel())
{
  qDebug() << "Core: construct";
}

Core::~Core()
{
  if (mode_label_) mode_label_.reset();
  if (display_) display_.reset();
  qDebug() << "Core: destruct";
}

auto Core::AddOperate() -> void
{
  PreOperate();
  factor_ = OpFactor::PLUS;
  wait_for_operand_ = true;
  display_->setText(QString::number(num_cache_));
}

auto Core::ChopText() -> void
{
  auto tmp = display_->text();
  tmp.chop(1);
  if (tmp.isEmpty()) {
    tmp = "0";
  }
  display_->setText(tmp);
}

auto Core::ClearOperate() -> void
{
  num_cache_ = 0.0;
  factor_ = OpFactor::NONE;
  wait_for_operand_ = false;
  display_->setText("0");
}

auto Core::DevideOperate() -> void
{
  PreOperate();
  factor_ = OpFactor::DEVIDE;
  wait_for_operand_ = true;
  display_->setText(QString::number(num_cache_));
}

auto Core::DisplayText(int num) -> void
{
  if (wait_for_operand_) {
    display_->clear();
    wait_for_operand_ = false;
  }
  auto tmp = display_->text() == "0" ? QString::number(num):
                                       display_->text() + QString::number(num);
  display_->setText(tmp);
}

auto Core::MultiplyOperate() -> void
{
  PreOperate();
  factor_ = OpFactor::MULTI;
  wait_for_operand_ = true;
  display_->setText(QString::number(num_cache_));
}

auto Core::OnBackSpace() -> void
{
  qDebug() << "Core: bc";
  ChopText();
}

auto Core::OnClear() -> void
{
  qDebug() << "Core: c";
  ClearOperate();
}

auto Core::OnNumber(int num) -> void
{
  DisplayText(num);
}

auto Core::OnOperate(OpFactor factor) -> void
{
  switch (factor) {
  case OpFactor::DEVIDE:
    return DevideOperate();
  case OpFactor::MINUS:
    return SubtractOperate();
  case OpFactor::MULTI:
    return MultiplyOperate();
  case OpFactor::PLUS:
    return AddOperate();
  case OpFactor::DOT:
    return PointText();
  case OpFactor::EQUAL:
    return ShowResult();
  case OpFactor::AND:
    return;
  case OpFactor::NOT:
    return;
  case OpFactor::XOR:
    return;
  case OpFactor::OR:
    return;
  case OpFactor::NONE:
    return;
  }
}

auto Core::OnOperate(OpMode mode) -> void
{
  mode_ = mode;
  mode_label_->setText(mode == OpMode::BIT ? "B":
                                             mode == OpMode::HEX ? "X": "D");
}

auto Core::PointText() -> void
{
  if (!display_->text().contains(".")) {
    display_->setText(display_->text() + ".");
  }
}

auto Core::PreOperate() -> void
{
  switch (factor_) {
  case OpFactor::NONE:
    num_cache_ = display_->text().toDouble();
    qDebug() << "cache = " << num_cache_;
    return;
  case OpFactor::PLUS:{
    auto tmp = num_cache_;
    num_cache_ += display_->text().toDouble();
    qDebug() << "cache = " << num_cache_ << "(" << tmp;
    return;}
  case OpFactor::MINUS:
    num_cache_ -= display_->text().toDouble();
    qDebug() << "cache = " << num_cache_;
    return;
  case OpFactor::MULTI:
    num_cache_ *= display_->text().toDouble();
    qDebug() << "cache = " << num_cache_;
    return;
  case OpFactor::DEVIDE:
    num_cache_ /= display_->text().toDouble();
    qDebug() << "cache = " << num_cache_;
    return;
  case OpFactor::EQUAL:
    return;
  case OpFactor::AND:
    return;
  case OpFactor::DOT:
    return;
  case OpFactor::NOT:
    return;
  case OpFactor::OR:
    return;
  case OpFactor::XOR:
    return;
  }
}

auto Core::SetDisplay(QLineEdit* line_edit) -> bool
{
  display_.reset(line_edit);
  if (display_.isNull()) return false;
  display_->setText("0");
  return true;
}

auto Core::SetModeLabel(QLabel* label) -> bool
{
  mode_label_.reset(label);
  if (mode_label_.isNull()) return false;
  mode_label_->setText("D");
  return true;
}

auto Core::ShowResult() -> void
{
  PreOperate();
  factor_ = OpFactor::NONE;
  display_->setText(QString::number(num_cache_));
  wait_for_operand_ = false;
}

auto Core::SubtractOperate() -> void
{
  PreOperate();
  factor_ = OpFactor::MINUS;
  wait_for_operand_ = true;
  display_->setText(QString::number(num_cache_));
}

}  // namespace NCALC
