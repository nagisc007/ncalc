/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#ifndef CPU_H
#define CPU_H

#include "common_types.h"

#include <QObject>
#include <QVector>
#include <QStack>
#include <QPair>

namespace CPU {

enum class RamAddr {
  ACC,
  INPUT,
  STACK,
  DIGIT,
  DOT,
};
const int RAM_MEM_SIZE = 2;
const int RAM_REG_SIZE = 3;
const int RAM_SIZE = RAM_MEM_SIZE + RAM_REG_SIZE;

enum class UndoAddr {
  UNDO,
  REDO,
};

using T_ram_addr = RamAddr;
using T_undo_addr = UndoAddr;
using T_is_written = bool;
using acc_input = QPair<T_base_num, T_base_num>;

struct Ram
{
  QVector<T_base_num> mem;
  QVector<int> reg;
  Ram():
    mem(QVector<T_base_num>(RAM_MEM_SIZE)),
    reg(QVector<int>(RAM_REG_SIZE)){}
};

struct Stacks
{
  QStack<acc_input> redo;
  QStack<acc_input> undo;
};

class Core : public QObject
{
  Q_OBJECT
public:
  // class constructor
  explicit Core(QObject *parent = nullptr);
  // members
  Ram ram;
  Stacks stack;
  // methods
  void Reset();
  void ToCmdButton(T_cmd_btn);
  void ToNumButton(T_num_btn);
  // - input
  bool IsNumInputting();
  void IncrementDotPos();
  // - ram
  template<typename T> T ReadRam(T_ram_addr);
  template<typename T> void WriteRam(T_ram_addr, T);
  T_base_num GetAcc();
  T_base_num GetInput();
  void ClearInput();
  // - command
  bool IsExistsCmdStack();
  void PushCmd(T_cmd_btn);
  T_cmd_btn PopCmd();
  void RunCmd(T_cmd_btn, T_is_written);
  void RunControlCmd(T_cmd_btn);
  T_base_num Calculated(T_cmd_btn, T_base_num, T_base_num);
  // - undo stacks
  void PushUndoStack(T_undo_addr, T_base_num, T_base_num);
  acc_input PopUndoStack(T_undo_addr);

signals:
  void ToGpu(T_bits, T_base_num, T_base_num);

public slots:
  void FromInput(T_bits);
};

}  // ns CPU

#endif // CPU_H
