/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/11 10:59:31 by kiborroq          #+#    #+#             */
/*   Updated: 2020/12/24 11:23:52 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define WIDTH 1800
#define HEIGHT 1000
#define MAP_HEIGHT 10
#define MAP_WIDTH 11

#define EXIT 65307
#define MOVE_FORWARD 119
#define MOVE_BACKWARD 115
#define MOVE_LEFT 97
#define MOVE_RIGHT 100
#define TURN_LEFT 65361
#define TURN_RIGHT 65363 

#define MOVE_SPEED 0.05
#define TURN_SPEED 0.05

#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "math.h"
#include "minilibx/mlx.h"

typedef struct  s_point_double
{
    double x;
    double y;
}               t_point_double;

typedef struct  s_point_int
{
    int x;
    int y;
}               t_point_int;

typedef struct      s_view
{
    t_point_double  pos;
    t_point_double  dir;
    t_point_double  cam_plane;
}                   t_view;

typedef struct s_turn_sin_cos
{
    double sin_pozotive;
    double sin_negative;
    double cos_pozitive;
    double cos_negative;
}               t_turn_sin_cos;

typedef struct s_event
{
    int move_forawrd;
    int move_backword;
    int move_left;
    int move_right;
    int turn_left;
    int turn_right;
}               t_event;


typedef struct      s_ray
{
    t_point_double  dir;
    t_point_int     pos;
    t_point_double  cur_dist;
    t_point_double  delt_dist;
    double          dist;
}                   t_ray;

typedef struct			s_image
{
	int					size_line;
	int					bpp;
	int					endian;
	void				*img_ptr;
	char				*data;
}						t_image;

typedef struct  s_inf
{
    t_view          view;
    t_ray           ray;
    t_turn_sin_cos  turn;
    void            *mlx_ptr;
    void            *win_ptr;
    t_image         img;
    t_image         img_buf;
    int             **map;
    t_event          event;
}                   t_inf;

void    my_mlx_pixel_put(t_image *img, int x, int y, int color)
{
    char    *dst;

    dst = img->data + (y * img->size_line + x * (img->bpp / 8));
    *(unsigned int*)dst = color;
}

int    raycast(t_inf *inf)
{
    int x_on_camera_plane = 0;
    double pos_on_plane = 0;
    while (x_on_camera_plane < WIDTH)
    {
        //set current ray (direction, position, ratio)
        pos_on_plane = 2 * x_on_camera_plane / (double)WIDTH - 1;
        inf->ray.dir.x = inf->view.dir.x + pos_on_plane * inf->view.cam_plane.x;
        inf->ray.dir.y = inf->view.dir.y + pos_on_plane * inf->view.cam_plane.y;
        inf->ray.pos.x = (int)inf->view.pos.x;
        inf->ray.pos.y = (int)inf->view.pos.y;

        if (inf->ray.dir.y != 0)
            inf->ray.delt_dist.x = inf->ray.dir.x == 0 ? 1 : fabs(1 / inf->ray.dir.x);
        else
            inf->ray.delt_dist.x = 0;
        if (inf->ray.dir.x != 0)
            inf->ray.delt_dist.y = inf->ray.dir.y == 0 ? 1 : fabs(1 / inf->ray.dir.y);
        else
            inf->ray.delt_dist.y = 0;
    
        //Set step for field iterator, initial distance and delta distance for x and y
        t_point_int step;
        if (inf->ray.dir.x < 0)
        {
            step.x = -1;
            inf->ray.cur_dist.x = (inf->view.pos.x - inf->ray.pos.x) * inf->ray.delt_dist.x;
        }
        else
        {
            step.x = 1;
            inf->ray.cur_dist.x = (inf->ray.pos.x + 1.0 - inf->view.pos.x) * inf->ray.delt_dist.x;
        }
        if (inf->ray.dir.y < 0)
        {
            step.y = -1;
            inf->ray.cur_dist.y = (inf->view.pos.y - inf->ray.pos.y) *  inf->ray.delt_dist.y;
        }
        else
        {
            step.y = 1;
            inf->ray.cur_dist.y = (inf->ray.pos.y + 1.0 - inf->view.pos.y) *  inf->ray.delt_dist.y;
        }
    
        //Ray casting for current ray
        int hit = 0;
        int hit_side;
        while (hit == 0)
        {
            if (inf->ray.cur_dist.x < inf->ray.cur_dist.y)
            {
                inf->ray.cur_dist.x += inf->ray.delt_dist.x;
                inf->ray.pos.x += step.x;
                hit_side = 1;
            }
            else
            {
                inf->ray.cur_dist.y += inf->ray.delt_dist.y;
                inf->ray.pos.y += step.y;
                hit_side = 0;
            }
            if (inf->map[inf->ray.pos.x][inf->ray.pos.y])
                hit = 1;
        }

        //Calculate shortly distance to wall sprite for currnt ray
        if (hit_side)
            inf->ray.dist = (inf->ray.pos.x - inf->view.pos.x + (1 - step.x) / 2) / inf->ray.dir.x;
        else
            inf->ray.dist = (inf->ray.pos.y - inf->view.pos.y + (1 - step.y) / 2) / inf->ray.dir.y;
        
        //Calculate height of wall stripe for current ray
        int height;
        height = (int)(HEIGHT / inf->ray.dist);
        
        //Define pixels of window for color filling 
        int draw_start;
        int draw_end;
        if ((draw_start = -height / 2 + HEIGHT / 2) < 0)
            draw_start = 0;
        if ((draw_end = height / 2 + HEIGHT / 2) > HEIGHT)
            draw_end = HEIGHT - 1;

        // printf("x=%d\n", x_on_camera_plane);
        // printf("dist=%.3f\n", ray.dist);
        // printf("height=%d\n", height);
        // printf("start=%d\n", draw_start);
        // printf("end  =%d\n\n", draw_end);

        //Draw in image
        //draw floor
        unsigned int color = 0x0059CAD5;
        int ceil = 0;
        while (ceil <= draw_start)
        {
            my_mlx_pixel_put(&inf->img, x_on_camera_plane, ceil, color);
            ceil++;
        }
        //draw wall
        color = 0x00FF0000;
        while (draw_start <= draw_end)
        {
            if (!hit_side)
                color = 0x00AA0000;
            my_mlx_pixel_put(&inf->img, x_on_camera_plane, draw_start, color);
            draw_start++;
        }
        //draw floor
        color = 0x55555555;
        while (draw_start <= HEIGHT)
        {
            my_mlx_pixel_put(&inf->img, x_on_camera_plane, draw_start, color);
            draw_start++;
        }
        
        //iter to next ray or to next camera plane point
        x_on_camera_plane++;
    }
    return (0);
}

int **fill_map(int map[MAP_HEIGHT][MAP_WIDTH])
{
    int **new_map;

    new_map = (int **)malloc(sizeof(int *) * MAP_HEIGHT);
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        new_map[i] = (int *)malloc(sizeof(int) * MAP_WIDTH);
        for (int j = 0; j < MAP_WIDTH; j++)
            new_map[i][j] = map[i][j];        
    }
    return (new_map);
}

void clear_map(int **map, int height)
{
    int i;

    i = 0;
    while (i < height)
    {
        free(map[i]);
        i++;
    }
    free(map);
}

void move_for_back(t_inf *inf, double dir)
{
    double delta;

    delta = inf->view.dir.x * MOVE_SPEED * dir;
    if (inf->map[(int)(inf->view.pos.x + delta * 2)][(int)inf->view.pos.y] == 0)
    {
        inf->view.pos.x += delta;
    }
    delta = inf->view.dir.y * MOVE_SPEED * dir;
    if (inf->map[(int)inf->view.pos.x][(int)(inf->view.pos.y + delta * 2)] == 0)
    {
        inf->view.pos.y += delta;
    }
}

void move_left_right(t_inf *inf, double dir)
{
    double delta;

    delta = inf->view.dir.y * MOVE_SPEED * dir;
    if (inf->map[(int)(inf->view.pos.x - delta * 2)][(int)inf->view.pos.y] == 0)
    {
        inf->view.pos.x -= delta;
    }
    delta = inf->view.dir.x * MOVE_SPEED * dir;
    if (inf->map[(int)inf->view.pos.x][(int)(inf->view.pos.y + delta * 2)] == 0)
    {
        inf->view.pos.y += delta;
    }
}

void turn_left_right(t_inf *inf, double turn_sin, double turn_cos)
{
    double old_x;

    old_x = inf->view.dir.x;
    inf->view.dir.x =       inf->view.dir.x * turn_cos -
                            inf->view.dir.y * turn_sin;
    inf->view.dir.y =       inf->view.dir.y * turn_cos +
                            old_x * turn_sin;
    old_x = inf->view.cam_plane.x;
    inf->view.cam_plane.x = inf->view.cam_plane.x * turn_cos -
                            inf->view.cam_plane.y * turn_sin;
    inf->view.cam_plane.y = inf->view.cam_plane.y * turn_cos +
                            old_x * turn_sin;
}

int close_game(t_inf *inf)
{
    clear_map(inf->map, MAP_HEIGHT);
    mlx_destroy_window(inf->mlx_ptr, inf->win_ptr);
    free(inf->mlx_ptr);
    free(inf);
    exit (1);
    return (0);
}

void change_img_ptrs(t_inf *inf)
{
    t_image tmp;

    tmp = inf->img;
    inf->img = inf->img_buf;
    inf->img_buf = tmp;
}

int event_nadling(t_inf *inf)
{
    raycast(inf);
    mlx_put_image_to_window(inf->mlx_ptr, inf->win_ptr, inf->img.img_ptr, 0 , 0);
    mlx_do_sync(inf->mlx_ptr);
    if (inf->event.move_forawrd == 1)
        move_for_back(inf, 1.0);
    if (inf->event.move_backword == 1)
        move_for_back(inf, -1.0);
    if (inf->event.move_left == 1)
        move_left_right(inf, 1.0);
    if (inf->event.move_right == 1)
        move_left_right(inf, -1.0);
    if (inf->event.turn_left == 1)
        turn_left_right(inf, inf->turn.sin_pozotive, inf->turn.cos_pozitive);
    if (inf->event.turn_right == 1)
        turn_left_right(inf, inf->turn.sin_negative, inf->turn.cos_negative);
    change_img_ptrs(inf);
    return (0);
}

int key_release(int event_key, t_inf *inf)
{
    if (event_key == MOVE_FORWARD)
       inf->event.move_forawrd = 0;
    else if (event_key == MOVE_BACKWARD)
       inf->event.move_backword = 0;
    else if (event_key == MOVE_LEFT)
        inf->event.move_left = 0;
    else if (event_key == MOVE_RIGHT)
        inf->event.move_right = 0;
    else if (event_key == TURN_LEFT)
        inf->event.turn_left = 0;
    else if (event_key == TURN_RIGHT)
        inf->event.turn_right = 0;
    return (0);
}

int key_press(int event_key, t_inf *inf)
{
    if (event_key == MOVE_FORWARD)
        inf->event.move_forawrd = 1;
    else if (event_key == MOVE_BACKWARD)
        inf->event.move_backword = 1;
    else if (event_key == MOVE_LEFT)
        inf->event.move_left = 1;
    else if (event_key == MOVE_RIGHT)
        inf->event.move_right = 1;
    else if (event_key == TURN_LEFT)
        inf->event.turn_left = 1;
    else if (event_key == TURN_RIGHT)
        inf->event.turn_right = 1;
    else if (event_key == EXIT)
        close_game(inf);
    // printf("forward=%d backword=%d left=%d right=%d rot_left=%d rot_right=%d\n",
    //         inf->event.move_forawrd, inf->event.move_backword, inf->event.move_left, inf->event.move_right, inf->event.turn_left, inf->event.turn_right);
    // printf("pos_x=%.3f pos_y=%.3f\n\n", inf->view.pos.x, inf->view.pos.y);
    return (0);
}

int main(void)
{
    //Init cub3d
    t_inf *inf;
    if (!(inf = (t_inf *)malloc(sizeof(t_inf))))
        return (0);

    inf->mlx_ptr = mlx_init();
    inf->win_ptr = mlx_new_window(inf->mlx_ptr, WIDTH, HEIGHT, "Test");

    inf->img.img_ptr = mlx_new_image(inf->mlx_ptr, WIDTH, HEIGHT);
    inf->img.data = mlx_get_data_addr( inf->img.img_ptr,
                                        &inf->img.bpp,
                                        &inf->img.size_line,
                                        &inf->img.endian);
                                        
    inf->img_buf.img_ptr = mlx_new_image(inf->mlx_ptr, WIDTH, HEIGHT);
    inf->img_buf.data = mlx_get_data_addr( inf->img_buf.img_ptr,
                                        &inf->img_buf.bpp,
                                        &inf->img_buf.size_line,
                                        &inf->img_buf.endian);

    int	map[MAP_HEIGHT][MAP_WIDTH] =
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };

    inf->map = fill_map(map);

    t_view view;
    view.dir.x = 1;
    view.dir.y = 0.0;
    view.pos.x = 8.9;
    view.pos.y = 8.9;
    view.cam_plane.x = 0;
    view.cam_plane.y = -0.66;

    inf->view = view;

    inf->turn.cos_negative = cos(-TURN_SPEED);
    inf->turn.cos_pozitive = cos(TURN_SPEED);
    inf->turn.sin_negative = sin(-TURN_SPEED);
    inf->turn.sin_pozotive = sin(TURN_SPEED);

    inf->event.move_backword = 0;
    inf->event.move_forawrd = 0;
    inf->event.move_left = 0;
    inf->event.move_right = 0;
    inf->event.turn_left = 0;
    inf->event.turn_right = 0;

    //Raycast, draw and events handling
    // raycast(inf);
    // mlx_put_image_to_window(inf->mlx_ptr, inf->win_ptr, inf->img.img_ptr, 0 , 0);
	
    mlx_hook(inf->win_ptr, 17, 1L << 17, close_game, inf);
	mlx_hook(inf->win_ptr, 2, 1L << 0, key_press, inf);
    mlx_loop_hook(inf->mlx_ptr, event_nadling, inf);
	mlx_hook(inf->win_ptr, 3, 1L << 1, key_release, inf);
    mlx_loop(inf->mlx_ptr);

    return 0;
}

// gcc -Werror -Wextra -Wall test.c -L minilibx/ -lmlx -lm -lbsd -lX11 -lXext
