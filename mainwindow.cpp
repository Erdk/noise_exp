#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>
#include <QPainter>
#include <QRadioButton>
#include <QGraphicsView>
#include <QFileDialog>

#if 0
#define DEBUG
#endif

#ifdef DEBUG
#include <iostream>
#endif

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  // create noise class
  noise.reset(new Noise());

  // set scene w/ preview
  scene = new QGraphicsScene(this);
  ui->noisePreview->setScene(scene);

  // check default radio box
  ui->radioNoise->setChecked(true);

  noiseOptions = ui->noiseOptions;
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event) {
  ui->noisePreview->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

  QMainWindow::resizeEvent(event);
}

void MainWindow::on_buttonGenerate_clicked()
{
  const int height = 128 * 8;
  const int width = 128 * 8;

  // create pixmap which we populate and present on QGraphicsScene & *View
  p = QPixmap(width, height);
  QPainter *paint = new QPainter(&p);

  noise->generateNoise();

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      double scaledX = double(x) / double(width);
      double scaledY = double(y) / double(height);

      switch(op) {
        case NOISE:
          paint->setPen(noise->getNoise(scaledY, scaledX));
          break;

        case SMOOTHNOISE:
          paint->setPen(noise->getSmoothNoise(scaledY, scaledX));
          break;

        case TURBULENCE:
          paint->setPen(noise->getTurbulence(scaledY, scaledX, 256.0));
          break;

        case CLOUD:
          paint->setPen(noise->getCloud(scaledY, scaledX));
          break;

        case MARBLE:
          paint->setPen(noise->getMarble(scaledY, scaledX, 5.0, 10.0, 5.0, 32.0));
          break;
      }

      paint->drawPoint(x, y);
    }
  }

  // accoridng to stackoverflow, need to check
  delete paint;
  scene->addPixmap(p);
  ui->noisePreview->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::on_buttonSave_clicked() {
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "noise.png", tr("Images (*.png *.xpm *.jpg)"));
  p.save(fileName);
}

void MainWindow::clearNoiseOptions() {
  this->noiseOptions->setLayout(nullptr);
}

void MainWindow::on_radioNoise_toggled(bool checked) {
  if (checked) {
    op = NOISE;
  }
}

void MainWindow::on_radioSmooth_toggled(bool checked) {
  if (checked) {
    op = SMOOTHNOISE;
  }
}

void MainWindow::on_radioTurbulence_toggled(bool checked) {
  if (checked) {
    op = TURBULENCE;
  }
}

void MainWindow::on_radioCloud_toggled(bool checked) {
  if (checked) {
    op = CLOUD;
  }
}

void MainWindow::on_radioMarble_toggled(bool checked) {
  if (checked) {
    op = MARBLE;
  }
}
