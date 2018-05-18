/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "core.h"

#include <QDebug>
#include <QtMath>

namespace NCALC {

/* class: Core */
Core::Core(QObject *parent) : QObject(parent),
  m_calc_ops(new QVector<OpFnc>(OpCodeSize)),
  m_stack(new QStack<OpSet>()),
  m_stack_of_redo(new QStack<OpSet>())
{
  cur_min_pos = 0;
  m_stack->push(qMakePair(0.0, 0.0));
  qDebug() << "Core: construct";
  if (!InitOperationTable()) {
    qWarning() << "Core: cannot create func table!";
  }
}

Core::~Core()
{
  if (m_stack) {
    m_stack->clear();
    m_stack.reset();
  }
  if (m_stack_of_redo) {
    m_stack_of_redo->clear();
    m_stack_of_redo.reset();
  }
  if (m_calc_ops) {
    m_calc_ops->clear();
    m_calc_ops.reset();
  }
  qDebug() << "Core: destruct";
}

/* base */
auto Core::InitOperationTable() -> bool
{
  for (int i = 0, size = m_calc_ops->size(); i < size; ++i) {
    (*m_calc_ops)[i] = [](double, double){return 0.0;};
  }
  (*m_calc_ops)[static_cast<int>(OpCode::Add)] = [](double a, double b){return a + b;};
  (*m_calc_ops)[static_cast<int>(OpCode::Subtract)] = [](double a, double b){return a - b;};
  (*m_calc_ops)[static_cast<int>(OpCode::Multiply)] = [](double a, double b){return a * b;};
  (*m_calc_ops)[static_cast<int>(OpCode::Devide)] = [](double a, double b){return a / b;};
  (*m_calc_ops)[static_cast<int>(OpCode::And)] = [](double a, double b){
    return static_cast<double>(static_cast<int>(a) & static_cast<int>(b));};
  (*m_calc_ops)[static_cast<int>(OpCode::Or)] = [](double a, double b){
    return static_cast<double>(static_cast<int>(a) | static_cast<int>(b));};
  (*m_calc_ops)[static_cast<int>(OpCode::Xor)] = [](double a, double b){
    return static_cast<double>(static_cast<int>(a) ^ static_cast<int>(b));};
  (*m_calc_ops)[static_cast<int>(OpCode::Not)] = [](double a, double){
    return static_cast<double>(~static_cast<int>(a));};
  return true;
}

auto Core::SetMemoryRef(Mem* m) -> bool
{
  if (m == nullptr) return false;

  r_mem = m;

  return true;
}

/* methods */
auto Core::ToReset() -> void
{
  r_mem->ToRegWrite(Reg::ACC, 0.0);
  r_mem->ToRegWrite(Reg::INPUT, 0.0);
  r_mem->ToStateWrite(State::NumType, static_cast<int>(NumType::Int));
  cur_min_pos = 0;
  m_stack->clear();
  m_stack_of_redo->clear();
  emit ToMWin();
}

auto Core::ToClear() -> void
{
  ToStack(r_mem->ToRegRead(Reg::ACC), r_mem->ToRegRead(Reg::INPUT));

  r_mem->ToRegWrite(Reg::INPUT, 0.0);
  cur_min_pos = 0;
  emit ToMWin();
}

auto Core::ToUndo() -> void
{
  if (m_stack->isEmpty()) return;

  auto stack = m_stack->pop();
  auto acc_val = stack.first;
  auto input_val = stack.second;
  m_stack_of_redo->push(qMakePair(r_mem->ToRegRead(Reg::ACC), r_mem->ToRegRead(Reg::INPUT)));

  r_mem->ToRegWrite(Reg::ACC, acc_val);
  r_mem->ToRegWrite(Reg::INPUT, input_val);
  emit ToMWin();
}

auto Core::ToRedo() -> void
{
  if (m_stack_of_redo->isEmpty()) return;

  auto stack = m_stack_of_redo->pop();
  auto acc_val = stack.first;
  auto input_val = stack.second;
  m_stack->push(qMakePair(r_mem->ToRegRead(Reg::ACC), r_mem->ToRegRead(Reg::INPUT)));

  r_mem->ToRegWrite(Reg::ACC, acc_val);
  r_mem->ToRegWrite(Reg::INPUT, input_val);
  emit ToMWin();
}

auto Core::ToCalc(T_opcode opcode) -> void
{
  auto acc_val = r_mem->ToRegRead(Reg::ACC);
  auto input_val = r_mem->ToRegRead(Reg::INPUT);
  ToStack(acc_val, input_val);

  auto result_val = m_calc_ops->at(static_cast<int>(opcode))(acc_val, input_val);
  r_mem->ToRegWrite(Reg::ACC, result_val);
  r_mem->ToRegWrite(Reg::INPUT, 0.0);
  cur_min_pos = 0;
  emit ToMWin();
}

auto Core::ToRegs(T_arg arg) -> void
{
  auto acc_val = r_mem->ToRegRead(Reg::ACC);
  auto input_val = r_mem->ToRegRead(Reg::INPUT);
  ToStack(acc_val, input_val);

  if (qFuzzyCompare(input_val, 0.0000)) {
    r_mem->ToRegWrite(Reg::INPUT, arg.toDouble());
  } else {
    r_mem->ToRegWrite(Reg::INPUT, ToCombine(input_val, arg.toDouble()));
  }
  emit ToMWin();
}

auto Core::ToChangeType() -> void
{
  auto type = static_cast<NumType>(r_mem->ToStateRead(State::NumType));
  if (type == NumType::Int) {
    r_mem->ToStateWrite(State::NumType, static_cast<int>(NumType::Float));
    emit ToMWin();
  }
}

auto Core::ToChangeDigit() -> void
{
  auto cur_digit = static_cast<Digit>(r_mem->ToStateRead(State::Digit));
  r_mem->ToStateWrite(State::Digit, static_cast<Digit>(cur_digit) == Digit::DECIMAL ?
                        static_cast<int>(Digit::HEXADECIMAL):
                        static_cast<int>(Digit::DECIMAL));
  emit ToMWin();
}

auto Core::ToCombine(double a, double b) -> double
{
  auto type = static_cast<NumType>(r_mem->ToStateRead(State::NumType));
  auto digit = static_cast<Digit>(r_mem->ToStateRead(State::Digit));

  if (type == NumType::Float) {
    ++cur_min_pos;
    return digit == Digit::DECIMAL ? a + b / qPow(10, cur_min_pos): a + b / qPow(16, cur_min_pos);
  } else {
    return digit == Digit::DECIMAL ? a * 10 + b: a * 16 + b;
  }
}

auto Core::ToStack(double acc, double input) -> void
{
  m_stack->push(qMakePair(acc, input));
  m_stack_of_redo->clear();
}

/* slots */
void Core::FromMWin(T_opcode opcode, T_arg arg)
{
  switch (opcode) {
  case OpCode::Store:
    ToRegs(arg);
    break;
  case OpCode::StoreDot:
    ToChangeType();
    break;
  case OpCode::Reset:
    ToReset();
    break;
  case OpCode::Clear:
    ToClear();
    break;
  case OpCode::ChangeDigit:
    ToChangeDigit();
    break;
  case OpCode::Undo:
    ToUndo();
    break;
  case OpCode::Redo:
    ToRedo();
    break;
  default:
    ToCalc(opcode);
    break;
  }
}

}  // namespace NCALC
