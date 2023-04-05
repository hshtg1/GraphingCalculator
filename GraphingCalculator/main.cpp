#include <simple2d.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <string>

class Function;
void render();
void update();

const int width = 1000, height = 600;

class Point
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

class Axis
{
    friend void drawAxis(Axis axis);
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
    Axis(int sc = 200,std::string n = "X", rotation rt = horizontal, int st = -100, int e = 100, float aof = 30, int w = 3)
    {
        if (st < e)
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
        else
        {
            std::cout << "Invalid data! range_start cannot be of a larger value than range_end!";
        }
    }
};

void drawFunc(Function& func)
{

}

void drawAxis(Axis axis)
{
    if (axis.rot == horizontal)
    {
        S2D_DrawLine(axis.axis_offset, height / 2, width - axis.axis_offset, height / 2,
            axis.lineWidth,
            0, 0, 0, 1,
            0, 0, 0, 1,
            0, 0, 0, 1,
            0, 0, 0, 1);
        
        for (int i = 0; i < axis.coor.size(); i++)
        {
            S2D_Text* txt = S2D_CreateText("fonts/times-new-roman.ttf", std::to_string(axis.coor[i]).c_str(), 15);
            txt->color.r = 0.0;
            txt->color.g = 0.0;
            txt->color.b = 0.0;
            txt->x = axis.px_coor[i];
            txt->y = height / 2 + 10;
            if(axis.coor[i]!=0) S2D_DrawText(txt);
            S2D_DrawLine(axis.px_coor[i], height / 2 + axis.lineWidth + 5, axis.px_coor[i], height / 2 - axis.lineWidth - 5,
                axis.lineWidth / 2,
                0, 0, 0, 1,
                0, 0, 0, 1,
                0, 0, 0, 1,
                0, 0, 0, 1);
        }
    }
    else
    {
        S2D_DrawLine(width/2, axis.axis_offset, width/2, height - axis.axis_offset,
            axis.lineWidth,
            0, 0, 0, 1,
            0, 0, 0, 1,
            0, 0, 0, 1,
            0, 0, 0, 1);
        for (int i = 0; i < axis.coor.size(); i++)
        {
            S2D_Text* txt = S2D_CreateText("fonts/times-new-roman.ttf", std::to_string(axis.coor[i]).c_str(), 15);
            txt->color.r = 0.0;
            txt->color.g = 0.0;
            txt->color.b = 0.0;
            txt->x =  width/2 + 10;
            txt->y = height - axis.px_coor[i] - 10;
            if (axis.coor[i] != 0) S2D_DrawText(txt);
            S2D_DrawLine(width / 2 - axis.lineWidth - 5, axis.px_coor[i], width / 2 + axis.lineWidth + 5, axis.px_coor[i],
                axis.lineWidth / 2,
                0, 0, 0, 1,
                0, 0, 0, 1,
                0, 0, 0, 1,
                0, 0, 0, 1);
        }
    }
    S2D_Text* txt = S2D_CreateText("fonts/times-new-roman.ttf", axis.name.c_str(), 20);
    txt->color.r = 0.0;
    txt->color.g = 0.0;
    txt->color.b = 0.0;
    if(axis.rot==horizontal)
    {
        
    }
    else
    {
        
    }
    //S2D_DrawText(txt);
}

void render()
{

}

void update()
{
    Axis ax1(10,"Y", vertical, -100, 1000);
    drawAxis(ax1);
    Axis ax2(5,"time");
    drawAxis(ax2);
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

