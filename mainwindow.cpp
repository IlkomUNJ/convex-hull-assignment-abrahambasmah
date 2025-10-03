#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("O(N log N) vs O(N^3) Convex Hull Comparison");
    this->resize(1000, 700);


    m_hullWidget = new ConvexHullWidget(this);


    m_calculateButton = new QPushButton("Calculate Hull");
    m_clearButton = new QPushButton("Clear Canvas");


    m_controlPanel = new QWidget(this);
    QVBoxLayout *controlLayout = new QVBoxLayout(m_controlPanel);
    controlLayout->addWidget(m_calculateButton);
    controlLayout->addWidget(m_clearButton);
    controlLayout->addStretch();
    m_controlPanel->setFixedWidth(180);


    QWidget *centralWidget = new QWidget;
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    mainLayout->addWidget(m_hullWidget, 1);
    mainLayout->addWidget(m_controlPanel);

    setCentralWidget(centralWidget);


    connect(m_calculateButton, SIGNAL(clicked()), m_hullWidget, SLOT(calculateHull()));
    connect(m_clearButton, SIGNAL(clicked()), m_hullWidget, SLOT(clearData()));
}

MainWindow::~MainWindow()
{

}
