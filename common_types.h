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
#include <QString>
#include <QVariant>

namespace CPU {

enum class Addr {
  NUM = 0x01,
  CMD = 0x02,
};

enum class NumButton {
  NUM_0,
  NUM_1, NUM_2, NUM_3,
  NUM_4, NUM_5, NUM_6,
  NUM_7, NUM_8, NUM_9,
  NUM_A, NUM_B, NUM_C,
  NUM_D, NUM_E, NUM_F,
  NUM_DOT,
};

enum class CmdButton {
  NOP,
  ADD, SUBTRACT, MULTIPLY, DIVIDE,
  AND, OR, XOR, NOT,
  CLEAR_INPUT, CLEAR_ALL,
  REDO, UNDO,
  CHANGE_DIGIT,
};

}  // ns CPU

namespace GPU {

enum class Addr {
  DIGIT = 0x01,
  CMD = 0x02,
  INPUT = 0x04,
  RESULT = 0x08,
  ALL = DIGIT | CMD | INPUT | RESULT,
};

}  // ns GPU

namespace DEV {

enum class Addr {
  RESULT = 0x01,
  INPUT = 0x02,
  DIGIT = 0x04,
  CMD = 0x08,
  ALL = RESULT | INPUT | DIGIT | CMD,
};

}  // ns DEV

using T_base_num = double;
using T_bits = int;
using T_str = QString;

using T_num_btn = CPU::NumButton;
using T_cmd_btn = CPU::CmdButton;
using T_cpu_addr = CPU::Addr;
using T_gpu_addr = GPU::Addr;
using T_dev_addr = DEV::Addr;

#endif // COMMON_TYPES_H
