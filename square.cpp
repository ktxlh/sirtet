#include <QFont>
#include <sstream>

#include "square.h"

#include <iostream>

using std::string;
using std::ostringstream;
using std::cerr;
using std::endl;

Square::Square(QWidget* parent, int _row, int _col) :
    QLabel(parent),
    row(_row),
    col(_col),
    color(0)
{
    this->render();
}

void Square::render() {
    setGeometry(QRect(OFFSET_X + SQUARE_WIDTH * this->col, OFFSET_Y + SQUARE_HEIGHT * (19-this->row), SQUARE_WIDTH, SQUARE_HEIGHT));
    setVisible(true);
    setAutoFillBackground(true);
    setText("");
    setStyle("border-image", "url(\":/resources/images/0.png\");");
    applyStyle();
}

void Square::setStyle(string key, string value) {
    this->style[key] = value;
}

void Square::applyStyle() {
    ostringstream s;
    for (StyleMap::iterator i=this->style.begin(); i!=this->style.end(); i++) {
        s << i->first << ":" << i->second << ";";
    }
    string style_string = s.str();
    setStyleSheet(QString::fromStdString(style_string));
}

void Square::set_color(int _color){
    color = _color;
    ostringstream buf;
    buf << ":/resources/images/square_" << color << ".png";
    string path=buf.str();
    setStyle("border-image", "url(\"" + path + "\");");

    applyStyle();
}

int Square::get_color() const{
    return color;
}
