#ifndef GROUPVISIBILITYWINDOW_H
#define GROUPVISIBILITYWINDOW_H


#include "UtochnenieSimple/Modules.h"

#include <toolboxEigen/Eigen/Dense>


using namespace std;
using namespace Eigen;

class VerticalLabel : public QLabel {
public:
    using QLabel::QLabel;

protected:
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        painter.setPen(palette().color(QPalette::Text));
        painter.setFont(font());

        // Сдвигаем систему координат и поворачиваем на 90 или 270 градусов
        painter.translate(0, height());
        painter.rotate(-90);

        // Рисуем текст. Учтите, что после поворота ширина и высота "меняются местами"
        painter.drawText(0, 0, height(), width(), alignment(), text());
    }

    QSize sizeHint() const override {
        QSize s = QLabel::sizeHint();
        return QSize(s.height(), s.width()); // Переворачиваем размеры
    }
};

namespace Ui {
class GroupVisibilityWindow;
}

class GroupVisibilityWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GroupVisibilityWindow(QWidget *parent = nullptr);
    ~GroupVisibilityWindow();
    void PastData(QVector<ChoosedStantion>, QVector<ChoosedSatellite>, QVector<ChoosedGroup>);
    QVector<ChoosedStantion> Stantions;
    QVector<ChoosedSatellite> Satellites;
    QVector<ChoosedGroup> Groups;
    void PastDataOnFrames(bool);
    void PastDataOnFrames2();
    bool fl_stan;
    Matrix<int, Dynamic, Dynamic> MakeCyclogramSat();
    Matrix<int, Dynamic, Dynamic> MakeCyclogramStan();
    QVector<QVector<QCheckBox*>> CBSat;
    QVector<QCheckBox*> CBStan;
    QVector<QVector<QPushButton*>> btns_cycl;
    Matrix<int, Dynamic, Dynamic> CyclSat, CyclStan;

private:
    Ui::GroupVisibilityWindow *ui;
    void btn_cycl_click(int, int);

private slots:
    void on_btnNext_clicked();

signals:
    void openMainWindowRequested();
};

#endif // GROUPVISIBILITYWINDOW_H








