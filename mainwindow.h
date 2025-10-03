#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "convexhullwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    ConvexHullWidget *m_hullWidget;
    QPushButton *m_calculateButton;
    QPushButton *m_clearButton;
    QWidget *m_controlPanel;
};

#endif // MAINWINDOW_H
