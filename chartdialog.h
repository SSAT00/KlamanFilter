#ifndef CHARTDIALOG_H
#define CHARTDIALOG_H


#include <QDialog>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QDebug>
#include "qcustomplot.h"

class ChartDialog : public QDialog {
public:
    explicit ChartDialog(QString sat_name, QWidget *parent = nullptr) : QDialog(parent) {

        QFile file("ResultsCoords/"+sat_name+".txt");

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    qDebug() << "wrrwgrgwrg" << endl;
                    return;
                }

        // Создаем векторы для хранения данных
        QVector<double> t_days, s_cm, t_cm, w_cm, r_cm;

        QTextStream in(&file);

        // 1. Пропускаем первую строку (заголовок)
        if (!in.atEnd()) {
            in.readLine();
        }
        double max_r = 0.0;
        // 2. Читаем данные до конца файла
        while (!in.atEnd()) {
            double val1, val2, val3, val4, val5;

            // Оператор >> автоматически пропускает пробелы и переходы на новую строку
            in >> val1 >> val2 >> val3 >> val4 >> val5;

            // Проверяем, что чтение прошло успешно (не конец файла и не ошибка формата)
            if (in.status() == QTextStream::Ok) {
                t_days.append(val1);
                s_cm.append(val2/100.0);
                t_cm.append(val3/100.0);
                w_cm.append(val4/100.0);
                r_cm.append(val5/100.0);
                if (max_r < val5/100.0){
                    max_r = val5 / 100.0;
                }
            }
        }


        file.close();

        setModal(true);
        resize(1000, 400);

        auto *layout = new QVBoxLayout(this);
        QCustomPlot *customPlot = new QCustomPlot(this);
        layout->addWidget(customPlot);

        // --- График 3 (W_CM) ---
        customPlot->addGraph();
        customPlot->graph(0)->setName("Δ W");
        customPlot->graph(0)->setPen(QPen(Qt::blue));
        customPlot->graph(0)->setData(t_days, w_cm);

        // --- График 2 (T_CM) ---
        customPlot->addGraph();
        customPlot->graph(1)->setName("Δ T");
        customPlot->graph(1)->setPen(QPen(Qt::darkGreen));
        customPlot->graph(1)->setData(t_days, t_cm);

        // Создаем график и рисуем данные
        customPlot->addGraph();
        customPlot->graph(2)->setName("Δ S");
        customPlot->graph(2)->setPen(QPen(Qt::red));
        customPlot->graph(2)->setData(t_days, s_cm);

        // 1. Вставляем новую пустую строку в макет сверху
        customPlot->plotLayout()->insertRow(0);

        // 2. Создаем элемент "Текстовый заголовок"
        QCPTextElement *title = new QCPTextElement(customPlot, sat_name, QFont("sans", 12, QFont::Bold));

        // 3. Добавляем его в созданную верхнюю ячейку
        customPlot->plotLayout()->addElement(0, 0, title);

        // Настройка осей
        customPlot->xAxis->setLabel("Время, дни");
        customPlot->yAxis->setLabel("Расхождения, м");
        customPlot->legend->setVisible(true);
        customPlot->legend->setFont(QFont("Helvetica", 9));
        customPlot->rescaleAxes();
        customPlot->xAxis->setRange(0, t_days[t_days.size()-1] + 2);
        customPlot->yAxis->setRange(-max_r*1.5, max_r*1.5);
        customPlot->replot();
    }
};

#endif // CHARTDIALOG_H
