/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "gpu.h"

#include <QDebug>

namespace GPU {

// utilities
namespace {

QString _digitString(bool is_hex_mode)
{
  return is_hex_mode ? "HEX": "DEC";
}

QString _displayNumberString(bool is_hex_mode, T_base_num val, int current_dot_pos)
{
  QString res;
  if (is_hex_mode) {
    auto q64v = static_cast<qint64>(val);
    res = QString("0x%1").arg(q64v, q64v < 0xffff ? 4: 8, 16, QLatin1Char( '0' ));
  } else {
    res = QString::number(val);
  }
  if (current_dot_pos > 0) {
    if (res.contains(".")) {
      return res;
    } else {
      return res + ".";
    }
  } else {
    return res;
  }
}

QString _commandString(T_cmd_btn c)
{
  switch (c){
  case CPU::CmdButton::ADD: return QString("+");
  case CPU::CmdButton::SUBTRACT: return QString("-");
  case CPU::CmdButton::MULTIPLY: return QString("*");
  case CPU::CmdButton::DIVIDE: return QString("/");
  case CPU::CmdButton::AND: return QString("AND");
  case CPU::CmdButton::OR: return QString("OR");
  case CPU::CmdButton::XOR: return QString("XOR");
  case CPU::CmdButton::NOT: return QString("NOT");
  default: return QString("");
  }
}

bool _IsExistsBitFlag(T_bits b, T_gpu_addr addr)
{
  return (static_cast<int>(b) & static_cast<int>(addr)) != 0;
}

int _GetAddrBit(T_bits b)
{
  return (static_cast<int>(b) >> 8*3) & 0xff;
}

int _GetDigitBit(T_bits b)
{
  return (static_cast<int>(b) >> 8*2) & 0xff;
}

int _GetCmdBit(T_bits b)
{
  return (static_cast<int>(b) >> 8*1) & 0xff;
}

int _GetDotBit(T_bits b)
{
  return static_cast<int>(b) & 0xff;
}

}  // ns inner global

// class: GPU::Core
Core::Core(QObject *parent) : QObject(parent),
  vram(QVector<int>(1))
{}

// methods
void Core::Reset()
{
  for (auto& v: vram) {
    v = 0;
  }
}

// slots
void Core::FromCpu(T_bits bits, T_base_num input_val, T_base_num result_val)
{
  auto addr = _GetAddrBit(bits);
  if (_IsExistsBitFlag(addr, Addr::DIGIT)) {
    WriteVram(VramAddr::DIGIT, _GetDigitBit(bits));
    emit ToDisplay(DEV::Addr::DIGIT, _digitString(ReadVram(VramAddr::DIGIT)));
  }
  if (_IsExistsBitFlag(addr, Addr::CMD)) {
    emit ToDisplay(DEV::Addr::CMD,
                   _commandString(static_cast<T_cmd_btn>(_GetCmdBit(bits))));
  }
  if (_IsExistsBitFlag(addr, Addr::INPUT)) {
    emit ToDisplay(DEV::Addr::INPUT,
                   _displayNumberString(ReadVram(VramAddr::DIGIT), input_val,
                                        _GetDotBit(bits)));
  }
  if (_IsExistsBitFlag(addr, Addr::RESULT)) {
    emit ToDisplay(DEV::Addr::RESULT,
                   _displayNumberString(ReadVram(VramAddr::DIGIT), result_val, 0));
  }
}

// vram
auto Core::ReadVram(T_vram_addr addr) -> int
{
  return vram.at(static_cast<int>(addr));
}

auto Core::WriteVram(T_vram_addr addr, int val) -> void
{
  vram.replace(static_cast<int>(addr), val);
}

}  // ns GPU
