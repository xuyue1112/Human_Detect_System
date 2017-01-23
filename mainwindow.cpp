#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAction>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QTableWidget>
#include <QLabel>
#include <QListView>
#include <QElapsedTimer>
#include <QDomDocument>
#include <stdlib.h>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //设置标题、菜单栏、工具栏
    setWindowTitle("Human Detection System");
    action_page_search_database = new QAction(QIcon(":/icons/book.png"), "&Database", this);
    action_page_search_database->setStatusTip("recognise from database");
    action_page_probe = new QAction(QIcon(":/icons/box.png"), "&probe", this);
    action_page_probe->setStatusTip("recognise from probe");
    action_page_chart = new QAction(QIcon(":/icons/aperture.png"), "&chart", this);
    action_page_chart->setStatusTip("make ROS");
    action_page_video = new QAction(QIcon(":/icons/layers.png"), "&main", this);
    action_page_video->setStatusTip("main page");
    action_page_camera = new QAction(QIcon(":/icons/key_fill.png"),"&camera", this);
    action_page_camera->setStatusTip("camera mode");
    QMenu *menu_recognise = menuBar()->addMenu("&Recognise");
    menu_recognise->addAction(action_page_search_database);
    menu_recognise->addAction(action_page_probe);
    menu_recognise->addAction(action_page_chart);
    menu_recognise->addAction(action_page_video);
    menu_recognise->addAction(action_page_camera);
    QToolBar *toolBar_recognise = addToolBar("&Recognise");
    toolBar_recognise->addAction(action_page_search_database);
    toolBar_recognise->addAction(action_page_probe);
    toolBar_recognise->addAction(action_page_chart);
    toolBar_recognise->addAction(action_page_video);
    toolBar_recognise->addAction(action_page_camera);
    connect(action_page_search_database, SIGNAL(triggered()), this, SLOT(page_search_database()));
    connect(action_page_probe,SIGNAL(triggered()), this, SLOT(page_probe()));
    connect(action_page_chart,SIGNAL(triggered()), this, SLOT(page_chart()));
    connect(action_page_video,SIGNAL(triggered()), this, SLOT(page_video()));
    connect(action_page_camera, SIGNAL(triggered()), this, SLOT(page_camera()));
    //将“查询数据库”界面的控件设置为不可见
    ui->img_target->setVisible(false);
    ui->btn_get_target->setVisible(false);
    ui->dir_target->setVisible(false);
    ui->table_similarity->setVisible(false);

    //ui->img_vedio_probe->setVisible(false);
    //ui->btn_begin_pause->setVisible(false);
    //ui->btn_stop->setVisible(false);
    ui->list_faces->setVisible(false);
    ui->btn_get_gallery->setVisible(false);
    ui->btn_delete_gallery->setVisible(false);
    ui->type_probe->setVisible(false);
    ui->type_camera->setVisible(false);
    ui->type_camera->setChecked(false);
    ui->btn_hist_equal->setVisible(false);

    //ui->chart_ros->setVisible(false);
    ui->btn_roc->setVisible(false);
    ui->btn_cms->setVisible(false);
    ui->btn_show_curve->setVisible(false);
    //设置页面大小
    //setGeometry(300,150,800,530);   //这是database, image&video, chart模式分开时的页面大小

    //初始化数据
    path_probe = "/home/xuyue/文档/4、人脸识别_训练数据/1_1_04_0/prob/dongnanmeneast_15_1920x1080_30/";
    count_probe = 16254;
    num_faces_after_cut = 0;
    gt_count = result_count = id_count = 0;
    cascade.load("haarcascade_frontalface_alt2.xml");


    //这是为了计算map
    //parseXML("/home/xuyue/文档/4、人脸识别_训练数据/1_1_04_0/face recognize label result/dongnanmeneast_15_1920x1080_30.xml");



    //这是把image video camera 三个页面合一之后的页面大小

    setGeometry(300,150,690, 600);
    grp_source_type = new QButtonGroup(this);
    grp_source_type->addButton(ui->radio_type_image);
    grp_source_type->addButton(ui->radio_type_video);
    grp_source_type->addButton(ui->radio_type_camera);
    connect(ui->btn_begin_pause, SIGNAL(clicked()), this, SLOT(pause_vedio()));
    connect(ui->btn_stop, SIGNAL(clicked()), this, SLOT(stop_vedio()));
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::page_search_database(){

    /*ui->img_vedio_probe->setVisible(false);
    ui->btn_begin_pause->setVisible(false);
    ui->btn_stop->setVisible(false);
    ui->list_faces->setVisible(false);
    ui->btn_get_gallery->setVisible(false);
    ui->type_probe->setVisible(false);
    ui->type_camera->setVisible(false);
    ui->type_probe->setVisible(false);
    ui->type_camera->setVisible(false);
    ui->btn_hist_equal->setVisible(false);*/

    /*ui->img_target->setVisible(true);
    ui->img_target->setGeometry(60,60,200,220);
    ui->btn_get_target->setVisible(true);
    ui->btn_get_target->setGeometry(90,320,100,27);
    ui->dir_target->setVisible(true);
    ui->dir_target->setGeometry(90,360,113,27);
    ui->table_similarity->setVisible(true);
    ui->table_similarity->setGeometry(320,70,210,330);*/

    //把图表页的内容设为不可见
    ui->chart_ros->setVisible(false);
    ui->btn_roc->setVisible(false);
    ui->btn_cms->setVisible(false);
    ui->btn_show_curve->setVisible(false);

    //把probe页的内容设置为不可见
    grp_video_type = new QButtonGroup(this);
    //grp_video_type->addButton(ui->type_probe);
    //grp_video_type->addButton(ui->type_camera);
    ui->type_probe->setVisible(false);
    //ui->type_probe->setChecked(true);
    ui->type_camera->setVisible(false);

    //设置页面状态
    page = 0;
    //设置页面大小
    setGeometry(300,150,800,530);
    ui->img_vedio_probe->setVisible(true);
    ui->img_vedio_probe->setGeometry(40,100,360,240);

    ui->btn_begin_pause->setVisible(true);
    ui->btn_begin_pause->setGeometry(90,440,99,27);
    ui->btn_stop->setVisible(true);
    ui->btn_stop->setGeometry(240,440,99,27);
    ui->list_faces->setVisible(true);
    ui->list_faces->setGeometry(450,80,260,280);
    ui->btn_get_gallery->setVisible(true);
    ui->btn_get_gallery->setGeometry(530,390,100,27);
    ui->btn_delete_gallery->setVisible(true);
    ui->btn_delete_gallery->setGeometry(530,430,100,27);
    ui->btn_hist_equal->setVisible(true);
    ui->btn_hist_equal->setGeometry(250,390,121,22);
    connect(ui->btn_begin_pause, SIGNAL(clicked()), this, SLOT(pause_vedio()));
    connect(ui->btn_stop, SIGNAL(clicked()), this, SLOT(stop_vedio()));
    video_state = -1;

}
void MainWindow::page_probe(){
    //把图表页的内容设为不可见
    ui->chart_ros->setVisible(false);
    ui->btn_roc->setVisible(false);
    ui->btn_cms->setVisible(false);
    ui->btn_show_curve->setVisible(false);

    page = 1;
    setGeometry(300,150,800,530);
    ui->img_target->setVisible(false);
    ui->btn_get_target->setVisible(false);
    ui->dir_target->setVisible(false);
    ui->table_similarity->setVisible(false);
    grp_video_type = new QButtonGroup(this);
    grp_video_type->addButton(ui->type_probe);
    grp_video_type->addButton(ui->type_camera);
    ui->type_probe->setVisible(true);
    ui->type_probe->setChecked(true);
    ui->type_probe->setGeometry(40,390,117,22);
    ui->type_camera->setVisible(true);
    ui->type_camera->setGeometry(120,390,117,22);
    ui->img_vedio_probe->setVisible(true);
    ui->img_vedio_probe->setGeometry(40,100,360,240);
    ui->btn_begin_pause->setVisible(true);
    ui->btn_begin_pause->setGeometry(90,440,99,27);
    ui->btn_stop->setVisible(true);
    ui->btn_stop->setGeometry(240,440,99,27);
    ui->list_faces->setVisible(true);
    ui->list_faces->setGeometry(450,80,260,280);
    ui->btn_get_gallery->setVisible(true);
    ui->btn_get_gallery->setGeometry(530,390,100,27);
    ui->btn_delete_gallery->setVisible(true);
    ui->btn_delete_gallery->setGeometry(530,430,100,27);
    ui->btn_hist_equal->setVisible(true);
    ui->btn_hist_equal->setGeometry(250,390,121,22);
    connect(ui->btn_begin_pause, SIGNAL(clicked()), this, SLOT(pause_vedio()));
    connect(ui->btn_stop, SIGNAL(clicked()), this, SLOT(stop_vedio()));
    video_state = -1;

}
void MainWindow::page_chart(){
    setGeometry(300,150,800,530);

    ui->img_vedio_probe->setVisible(false);
    ui->btn_begin_pause->setVisible(false);
    ui->btn_stop->setVisible(false);
    ui->list_faces->setVisible(false);
    ui->btn_get_gallery->setVisible(false);
    ui->btn_delete_gallery->setVisible(false);
    ui->type_probe->setVisible(false);
    ui->type_camera->setVisible(false);
    ui->btn_hist_equal->setVisible(false);

    ui->chart_ros->setVisible(true);
    grp_curve_type = new QButtonGroup();
    grp_curve_type->addButton(ui->btn_roc);
    grp_curve_type->addButton(ui->btn_cms);
    ui->btn_roc->setVisible(true);
    ui->btn_cms->setVisible(true);
    ui->btn_roc->setChecked(true);
    ui->btn_show_curve->setVisible(true);
}
void MainWindow::page_video(){
    //绘制页面


    ui->radio_type_image->setChecked(true);
    grp_source_type->setExclusive(false);
    ui->radio_type_image->setGeometry(30,20,117,22);
    ui->radio_type_video->setGeometry(30,50,117,22);
    ui->radio_type_camera->setGeometry(170,50,117,22);
    ui->Check_analy->setGeometry(170,20,97,22);

    ui->img_vedio_probe->setGeometry(40,140,360,240);
    ui->btn_begin_pause->setGeometry(110,470,61,27);
    ui->btn_stop->setGeometry(190,470,61,27);

    ui->label_face1->setGeometry(420,70,80,80);
    ui->label_name_face1->setGeometry(520,70,31,17);
    ui->label_age_face1->setGeometry(513,100,31,17);
    ui->label_score_face1->setGeometry(510,130,51,20);
    ui->text_name_face1->setGeometry(562,70,91,27);
    ui->text_age_face1->setGeometry(562,100,91,27);
    ui->text_score_face1->setGeometry(562,130,91,27);

    ui->label_face2->setGeometry(420,190,80,80);
    ui->label_name_face2->setGeometry(520,190,31,17);
    ui->label_age_face2->setGeometry(513,220,31,17);
    ui->label_score_face2->setGeometry(510,250,51,20);
    ui->text_name_face2->setGeometry(562,190,91,27);
    ui->text_age_face2->setGeometry(562,220,91,27);
    ui->text_score_face2->setGeometry(562,250,91,27);

    ui->label_face3->setGeometry(420,310,80,80);
    ui->label_name_face3->setGeometry(520,310,31,17);
    ui->label_age_face3->setGeometry(513,340,31,17);
    ui->label_score_face3->setGeometry(510,370,51,20);
    ui->text_name_face3->setGeometry(562,310,91,27);
    ui->text_age_face3->setGeometry(562,340,91,27);
    ui->text_score_face3->setGeometry(562,370,91,27);
    ui->btn_get_probe->setGeometry(490,430,99,27);

    //设置页面状态,为了方便，暂时让它和probe一致
    page = 1;
    ui->btn_begin_pause->setVisible(true);
    ui->btn_stop->setVisible(true);
    ui->img_vedio_probe->setVisible(true);

    video_state = -1;
}
void MainWindow::page_camera(){
    //绘制页面
    setGeometry(300,50,690,670);
    ui->radio_type_camera->setChecked(true);
    grp_source_type->setExclusive(false);
    ui->radio_type_image->setGeometry(30,20,117,22);
    ui->radio_type_video->setGeometry(30,50,117,22);
    ui->radio_type_camera->setGeometry(170,50,117,22);
    ui->Check_analy->setGeometry(170,20,97,22);

    ui->img_vedio_probe->setGeometry(110,150,360,240);
    ui->btn_begin_pause->setGeometry(110,570,61,27);
    ui->btn_stop->setGeometry(190,570,61,27);

    ui->label_camera_name->setGeometry(470,180,67,17);
    ui->label_camera_id->setGeometry(470,230,67,17);
    ui->label_camera_age->setGeometry(470,280,67,17);
    ui->label_camera_gender->setGeometry(470,330,67,17);
    ui->text_camera_name->setGeometry(540,175,113,27);
    ui->text_camera_id->setGeometry(540,225,113,27);
    ui->text_camera_age->setGeometry(540,275,113,27);
    ui->text_camera_gender->setGeometry(540,325,113,27);

    ui->label_camera_face1->setGeometry(30,410,80,80);
    ui->text_camera_score1->setGeometry(40,510,80,27);
    ui->label_camera_face2->setGeometry(160,410,80,80);
    ui->text_camera_score2->setGeometry(170,510,80,27);
    ui->label_camera_face3->setGeometry(290,410,80,80);
    ui->text_camera_score3->setGeometry(300,510,80,27);
    ui->label_camera_face4->setGeometry(420,410,80,80);
    ui->text_camera_score4->setGeometry(430,510,80,27);

    //一些状态值
    page = 2;
    video_state = -1;

}

/*QVector<QString> MainWindow::recognise_from_db(QString addr_target){
    //Mat mat_target = QImage2cvMat(target);

    QVector<float> recognise_result = br_recognise(addr_target);
    QVector<QString> similarity;
    for(int i=0; i<10; i++)
        similarity.push_back(QString("%1").arg(recognise_result.at(i)));
    return similarity;
}*/
QVector<QString> MainWindow::recognise_from_db(QString addr_target)
{
    // Retrieve classes for enrolling and comparing templates using the FaceRecognition algorithm
    QSharedPointer<br::Transform> transform = br::Transform::fromAlgorithm("FaceRecognition");
    QSharedPointer<br::Distance> distance = br::Distance::fromAlgorithm("FaceRecognition");

    // Initialize templates
    br::Template query1("/home/xuyue/programs/Human_Detection_System/images/ORL_faces/s1/1.bmp");
    br::Template query2("/home/xuyue/programs/Human_Detection_System/images/ORL_faces/s2/1.bmp");
    br::Template query3("/home/xuyue/programs/Human_Detection_System/images/ORL_faces/s3/1.bmp");
    br::Template query4("/home/xuyue/programs/Human_Detection_System/images/ORL_faces/s4/1.bmp");
    br::Template query5("/home/xuyue/programs/Human_Detection_System/images/ORL_faces/s5/1.bmp");
    br::Template query6("/home/xuyue/programs/Human_Detection_System/images/ORL_faces/s6/1.bmp");
    br::Template query7("/home/xuyue/programs/Human_Detection_System/images/ORL_faces/s7/1.bmp");
    br::Template query8("/home/xuyue/programs/Human_Detection_System/images/ORL_faces/s8/1.bmp");
    br::Template query9("/home/xuyue/programs/Human_Detection_System/images/ORL_faces/s9/1.bmp");
    br::Template query10("/home/xuyue/programs/Human_Detection_System/images/ORL_faces/s10/1.bmp");

    br::Template target(addr_target);

    // Enroll templates
    query1 >> *transform;
    query2 >> *transform;
    query3 >> *transform;
    query4 >> *transform;
    query5 >> *transform;
    query6 >> *transform;
    query7 >> *transform;
    query8 >> *transform;
    query9 >> *transform;
    query10 >> *transform;
    target >> *transform;

    //printTemplate(queryA);
    //printTemplate(queryB);
    //printTemplate(target);

    // Compare templates
    float comparison1 = distance->compare(target, query1);
    float comparison2 = distance->compare(target, query2);
    float comparison3 = distance->compare(target, query3);
    float comparison4 = distance->compare(target, query4);
    float comparison5 = distance->compare(target, query5);
    float comparison6 = distance->compare(target, query6);
    float comparison7 = distance->compare(target, query7);
    float comparison8 = distance->compare(target, query8);
    float comparison9 = distance->compare(target, query9);
    float comparison10 = distance->compare(target, query10);

    QVector<QString> recognise_result;
    recognise_result.push_back(QString("%1").arg(comparison1));
    recognise_result.push_back(QString("%1").arg(comparison2));
    recognise_result.push_back(QString("%1").arg(comparison3));
    recognise_result.push_back(QString("%1").arg(comparison4));
    recognise_result.push_back(QString("%1").arg(comparison5));
    recognise_result.push_back(QString("%1").arg(comparison6));
    recognise_result.push_back(QString("%1").arg(comparison7));
    recognise_result.push_back(QString("%1").arg(comparison8));
    recognise_result.push_back(QString("%1").arg(comparison9));
    recognise_result.push_back(QString("%1").arg(comparison10));

    // Scores range from 0 to 1 and represent match probability

    //br::Context::finalize();

    return recognise_result;
}
void MainWindow::recognise_from_db(){
    Mat frame;
    int i = 0;
    while(true)
    {
        if(video_state == 0){
            //frame = get_img_to_detect();
            //frame = imread(QString("%1%2.jpg").arg(path).arg(count).toStdString(),1);
            if( !cap.isOpened() )
                qDebug() << "camera is not opened" << endl;
            cap >> frame;
            if(i==0){
                detectAndDraw_db(frame);
            }
            else{
                Draw(frame);
            }
                //ui->list_faces->setModel(model_faces);
            QElapsedTimer t;
            t.start();
            while(t.elapsed() < 40)
                QCoreApplication::processEvents();

            i= (i+1)%8;//周期为4
        }
        else if(video_state == 1){
            QElapsedTimer t;
            t.start();
            while(t.elapsed() < 1000)
                QCoreApplication::processEvents();
        }
        else if(video_state == -1)
            break;
    }

    return;
}
void MainWindow::recognise_from_probe(){
    /*VideoCapture cap(0);    //打开默认摄像头
    if(!cap.isOpened())
    {
        return;
    }*/
    Mat frame;
    int i = 0; //每隔四帧框一次人脸。
    while(count_probe < 16330)
    {
        if(video_state == 0){
            //model_faces = new QStandardItemModel(this);
            frame = get_img_to_detect();
            //frame = imread(QString("%1%2.jpg").arg(path).arg(count).toStdString(),1);
            QVector<Rect> *faces;
            if(i==0){
                //if(ui->type_probe->isChecked())
                    faces = detectAndDraw(frame, 3, 0);
                //else if(ui->type_camera->isChecked())
                    //faces = detectAndDraw(frame, 1, 0);
            }
            else{
                Draw(frame,faces);
            }
                //ui->list_faces->setModel(model_faces);
            QElapsedTimer t;
            t.start();
            while(t.elapsed() < 40)
                QCoreApplication::processEvents();

            //i= (i+1)%4;//周期为4  // 如果没有这行代码，则每一帧都会检测
        }
        else if(video_state == 1){
            QElapsedTimer t;
            t.start();
            while(t.elapsed() < 1000)
                QCoreApplication::processEvents();
        }
        else if(video_state == -1)
            break;
    }

    /*qDebug() << "result";
    for(int i=0; i<result_count; i++)
        result[i].print();
    qDebug() << "gt";
    for(int i=0; i<gt_count; i++)
        ground_truth[i].print();
    return;*/
    //这些是用来显示MAP的计算结果的
    //float* result1 = getAP(30);
    //float* result2 = getAP(32);
    //QMessageBox::information(this,"MAP",QString("MAP@1 : %1    \nMAP@5 : %2    \nMAP@10 : %3    \nMAP@20 : %4   ").arg( (result1[0]+result2[0])/2 ).arg( (result1[1]+result2[1])/2 ).arg( (result1[2]+result2[2])/2 ).arg( (result1[3]+result2[3])/2 ), QMessageBox::Ok);
    qDebug() << "before";
    writeXML();

    /*
    frame = imread("persons.jpg");
    detectAndDraw( frame, cascade, nestedCascade,2,0 );
    */
    // 保存ROI
    /*CvSize size = cvSize(300,300);
    IplImage pSrc = frame;
    IplImage *pointer_pSrc = cvCloneImage(&pSrc);
    cvSetImageROI(pointer_pSrc,cvRect(0,0,size.width,size.height));
    IplImage* pDest = cvCreateImage(size,pointer_pSrc->depth,pointer_pSrc->nChannels);
    cvCopy(pointer_pSrc,pDest);
    cvResetImageROI(pDest);
    //cvSaveImage("Roi.jpg",pDest);
    Mat M(&pSrc,true);
    cv::imshow("result",M);
    waitKey();*/

}
void MainWindow::recognise_from_video(){
    VideoCapture capture("/home/xuyue/文档/同学的正面照和视频/camera.mp4");
    if(!capture.isOpened())
        return;
    double rate=capture.get(CV_CAP_PROP_FPS);
    bool stop(false);
    Mat frame;
    //namedWindow("Canny Video");
    int delay=1000/rate;
    int i=0;
    vector<Rect> rect;
    while(!stop)
    {
        if(!capture.read(frame))
            break;
        //capture >> frame;
        if(i == 0 )
           rect = detectAndDraw_camera(frame);
        else
            Draw_camera(frame,rect);
        //imshow("Canny Video",frame);
        if(waitKey(delay)>=0)
            stop=true;
        /*QElapsedTimer t;
        t.start();
        while(t.elapsed() < 40)
            QCoreApplication::processEvents();*/
        i = (i+1) % 10;
    }
    capture.release();
}
void MainWindow::Draw(Mat &img){
    QImage img_small = cvMat2QImage(img);
    img_small = img_small.scaled(360,240,Qt::KeepAspectRatio);
    ui->img_vedio_probe->setPixmap(QPixmap::fromImage(img_small));
}
void MainWindow::Draw(Mat &img, QVector<Rect>* faces){
    int i = 0;
    for(QVector<Rect>::const_iterator r = faces->begin(); r != faces->end(); r++,i++){
        rectangle( img, *r, CV_RGB(0,0,255), 3, 8, 0);
    }
    //qDebug() << i << endl;
    QImage img_small = cvMat2QImage(img);
    img_small = img_small.scaled(360,240,Qt::KeepAspectRatio);
    ui->img_vedio_probe->setPixmap(QPixmap::fromImage(img_small));
}
void MainWindow::detectAndDraw_db(Mat &img){
    int i = 0;

    vector<Rect> faces;
    Mat gray;
    cvtColor( img, gray, CV_BGR2GRAY );
    equalizeHist( gray, gray );
    cascade.detectMultiScale( gray, faces,1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30));
    vector<Rect> rect_faces;
    for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++ )
    {
        IplImage temp = img;
        IplImage* point_Src,*point_Dest;
        point_Src = cvCloneImage(&temp);
        Rect rect;
        rect.x = cvRound(r->x); //-30;
        rect.y = cvRound(r->y); //-30;
        rect.width = cvRound(r->x + r->width-1)-rect.x; //+60;
        rect.height = cvRound(r->y + r->height-1)-rect.y; //+60;
        rect_faces.push_back(rect);
        cvSetImageROI(point_Src,rect);
        CvSize size_dest;
        size_dest.width = size_dest.height = 128;
        point_Dest = cvCreateImage(size_dest,point_Src->depth, point_Src->nChannels);
        cvResize(point_Src,point_Dest);
        cvSaveImage(QString("target%1.jpg").arg(i).toLatin1().data(),point_Dest);
        cvResetImageROI(point_Src);
    }

    //计算gallery中每一张人脸和视频中人脸的相似度，然后将gallery中的人脸按照相似度排序
    int num_faces = i;
    float similarity_of_galleries[num_faces_after_cut];

    for(i=0; i<num_faces_after_cut;i++){
        float similarity = -1.0;
        for(int j=0; j<num_faces; j++){
            float temp_rst = Get_Similarity(QString("target%1.jpg").arg(j), QString("gallery_face%1.jpg").arg(i));
            if(temp_rst > similarity){
                similarity = temp_rst;
            }
        }
        similarity_of_galleries[i] = similarity;
    }
    for(i=0; i<num_faces_after_cut; i++){
        ui->list_faces->item(i)->setText(QString("%1").arg(QString::number(similarity_of_galleries[i],'f',2)));
    }
    for(i=0; i<num_faces_after_cut; i++){
        qDebug() << QString("before %1  ").arg(ui->list_faces->item(i)->text());
    }
    for(i=0; i<num_faces_after_cut-1; i++){
        for(int j=i+1; j<num_faces_after_cut; j++){
            if(similarity_of_galleries[i] < similarity_of_galleries[j]){
                QListWidgetItem* temp1 =  ui->list_faces->takeItem(i);
                QListWidgetItem* temp2 =  ui->list_faces->takeItem(j-1);
                ui->list_faces->insertItem(i,temp2);
                ui->list_faces->insertItem(j,temp1);
                float temp = similarity_of_galleries[i];
                similarity_of_galleries[i] = similarity_of_galleries[j];
                similarity_of_galleries[j] = temp;
                //qDebug() << ui->list_faces->count() << endl;
            }
        }
    }
    for(i=0; i<num_faces_after_cut; i++){
        qDebug() << QString("after %1  ").arg(ui->list_faces->item(i)->text());
    }
    qDebug() << endl;

    QImage img_small = cvMat2QImage(img);
    img_small = img_small.scaled(360, 240,Qt::KeepAspectRatio);
    ui->img_vedio_probe->setPixmap(QPixmap::fromImage(img_small));
}
QVector<Rect>* MainWindow::detectAndDraw( Mat& img, double scale, bool tryflip){
    int i = 0;
    //double t = 0;
    //建立用于存放人脸的向量容器
    vector<Rect> faces, faces2;
    //定义一些颜色，用来标示不同的人脸
    const static Scalar colors[] =  {
        CV_RGB(0,0,255),
        CV_RGB(0,255,0),
        CV_RGB(255,0,0),
        CV_RGB(0,255,0),
        CV_RGB(255,128,0),
        CV_RGB(255,255,0),
        CV_RGB(255,0,0),
        CV_RGB(255,0,255)   } ;
    //建立缩小的图片，加快检测速度
    //nt cvRound (double value) 对一个double型的数进行四舍五入，并返回一个整型数！
    Mat gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );
    //转成灰度图像，Harr特征基于灰度图
    cvtColor( img, gray, CV_BGR2GRAY );
    //改变图像大小，使用双线性差值
    cv::resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );
    //变换后的图像进行直方图均值化处理
    equalizeHist( smallImg, smallImg );

    //程序开始和结束插入此函数获取时间，经过计算求得算法执行时间
    //t = (double)cvGetTickCount();
    //检测人脸
    //detectMultiScale函数中smallImg表示的是要检测的输入图像为smallImg，faces表示检测到的人脸目标序列，1.1表示
    //每次图像尺寸减小的比例为1.1，2表示每一个目标至少要被检测到3次才算是真的目标(因为周围的像素和不同的窗口大
    //小都可以检测到人脸),CV_HAAR_SCALE_IMAGE表示不是缩放分类器来检测，而是缩放图像，Size(30, 30)为目标的
    //最小最大尺寸
    cascade.detectMultiScale( smallImg, faces,
        1.1, 2, 0
        //|CV_HAAR_FIND_BIGGEST_OBJECT
        //|CV_HAAR_DO_ROUGH_SEARCH
        |CV_HAAR_SCALE_IMAGE
        ,
        Size(30, 30));
    //如果使能，翻转图像继续检测
    /*if( tryflip )
    {
        flip(smallImg, smallImg, 1);
        cascade.detectMultiScale( smallImg, faces2,
                                 1.1, 2, 0
                                 //|CV_HAAR_FIND_BIGGEST_OBJECT
                                 //|CV_HAAR_DO_ROUGH_SEARCH
                                 |CV_HAAR_SCALE_IMAGE
                                 ,
                                 Size(30, 30) );
        for( vector<Rect>::const_iterator r = faces2.begin(); r != faces2.end(); r++ )
        {
            faces.push_back(Rect(smallImg.cols - r->x - r->width, r->y, r->width, r->height));
        }
    }*/
    //t = (double)cvGetTickCount() - t;
    //qDebug( "detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000.) );
    vector<Rect> rect_faces;
    //将检测的人脸保存到本地，并记录每张脸在图片中的边界
    for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++ )
    {
        //Mat smallImgROI;
        //smallImgROI = smallImg(*r);

        //Point center;
        //Scalar color = colors[i%8];
        //int radius;
        CvFont font;
        cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX,  1,  1,  0,  2);

            //标示人脸时在缩小之前的图像上标示，所以这里根据缩放比例换算回去
            /*
            center.x = cvRound((r->x + r->width*0.5)*scale);
            center.y = cvRound((r->y + r->height*0.5)*scale);
            radius = cvRound((r->width + r->height)*0.25*scale);
            circle( img, center, radius, color, 3, 8, 0 );*/

        //下面的代码是为了保存彩色ROI区域，上面的smallImgROI是灰度图。
        IplImage temp = img;
        IplImage* point_Src,*point_Dest;
        point_Src = cvCloneImage(&temp);
        Rect rect;
        rect.x = cvRound(r->x*scale); //-30;
        rect.y = cvRound(r->y*scale); //-30;
        rect.width = cvRound(r->x + r->width-1)*scale-rect.x; //+60;
        rect.height = cvRound(r->y + r->height-1)*scale-rect.y; //+60;
        rect_faces.push_back(rect);
        //point_Dest = cvCreateImage(cvSize(rect.width,rect.height),point_Src->depth,point_Src->nChannels);
        cvSetImageROI(point_Src,rect);
        //QImage img_ROI = cvMat2QImage(Mat(point_Src,true));
        //QStandardItem *list_item_face = new QStandardItem(QIcon(QPixmap::fromImage(img_ROI)),QString("face%1").arg(i+1));
        //model_faces->appendRow(list_item_face);
        CvSize size_dest;
        size_dest.width = size_dest.height = 128;
        point_Dest = cvCreateImage(size_dest,point_Src->depth, point_Src->nChannels);
        //可以这么理解：resize用于Mat对象的缩放，而cvResize用于Iplimage对象的缩放。
        cvResize(point_Src,point_Dest);
        cvSaveImage(QString("target%1.jpg").arg(i).toLatin1().data(),point_Dest);
        cvResetImageROI(point_Src);
        //double aspect_ratio = (double)r->width/r->height;
        /*
        if( 0.75 < aspect_ratio && aspect_ratio < 1.3 )
        {
            rectangle( img, cvPoint(cvRound(r->x*scale), cvRound(r->y*scale)),
                       cvPoint(cvRound((r->x + r->width-1)*scale), cvRound((r->y + r->height-1)*scale)),
                       color, 3, 8, 0);


        }*/

        /*
         * 检测
         */
        /*
        //QString similarity = SimilarityEstimation(QString("target%1.jpg").arg(i));
        QString gender = GenderEstimation(QString("target%1.jpg").arg(i));
        QString age = AgeEstimation(QString("target%1.jpg").arg(i));
        //cvPutText(&temp,similarity.toLatin1().data(),cvPoint(cvRound(r->x+r->width-1)*scale+10,cvRound(r->y*scale+10)),&font,CV_RGB(0,0,0));
        cvPutText(&temp,gender.toLatin1().data(),cvPoint(cvRound(r->x+r->width-1)*scale+10,cvRound(r->y*scale+10)),&font,CV_RGB(0,0,0));
        cvPutText(&temp,age.toLatin1().data(),cvPoint(cvRound(r->x+r->width-1)*scale+10,cvRound(r->y*scale+40)),&font,CV_RGB(0,0,0));
        */
        //cvSaveImage("result.jpg",&temp);
        //同样方法检测人眼
        /*nestedCascade.detectMultiScale( smallImgROI, nestedObjects,
            1.1, 2, 0
            //|CV_HAAR_FIND_BIGGEST_OBJECT
            //|CV_HAAR_DO_ROUGH_SEARCH
            //|CV_HAAR_DO_CANNY_PRUNING
            |CV_HAAR_SCALE_IMAGE
            ,
            Size(30, 30) );
        for( vector<Rect>::const_iterator nr = nestedObjects.begin(); nr != nestedObjects.end(); nr++ )
        {
            center.x = cvRound((r->x + nr->x + nr->width*0.5)*scale);
            center.y = cvRound((r->y + nr->y + nr->height*0.5)*scale);
            radius = cvRound((nr->width + nr->height)*0.25*scale);
            circle( img, center, radius, color, 3, 8, 0 );
        }*/
    }
    QVector<Rect> *rtn_faces = new QVector<Rect>(); //返回值
    int num_faces = i;
    //将gallery与检测到的人脸进行匹配，在超过阈值的人脸中，选择最大的一个框起来。
    int indexs[3]; //每个probe对应的人脸编号
    float similarities[3]; //每个Probe对应的相似度
    for(int i=0; i<3; i++){
        indexs[i] = -1;
        similarities[i] = 0.0;
    }
    for(i=0; i<num_faces_after_cut;i++){
        float similarity = -1.0;
        int index = -1; //相似度最高的脸的索引号
        for(int j=0; j<num_faces; j++){
            float temp_rst = Get_Similarity(QString("target%1.jpg").arg(j), QString("probe_face%1.jpg").arg(i));
            if(temp_rst>0.57 && temp_rst > similarity){
                similarity = temp_rst;
                index = j;
            }
        }
        if(index != -1){
            //indexs[i] = index;
            int flag = 1;
            for(int k=0; k<i; k++)
                if(indexs[k] == index){
                    if((similarity+0.1) < similarities[k]){
                        flag = 0;
                        break;
                    }
                    else{
                        indexs[k] = -1;
                        similarities[k] = 0.0;
                    }
                }
            if(flag == 0)
                continue;
            else{
                indexs[i] = index;
                similarities[i] = similarity;
            }
        }
    }
    for(int i=0; i<3; i++){
        if(indexs[i] != -1){
            rectangle( img, rect_faces.at(indexs[i]), colors[i%8], 3, 8, 0);
            IplImage temp = img;
            CvFont font;
            cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX,  3,  3,  2,  6);
            if(i==0){
                ui->text_score_face1->setText(QString("%1").arg( (similarities[i]-0.8)/10 + 0.8) );
                cvPutText(&temp,"face 1",cvPoint(rect_faces.at(indexs[i]).x, rect_faces.at(indexs[i]).y-5),&font,CV_RGB(0,0,255));
            }
            if(i==1){
                ui->text_score_face2->setText(QString("%1").arg( (similarities[i]-0.8)/10 + 0.8) );
                cvPutText(&temp,"face 2",cvPoint(rect_faces.at(indexs[i]).x, rect_faces.at(indexs[i]).y-5),&font,CV_RGB(0,255,0));
            }
            if(i==2){
                ui->text_score_face3->setText(QString("%1").arg( (similarities[i]-0.8)/10 + 0.8) );
                cvPutText(&temp,"face 3",cvPoint(rect_faces.at(indexs[i]).x, rect_faces.at(indexs[i]).y-5),&font,CV_RGB(255,0,0));
            }
            rtn_faces->push_back(rect_faces.at(indexs[i]));
            result[result_count++] = Recognise_result(similarities[i],rect_faces.at(indexs[i]),id[i].toInt(),imageName);

            //qDebug() << "get one" << endl;
            //qDebug() << similarity <<endl;
            //qDebug() << QString("target%1.jpg").arg(index) <<endl;
            //qDebug() << QString("%1").arg(dirs_gallery.at(i)) << endl;
        }
        else{
            if(i==0){
                ui->text_score_face1->setText(QString("%1").arg("0.0"));
            }

            if(i==1){
                ui->text_score_face2->setText(QString("%1").arg("0.0"));
            }

            if(i==2){
                ui->text_score_face3->setText(QString("%1").arg("0.0"));
            }
        }

    }
    //将保存在本地的人脸删除
    /*
    for(i=0; i<num_faces; i++)
        QFile::remove(QString("target%1.jpg").arg(i));
    */


    //这是为了将图片缩小，便于显示
    //Mat small( 120, 180, CV_8UC3 );
    //cv::resize( img, small, small.size(), 0, 0, INTER_LINEAR );

    //cv::imshow( "result", img );
    QImage img_small = cvMat2QImage(img);
    img_small = img_small.scaled(360, 240,Qt::KeepAspectRatio);
    ui->img_vedio_probe->setPixmap(QPixmap::fromImage(img_small));
    return rtn_faces;
}
vector<Rect> MainWindow::detectAndDraw_camera(Mat& img){
    int i = 0;
    vector<Rect> faces;
    Mat gray;
    cvtColor( img, gray, CV_BGR2GRAY );
    equalizeHist( gray, gray );
    cascade.detectMultiScale( gray, faces,1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30));
    for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++ )
    {
        rectangle(img, faces.at(i), CV_RGB(0,0,255), 3, 8, 0);
        IplImage temp = img;
        IplImage* point_Src,*point_Dest;
        point_Src = cvCloneImage(&temp);
        Rect rect;
        rect.x = cvRound(r->x); //-30;
        rect.y = cvRound(r->y); //-30;
        rect.width = cvRound(r->x + r->width-1)-rect.x; //+60;
        rect.height = cvRound(r->y + r->height-1)-rect.y; //+60;
        cvSetImageROI(point_Src,rect);
        CvSize size_dest;
        size_dest.width = size_dest.height = 128;
        point_Dest = cvCreateImage(size_dest,point_Src->depth, point_Src->nChannels);
        cvResize(point_Src,point_Dest);
        cvSaveImage(QString("target%1.jpg").arg(i).toLatin1().data(),point_Dest);
        cvResetImageROI(point_Src);

    }
    /*
     * 将视频中的人脸和probe中的人脸一一比对
     * 若视频中有多个人脸，则选取相似度最大的probe
     */
    int num_faces = i;
    float* similarities = (float*)malloc(sizeof(float)*7);
    for(i=0; i<7 ;i++){
        similarities[i] = 0;
        float similarity = 0;
        for(int j=0; j<num_faces; j++){
            float temp_rst = Get_Similarity(QString("target%1.jpg").arg(j), QString("/home/xuyue/文档/同学的正面照和视频/裁剪后/%1.jpg").arg(i+1));
            if(temp_rst > similarity){
                similarity = temp_rst;
            }
        }
        similarities[i] = similarity;
    }
    int sort[7] = {0,1,2,3,4,5,6}; //用来排序
    for(int i=0; i<5; i++){
        for(int j=i+1; j<7; j++){
            if(similarities[i] < similarities[j]){
                float temp = similarities[i];
                similarities[i] = similarities[j];
                similarities[j] = temp;
                int temp2 = sort[i];
                sort[i] = sort[j];
                sort[j] = temp2;
            }
        }
    }
    for(int i=0; i<5; i++){
        if(similarities[i] > 0.8)
            similarities[i] = (similarities[i]-0.8)/10 + 0.8;
        else if(similarities[i] <= 0)
            similarities[i] = 0;
        else if(similarities[i] < 0.2)
            similarities[i] = (similarities[i]-0.2)/10 + 0.2;

        if(similarities[i] > 0.9)
            similarities[i] = (similarities[i]-0.9)/10 + 0.9;
        else if(similarities[i] <= 0)
            similarities[i] = 0;
        else if(similarities[i] < 0.1)
            similarities[i] = (similarities[i]-0.1)/10 + 0.1;

        if(similarities[i] > 1)
            similarities[i] = 1;
        else if(similarities[i] < 0)
            similarities[i] = 0;
    }

    switch(sort[0]){
    case 0:
        ui->text_camera_name->setText("Pengzhao Xu");
        ui->text_camera_id->setText("1");
        ui->text_camera_age->setText("22");
        ui->text_camera_gender->setText("male");
        break;
    case 1:
        ui->text_camera_name->setText("Xingxu Liu");
        ui->text_camera_id->setText("2");
        ui->text_camera_age->setText("22");
        ui->text_camera_gender->setText("male");
        break;
    case 2:
        ui->text_camera_name->setText("ZiTong Xiao");
        ui->text_camera_id->setText("3");
        ui->text_camera_age->setText("21");
        ui->text_camera_gender->setText("female");
        break;
    case 3:
        ui->text_camera_name->setText("Yue Xu");
        ui->text_camera_id->setText("4");
        ui->text_camera_age->setText("22");
        ui->text_camera_gender->setText("male");
        break;
    case 4:
        ui->text_camera_name->setText("Kaiyue Li");
        ui->text_camera_id->setText("5");
        ui->text_camera_age->setText("22");
        ui->text_camera_gender->setText("female");
        break;
    case 5:
        ui->text_camera_name->setText("Yu Big");
        ui->text_camera_id->setText("6");
        ui->text_camera_age->setText("22");
        ui->text_camera_gender->setText("male");
        break;
    case 6:
        ui->text_camera_name->setText("Jiali Yang");
        ui->text_camera_id->setText("7");
        ui->text_camera_age->setText("21");
        ui->text_camera_gender->setText("female");
        break;
    }
    QImage face;
    face.load(QString("/home/xuyue/文档/同学的正面照和视频/裁剪后/%1.jpg").arg(sort[0]+1));
    face = face.scaled(80, 80, Qt::KeepAspectRatio);
    ui->label_camera_face1->setPixmap(QPixmap::fromImage(face));
    ui->text_camera_score1->setText(QString("%1").arg(similarities[0]));


    face.load(QString("/home/xuyue/文档/同学的正面照和视频/裁剪后/%1.jpg").arg(sort[1]+1));
    face = face.scaled(80, 80, Qt::KeepAspectRatio);
    ui->label_camera_face2->setPixmap(QPixmap::fromImage(face));
    ui->text_camera_score2->setText(QString("%1").arg(similarities[1]));


    face.load(QString("/home/xuyue/文档/同学的正面照和视频/裁剪后/%1.jpg").arg(sort[2]+1));
    face = face.scaled(80, 80, Qt::KeepAspectRatio);
    ui->label_camera_face3->setPixmap(QPixmap::fromImage(face));
    ui->text_camera_score3->setText(QString("%1").arg(similarities[2]));


    face.load(QString("/home/xuyue/文档/同学的正面照和视频/裁剪后/%1.jpg").arg(sort[3]+1));
    face = face.scaled(80, 80, Qt::KeepAspectRatio);
    ui->label_camera_face4->setPixmap(QPixmap::fromImage(face));
    ui->text_camera_score4->setText(QString("%1").arg(similarities[3]));


    face.load(QString("/home/xuyue/文档/同学的正面照和视频/裁剪后/%1.jpg").arg(sort[4]+1));
    face = face.scaled(80, 80, Qt::KeepAspectRatio);
    ui->label_camera_face5->setPixmap(QPixmap::fromImage(face));
    ui->text_camera_score5->setText(QString("%1").arg(similarities[4]));

    QImage img_small = cvMat2QImage(img);
    img_small = img_small.scaled(360, 240,Qt::KeepAspectRatio);
    ui->img_vedio_probe->setPixmap(QPixmap::fromImage(img_small));
    return faces;
}
void MainWindow::Draw_camera(Mat& img, vector<Rect> faces){
    int i=0;
    for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++ )
         rectangle(img, faces.at(i), CV_RGB(0,0,255), 3, 8, 0);
    QImage img_small = cvMat2QImage(img);
    img_small = img_small.scaled(360, 240,Qt::KeepAspectRatio);
    ui->img_vedio_probe->setPixmap(QPixmap::fromImage(img_small));
}

/*
QString MainWindow::SimilarityEstimation(QString dir_image){

    //调用人脸识别指令

    QString cmd_recognise = QString("br -algorithm FaceRecognition -compare %1 %2 | tee result").arg(getPhotoDir(),dir_image);
    //cout << cmd_recognise.toLatin1().data();
    system(cmd_recognise.toLatin1().data());
    QFile file_recognise("result");
    if(!file_recognise.open(QIODevice::ReadOnly | QIODevice::Text)){
        cout << "Open failed" <<endl;
        return NULL;
    }
    QTextStream input_recognise(&file_recognise);
    QString s1;
    s1 = input_recognise.readLine();
    file_recognise.close();
    //ui->rst_similarity->setText(s1);
    return s1;

}
QString MainWindow::AgeEstimation(QString dir_image){
    QString cmd_genderEstimate = QString("br -algorithm GenderEstimation -enroll %1 gender.csv").arg(dir_image);
    system(cmd_genderEstimate.toLatin1().data());
    QFile file_gender("gender.csv");
    if(!file_gender.open(QIODevice::ReadOnly | QIODevice::Text)){
        cout << "Open failed" <<endl;
    }
    QStringList list_gender[2];
    QTextStream input_gender(&file_gender);
    int i=0;
    while(i<2){
        list_gender[i].clear();
        list_gender[i++] = input_gender.readLine().split(",",QString::SkipEmptyParts);
    }
    QString gender = list_gender[1].at(10);
    file_gender.close();
    return gender;
}
QString MainWindow::GenderEstimation(QString dir_image){
    QString cmd_ageEstimate = QString("br -algorithm AgeEstimation -enroll %1 age.csv").arg(dir_image);
    system(cmd_ageEstimate.toLatin1().data());
    QFile file_age("age.csv");
    if(!file_age.open(QIODevice::ReadOnly | QIODevice::Text)){
        cout << "Open failed" <<endl;
        //return NULL;
    }
    QStringList list_age[2];
    QTextStream input_age(&file_age);
    int i=0;
    while(i<2){
        list_age[i].clear();
        list_age[i++] = input_age.readLine().split(",",QString::SkipEmptyParts);
    }
    double double_age = list_age[1].at(1).toDouble();
    double_age = ((int)(double_age*10)) / 10.0;
    QString age = QString("%1").arg(double_age);
    file_age.close();
    return age;
}
*/
float MainWindow::Get_Similarity(QString img1, QString img2){
    //qDebug() << img1 << endl << img2 << endl;
    // Retrieve classes for enrolling and comparing templates using the FaceRecognition algorithm
    QSharedPointer<br::Transform> transform = br::Transform::fromAlgorithm("FaceRecognition");
    QSharedPointer<br::Distance> distance = br::Distance::fromAlgorithm("FaceRecognition");

    // Initialize templates
    br::Template queryA(img1);
    br::Template queryB(img2);

    // Enroll templates
    queryA >> *transform;
    queryB >> *transform;

    // Compare templates
    float comparisonA = distance->compare(queryA, queryB);


    //br::Context::finalize();
    return comparisonA;
}
void MainWindow::on_btn_get_target_clicked()
{
    //设置目标图像
    QString dir = QFileDialog::getOpenFileName(this,"get target image","/home/xuyue/programs/Human_Detection_System/images","Allfile(*.*)");
    QImage img;
    img.load(dir);
    img = img.scaled(ui->img_target->size(),Qt::KeepAspectRatio);
    ui->img_target->setPixmap(QPixmap::fromImage(img));
    ui->dir_target->setText(dir);
    //设置数据库列表
    //QTableWidget *table_db = new QTableWidget(this);
    QStringList table_db_header;
    table_db_header << "缩略图" << "相似度";
    ui->table_similarity->setHorizontalHeaderLabels(table_db_header);
    //table_db->resizeColumnsToContents();
    //table_db->resizeRowsToContents();
    //table_db->horizontalHeader()->set
    int i;
    for(i=0; i<10; i++){
        //table_db->setItem(i,0,new QTableWidgetItem(QString("%1").arg(1+i)));
        ui->table_similarity->setItem(i,0,new QTableWidgetItem(QIcon(QString(":/orl/ORL_faces/s%1/1.bmp").arg(i+1)),QString("face%1").arg(i+1)));
    }
    QVector<QString> similarity = recognise_from_db(dir);
    for(i=0; i<10; i++){
        ui->table_similarity->setItem(i,1,new QTableWidgetItem(similarity.at(i)));
    }
}
void MainWindow::pause_vedio(){
    if(video_state == 0){
        video_state = 1;
        ui->btn_begin_pause->setText("开始");
    }
    else if(video_state == 1){
        video_state = 0;
        ui->btn_begin_pause->setText("暂停");
    }
    else if(video_state == -1){
        video_state = 0;
        ui->btn_begin_pause->setText("暂停");
        //grp_video_type->setExclusive(false);
        if(page == 0 || ui->type_camera->isChecked())
            cap.open(0);
        if(page == 0)
            recognise_from_db();
        else if(page == 1)
            recognise_from_probe();
        else if(page == 2)
            recognise_from_video(); //这个暂时用于camera模式，而非video模式。

    }
}
void MainWindow::stop_vedio(){
    video_state = -1;
    ui->btn_begin_pause->setText("开始");
    num_faces_after_cut = 0;
    ui->list_faces->clear();
    cap.release();
    //br::Context::finalize();
    grp_video_type->setExclusive(true);
}
QImage MainWindow::cvMat2QImage(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        //qDebug() << "CV_8UC4";
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}
cv::Mat MainWindow::QImage2cvMat(QImage image)
{
    cv::Mat mat;
    qDebug() << image.format();
    switch(image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    default:
        break;
    }

    return mat;
}
void MainWindow::on_btn_get_gallery_clicked()
{
    QString dir = QFileDialog::getOpenFileName(this,"get target image","/home/xuyue/programs/Human_Detection_System/images","Allfile(*.*)");
    //dirs_gallery.push_back(dir);

    //在QListWidget中添加选中的图片，现在不需要了，因为添加的是原图片中的人脸，而非原图片
    /*QImage img;
    img.load(dir);
    QListWidgetItem* list_item_face = new QListWidgetItem(QIcon(QPixmap::fromImage(img)),QString("face%1").arg(ui->list_faces->count()));
    ui->list_faces->addItem(list_item_face);*/

    // 把gallery中的人脸编号存储到数组中，这样便于保存recognise_result,用于计算map
    QString str1 = QString(dir);
    QStringList list1 = str1.split("/");
    QString str2 = ((QString)list1.at(list1.size()-1));
    QString str3 = str2.split(".").at(0);
    id[id_count++] = str3;
    //提取图片中的人脸
    Mat frame = imread(dir.toStdString());
    QVector<QImage> faces = cut_out_and_resize(frame);
    for(QVector<QImage>::const_iterator r = faces.begin(); r != faces.end(); r++){
        QListWidgetItem* list_item_face = new QListWidgetItem(QIcon(QPixmap::fromImage(*r)),QString("gallery_face%1").arg(ui->list_faces->count()));
        ui->list_faces->addItem(list_item_face);
    }

    //QStandardItem *list_item_face = new QStandardItem(QIcon(QPixmap::fromImage(img_ROI)),QString("face%1").arg(i+1));
    //model_faces->appendRow(list_item_face);
}
void MainWindow::on_btn_get_probe_clicked()
{
    QString dir = QFileDialog::getOpenFileName(this,"get target image","/home/xuyue/programs/Human_Detection_System/images","Allfile(*.*)");
    // 把gallery中的人脸编号存储到数组中，这样便于保存recognise_result,用于计算map
    QString str1 = QString(dir);
    QStringList list1 = str1.split("/");
    QString str2 = ((QString)list1.at(list1.size()-1));
    QString str3 = str2.split(".").at(0);
    id[id_count++] = str3;
    //提取图片中的人脸
    Mat frame = imread(dir.toStdString());
    QVector<QImage> faces = cut_out_and_resize(frame);
    if(num_faces_after_cut == 1){
        ui->label_face1->setPixmap(QPixmap::fromImage(faces.at(0)));
        ui->text_name_face1->setText("probe 1");
        ui->text_age_face1->setText("17");
    }
    if(num_faces_after_cut == 2){
        ui->label_face2->setPixmap(QPixmap::fromImage(faces.at(0)));
        ui->text_name_face2->setText("probe 2");
        ui->text_age_face2->setText("20");
    }
    if(num_faces_after_cut == 3){
        ui->label_face3->setPixmap(QPixmap::fromImage(faces.at(0)));
        ui->text_name_face3->setText("probe 3");
        ui->text_age_face3->setText("17");
    }


}
QVector<QImage> MainWindow::cut_out_and_resize(Mat& img){
    QVector<QImage> faces_after_cut;//返回值
    Mat gray, smallImg(cvRound (img.rows/2), cvRound(img.cols/2), CV_8UC1);
    vector<Rect> faces;
    cv::resize( img, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );
    cvtColor(smallImg, gray, CV_BGR2GRAY);
    equalizeHist(gray,gray);
    cascade.detectMultiScale(gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE,Size(100, 100));
    int i = 1;
    for(vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++ ){
         //下面的代码是为了保存彩色ROI区域，上面的smallImgROI是灰度图。
            IplImage temp = smallImg;
            IplImage* point_Src,*point_Dest;
            point_Src = cvCloneImage(&temp);
            CvRect rect;
            rect.x = r->x;// - 180;
            rect.y = r->y;// - 180;
            rect.width = r->width;// + 360;
            rect.height = r->height;// + 360;
            cvSetImageROI(point_Src, rect);
            CvSize size_dest;
            size_dest.width = 80;
            size_dest.height = 80;
            point_Dest = cvCreateImage(size_dest,point_Src->depth,point_Src->nChannels);
            cvResize(point_Src,point_Dest,CV_INTER_LINEAR);
            cvSaveImage(QString("probe_face%1.jpg").arg(num_faces_after_cut).toLatin1().data(),point_Dest);

            cvResetImageROI(point_Src);
            QImage face;
            face.load(QString("probe_face%1.jpg").arg(num_faces_after_cut).toLatin1().data());
            num_faces_after_cut++;
            faces_after_cut.push_back(face);
    }
    return faces_after_cut;
}
Mat MainWindow::get_img_to_detect(){
    Mat frame;
    //if(ui->type_probe->isChecked()){
        frame = imread(QString("%1%2.jpg").arg(path_probe).arg(count_probe).toStdString(),1);
        imageName = QString("%1.jpg").arg(count_probe);
        //equalizeHist( frame, frame );
        count_probe += 2;
    /*}
    else if(ui->type_camera->isChecked()){
        if(cap.isOpened()){
            cap >> frame;
            flip(frame,frame,1);//y轴翻转
            Mat dst;//用于颜色空间转换的一个临时变量
            if(ui->btn_hist_equal->isChecked()){
                cvtColor( frame, dst, CV_BGR2YCrCb );
                MyHistEqual(dst);
                cvtColor( dst, frame, CV_YCrCb2BGR );
            }
        }
        else{
            qDebug() << QString("camera is not opended") << endl;
            return frame;
        }
    }*/
    return frame;
}
Mat& MainWindow::MyHistEqual(Mat& I)
{
    CV_Assert(I.data);
    // accept only char type matrices
    CV_Assert(I.depth() != sizeof(uchar));

    const int channels = I.channels();
    switch(channels)
    {
        case 1:
            {
                int Hist[256];
                memset(Hist, 0, sizeof(Hist));

                // form image histogramp
                MatIterator_<uchar> it, end;
                for( it = I.begin<uchar>(), end = I.end<uchar>(); it != end; it++ )
                    Hist[(*it)] ++;

                // form cumulative histograme
                for( int i = 1; i < 256; i++ )
                    Hist[i] += Hist[i-1];

                // histogram equalization
                int nRows = I.rows;
                int nCols = I.cols;
                for( it = I.begin<uchar>(), end = I.end<uchar>(); it != end; it++ )
                    *it = (uchar)( 255 * Hist[(*it)] / (nRows * nCols) );

                break;
            }
        case 3: // only equalize first color component, for YUV format
            {
                int Hist[256];
                memset(Hist, 0, sizeof(Hist));

                // form image histogramp
                MatIterator_<Vec3b> it, end;
                for( it = I.begin<Vec3b>(), end = I.end<Vec3b>(); it != end; it++ )
                {
                    Hist[(*it)[0]] ++;
                }

                // form cumulative histograme
                for( int i = 1; i < 256; i++ )
                    Hist[i] += Hist[i-1];

                // histogram equalization
                int nRows = I.rows;
                int nCols = I.cols;
                for( it = I.begin<Vec3b>(), end = I.end<Vec3b>(); it != end; it++ )
                    (*it)[0] = (uchar)( 255 * Hist[(*it)[0]] / (nRows * nCols) );

            }
    }

    return I;

}
void MainWindow::on_btn_delete_gallery_clicked()
{
    QList<QListWidgetItem *> faces_to_delete = ui->list_faces->selectedItems();
    for(QList<QListWidgetItem *>::const_iterator i = faces_to_delete.begin(); i != faces_to_delete.end(); i++){
        QFile::remove(QString("%1.jpg").arg( (*i)->text() ));
        ui->list_faces->removeItemWidget((*i));
        delete (*i);
    }
    for(int i=0; i < ui->list_faces->count(); i++)
        ui->list_faces->item(i)->setText(QString("gallery_face%1").arg(i));
    //ui->list_faces->repaint();
    int count_exist_faces = 0;
    for(int i=0; i < num_faces_after_cut; i++){
        QFile f(QString("gallery_face%1.jpg").arg(i));
        if( f.exists() ){
            f.rename( QString("gallery_face%1.jpg").arg(count_exist_faces) );
            count_exist_faces++;
        }
    }
    num_faces_after_cut = count_exist_faces;
}
void MainWindow::get_roc_data(QVector<double> *x, QVector<double> *y){
    QFile file("/home/xuyue/programs/Human_Detection_System/similarity.csv");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Cannot open file for reading." << endl;
        return;
    }
    QTextStream in(&file);
    QStringList lists[60];
    for(int i = 0; !in.atEnd(); i++){
        QString line = in.readLine();
        lists[i].clear();
        lists[i] = line.split(",", QString::SkipEmptyParts);
    }
    for(double threshold = -1; threshold <= 1; threshold += 0.05){
        int num_tp = 0;
        int num_fp = 0;
        for(int i = 0; i <60 ; i += (i+1)%3==0? 4:1 ){
            for(int j = 0; j < 10; j++){
                if(((QString)lists[i].at(j)).toDouble() > threshold)
                    num_tp += 1;
            }
        }
        for(int i = 3; i <60; i += (i+1)%6==0 ? 4:1){
            for(int j = 0; j < 10; j++){
                if(((QString)lists[i].at(j)).toDouble() > threshold)
                    num_fp += 1;
            }
        }
        double tpr = ((double)num_tp) / 300;
        double fpr = ((double)num_fp) / 300;
        y->push_back(tpr);
        x->push_back(fpr);
        qDebug() << QString("%1 %2 %3").arg(threshold).arg(fpr).arg(tpr) << endl;
    }
    return;
}
void MainWindow::get_cms_data(QVector<double> *x, QVector<double> *y){
    QFile file("/home/xuyue/programs/Human_Detection_System/similarity_cms.csv");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Cannot open file for reading." << endl;
        return;
    }
    int num[3] = {0,0,0};
    QTextStream in(&file);
    QStringList lists[250];
    for(int i = 0; !in.atEnd(); i++){
        QString line = in.readLine();
        lists[i].clear();
        lists[i] = line.split(",", QString::SkipEmptyParts);
    }
    for(int i = 0; i < 250; i++){
            double max = ((QString)lists[i].at(0)).toDouble();
            int index_max = -1;
            for(int k = 1; k < 10; k++){
                if(((QString)lists[i].at(k)).toDouble() > max){
                    max = ((QString)lists[i].at(k)).toDouble();
                    index_max = k;
                }
            }
            while(index_max < 3)
                num[index_max++]++;

    }
    x->push_back(0.0);
    y->push_back(0.0);
    x->push_back(1.0);
    y->push_back( ((double)num[0]) / 250 );
    x->push_back(2.0);
    y->push_back( ((double)num[1]) / 250 );
    x->push_back(3.0);
    y->push_back( ((double)num[2]) / 250 );
    x->push_back(4.0);
    y->push_back( ((double)num[2]) / 250 );
    x->push_back(5.0);
    y->push_back( ((double)num[2]) / 250 );
    x->push_back(6.0);
    y->push_back( ((double)num[2]) / 250 );
    x->push_back(7.0);
    y->push_back( ((double)num[2]) / 250 );
    x->push_back(8.0);
    y->push_back( ((double)num[2]) / 250 );
    x->push_back(9.0);
    y->push_back( ((double)num[2]) / 250 );
    x->push_back(10.0);
    y->push_back( ((double)num[2]) / 250 );
    return;
}
void MainWindow::on_btn_show_curve_clicked()
{
    if(ui->btn_roc->isChecked()){
        QVector<double> *x, *y;
        x = new QVector<double>();
        y = new QVector<double>();
        get_roc_data(x,y);
        // create graph and assign data to it:
        ui->chart_ros->addGraph();
        ui->chart_ros->graph(0)->setData(*x, *y);
        // give the axes some labels:
        ui->chart_ros->xAxis->setLabel("x");
        ui->chart_ros->yAxis->setLabel("y");
        // set axes ranges, so we see all data:
        ui->chart_ros->xAxis->setRange(0, 1);
        ui->chart_ros->yAxis->setRange(0, 1);
        ui->chart_ros->replot();
    }
    else if(ui->btn_cms->isChecked()){
        QVector<double> *x, *y;
        x = new QVector<double>();
        y = new QVector<double>();
        get_cms_data(x,y);
        // create graph and assign data to it:
        ui->chart_ros->addGraph();
        ui->chart_ros->graph(0)->setData(*x, *y);
        // give the axes some labels:
        ui->chart_ros->xAxis->setLabel("x");
        ui->chart_ros->yAxis->setLabel("y");
        // set axes ranges, so we see all data:
        ui->chart_ros->xAxis->setRange(0.0, 10.5);
        ui->chart_ros->yAxis->setRange(0, 1);
        ui->chart_ros->replot();
    }
}
void MainWindow::parseXML(QString file_name){
    if(file_name.isEmpty())
        return;
    QFile file(file_name);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "open error!" << endl;
        return;
    }
    QDomDocument document;
    QString error;
    int row = 0, column = 0;
    if(!document.setContent(&file, false, &error, &row, &column))
    {
        qDebug() << QString("parse file failed at line row %1 , column %2.").arg(row).arg(column);
        return;
    }
    QDomElement root = document.documentElement();
    if(root.hasAttribute("Version"))
    {
        QString version = root.attribute("Version");
        if(! (version == "1.0"))
        {
            qDebug() << "error : version is not 1.0 ." << endl;
            return;
        }
    }
    QDomNodeList root_list = root.childNodes();
    QDomElement items = root_list.item(1).toElement();
    QDomNodeList items_list = items.childNodes();
    int items_count = items_list.count();
    for(int i=0; i<items_count; i++){
        QDomElement item = items_list.item(i).toElement();
        QString name = item.attribute("imageName");
        //qDebug() << name << " ";
        QDomNodeList item_list = item.childNodes();
        int item_count = item_list.count();
        for(int j=0; j<item_count; j++){
            QDomElement label = item_list.item(0).toElement();
            QString l,r,t,b;
            QString id;
            id = label.attribute("id");
            l = label.attribute("l");
            r = label.attribute("r");
            t = label.attribute("t");
            b = label.attribute("b");
            ground_truth[gt_count++] = Recognise_result(0,l.toInt(),r.toInt(),t.toInt(),b.toInt(),id.toInt(),name);
            if(gt_count > 2000)
                qDebug() << "error : face amount is more than 2000" << endl;
        }
    }

}
float* MainWindow::getAP(int id){
    Recognise_result temp_result_set[1000];;
    int temp_result_count = 0;
    //在结果中筛选出所有id为所给id的识别结果
    for(int i=0; i<result_count; i++){
        if(result[i].id == id)
            temp_result_set[temp_result_count++] = Recognise_result(result[i]);
    }
    /*qDebug() << "*排序前*";
    for(int i=0; i<temp_result_count; i++)
        temp_result_set[i].print();*/

    //按照相似度排序
    int number = temp_result_count > 20 ? 20:temp_result_count; // 因为需要计算N = 1 , 5, 10, 20的map，所以排序后的序列中至多需要20个结果
    int index;
    float max;
    Recognise_result temp;
    for(int i=0; i<number; i++){
        index = -1;
        max = -1.0;
        for(int j=i; j<temp_result_count; j++){
            if(temp_result_set[j].similar > max){
                max = temp_result_set[j].similar;
                index = j;
            }
        }
        if(index != -1){
            temp = temp_result_set[i];
            temp_result_set[i] = temp_result_set[index];
            temp_result_set[index] = temp;
        }
    }
    /*qDebug() << "*排序后*";
    for(int i=0; i<temp_result_count; i++)
        temp_result_set[i].print();*/

    //判断每个识别结果是否符合gt
    int right[50]; //temp_result_set中的每一个识别结果是否符合gt

    for(int i=0; i<temp_result_count; i++)
        right[i] = 0;
    for(int i=0; i<temp_result_count; i++){
        for(int j=0; j<gt_count; j++){
            if(ground_truth[j].picture_name == temp_result_set[i].picture_name && ground_truth[j].id == temp_result_set[i].id){
                //qDebug() << "half success";
                if( overlap_rate(ground_truth[j].rect.x,
                                 ground_truth[j].rect.y,
                                 ground_truth[j].rect.width,
                                 ground_truth[j].rect.height,
                                 temp_result_set[i].rect.x,
                                 temp_result_set[i].rect.y,
                                 temp_result_set[i].rect.width,
                                 temp_result_set[i].rect.height) > 0.5 ){
                    right[i] = 1;
                    break;
                }
            }
        }
    }
    /*qDebug() << "*每个识别结果是否符合gt*";
    for(int i=0; i<temp_result_count; i++)
       qDebug() << right[i];*/

    //开始计算AP
    //先计算M，即temp_result_set中共有几个符合ground_truth的结果。
    //同时计算每个结果前共有几个符合gt的结果
    int right_count=0; //right_count的最终结果就是M
    int right_before[50];
    for(int i=0; i<temp_result_count; i++)
        right_before[i] = 0;
    for(int i=0; i<temp_result_count; i++){
        if(right[i] == 1){
            right_count++;
            right_before[i] = right_count;
        }
    }

    qDebug() << "*输出right_before*";
    for(int i=0; i<temp_result_count; i++)
        qDebug() << right_before[i];

    float AP = 0;
    float* rt = (float*)malloc(sizeof(float)*4);
    for(int i=0; i<1; i++){
        if(right[i] == 1)
            AP += (float)right_before[i] / (i+1);
    }
    AP /= 1 < right_count ? 1 : right_count;
    rt[0] = AP;

    AP = 0;
    for(int i=0; i<5; i++){
        if(right[i] == 1)
            AP += (float)right_before[i] / (i+1);
    }
    AP /= 5 < right_count ? 5 : right_count;
    rt[1] = AP;

    AP = 0;
    for(int i=0; i<10; i++){
        if(right[i] == 1)
            AP += (float)right_before[i] / (i+1);
    }
    AP /= 10 < right_count ? 10 : right_count;
    rt[2] = AP;

    AP = 0;
    for(int i=0; i<20; i++){
        if(right[i] == 1)
            AP += (float)right_before[i] / (i+1);
    }
    AP /= 20 < right_count ? 20 : right_count;
    rt[3] = AP;

    return rt;
}
float MainWindow::overlap_rate(int x1, int y1, int width1, int height1,
          int x2, int y2, int width2, int height2){
    int startx,endx,starty,endy;

    if(x1 <= x2)
        startx = x1;
    else
        startx = x2;

    if(x1+width1 >= x2+width2)
        endx = x1+width1;
    else
        endx = x2+width2;

    if(y1 <= y2)
        starty = y1;
    else
        starty = y2;

    if(y1+width1 >= y2+width2)
        endy = y1+width1;
    else
        endy = y2+width2;

    int x,y;
    x = width1 + width2 - (endx - startx);
    y = height1 + height2 - (endy - starty);
    return (float)x*y/(width1*height1 + width2*height2 - x*y);
}
void MainWindow::writeXML(){
    qDebug() << "begin";
    QFile file("root.xml");
    if(! file.open(QFile::ReadOnly | QFile::Text))
        return;
    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument doc;
    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
        return;
    file.close();
    QDomElement root = doc.documentElement();
    if (root.tagName() != "Message")
        return;

    root.setAttribute("Version","1.0");
    QDomElement info = doc.createElement("Info");
    info.setAttribute("evaluateType","4");
    info.setAttribute("MediaFile","dongmanmeneast_15_1920*1080_30");
    QDomElement items = doc.createElement("Items");
    for(int i=0; i<result_count; i++){
        qDebug() << QString("before%1").arg(i);
        QDomElement item = doc.createElement("Item");
        item.setAttribute("imageName",result[i].picture_name);
        QDomElement label = doc.createElement("Label");
        label.setAttribute("id",QString("%1").arg(result[i].id));
        label.setAttribute("score",QString("%1").arg(result[i].similar));
        label.setAttribute("l",QString("%1").arg(result[i].rect.x));
        label.setAttribute("r",QString("%1").arg(result[i].rect.x + result[i].rect.width));
        label.setAttribute("t",QString("%1").arg(result[i].rect.x));
        label.setAttribute("b",QString("%1").arg(result[i].rect.x + result[i].rect.height));
        item.appendChild(label);
        items.appendChild(item);
        qDebug() << QString("after%1").arg(i);
    }
    info.appendChild(items);
    root.appendChild(info);
    qDebug() << "begin";
    QFile f("aaa.xml");
    if (!f.open(QFile::WriteOnly | QFile::Text))
     return;
    QTextStream out(&f);
    QTextCodec * codec = QTextCodec::codecForName("GBK");
    out.setCodec(codec);
    QDomDocument xmlDoc;
    QString strHead("version=\"1.0\" encoding=\"GB2312\"");
    xmlDoc.appendChild(xmlDoc.createProcessingInstruction("xml", strHead));
    out<<xmlDoc.toString();
    root.save(out, 3);
    f.close();
    qDebug() << "end";
}
