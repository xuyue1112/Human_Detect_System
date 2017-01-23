#ifndef RECOGNISE_RESULT
#define RECOGNISE_RESULT

#endif // RECOGNISE_RESULT
#include <QtCore>
#include <opencv2/core/core.hpp>
using namespace cv;
using namespace std;
class Recognise_result
{
public:
    Recognise_result();
    ~Recognise_result();
    Recognise_result(const Recognise_result &other);
    Recognise_result(float similar, Rect rect,int id, QString picture_name);
    Recognise_result(float similar, int l, int r, int t, int b, int id, QString picture_name);
    void print();

    float similar;
    Rect rect;
    int id;
    QString picture_name;
};
