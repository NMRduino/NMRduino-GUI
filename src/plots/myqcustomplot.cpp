#include "myqcustomplot.h"

#include <QCheckBox>
#include <QLabel>

using namespace Plot;

MyQCustomPlot::MyQCustomPlot(QWidget *parent):
    QCustomPlot(parent)
{
    show(); // Ugly hack to load stylesheet at time of object creation
#ifdef QCUSTOMPLOT_USE_OPENGL
    setOpenGl(true);
    if (not QCustomPlot::openGl()){
        qCritical() << "Couldn't load OpenGL";
    }
#endif
}

MyQCustomPlot::~MyQCustomPlot()
{

}

void MyQCustomPlot::configureCursor(QCheckBox *toggle, QLabel *readout,
                                    const QString &xUnit, const QString &yUnit)
{
    cursorToggle = toggle;
    cursorReadout = readout;
    toggle->setCursor(Qt::ArrowCursor);
    toggle->setAttribute(Qt::WA_NoMousePropagation);
    toggle->installEventFilter(this);
    readout->setAttribute(Qt::WA_TransparentForMouseEvents);
    readout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    readout->hide();

    auto placeControls = [this, toggle, readout]() {
        const QRect area = axisRect()->rect().adjusted(4, 3, -4, -3);
        toggle->setGeometry(QRect(area.topLeft(), toggle->sizeHint()));
        const int left = toggle->geometry().right() + 6;
        readout->setGeometry(left, area.top(), qMax(0, area.right() - left + 1),
                             2 * readout->fontMetrics().lineSpacing() + 4);
    };
    placeControls();
    connect(this, &QCustomPlot::afterReplot, this, placeControls);
    cursorFormat = "x = %1 " + xUnit + "     y = %2 " + yUnit;
    deltaFormat = "Δx = %1 " + xUnit + "     Δy = %2 " + yUnit;
    toggle->setToolTip(tr("Left-click to set the reference. Right-click to clear it. Drag to pan."));
    connect(toggle, &QCheckBox::toggled, this, [this](bool enabled) {
        cursorEnabled = enabled;
        referenceLocked = false;

        updateCursorReadout(mapFromGlobal(QCursor::pos()));
    });
    connect(this, &QCustomPlot::mouseMove, this, [this](QMouseEvent *event) {
        if (cursorEnabled)
            updateCursorReadout(event->pos());
    });
    connect(this, &QCustomPlot::mouseRelease, this, [this](QMouseEvent *event) {
        const QRect area = axisRect()->rect();
        if (!cursorEnabled || mMouseHasMoved || !area.contains(mMousePressPos) ||
            !area.contains(event->pos()))
            return;

        if (event->button() == Qt::LeftButton) {
            referenceCoords = QPointF(xAxis->pixelToCoord(event->x()),
                                      yAxis->pixelToCoord(event->y()));
            referenceLocked = true;
        } else if (event->button() == Qt::RightButton) {
            referenceLocked = false;
        } else {
            return;
        }
        updateCursorReadout(event->pos());
    });
    connect(this, &QCustomPlot::afterReplot, this, [this]() {
        if (cursorEnabled && underMouse())
            updateCursorReadout(mapFromGlobal(QCursor::pos()));
    });
}

void MyQCustomPlot::updateCursorReadout(const QPoint &position)
{
    livePixel = position;
    liveVisible = cursorEnabled && axisRect()->rect().contains(position)
                  && !cursorToggle->geometry().contains(position);
    cursorReadout->setVisible(liveVisible);
    if (liveVisible) {
        setCursor(Qt::BlankCursor); // The overlay supplies the pointer in the theme color.
        const double x = xAxis->pixelToCoord(position.x());
        const double y = yAxis->pixelToCoord(position.y());
        QString text = cursorFormat.arg(x, 0, 'g', 6).arg(y, 0, 'g', 6);
        if (referenceLocked)
            text += "\n" + deltaFormat.arg(x - referenceCoords.x(), 0, 'g', 6)
                                      .arg(y - referenceCoords.y(), 0, 'g', 6);
        cursorReadout->setText(text);
    } else {
        unsetCursor();
        cursorReadout->clear();
    }
    update();
}

void MyQCustomPlot::paintEvent(QPaintEvent *event)
{
    QCustomPlot::paintEvent(event);
    if (!cursorEnabled || (!liveVisible && !referenceLocked))
        return;

    const QRect area = axisRect()->rect();
    const QColor color = mainColorStyle.isValid() ? mainColorStyle : palette().highlight().color();
    QPainter painter(this);
    painter.setClipRect(area);
    painter.setRenderHint(QPainter::Antialiasing);
    auto drawCrosshair = [&](QPointF point, bool locked) {
        painter.setPen(QPen(color, 1, Qt::DotLine));
        if (point.x() >= area.left() && point.x() <= area.right())
            painter.drawLine(QPointF(point.x(), area.top()), QPointF(point.x(), area.bottom()));
        if (point.y() >= area.top() && point.y() <= area.bottom())
            painter.drawLine(QPointF(area.left(), point.y()), QPointF(area.right(), point.y()));
        if (!QRectF(area).contains(point))
            return;
        painter.setPen(QPen(color, 2, Qt::SolidLine, Qt::RoundCap));
        painter.drawLine(point - QPointF(6, 0), point + QPointF(6, 0));
        painter.drawLine(point - QPointF(0, 6), point + QPointF(0, 6));
        const qreal radius = locked ? 6 : 4;
        painter.drawEllipse(point, radius, radius);
    };

    if (referenceLocked)
        drawCrosshair(QPointF(xAxis->coordToPixel(referenceCoords.x()),
                              yAxis->coordToPixel(referenceCoords.y())), true);
    if (liveVisible)
        drawCrosshair(livePixel, false);
}

bool MyQCustomPlot::eventFilter(QObject *object, QEvent *event)
{
    if (object == cursorToggle && event->type() == QEvent::Enter && cursorEnabled)
        updateCursorReadout(QPoint(-1, -1));
    return QCustomPlot::eventFilter(object, event);
}

void MyQCustomPlot::leaveEvent(QEvent *event)
{
    if (cursorEnabled)
        updateCursorReadout(QPoint(-1, -1));
    QCustomPlot::leaveEvent(event);
}

QColor MyQCustomPlot::getColorFromPalette(int i, int n, Palettes palette)
{
    switch(palette){
    case Palettes::Main:
        return getColorFromPalette(i, n, mainColorStyle, palette);
    case Palettes::Secondary:
        return getColorFromPalette(i, n, secondaryColorStyle, palette);
    case Palettes::None:
        return getColorFromPalette(i, n, QColor(), palette);
    }
    return getColorFromPalette(i, n, QColor(), palette);
}

QColor MyQCustomPlot::getColorFromPalette(int i, int n, QColor mainColor, Palettes palette)
{
    generatePalette(mainColor, n, palette);
    return palettes.value(palette).colors.value(i);
}

void MyQCustomPlot::setBackgroundColor(QColor color)
{
    backgroundColor = color;
    setBackground(color);
}

void MyQCustomPlot::setXAxisBasePenColor(QColor color)
{
    xAxisBasePenColor = color;

    auto pen = xAxis->property("basePen").value<QPen>();
    pen.setColor(color);
    xAxis->setBasePen(pen);
}

void MyQCustomPlot::setYAxisBasePenColor(QColor color)
{
    yAxisBasePenColor = color;

    auto pen = yAxis->property("basePen").value<QPen>();
    pen.setColor(color);
    yAxis->setBasePen(pen);
}

void MyQCustomPlot::setXAxis2BasePenColor(QColor color)
{
    xAxis2BasePenColor = color;

    auto pen = xAxis2->property("basePen").value<QPen>();
    pen.setColor(color);
    xAxis2->setBasePen(pen);
}

void MyQCustomPlot::setYAxis2BasePenColor(QColor color)
{
    yAxis2BasePenColor = color;

    auto pen = yAxis2->property("basePen").value<QPen>();
    pen.setColor(color);
    yAxis2->setBasePen(pen);
}

void MyQCustomPlot::setXAxisTickPenColor(QColor color)
{
    xAxisTickPenColor = color;

    auto pen = xAxis->property("tickPen").value<QPen>();
    pen.setColor(color);
    xAxis->setTickPen(pen);
}

void MyQCustomPlot::setYAxisTickPenColor(QColor color)
{
    yAxisTickPenColor = color;

    auto pen = yAxis->property("tickPen").value<QPen>();
    pen.setColor(color);
    yAxis->setTickPen(pen);
}

void MyQCustomPlot::setXAxisSubTickPenColor(QColor color)
{
    xAxisSubTickPenColor = color;

    auto pen = xAxis->property("subTickPen").value<QPen>();
    pen.setColor(color);
    xAxis->setSubTickPen(pen);
}

void MyQCustomPlot::setYAxisSubTickPenColor(QColor color)
{
    yAxisSubTickPenColor = color;

    auto pen = yAxis->property("subTickPen").value<QPen>();
    pen.setColor(color);
    yAxis->setSubTickPen(pen);
}

void MyQCustomPlot::setGraph0PenColor(QColor color)
{
    mainColorStyle = color;
}

void MyQCustomPlot::setGraph1PenColor(QColor color)
{
    secondaryColorStyle = color;
}

void MyQCustomPlot::generatePalette(QColor mainColor, int n, Palettes palette)
{
    Palette newPalette = palettes.value(palette, Palette());
    if ((newPalette.mainColor == mainColor) and (newPalette.colors.length() == n)){
        return;
    }

    newPalette.mainColor = mainColor;
    newPalette.colors.clear();

    if (n < 1){
        return;
    } else if (n == 1){
        newPalette.colors.append(mainColor);
        palettes.insert(palette, newPalette);
        return;
    }

    float weightLast, weightMain;
    int r, g, b;

    QColor lastColor(192, 192, 192, 255);
    for (int i=0; i<n; i++){
        weightLast = float(i) / (n-1);
        weightMain = 1 - weightLast;

        r = round(weightMain*mainColor.red()   + weightLast*lastColor.red());
        g = round(weightMain*mainColor.green() + weightLast*lastColor.green());
        b = round(weightMain*mainColor.blue()  + weightLast*lastColor.blue());

        newPalette.colors.append(QColor(r, g, b, 255));
    }
    palettes.insert(palette, newPalette);
}
