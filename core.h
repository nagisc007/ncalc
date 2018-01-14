/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv2 or any later version                             *
 *                                                                         *
 ***************************************************************************/
#ifndef CORE_H
#define CORE_H

#include <QLineEdit>
#include <QObject>

namespace NCALC {

enum class OpFactor {
  NONE,
  PLUS,
  MINUS,
  MULTI,
  DEVIDE,
};

class Core : public QObject
{
  Q_OBJECT
public:
  explicit Core(QObject *parent = nullptr);
  ~Core();
  // methods: base
  bool SetDisplay(QLineEdit*);
  // methods: calc
  void AddOperate();
  void ClearOperate();
  void DevideOperate();
  void DisplayText(int);
  void MultiplyOperate();
  void PreOperate();
  void ShowResult();
  void SubtractOperate();
  // methods: menu
  void OnTo2Bin();
  void OnToHex();
  void OnBackSpace();
  void OnClear();
  void OnDevide();
  void OnMultiply();
  void OnMinus();
  void OnPlus();
  void OnEqual();
  void OnDot();
  void OnNum0();
  void OnNum1();
  void OnNum2();
  void OnNum3();
  void OnNum4();
  void OnNum5();
  void OnNum6();
  void OnNum7();
  void OnNum8();
  void OnNum9();

signals:

public slots:

private:
  bool wait_for_operand_ = false;
  double num_cache_;
  OpFactor factor_;
  QScopedPointer<QLineEdit> display_;
};

}  // namespace NCALC

#endif // CORE_H
