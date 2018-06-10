/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "cpu.h"
#include <QDebug>
#include <QtMath>

namespace CPU {

// utilities
namespace {

T_bits _bitsCombined(T_gpu_addr addr, T_bits digit, T_bits cmd, T_bits dot)
{
  return (static_cast<int>(addr) << 8*3) | (digit << 8*2) | (cmd << 8*1) | dot;
}

T_base_num _numberDotTested(T_base_num n, bool is_hex_mode, int current_dot_pos)
{
  if (current_dot_pos == 0 || qFuzzyCompare(n, 0.0000)) return n;

  Q_ASSERT(current_dot_pos > 1);
  return is_hex_mode ? n / qPow(16, current_dot_pos - 1):
                       n / qPow(10, current_dot_pos - 1);
}

T_base_num _numberCombined(T_base_num base, T_base_num appended,
                           bool is_hex_mode, int current_dot_pos)
{
  if (current_dot_pos > 0) {
    return base + _numberDotTested(appended, is_hex_mode, current_dot_pos);
  } else {
    return is_hex_mode ? base * 16 + appended:
                         base * 10 + appended;
  }
}

int _GetAddrBit(T_bits b)
{
  return (static_cast<int>(b) >> 8*1) & 0xff;
}

int _GetValueBit(T_bits b)
{
  return static_cast<int>(b) & 0xff;
}

bool _IsControlCmd(T_cmd_btn c)
{
  return c == CmdButton::CLEAR_INPUT ||
      c == CmdButton::CLEAR_ALL ||
      c == CmdButton::UNDO ||
      c == CmdButton::REDO ||
      c == CmdButton::CHANGE_DIGIT;
}

bool _IsExistsCmdBit(int addr_bit)
{
  return (addr_bit & static_cast<int>(Addr::CMD)) != 0;
}

bool _IsDotNum(T_num_btn n)
{
  return n == NumButton::NUM_DOT;
}

}  // ns inner global

// class: CPU::Core
Core::Core(QObject *parent) : QObject(parent)
{}

// slots
void Core::FromInput(T_bits b)
{
  if (_IsExistsCmdBit(_GetAddrBit(b))) {
    ToCmdButton(static_cast<T_cmd_btn>(_GetValueBit(b)));
  } else {
    ToNumButton(static_cast<T_num_btn>(_GetValueBit(b)));
  }
}

// Ram
template<>
T_base_num Core::ReadRam<T_base_num>(T_ram_addr addr)
{
  auto va_addr = static_cast<int>(addr);
  Q_ASSERT(va_addr >= 0 && va_addr < RAM_MEM_SIZE);

  return ram.mem.at(va_addr);
}

template<>
int Core::ReadRam<int>(T_ram_addr addr)
{
  auto va_addr = static_cast<int>(addr);
  Q_ASSERT(va_addr >= RAM_MEM_SIZE && va_addr < RAM_SIZE);

  return ram.reg.at(va_addr - RAM_MEM_SIZE);
}

template<>
void Core::WriteRam<T_base_num>(T_ram_addr addr, T_base_num val)
{
  auto va_addr = static_cast<int>(addr);
  Q_ASSERT(va_addr >= 0 & va_addr < RAM_MEM_SIZE);

  ram.mem.replace(va_addr, val);
}

template<>
void Core::WriteRam<int>(T_ram_addr addr, int val)
{
  auto va_addr = static_cast<int>(addr);
  Q_ASSERT(va_addr >= RAM_MEM_SIZE || va_addr < RAM_SIZE);

  ram.reg.replace(va_addr - RAM_MEM_SIZE, val);
}

auto Core::GetAcc() -> T_base_num
{
  return ReadRam<T_base_num>(RamAddr::ACC);
}

auto Core::GetInput() -> T_base_num
{
  return ReadRam<T_base_num>(RamAddr::INPUT);
}

auto Core::ClearInput() -> void
{
  WriteRam<T_base_num>(RamAddr::INPUT, 0.0);
  WriteRam<int>(RamAddr::DOT, 0);
}

// base methods
auto Core::Reset() -> void
{
  for (auto& v: ram.reg) v = 0;
  for (auto& v: ram.mem) v = 0.0;
  WriteRam<int>(RamAddr::STACK, static_cast<int>(CmdButton::ADD));
  emit ToGpu(_bitsCombined(GPU::Addr::ALL,
                           0,
                           static_cast<T_bits>(CmdButton::NOP),
                           0),
             0.0, 0.0);
}

// stream
void Core::ToCmdButton(T_cmd_btn c)
{
  int cmd_val = 0;
  if (_IsControlCmd(c)) {
    RunCmd(c, false);
  } else {
    PushUndoStack(UndoAddr::UNDO,
                  ReadRam<T_base_num>(RamAddr::ACC), ReadRam<T_base_num>(RamAddr::INPUT));
    if (IsExistsCmdStack()) {
      RunCmd(PopCmd(), true);
    }
    PushCmd(c);
    cmd_val = static_cast<int>(c);
  }
  emit ToGpu(_bitsCombined(GPU::Addr::ALL,
                           ReadRam<int>(RamAddr::DIGIT),
                           cmd_val,
                           ReadRam<int>(RamAddr::DOT)),
             ReadRam<T_base_num>(RamAddr::INPUT),
             ReadRam<T_base_num>(RamAddr::ACC));
}

void Core::ToNumButton(T_num_btn n)
{
  PushUndoStack(UndoAddr::UNDO,
                ReadRam<T_base_num>(RamAddr::ACC), ReadRam<T_base_num>(RamAddr::INPUT));
  if (_IsDotNum(n)) {
    if (ReadRam<int>(RamAddr::DOT) > 0) return;
    IncrementDotPos();
  } else {
    if (ReadRam<int>(RamAddr::DOT) > 0) IncrementDotPos();
    WriteRam<T_base_num>(RamAddr::INPUT,
          (IsNumInputting() ||
           ReadRam<int>(RamAddr::DOT) > 0) ?
                           _numberCombined(ReadRam<T_base_num>(RamAddr::INPUT),
                                           static_cast<T_base_num>(n),
                                           ReadRam<int>(RamAddr::DIGIT),
                                           ReadRam<int>(RamAddr::DOT)):
                           static_cast<T_base_num>(n));
  }
  emit ToGpu(_bitsCombined(GPU::Addr::INPUT,
                           0,
                           0,
                           ReadRam<int>(RamAddr::DOT)),
             ReadRam<T_base_num>(RamAddr::INPUT),
             0.0);
}

// Input
auto Core::IsNumInputting() -> bool
{
  return !(qFuzzyCompare(ReadRam<T_base_num>(RamAddr::INPUT), 0.0000));
}

auto Core::IncrementDotPos() -> void
{
  WriteRam<int>(RamAddr::DOT, ReadRam<int>(RamAddr::DOT) + 1);
}

// Stack
auto Core::IsExistsCmdStack() -> bool
{
  return !(ReadRam<int>((RamAddr::STACK)) == 0);
}

auto Core::PushCmd(T_cmd_btn c) -> void
{
  WriteRam<int>(RamAddr::STACK, static_cast<int>(c));
}

auto Core::PopCmd() -> T_cmd_btn
{
  return static_cast<T_cmd_btn>(ReadRam<int>(RamAddr::STACK));
}

// Undo and Redo
auto Core::PushUndoStack(T_undo_addr addr, T_base_num acc, T_base_num input) -> void
{
  if (addr == UndoAddr::UNDO) {
    stack.undo.push(qMakePair(acc, input));
  } else {
    stack.redo.push(qMakePair(acc, input));
  }
}

auto Core::PopUndoStack(T_undo_addr addr) -> acc_input
{
  auto get_current = [&]{
    return qMakePair(ReadRam<T_base_num>(RamAddr::ACC), ReadRam<T_base_num>(RamAddr::INPUT));
  };
  if (addr == UndoAddr::UNDO) {
    return stack.undo.empty() ? get_current(): stack.undo.pop();
  } else {
    return stack.redo.empty() ? get_current(): stack.redo.pop();
  }
}

// Command
auto Core::RunCmd(T_cmd_btn c, T_is_written is_written) -> void
{
  if (is_written) {
    WriteRam<T_base_num>(RamAddr::ACC,
                         Calculated(c, ReadRam<T_base_num>(RamAddr::ACC),
                                    ReadRam<T_base_num>(RamAddr::INPUT)));
    ClearInput();
  } else {
    RunControlCmd(c);
  }
}

auto Core::Calculated(T_cmd_btn c, T_base_num acc, T_base_num input) -> T_base_num
{
  switch (c) {
  case CmdButton::ADD: return acc + input;
  case CmdButton::SUBTRACT: return acc - input;
  case CmdButton::MULTIPLY: return acc * input;
  case CmdButton::DIVIDE: {
    if (qFuzzyCompare(input, 0.0000)) return 0.0;  // safe value
    return acc / input;
  }
  case CmdButton::AND:
    return static_cast<T_base_num>(static_cast<int>(acc) & static_cast<int>(input));
  case CmdButton::OR:
    return static_cast<T_base_num>(static_cast<int>(acc) | static_cast<int>(input));
  case CmdButton::XOR:
    return static_cast<T_base_num>(static_cast<int>(acc) ^ static_cast<int>(input));
  case CmdButton::NOT: return static_cast<T_base_num>(~static_cast<int>(acc));
  case CmdButton::NOP:
  default:
    return acc;
  }
}

auto Core::RunControlCmd(T_cmd_btn c) -> void
{
  auto write_back = [&](T_base_num a, T_base_num i){
    WriteRam<T_base_num>(RamAddr::ACC, a);
    WriteRam<T_base_num>(RamAddr::INPUT, i);
  };
  switch (c) {
  case CmdButton::CLEAR_INPUT:
    ClearInput();
    break;
  case CmdButton::CLEAR_ALL:
    Reset();
    break;
  case CmdButton::UNDO: {
    PushUndoStack(UndoAddr::REDO,
                  ReadRam<T_base_num>(RamAddr::ACC), ReadRam<T_base_num>(RamAddr::INPUT));
    auto values = PopUndoStack(UndoAddr::UNDO);
    write_back(values.first, values.second);
    break;
  }
  case CmdButton::REDO: {
    PushUndoStack(UndoAddr::UNDO,
                  ReadRam<T_base_num>(RamAddr::ACC), ReadRam<T_base_num>(RamAddr::INPUT));
    auto values = PopUndoStack(UndoAddr::REDO);
    write_back(values.first, values.second);
    break;
  }
  case CmdButton::CHANGE_DIGIT: {
    auto current = ReadRam<int>(RamAddr::DIGIT);
    WriteRam<int>(RamAddr::DIGIT, !current);
    break;
  }
  default:
    break;
  }
}

}  // ns CPU
