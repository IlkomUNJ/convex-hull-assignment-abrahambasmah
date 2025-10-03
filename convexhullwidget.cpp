#include "convexhullwidget.h"
#include <QVector>
#include <QDebug>


struct PolarAngleComparator {
    QPointF p0;
    int *counter;

    PolarAngleComparator(QPointF referencePoint, int *iterCounter)
        : p0(referencePoint), counter(iterCounter) {}

    int orientation(QPointF p, QPointF q, QPointF r) const {
        qreal val = (q.y() - p.y()) * (r.x() - q.x()) -
                    (q.x() - p.x()) * (r.y() - q.y());
        if (qFuzzyIsNull(val)) return 0;
        return (val > 0) ? 1 : 2;
    }

    bool operator()(const QPointF& a, const QPointF& b) const {
        (*counter)++;
        int o = orientation(p0, a, b);

        if (o == 0) {
            qreal distSqA = std::pow(a.x() - p0.x(), 2) + std::pow(a.y() - p0.y(), 2);
            qreal distSqB = std::pow(b.x() - p0.x(), 2) + std::pow(b.y() - p0.y(), 2);
            return distSqA < distSqB;
        }
        return (o == 2);
    }
};



int ConvexHullWidget::orientation(QPointF p, QPointF q, QPointF r) {
    qreal val = (q.y() - p.y()) * (r.x() - q.x()) -
                (q.x() - p.x()) * (r.y() - q.y());

    if (qFuzzyIsNull(val)) return 0;
    return (val > 0) ? 1 : 2;
}


QList<QPointF> ConvexHullWidget::grahamScan(const QList<QPointF>& points) {
    int n = points.size();
    if (n < 3) return points;

    m_grahamScanIterations = 0;

    QPointF p0 = points.first();
    for (const QPointF& p : points) {
        if (p.y() < p0.y() || (qFuzzyCompare(p.y(), p0.y()) && p.x() < p0.x())) {
            p0 = p;
        }
    }

    QList<QPointF> sortedPoints = points;
    sortedPoints.removeOne(p0);

    if (n > 1) {
        m_grahamScanIterations += (int)(n * std::log2(n) * 1.5);
    }

    PolarAngleComparator comparator(p0, &m_grahamScanIterations);
    std::sort(sortedPoints.begin(), sortedPoints.end(), comparator);

    while (sortedPoints.size() > 1 && orientation(p0, sortedPoints.first(), sortedPoints.at(1)) == 0) {
        sortedPoints.removeFirst();
    }

    if (sortedPoints.size() < 2) return points;

    QList<QPointF> hullStack;
    hullStack.append(p0);
    hullStack.append(sortedPoints[0]);
    hullStack.append(sortedPoints[1]);

    for (int i = 2; i < sortedPoints.size(); ++i) {
        while (hullStack.size() > 1 && (m_grahamScanIterations++, orientation(hullStack[hullStack.size() - 2],
                                                                              hullStack.last(),
                                                                              sortedPoints[i]) != 2))
        {
            hullStack.removeLast();
        }
        hullStack.append(sortedPoints[i]);
    }
    return hullStack;
}

QList<QPointF> ConvexHullWidget::bruteForceHull(const QList<QPointF>& points) {
    int n = points.size();
    if (n < 3) return points;

    m_bruteForceIterations = 0;


    QList<QPointF> hullPoints;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) continue;

            bool isHullEdge = true;
            int referenceSign = 0;

            for (int k = 0; k < n; k++) {
                if (k == i || k == j) continue;

                m_bruteForceIterations++;
                int o = orientation(points[i], points[j], points[k]);

                if (o != 0) {
                    int currentSign = (o == 1) ? 1 : 2;

                    if (referenceSign == 0) {
                        referenceSign = currentSign;
                    } else if (currentSign != referenceSign) {
                        isHullEdge = false;
                        break;
                    }
                }
            }

            if (isHullEdge) {
                // Check and insert unique points
                if (!hullPoints.contains(points[i])) {
                    hullPoints.append(points[i]);
                }
                if (!hullPoints.contains(points[j])) {
                    hullPoints.append(points[j]);
                }
            }
        }
    }

    return hullPoints;
}



ConvexHullWidget::ConvexHullWidget(QWidget *parent)
    : QWidget(parent)
{
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::white);
    setPalette(pal);
}

void ConvexHullWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_hullPoints_fast.clear();
        m_hullPoints_slow.clear();
        m_points.append(event->pos());
        update();
    }
}

void ConvexHullWidget::calculateHull() {
    if (m_points.size() >= 3) {
        m_hullPoints_fast = grahamScan(m_points);
        m_hullPoints_slow = bruteForceHull(m_points);
        update();
    }
}

void ConvexHullWidget::clearData() {
    m_points.clear();
    m_hullPoints_fast.clear();
    m_hullPoints_slow.clear();
    m_bruteForceIterations = 0;
    m_grahamScanIterations = 0;
    update();
}

void ConvexHullWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);


    painter.setPen(QPen(Qt::red, 1));
    painter.setBrush(Qt::red);
    for (const QPointF& p : m_points) {
        painter.drawEllipse(p, 4, 4);
    }


    if (m_hullPoints_slow.size() >= 3) {
        painter.setPen(QPen(Qt::darkCyan, 5));
        painter.setBrush(Qt::darkCyan);
        for (const QPointF& p : m_hullPoints_slow) {
            painter.drawEllipse(p, 6, 6);
        }
    }


    if (m_hullPoints_fast.size() >= 3) {
        QPolygonF hull(m_hullPoints_fast);
        hull << m_hullPoints_fast.first();
        painter.setPen(QPen(Qt::blue, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.setBrush(QColor(100, 100, 255, 80));
        painter.drawPolygon(hull);
    }
    else if (m_points.size() == 2 && m_hullPoints_fast.size() == 2) {
        painter.setPen(QPen(Qt::blue, 3));
        painter.drawLine(m_points.first(), m_points.last());
    }


    QFont font = painter.font();
    font.setBold(true);

    int padding = 20;


    painter.setFont(font);
    painter.setPen(Qt::darkCyan);
    QString slowText = QString("Brute-Force Iterations: %L1 (O(N^3))").arg(m_bruteForceIterations);
    painter.drawText(padding, padding, slowText);


    painter.setPen(Qt::blue);
    QString fastText = QString("General Hull Iterations: %L1 (O(N log N))").arg(m_grahamScanIterations);
    painter.drawText(padding, padding + 30, fastText);


    font.setPointSize(12);
    painter.setFont(font);
    painter.setPen(Qt::black);
    QString totalText = QString("Total Points: %1").arg(m_points.size());
    painter.drawText(padding, padding + 70, totalText);
}
