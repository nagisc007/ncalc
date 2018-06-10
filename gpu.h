/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#ifndef GPU_H
#define GPU_H

#include "common_types.h"

#include <QObject>
#include <QVector>

namespace GPU {

enum class VramAddr {
  DIGIT,
};

using T_vram_addr = VramAddr;

class Core : public QObject
{
  Q_OBJECT
public:
  explicit Core(QObject *parent = nullptr);
  // members
  QVector<int> vram;
  // methods
  void Reset();
  // - vram
  int ReadVram(T_vram_addr);
  void WriteVram(T_vram_addr, int);

signals:
  void ToDisplay(T_dev_addr, T_str);

public slots:
  void FromCpu(T_bits, T_base_num, T_base_num);
};

}  // ns GPU

#endif // GPU_H
