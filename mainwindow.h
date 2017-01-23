#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <iostream>
#include "QFileDialog"
#include <openbr/openbr_plugin.h>
#include <QListWidget>
#include <QStandardItemModel>
#include "vector"
#include "QButtonGroup"
#include "QString"
#include "recognise_result.h"
using namespace cv;
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    //QString getPhotoDir();
    //QString SimilarityEstimation(QString dir_image);
    //QString AgeEstimation(QString dir_image);
    //QString GenderEstimation(QString dir_image);

    ~MainWindow();

private slots:
    void page_search_database();
    void page_probe();
    void page_chart();
    void page_video();
    void page_camera();
    void pause_vedio();
    void stop_vedio();
    void on_btn_get_target_clicked();
    void on_btn_get_gallery_clicked();
    //void on_btn_cut_gallery_clicked();
    void on_btn_delete_gallery_clicked();

    void on_btn_show_curve_clicked();

    void on_btn_get_probe_clicked();

private:
    Ui::MainWindow *ui;
    QAction *action_page_search_database;
    QAction *action_page_probe;
    QAction *action_page_chart;
    QAction *action_page_video; //用于调出新页面，这个页面把image,video,camera三种模式整合了
    QAction *action_page_camera; //用于调用camera界面
    int video_state; // 0:播放 1:暂停 -1：退出
    //vector<QString> dirs_gallery;
    //QStandardItemModel *model_faces;
    QButtonGroup *grp_video_type;
    QButtonGroup *grp_curve_type;
    QButtonGroup *grp_source_type; //video,image,camera;
    int count_probe; //probe图片的计数器
    QString path_probe;
    cv::VideoCapture cap;
    int num_faces_after_cut;
    CascadeClassifier cascade;
    int page; //0:数据库 1：探针
    Recognise_result ground_truth[2000];
    int gt_count;
    Recognise_result result[2000];
    int result_count;
    QString imageName; //正在播放的图片的名称 e.g. 13165.jpg， 用于计算map
    QString id[5]; //选择的target的编号，都是gallery中的人脸 e.g. 30.jpg
    int id_count; //数组 id[] 的指针

    //QVector<float> br_recognise(QString addr_target);
    QVector<QString> recognise_from_db(QString addr_target);
    void recognise_from_db();
    void recognise_from_probe();
    void recognise_from_video();
    QImage cvMat2QImage(const cv::Mat& mat);
    cv::Mat QImage2cvMat(QImage image);
    float Get_Similarity(QString img1, QString img2);
    Mat get_img_to_detect();
    QVector<QImage> cut_out_and_resize(Mat& img);
    Mat& MyHistEqual(Mat& I);
    void Draw(Mat& img);
    void Draw(Mat& img, QVector<Rect>* faces);
    void detectAndDraw_db(Mat& img);
    void Draw_camera(Mat& img, vector<Rect> faces);
    vector<Rect> detectAndDraw_camera(Mat& img);
    QVector<Rect>* detectAndDraw( Mat& img, double scale, bool tryflip);
    void get_roc_data(QVector<double> *x, QVector<double> *y);
    void get_cms_data(QVector<double> *x, QVector<double>* y);
    void parseXML(QString file_name);
    float* getAP(int id);
    float overlap_rate(int x1, int y1, int width1, int height1,
              int x2, int y2, int width2, int height2);
    void writeXML();
};

#endif // MAINWINDOW_H
