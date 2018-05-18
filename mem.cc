/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "mem.h"

namespace NCALC {

/* class: Mem */
Mem::Mem()
{}

/* methods */
auto Mem::ToRegRead(T_reg_addr addr) -> double
{
  return m_reg0[static_cast<int>(addr)].load();
}

auto Mem::ToRegWrite(T_reg_addr addr, double val) -> void
{
  double expected = m_reg0[static_cast<int>(addr)].load();
  double desired = val;
  do {
    desired = val;
  } while (!m_reg0[static_cast<int>(addr)].compare_exchange_weak(expected, desired));
}

auto Mem::ToStateRead(T_stat_addr addr) -> int
{
  return m_reg1[static_cast<int>(addr)].load();
}

auto Mem::ToStateWrite(T_stat_addr addr, int val) -> void
{
  int expected = m_reg1[static_cast<int>(addr)].load();
  int desired = val;
  do {
    desired = val;
  } while (!m_reg1[static_cast<int>(addr)].compare_exchange_weak(expected, desired));
}

}  // ns NCALC
