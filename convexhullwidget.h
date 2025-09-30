#ifndef CONVEXHULLWIDGET_H
#define CONVEXHULLWIDGET_H

#include <QWidget>
#include <QList>
#include <QPointF>
#include <QPainter>
#include <QMouseEvent>
#include <QSet>
#include <algorithm>
#include <cmath>


uint qHash(const QPointF &key, uint seed = 0) Q_DECL_NOEXCEPT;

class ConvexHullWidget : public QWidget
{
    Q_OBJECT

public:
    ConvexHullWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

public slots:

    void calculateHull();
    void clearData();

private:
    QList<QPointF> m_points;
    QList<QPointF> m_hullPoints_fast;
    QList<QPointF> m_hullPoints_slow;


    int m_bruteForceIterations = 0;
    int m_grahamScanIterations = 0;


    int orientation(QPointF p, QPointF q, QPointF r);


    QList<QPointF> grahamScan(const QList<QPointF>& points);


    QList<QPointF> bruteForceHull(const QList<QPointF>& points);
};

#endif // CONVEXHULLWIDGET_H
