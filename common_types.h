/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <QtGlobal>
#include <QVariant>

enum class OpCode {
  NOP,
  Reset,
  Add, Subtract, Multiply, Devide,
  And, Or, Xor, Not,
  Return,
  Clear, Undo, Redo,
  ChangeDigit,
  Store, StoreDot,
};
constexpr int OpCodeSize = static_cast<int>(OpCode::StoreDot) + 1;

enum class Digit {
  DECIMAL,
  HEXADECIMAL,
};

enum class NumType {
  Int,
  Float,
};

enum class Reg {
  ACC,
  INPUT,
};
constexpr int RegSize = static_cast<int>(Reg::INPUT) + 1;

enum class State {
  NumType,
  Digit,
};
constexpr int StateSize = static_cast<int>(State::Digit) + 1;

using T_opcode = OpCode;
using T_addr = int;
using T_arg = QVariant;
using T_reg_addr = Reg;
using T_stat_addr = State;

#endif // COMMON_TYPES_H
