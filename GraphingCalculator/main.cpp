#include <simple2d.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <string>

class Function;
class Scale;
void render();
void update();

const int width = 800, height = 600;

enum rotation
{
    vertical, horizontal
};

struct Zero_return
{
    float Opoint;
    bool start_lower;
};

struct valid_float
{
    float value;
    bool valid;
    valid_float(float v, bool vd)
    {
        value = v;
        valid = vd;
    }
};

class Scale
{
protected:
    float scale_offset;
    int range_start, range_end;
    rotation rot;
public:
    Scale(rotation r = horizontal, int st = -10, int e = 10, int of = 30)
    {
        if (st < e && (!(st + e >= abs(st) + abs(e)) || st == 0))
        {
            rot = r;
            range_start = st;
            range_end = e;
            scale_offset = of;
        }
        else if (st + e >= abs(st) + abs(e))
        {
            std::cout << "Invalid data! This scale doesn't contain a zero!" << std::endl;
        }
        else
        {
            std::cout << "Invalid data! range_start cannot be of a larger value than range_end (or equal)!" << std::endl;
        }
    }
    Zero_return CalculateZero()
    {
        int l = (rot == horizontal) ? width : height;
        int lower_range;
        bool start_lower;
        if (abs(range_start) > abs(range_end))
        {
            lower_range = range_end;
            start_lower = false;
        }
        else
        {
            lower_range = range_start;
            start_lower = true;
        }
        float Opoint = ((l - 2 * scale_offset) * (float)abs(lower_range) / (float)(range_end - range_start)) + scale_offset;
        Zero_return zero;
        zero.start_lower = start_lower;
        zero.Opoint = Opoint;
        return zero;
    }
    int get_start() { return range_start; }
    int get_end() { return range_end; }
    int get_offset() { return scale_offset; }
};

struct Point
{
    float x, y;
    Point(float x, float y)
    {
        this->x = x;
        this->y = y;
    }
};

class Function
{
    friend void drawFunc(Function &func, int r, int g, int b, int a);
    int n_points;
    std::vector<Point> points;
    Scale scale_x, scale_y;
public:
    virtual valid_float func(float x) = 0;
    Function(int np = 1000)
    {
        n_points = np;
    }
    void calculate_points(Scale sx, Scale sy)
    {
        scale_x = sx;
        scale_y = sy;
        int range_x = scale_x.get_end() - scale_x.get_start();
        int range_y = scale_y.get_end() - scale_y.get_start();
        float x_dif = (float)range_x/n_points;
        for (int i = 0; i <= n_points; i++)
        {
            float x = scale_x.get_start() + (x_dif * i);
            float px = x / range_x * (width - 2 * sx.get_offset());
            if (func(x).valid)
            {
                float y = func(x).value;
                float py = y / range_y * (height - 2 * sy.get_offset());
                Point p(px, py);
                points.push_back(p);
            }
            //std::cout << "p" << i << ": " << px << ", " << y << std::endl;
        }
    }
};

class Axis : public Scale
{
    friend void drawAxis(Axis axis, Zero_return zero);
    std::vector<float> px_coor;
    std::vector<int> coor;
    int middle_offset, scale, n_scale;
    float lineWidth, px_scale;
    std::string name;
    rotation rot;
    void calculatePoints()
    {
        scale = (range_end - range_start) / n_scale;
        for (int i = 0; i <= n_scale; i++)
        {
            coor.push_back(range_start + (scale * i));
        }
        px_scale =  (rot==horizontal) ?  (width - (scale_offset * 2)) / n_scale : (height - (scale_offset * 2)) / n_scale;
        for (int i = 0; i <= n_scale; i++)
        {
            px_coor.push_back(scale_offset + (px_scale * i));
        }
    }
public:
    Axis(int sc = 200, std::string n = "X", rotation rt = horizontal, int st = -100, int e = 100, float aof = 30, int w = 3)
        :Scale(rt, st, e, aof)
    {
        n_scale = sc;
        name = n;
        rot = rt;
        lineWidth = w;
        calculatePoints();
    }
    void set_middle_offset(float mo)
    {
        middle_offset = mo;
    }
};

void drawFunc(Function &func, int r, int g, int b, int a)
{
    float zero_x = func.scale_x.CalculateZero().Opoint;
    float zero_y = func.scale_y.CalculateZero().Opoint;
    for (int i = 0; i < func.points.size(); i++)
    {
        S2D_DrawCircle(zero_x+func.points[i].x, height-zero_y-func.points[i].y, 2, 1000, r,g,b,a);
    }
}

void drawAxis(Axis axis, Zero_return zero)
{
    float middle = zero.Opoint;
    if (axis.rot == horizontal)
    {
        if (zero.start_lower)
        {
            middle = height-middle;
        }
        S2D_DrawLine(axis.scale_offset, middle+axis.middle_offset, width - axis.scale_offset, middle + axis.middle_offset,
            axis.lineWidth,
            0, 0, 0, 1,
            0, 0, 0, 1,
            0, 0, 0, 1,
            0, 0, 0, 1);
        
        for (int i = 0; i < axis.coor.size(); i++)
        {
            S2D_Text* txt = S2D_CreateText("fonts/times-new-roman.ttf", std::to_string(axis.coor[i]).c_str(), 10);
            txt->color.r = 0.0;
            txt->color.g = 0.0;
            txt->color.b = 0.0;
            txt->x = axis.px_coor[i];
            txt->y = middle + 10 + axis.middle_offset;
            if(axis.coor[i]!=0) S2D_DrawText(txt);
            S2D_DrawLine(axis.px_coor[i], middle + axis.lineWidth + 5 + axis.middle_offset, axis.px_coor[i], middle - axis.lineWidth - 5 + axis.middle_offset,
                axis.lineWidth / 2,
                0, 0, 0, 1,
                0, 0, 0, 1,
                0, 0, 0, 1,
                0, 0, 0, 1);
            S2D_FreeText(txt);
        }
    }
    else
    {
        if (!zero.start_lower)
        {
            middle = width - zero.Opoint;
        }
        S2D_DrawLine(middle + axis.middle_offset, axis.scale_offset, middle + axis.middle_offset, height - axis.scale_offset,
            axis.lineWidth,
            0, 0, 0, 1,
            0, 0, 0, 1,
            0, 0, 0, 1,
            0, 0, 0, 1);
        for (int i = 0; i < axis.coor.size(); i++)
        {
            S2D_Text* txt = S2D_CreateText("fonts/times-new-roman.ttf", std::to_string(axis.coor[i]).c_str(), 10);
            txt->color.r = 0.0;
            txt->color.g = 0.0;
            txt->color.b = 0.0;
            txt->x =  middle + axis.middle_offset + 10;
            txt->y = height - axis.px_coor[i] - 10;
            if (axis.coor[i] != 0) S2D_DrawText(txt);
            S2D_DrawLine(middle - axis.lineWidth - 5 + axis.middle_offset, axis.px_coor[i], middle + axis.lineWidth + 5 + axis.middle_offset, axis.px_coor[i],
                axis.lineWidth / 2,
                0, 0, 0, 1,
                0, 0, 0, 1,
                0, 0, 0, 1,
                0, 0, 0, 1);
            S2D_FreeText(txt);
        }
    }
    S2D_Text* txt = S2D_CreateText("fonts/times-new-roman.ttf", axis.name.c_str(), 20);
    txt->color.r = 0.0;
    txt->color.g = 0.0;
    txt->color.b = 0.0;
    if(axis.rot==horizontal)
    {
        txt->x = width-axis.scale_offset;
        txt->y = middle-30;
    }
    else
    {
        txt->x = middle;
        txt->y = 0;
    }
    S2D_DrawText(txt);
    S2D_FreeText(txt);
}


class Polynomial1 : public Function
{
public:
    virtual valid_float func(float x)
    {
        float y = 10*pow(x, 2);
        return valid_float(y,true);
    }
    using Function::Function;
};

class Polynomial2 : public Function
{
public:
    virtual valid_float func(float x)
    {
        float y = pow(x, 3) + pow(x,2) + 1;
        return valid_float(y, true);;
    }
    using Function::Function;
};

class Linear1: public Function
{
public:
    virtual valid_float func(float x)
    {
        float y = x;
        return valid_float(y, true);
    }
    using Function::Function;
};

class Trig1 : public Function
{
public:
    virtual valid_float func(float x)
    {
        float y = sin(x*2*M_PI/360.0);
        return valid_float(y, true);;
    }
    using Function::Function;
};

class Rational1 : public Function
{
public:
    virtual valid_float func(float x)
    {
        if (x != 0)
        {
            float y = 1 / x;
            return valid_float(y, true);
        }
        else return valid_float(0, false);
    }
    using Function::Function;
};

void update()
{
    Axis ax1(2, "Y", vertical, -1, 1);
    Axis ax2(2, "X", horizontal, -10, 10);
    drawAxis(ax1, ax2.CalculateZero());
    drawAxis(ax2, ax1.CalculateZero());
    //Scale sc1(horizontal,0,10);
    //Scale sc2(vertical,0,10);
    /*Polynomial1 poly1;
    poly1.calculate_points(ax2,ax1);
    drawFunc(poly1, 255, 0, 0, 1);
    Polynomial2 poly2;
    poly2.calculate_points(ax2, ax1);
    drawFunc(poly2, 0, 255, 0, 1);
    Linear1 lin1;
    lin1.calculate_points(ax2, ax1);
    drawFunc(lin1, 0, 0, 255, 1);
    Trig1 trig1;
    trig1.calculate_points(ax2, ax1);
    drawFunc(trig1, 0, 0, 0, 1);*/
    Rational1 rat1(5000);
    rat1.calculate_points(ax2, ax1);
    drawFunc(rat1, 0, 0, 0, 1);
}

int main(int argc, char* argv[])
{
    S2D_Window* window = S2D_CreateWindow(
        "Graphing Calculator",  // title of the window
        width, height,        // width and height
        NULL, update,  // callback function pointers (these can be NULL)
        S2D_RESIZABLE              // flags
    );
    window->viewport.mode = S2D_STRETCH;
    window->background.r = 1.0;
    window->background.g = 1.0;
    window->background.b = 1.0;
    S2D_Show(window);
    return 0;
}