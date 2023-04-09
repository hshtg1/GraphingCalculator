#include <simple2d.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <string>

class Function;
void render();
void update();

const int width = 800, height = 600;

struct Point
{
    float x, y;
};

class Function
{
    friend void drawFunc(Function &func);
    std::vector<Point> points;
    void calculate_points()
    {

    }
public:
    virtual float func(float x) = 0;
    Function()
    {

    }
};

class Polynomial1
{
public:
    float func(float x)
    {
        float y = pow(x, 3) + pow(x, 2) + 1;
        return y;
    }
};

enum rotation
{
    vertical,horizontal
};

struct Zero_return
{
    float Opoint;
    bool start_lower;
};

class Axis
{
    friend void drawAxis(Axis axis, Zero_return zero);
    std::vector<float> px_coor;
    std::vector<int> coor;
    int n_scale,middle_offset;
    float lineWidth, px_scale;
    int range_start, range_end, scale;
    float axis_offset;
    std::string name;
    rotation rot;
    void calculatePoints()
    {
        scale = (range_end - range_start) / n_scale;
        for (int i = 0; i <= n_scale; i++)
        {
            coor.push_back(range_start + (scale * i));
        }
        px_scale =  (rot==horizontal) ?  (width - (axis_offset * 2)) / n_scale : (height - (axis_offset * 2)) / n_scale;
        for (int i = 0; i <= n_scale; i++)
        {
            px_coor.push_back(axis_offset + (px_scale * i));
        }
    }
public:
    Axis(int sc = 200, std::string n = "X", rotation rt = horizontal, int st = -100, int e = 100, float aof = 30, int w = 3)
    {
        if (st < e && (!(st + e >= abs(st) + abs(e)) || st==0))
        {
            n_scale = sc;
            name = n;
            rot = rt;
            range_start = st;
            range_end = e;
            axis_offset = aof;
            lineWidth = w;
            calculatePoints();
        }
        else if(st + e >= abs(st) + abs(e))
        {
            std::cout << "Invalid data! This axis doesn't contain a zero!" << std::endl;
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
        float Opoint = ((l-2*axis_offset)* (float)abs(lower_range) / (float)(range_end - range_start))+axis_offset;
        Zero_return zero;
        zero.start_lower = start_lower;
        zero.Opoint = Opoint;
        return zero;
    }
    void set_middle_offset(float mo)
    {
        middle_offset = mo;
    }
};

void drawFunc(Function& func)
{

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
        S2D_DrawLine(axis.axis_offset, middle+axis.middle_offset, width - axis.axis_offset, middle + axis.middle_offset,
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
        S2D_DrawLine(middle + axis.middle_offset, axis.axis_offset, middle + axis.middle_offset, height - axis.axis_offset,
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
        txt->x = width-axis.axis_offset-30;
        txt->y = middle - 30;
    }
    else
    {
        txt->x = middle;
        txt->y = 0;
    }
    S2D_DrawText(txt);
    S2D_FreeText(txt);
}

void render()
{
    
}

void update()
{
    Axis ax1(2, "Y", vertical, -100, 100);
    Axis ax2(20, "time", horizontal, 0, 100);
    drawAxis(ax1, ax2.CalculateZero());
    drawAxis(ax2, ax1.CalculateZero());
}

int main(int argc, char* argv[])
{
    S2D_Window* window = S2D_CreateWindow(
        "Graphing Calculator",  // title of the window
        width, height,        // width and height
        render, update,  // callback function pointers (these can be NULL)
        S2D_RESIZABLE              // flags
    );
    window->viewport.mode = S2D_STRETCH;
    window->background.r = 1.0;
    window->background.g = 1.0;
    window->background.b = 1.0;
    S2D_Show(window);
    return 0;
}