/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/11 10:59:31 by kiborroq          #+#    #+#             */
/*   Updated: 2020/12/12 13:19:21 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define WIDTH 800
#define HEIGHT 600

#include <math.h>

typedef struct  s_point
{
    double x;
    double y;
}               t_point;

typedef struct  s_view
{
    t_point pos;
    t_point dir;
    t_point cam_plane;
}               t_view;

typedef struct s_ray
{
    t_point dir;
    t_point pos;
    t_point cur_dist;
    t_point delt_dist;
    t_point ratio;
    double dist;
}               t_ray;

int main(void)
{
    int	map[10][10]=
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };

    t_view view;
    view.dir.x = -1;
    view.dir.y = 0;
    view.pos.x = 5;
    view.pos.y = 5;
    view.cam_plane.x = 0;
    view.cam_plane.y = 0.66;

    t_ray ray;
    int x_on_camera_plane;
    x_on_camera_plane = 0;
    double pos_on_plane;
    while (x_on_camera_plane < WIDTH)
    {
        //set current ray (direction, position, ratio)
        pos_on_plane = 2 * d / double(WIDTH) - 1;
        ray.dir.x = view.dir.x * pos_on_plane + view.cam_plane.x;
        ray.dir.y = view.dir.y * pos_on_plane + view.cam_plane.y;
        ray.pos.x = int(view.pos.x);
        ray.pos.y = int(view.pos.y);
        if (ray.dir.y != 0)
            ray.ratio.x = ray.dir.x == 0 ? 1 : 1 / ray.dir.x;
        else
            ray.ratio.x = 0;
        if (ray.dir.x != 0)
            ray.ratio.y = ray.dir.y == 0 ? 1 : 1 / ray.dir.y;
        else
            ray.ratio.y = 0;
    
        //Set step for field iterator, initial distance and delta distance for x and y
        t_point step;
        if (ray.dir.x > 0)
        {
            step.x = 1;
            ray.cur_dist.x = (view.pos.x - ray.pos.x) * ray.ratio.x;
        }
        else
        {
            step.x = -1;
            ray.cur_dist.x = (view.pos.x + 1.0 - ray.pos.x) * ray.ratio.x;
        }
        if (ray.dir.y > 0)
        {
            step.y = 1;
            ray.cur_dist.y = (view.pos.y - ray.pos.y) *  ray.ratio.y;
        }
        else
        {
            step.y = -1;
            ray.cur_dist.y = (view.pos.y + 1.0 - ray.pos.y) *  ray.ratio.y;
        }
    
        //Ray casting for current ray
        int hit;
        int hit_side;
        hit = 0;
        while (hit == 0)
        {
            if (ray.cur_dist.x < ray.cur_dist.y)
            {
                ray.cur_dist.x += ray.delt_dist.x;
                ray.pos.x += step.x;
                hit_side = 1;
            }
            else
            {
                ray.cur_dist.y += ray.delt_dist.y;
                ray.pos.y += step.y;
                hit_side = 0;
            }
            if (map[ray.pos.y][ray.pos.x])
                hit = 1;
        }

        //Calculate shortly distance to wall sprite for currnt ray
        if (hit_side)
            ray.dist = (ray.cur_dist.x - ray.pos.x + (1 - step.x) / 2) * ray.ratio.x;
        else
            ray.dist = (ray.cur_dist.y - ray.pos.y + (1 - step.y) / 2) * ray.ratio.y;
        
        //Calculate height of wall stripe for current ray
        int height;
        height = (int)(HEIGHT / ray.dist);
        
        //Define pixels of window for color filling 
        int draw_start;
        int draw_end;
        if ((draw_start = -height / 2 + HEIGHT / 2) < 0)
            draw_start = 0;
        if ((draw_start = height / 2 + HEIGHT / 2) > HEIGHT - 1)
            draw_start = HEIGHT - 1;
        
        //Draw in window
        while (draw_start < draw_end + 1)
        {
            mlx_pixel_put(mlx_ptr, win_ptr, x_on_camera_plane, draw_start, 0123123123);
            draw_start++;
        }

        //iter to next ray or to next camera plane point
        x_on_camera_plane++;
    }
    return 0;
}
