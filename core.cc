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
  display_(new QLineEdit())
{
  qDebug() << "Core: construct";
}

Core::~Core()
{
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

auto Core::ClearOperate() -> void
{
  num_cache_ = 0.0;
  factor_ = OpFactor::NONE;
  wait_for_operand_ = false;
  display_->setText(0);
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
}

auto Core::OnClear() -> void
{
  qDebug() << "Core: c";
  display_->setText(QString::number(0));
}

auto Core::OnDevide() -> void
{
  qDebug() << "Core: /";
  DevideOperate();
}

auto Core::OnDot() -> void
{
  qDebug() << "Core: .";
}

auto Core::OnEqual() -> void
{
  qDebug() << "Core: =";
  ShowResult();
}

auto Core::OnMinus() -> void
{
  qDebug() << "Core: -";
  SubtractOperate();
}

auto Core::OnMultiply() -> void
{
  qDebug() << "Core: *";
  MultiplyOperate();
}

auto Core::OnNum0() -> void
{
  qDebug() << "Core: 0";
  DisplayText(0);
}

auto Core::OnNum1() -> void
{
  qDebug() << "Core: 1";
  DisplayText(1);
}

auto Core::OnNum2() -> void
{
  qDebug() << "Core: 2";
  DisplayText(2);
}

auto Core::OnNum3() -> void
{
  qDebug() << "Core: 3";
  DisplayText(3);
}

auto Core::OnNum4() -> void
{
  qDebug() << "Core: 4";
  DisplayText(4);
}

auto Core::OnNum5() -> void
{
  qDebug() << "Core: 5";
  DisplayText(5);
}

auto Core::OnNum6() -> void
{
  qDebug() << "Core: 6";
  DisplayText(6);
}

auto Core::OnNum7() -> void
{
  qDebug() << "Core: 7";
  DisplayText(7);
}

auto Core::OnNum8() -> void
{
  qDebug() << "Core: 8";
  DisplayText(8);
}

auto Core::OnNum9() -> void
{
  qDebug() << "Core: 9";
  DisplayText(9);
}

auto Core::OnPlus() -> void
{
  qDebug() << "Core: +";
  AddOperate();
}

auto Core::OnTo2Bin() -> void
{
  qDebug() << "Core: to 0b";
}

auto Core::OnToHex() -> void
{
  qDebug() << "Core: to 0x";
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
  }
}

auto Core::SetDisplay(QLineEdit* line_edit) -> bool
{
  display_.reset(line_edit);
  display_->setText("0");
  return !display_.isNull();
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
