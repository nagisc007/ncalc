/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv2 or any later version                             *
 *                                                                         *
 ***************************************************************************/
#ifndef CORE_H
#define CORE_H

#include <QLabel>
#include <QLineEdit>
#include <QObject>

namespace NCALC {

enum class OpFactor {
  NONE,
  PLUS,
  MINUS,
  MULTI,
  DEVIDE,
  DOT,
  EQUAL,
  AND,
  OR,
  NOT,
  XOR,
};

enum class OpMode {
  BIT,
  DECIMAL,
  HEX,
};

class Core : public QObject
{
  Q_OBJECT
public:
  explicit Core(QObject *parent = nullptr);
  ~Core();
  // methods: base
  bool SetDisplay(QLineEdit*);
  bool SetModeLabel(QLabel*);
  // methods: calc
  void AddOperate();
  void ChopText();
  void ClearOperate();
  void DevideOperate();
  void DisplayText(int);
  void MultiplyOperate();
  void PointText();
  void PreOperate();
  void ShowResult();
  void SubtractOperate();
  // methods: menu
  void OnBackSpace();
  void OnClear();
  void OnOperate(OpFactor);
  void OnOperate(OpMode);
  void OnNumber(int);

signals:

public slots:

private:
  bool wait_for_operand_ = false;
  double num_cache_;
  OpFactor factor_;
  OpMode mode_;
  QScopedPointer<QLineEdit> display_;
  QScopedPointer<QLabel> mode_label_;
};

}  // namespace NCALC

#endif // CORE_H
