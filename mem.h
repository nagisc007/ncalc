/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#ifndef MEM_H
#define MEM_H

#include "common_types.h"
#include <atomic>

namespace NCALC {

class Mem
{
public:
  Mem();
  // members
  std::array<std::atomic<double>, RegSize> m_reg0{};
  std::array<std::atomic<int>, StateSize> m_reg1{};
  // methods
  double ToRegRead(T_reg_addr);
  void ToRegWrite(T_reg_addr, double);
  int ToStateRead(T_stat_addr);
  void ToStateWrite(T_stat_addr, int);
};

}  // ns NCALC

#endif // MEM_H
