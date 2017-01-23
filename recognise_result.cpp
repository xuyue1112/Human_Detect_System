#include<recognise_result.h>
Recognise_result::Recognise_result()
{
}
Recognise_result::Recognise_result(const Recognise_result &other){
    similar = other.similar;
    rect = other.rect;
    id = other.id;
    picture_name = other.picture_name;
}
Recognise_result::~Recognise_result()
{
}

Recognise_result::Recognise_result(float similar, Rect rect,int id, QString picture_name){
    this->similar = similar;
    this->rect = rect;
    this->id = id;
    this->picture_name = picture_name;
}
Recognise_result::Recognise_result(float similar, int l, int r, int t, int b, int id, QString picture_name){
    this->similar = similar;
    this->rect.x = l;
    this->rect.y = t;
    this->rect.width = r-l;
    this->rect.height = b - t;
    this->id = id;
    this->picture_name = picture_name;
}
void Recognise_result::print(){
    qDebug() << QString("name:%1  id:%2 similar:%3 l:%4 r:%5 t:%6 b:%7").arg(picture_name).arg(id).arg(similar).arg(rect.x).arg(rect.x + rect.width).arg(rect.y).arg(rect.y + rect.height);
}
