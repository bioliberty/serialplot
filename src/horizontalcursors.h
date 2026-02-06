/*
  Copyright © 2026 Hasan Yavuz Özderya

  This file is part of serialplot.

  serialplot is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  serialplot is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with serialplot.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HORIZONTALCURSORS_H
#define HORIZONTALCURSORS_H

#include <QObject>
#include <qwt_plot_marker.h>
#include <qwt_plot.h>

class HorizontalCursors : public QObject
{
    Q_OBJECT

public:
    explicit HorizontalCursors(QwtPlot* plot, QObject *parent = nullptr);
    ~HorizontalCursors();

    /// Enable or disable cursor display
    void setEnabled(bool enabled);
    bool isEnabled() const;

    /// Set cursor positions
    void setCursor1Position(double yValue);
    void setCursor2Position(double yValue);

    /// Get cursor positions
    double cursor1Position() const;
    double cursor2Position() const;

    /// Get the difference between the two cursors
    double cursorDifference() const;

    /// Set cursor colors
    void setCursor1Color(const QColor& color);
    void setCursor2Color(const QColor& color);

    /// Set cursor line style
    void setCursorLineStyle(Qt::PenStyle style);
    void setCursorLineWidth(int width);

    /// Update dark mode colors
    void setDarkBackground(bool enabled);

signals:
    void cursor1PositionChanged(double yValue);
    void cursor2PositionChanged(double yValue);
    void cursorDifferenceChanged(double difference);

public slots:
    void replot();

private:
    QwtPlot* m_plot;
    QwtPlotMarker* m_cursor1;
    QwtPlotMarker* m_cursor2;
    bool m_enabled;
    bool m_darkBackground;

    void updateCursorVisibility();
    void setupCursor(QwtPlotMarker* cursor, const QColor& color);
};

#endif // HORIZONTALCURSORS_H
