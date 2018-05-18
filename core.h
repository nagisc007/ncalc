/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#ifndef CORE_H
#define CORE_H

#include "common_types.h"
#include "mem.h"

#include <QObject>
#include <QStack>
#include <QVector>
#include <QPair>

namespace NCALC {

using OpFnc = std::function<double(double, double)>;
using OpSet = QPair<double, double>;

class Core : public QObject
{
  Q_OBJECT
public:
  explicit Core(QObject *parent = nullptr);
  ~Core();
  // members
  Mem* r_mem;
  QScopedPointer<QVector<OpFnc>> m_calc_ops;
  int cur_min_pos;
  QScopedPointer<QStack<OpSet>> m_stack;
  QScopedPointer<QStack<OpSet>> m_stack_of_redo;
  // base
  bool InitOperationTable();
  bool SetMemoryRef(Mem*);
  // methods
  void ToReset();
  void ToClear();
  void ToChangeDigit();
  void ToCalc(T_opcode);
  void ToRegs(T_arg);
  void ToChangeType();
  void ToUndo();
  void ToRedo();
  double ToCombine(double, double);
  void ToStack(double, double);

signals:
  void ToMWin();

public slots:
  void FromMWin(T_opcode, T_arg);
};

}  // namespace NCALC

#endif // CORE_H
