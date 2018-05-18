/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "common_types.h"
#include "common_values.h"
#include "core.h"
#include "mem.h"

#include <QMainWindow>
#include <QThread>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  // member
  Ui::MainWindow *ui;
  QScopedPointer<NCALC::Mem> m_mem;
  QScopedPointer<NCALC::Core> m_core;
  QScopedPointer<QThread> cpu_thread;
  // base
  bool InitMemory();
  bool InitCore();
  bool InitConnections();
  bool InitThread();
  // methods
  void UpdateGPU();
  void UpdateDisplays();
  void UpdateSubDisplays();

signals:
  void ToCore(T_opcode, T_arg);

public slots:
  void FromCore();

private slots:
  /* menus */
  void on_actQuit_triggered();
  void on_actAboutQt_triggered();
  void on_actAboutApp_triggered();
  /* command buttons */
  void on_Button_Undo_clicked();
  void on_Button_Redo_clicked();
  void on_Button_Digits_clicked();
  void on_Button_Return_clicked();
  void on_Button_CL_clicked();
  void on_Button_AC_clicked();
  /* logical arithmetics */
  void on_Button_AND_clicked();
  void on_Button_OR_clicked();
  void on_Button_XOR_clicked();
  void on_Button_NOT_clicked();
  /* arithmetics */
  void on_Button_Add_clicked();
  void on_Button_Subtract_clicked();
  void on_Button_Multiply_clicked();
  void on_Button_Devide_clicked();
  /* number buttons */
  void on_Button_NUM_0_clicked();
  void on_Button_NUM_1_clicked();
  void on_Button_NUM_2_clicked();
  void on_Button_NUM_3_clicked();
  void on_Button_NUM_4_clicked();
  void on_Button_NUM_5_clicked();
  void on_Button_NUM_6_clicked();
  void on_Button_NUM_7_clicked();
  void on_Button_NUM_8_clicked();
  void on_Button_NUM_9_clicked();
  void on_Button_NUM_A_clicked();
  void on_Button_NUM_B_clicked();
  void on_Button_NUM_C_clicked();
  void on_Button_NUM_D_clicked();
  void on_Button_NUM_E_clicked();
  void on_Button_NUM_F_clicked();
  void on_Button_NUM_DOT_clicked();
};

#endif // MAINWINDOW_H
