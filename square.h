#ifndef SQUARE_H
#define SQUARE_H

#include <QLabel>
#include <QString>

#include <map>

typedef std::map<std::string, std::string> StyleMap;

class Square : public QLabel
{
    Q_OBJECT
public:
    Square(QWidget* parent=nullptr, int _row=0, int _col=0);

    void set_color(int _color);
    int get_color() const;

private:
    void render();

    void setStyle(std::string key, std::string value);
    void applyStyle();
    int row, col;
    int color;
    StyleMap style;

    static const int OFFSET_X = 16;
    static const int OFFSET_Y = 16;
    static const int SQUARE_WIDTH = 32;
    static const int SQUARE_HEIGHT = 32;

};
#endif // SQUARE_H
