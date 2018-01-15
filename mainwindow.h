/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv2 or any later version                             *
 *                                                                         *
 ***************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "core.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  // methods: base
  void InitWidgets();
  void Quit();

public slots:
  void OnCatchFailure(const QString&);

private slots:
  void on_actionQuit_triggered();
  void on_Button_To2Bin_clicked();
  void on_Button_ToDecimal_clicked();
  void on_Button_ToHex_clicked();
  void on_Button_BC_clicked();
  void on_Button_C_clicked();
  void on_Button_Devide_clicked();
  void on_Button_Multiply_clicked();
  void on_Button_Minus_clicked();
  void on_Button_Plus_clicked();
  void on_Button_Equal_clicked();
  void on_Button_Dot_clicked();
  void on_Button_0_clicked();
  void on_Button_1_clicked();
  void on_Button_2_clicked();
  void on_Button_3_clicked();
  void on_Button_4_clicked();
  void on_Button_5_clicked();
  void on_Button_6_clicked();
  void on_Button_7_clicked();
  void on_Button_8_clicked();
  void on_Button_9_clicked();
  void on_Button_And_clicked();
  void on_Button_Or_clicked();
  void on_Button_Not_clicked();
  void on_Button_Xor_clicked();

private:
  Ui::MainWindow *ui;
  QScopedPointer<NCALC::Core> core_;
};

#endif // MAINWINDOW_H
