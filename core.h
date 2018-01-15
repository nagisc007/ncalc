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
#include <QList>
#include <QLineEdit>
#include <QObject>
#include <QStack>

namespace NCALC {

enum OpCode {
  NOP,
  ADD,
  SUBTRACT,
  MULTIPLY,
  DEVIDE,
  L_AND,
  L_OR,
  L_NOT,
  L_XOR,
};

struct OpCodeInfo {
  static const int SIZE = 9;
};

enum class DispMode {
  BIN,
  DECIMAL,
  HEX,
};

using OpFnc = std::function<double(double, double)>;

class Core : public QObject
{
  Q_OBJECT
public:
  explicit Core(QObject *parent = nullptr);
  ~Core();
  // utils
  class NumConverter {
  public:
    double operator()(const QString&, DispMode) const;
    QString operator()(double, DispMode);
  };
  // utils: operate
  class Nothing {
  public:
    double operator()(double, double);
  };
  class AddFnc {
  public:
    double operator()(double, double);
  };
  class SubtractFnc {
  public:
    double operator()(double, double);
  };
  class MultiplyFnc {
  public:
    double operator()(double, double);
  };
  class DevideFnc {
  public:
    double operator()(double, double);
  };
  class LogicalAndFnc {
  public:
    double operator()(double, double);
  };
  class LogicalOrFnc {
  public:
    double operator()(double, double);
  };
  class LogicalNotFnc {
  public:
    double operator()(double, double);
  };
  class LogicalXorFnc {
  public:
    double operator()(double, double);
  };
  class AppendNumToStr {
  public:
    double operator()(double, int);
  };
  // methods: base
  bool InitFncTable();
  bool SetDisplay(QLineEdit*, QLineEdit*);
  bool SetModeLabel(QLabel*);
  // methods: calc
  void AppendNumber(int);
  void AppendPoint();
  void ChopCurrent();
  // methods
  void ChangeMode(DispMode);
  void Reset();
  void UpdateDisplay(double, double, DispMode);
  // methods: menu
  void OnBackSpace();
  void OnClear();
  void OnOperate(OpCode);
  void OnNumber(int);

signals:
  void failed(const QString&);

public slots:

private:
  double acc_;
  double current_;
  DispMode mode_;
  QScopedPointer<QList<OpFnc>> table_;
  QScopedPointer<QStack<OpFnc>> stack_;
  QScopedPointer<QLineEdit> display_;
  QScopedPointer<QLineEdit> display2_;
  QScopedPointer<QLabel> mode_label_;
};

}  // namespace NCALC

#endif // CORE_H
