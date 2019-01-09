#include <stdlib.h>
#include <time.h>
#include "stdio.h"
/*
基于bresenham实现了一个3d体素的射线检测算法，可以快速列出射线穿过的所有体素
特点:
1.主循环逻辑仅使用了加法运行和比较运算
2.全部整数计算
*/
static const int VOXEL_CELL_SIZE = 50;
static const int VOXEL_CELL_HEIGHT = 10;

struct SMD_POS_3D
{
    int px;
    int py;
    int pz;
};
struct SMD_GRID_3D
{
    int gx;
    int gy;
    int gz;
};

inline void pos2grid(SMD_GRID_3D *grid, const SMD_POS_3D *pos)
{
    grid->gx = pos->px / VOXEL_CELL_SIZE;
    grid->gy = pos->py / VOXEL_CELL_SIZE;
    grid->gz = pos->pz / VOXEL_CELL_HEIGHT;
}
inline void grid2pos(SMD_POS_3D *pos, const SMD_GRID_3D *grid)
{
    pos->px = grid->gx * VOXEL_CELL_SIZE;
    pos->py = grid->gy * VOXEL_CELL_SIZE;
    pos->pz = grid->gz * VOXEL_CELL_HEIGHT;
    
}
int bresenham3d_line(const SMD_POS_3D *start_param, const SMD_POS_3D *end_param)
{
    if (!start_param || !end_param) return -1;
    
    SMD_POS_3D start = *start_param;
    SMD_POS_3D end = *end_param;

    if(start.px < 0) start.px -= (VOXEL_CELL_SIZE - 1);
    if(start.py < 0) start.py -= (VOXEL_CELL_SIZE - 1);
    if(start.pz < 0) start.pz -= (VOXEL_CELL_HEIGHT - 1);
    if(end.px < 0) end.px -= (VOXEL_CELL_SIZE - 1);
    if(end.py < 0) end.py -= (VOXEL_CELL_SIZE - 1);
    if(end.pz < 0) end.pz -= (VOXEL_CELL_HEIGHT - 1);


    int x_inc = end.px > start.px ? 1 : -1;
    int y_inc = end.py > start.py ? 1 : -1;
    int z_inc = end.pz > start.pz ? 1 : -1;
    
    SMD_GRID_3D from, to;
    pos2grid(&from, &start);
    pos2grid(&to, &end);

    SMD_POS_3D base;
    grid2pos(&base, &from);
    if (x_inc > 0) base.px += VOXEL_CELL_SIZE;
    if (y_inc > 0) base.py += VOXEL_CELL_SIZE;
    if (z_inc > 0) base.pz += VOXEL_CELL_HEIGHT;

    int delta_x = end.px - start.px;
    int delta_y = end.py - start.py;
    int delta_z = end.pz - start.pz;

    int xy_diff_slope = delta_y * base.px - delta_x * base.py
            + end.px * start.py - end.py * start.px;
    int xy_step_x = delta_y * VOXEL_CELL_SIZE * x_inc;
    int xy_step_y = -delta_x * VOXEL_CELL_SIZE * y_inc;

    if (x_inc != y_inc)
    {
        xy_diff_slope = -xy_diff_slope;
        xy_step_x = -xy_step_x;
        xy_step_y = -xy_step_y;
    }
    int xz_diff_slope = delta_z * base.px - delta_x * base.pz
            + end.px * start.pz - end.pz * start.px;
    int xz_step_x = delta_z * VOXEL_CELL_SIZE * x_inc;
    int xz_step_z = -delta_x * VOXEL_CELL_HEIGHT * z_inc;

    if (x_inc != z_inc)
    {
        xz_diff_slope = -xz_diff_slope;
        xz_step_x = -xz_step_x;
        xz_step_z = -xz_step_z;
    }
    int yz_diff_slope = delta_z * base.py - delta_y * base.pz
            + end.py * start.pz - end.pz * start.py;
    int yz_step_y = delta_z * VOXEL_CELL_SIZE * y_inc;
    int yz_step_z = -delta_y * VOXEL_CELL_HEIGHT * z_inc;

    if (y_inc != z_inc)
    {
        yz_diff_slope = -yz_diff_slope;
        yz_step_z = -yz_step_z;
        yz_step_y = -yz_step_y;
    }
    SMD_GRID_3D sel = from;
    int ret = 0;

    printf("from (%d,%d,%d) --> to (%d,%d,%d)\n", start.px, start.py, start.pz, end.px, end.py, end.pz);
    printf("(%d,%d,%d)\n", sel.gx, sel.gz, sel.gz);
    while ((sel.gx != to.gx) || (sel.gy != to.gy) || (sel.gz != to.gz))
    {
        if (xy_diff_slope > 0 ||(xy_diff_slope == 0 && sel.gy != to.gy))
        {
            if (yz_diff_slope > 0 ||(yz_diff_slope == 0 && sel.gz != to.gz))
            {
                sel.gz += z_inc;
                xz_diff_slope += xz_step_z;
                yz_diff_slope += yz_step_z;
            }
            else
            {
                sel.gy += y_inc;
                xy_diff_slope += xy_step_y;
                yz_diff_slope += yz_step_y;
             }
        }
        else if (xz_diff_slope > 0 || (xz_diff_slope == 0 && sel.gz != to.gz))
        {
            sel.gz += z_inc;
            xz_diff_slope += xz_step_z;
            yz_diff_slope += yz_step_z;
        }
        else
        {
            sel.gx += x_inc;
            xy_diff_slope += xy_step_x;
            xz_diff_slope += xz_step_x;
        }
        printf("(%d,%d,%d)\n", sel.gx, sel.gz, sel.gz);
    }
    return ret;
}
int main()
{

    SMD_POS_3D start={0,0,0};
    SMD_POS_3D end={101,101,101};
    bresenham3d_line(&start, &end);
    return 0;
}
