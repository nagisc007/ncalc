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
  display_(new QLineEdit())
{
  qDebug() << "Core: construct";
}

Core::~Core()
{
  if (display_) display_.reset();
  qDebug() << "Core: destruct";
}

auto Core::DisplayText(int num) -> void
{
  display_->setText(display_->text() + QString::number(num));
}

auto Core::OnBackSpace() -> void
{
  qDebug() << "Core: bc";
}

auto Core::OnClear() -> void
{
  qDebug() << "Core: c";
}

auto Core::OnDevide() -> void
{
  qDebug() << "Core: /";
}

auto Core::OnDot() -> void
{
  qDebug() << "Core: .";
}

auto Core::OnEqual() -> void
{
  qDebug() << "Core: =";
}

auto Core::OnMinus() -> void
{
  qDebug() << "Core: -";
}

auto Core::OnMultiply() -> void
{
  qDebug() << "Core: *";
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
}

auto Core::OnTo2Bin() -> void
{
  qDebug() << "Core: to 0b";
}

auto Core::OnToHex() -> void
{
  qDebug() << "Core: to 0x";
}

auto Core::SetDisplay(QLineEdit* line_edit) -> bool
{
  display_.reset(line_edit);
  return !display_.isNull();
}

}  // namespace NCALC
