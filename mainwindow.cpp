#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>
#include <QPainter>
#include <QGraphicsView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // create noise class
    noise.reset(new Noise(128, 128));

    // set scene w/ preview
    scene = new QGraphicsScene(this);
    ui->noisePreview->setScene(scene);

    // check default radio box
    ui->radioNoise->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    ui->noisePreview->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

    QMainWindow::resizeEvent(event);
}

void MainWindow::on_generate_clicked()
{
    QPixmap p(128 * 8, 128 * 8);
    QPainter *paint = new QPainter(&p);

    for (int y = 0; y < 128 * 8; y++) {
      for (int x = 0; x < 128 * 8; x++) {
        unsigned grey = noise->getSmoothNoise(double(y) / 8.0, double(x) / 8.0);
        paint->setPen(QColor(grey, grey, grey));
        paint->drawPoint(x, y);
      }
    }

    delete paint;
    scene->addPixmap(p);
    ui->noisePreview->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}
