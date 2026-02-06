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

#include "horizontalcursors.h"
#include <qwt_symbol.h>
#include <qwt_text.h>

HorizontalCursors::HorizontalCursors(QwtPlot* plot, QObject *parent)
    : QObject(parent)
    , m_plot(plot)
    , m_enabled(false)
    , m_darkBackground(false)
{
    // Create cursor 1
    m_cursor1 = new QwtPlotMarker();
    m_cursor1->setLineStyle(QwtPlotMarker::HLine);
    m_cursor1->setYValue(0.0);
    m_cursor1->setLabel(QwtText("C1"));
    setupCursor(m_cursor1, QColor(0, 255, 255)); // Cyan

    // Create cursor 2
    m_cursor2 = new QwtPlotMarker();
    m_cursor2->setLineStyle(QwtPlotMarker::HLine);
    m_cursor2->setYValue(0.0);
    m_cursor2->setLabel(QwtText("C2"));
    setupCursor(m_cursor2, QColor(255, 255, 0)); // Yellow

    // Initially hidden
    updateCursorVisibility();
}

HorizontalCursors::~HorizontalCursors()
{
    if (m_cursor1)
    {
        m_cursor1->detach();
        delete m_cursor1;
    }
    if (m_cursor2)
    {
        m_cursor2->detach();
        delete m_cursor2;
    }
}

void HorizontalCursors::setupCursor(QwtPlotMarker* cursor, const QColor& color)
{
    QPen pen(color);
    pen.setWidth(1);
    pen.setStyle(Qt::DashLine);
    cursor->setLinePen(pen);

    QwtText label = cursor->label();
    label.setColor(color);
    label.setBorderPen(QPen(color));
    label.setBackgroundBrush(QBrush(QColor(color.red(), color.green(), color.blue(), 50)));
    cursor->setLabel(label);
    cursor->setLabelAlignment(Qt::AlignRight | Qt::AlignTop);
}

void HorizontalCursors::setEnabled(bool enabled)
{
    m_enabled = enabled;
    updateCursorVisibility();
    replot();
}

bool HorizontalCursors::isEnabled() const
{
    return m_enabled;
}

void HorizontalCursors::updateCursorVisibility()
{
    if (m_enabled)
    {
        m_cursor1->attach(m_plot);
        m_cursor2->attach(m_plot);
    }
    else
    {
        m_cursor1->detach();
        m_cursor2->detach();
    }
}

void HorizontalCursors::setCursor1Position(double yValue)
{
    m_cursor1->setYValue(yValue);
    emit cursor1PositionChanged(yValue);
    emit cursorDifferenceChanged(cursorDifference());
    replot();
}

void HorizontalCursors::setCursor2Position(double yValue)
{
    m_cursor2->setYValue(yValue);
    emit cursor2PositionChanged(yValue);
    emit cursorDifferenceChanged(cursorDifference());
    replot();
}

double HorizontalCursors::cursor1Position() const
{
    return m_cursor1->yValue();
}

double HorizontalCursors::cursor2Position() const
{
    return m_cursor2->yValue();
}

double HorizontalCursors::cursorDifference() const
{
    return m_cursor2->yValue() - m_cursor1->yValue();
}

void HorizontalCursors::setCursor1Color(const QColor& color)
{
    QPen pen = m_cursor1->linePen();
    pen.setColor(color);
    m_cursor1->setLinePen(pen);

    QwtText label = m_cursor1->label();
    label.setColor(color);
    label.setBorderPen(QPen(color));
    label.setBackgroundBrush(QBrush(QColor(color.red(), color.green(), color.blue(), 50)));
    m_cursor1->setLabel(label);
    replot();
}

void HorizontalCursors::setCursor2Color(const QColor& color)
{
    QPen pen = m_cursor2->linePen();
    pen.setColor(color);
    m_cursor2->setLinePen(pen);

    QwtText label = m_cursor2->label();
    label.setColor(color);
    label.setBorderPen(QPen(color));
    label.setBackgroundBrush(QBrush(QColor(color.red(), color.green(), color.blue(), 50)));
    m_cursor2->setLabel(label);
    replot();
}

void HorizontalCursors::setCursorLineStyle(Qt::PenStyle style)
{
    QPen pen1 = m_cursor1->linePen();
    pen1.setStyle(style);
    m_cursor1->setLinePen(pen1);

    QPen pen2 = m_cursor2->linePen();
    pen2.setStyle(style);
    m_cursor2->setLinePen(pen2);
    replot();
}

void HorizontalCursors::setCursorLineWidth(int width)
{
    QPen pen1 = m_cursor1->linePen();
    pen1.setWidth(width);
    m_cursor1->setLinePen(pen1);

    QPen pen2 = m_cursor2->linePen();
    pen2.setWidth(width);
    m_cursor2->setLinePen(pen2);
    replot();
}

void HorizontalCursors::setDarkBackground(bool enabled)
{
    m_darkBackground = enabled;
    
    // Adjust colors for visibility
    if (enabled)
    {
        // Brighter colors for dark background
        setCursor1Color(QColor(100, 255, 255));  // Bright cyan
        setCursor2Color(QColor(255, 255, 100));  // Bright yellow
    }
    else
    {
        // Standard colors for light background
        setCursor1Color(QColor(0, 200, 200));    // Cyan
        setCursor2Color(QColor(200, 200, 0));    // Yellow
    }
}

void HorizontalCursors::replot()
{
    if (m_plot)
    {
        m_plot->replot();
    }
}
