/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/11 10:59:31 by kiborroq          #+#    #+#             */
/*   Updated: 2020/12/28 18:40:30 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define OK 1
#define KO -1

#define WIDTH 1820
#define HEIGHT 1000
#define MAP_HEIGHT 10
#define MAP_WIDTH 11
#define textr_HEIGHT 64
#define textr_WIDTH 64

#define EXIT 65307
#define MOVE_FORWARD 119
#define MOVE_BACKWARD 115
#define MOVE_LEFT 97
#define MOVE_RIGHT 100
#define TURN_LEFT 65361
#define TURN_RIGHT 65363 

#define MOVE_SPEED 0.0455
#define TURN_SPEED 0.03276

#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "math.h"
#include "../minilibx/mlx.h"
#include "../libft/libft.h"

typedef struct s_color
{
    int ceil;
    int floor;
}              t_color;


typedef struct  s_point_d
{
    double x;
    double y;
}               t_point_d;

typedef struct  s_point_i
{
    int x;
    int y;
}               t_point_i;

typedef struct  s_sprite
{
    t_point_i   pos;
    double      dist;
}               t_sprite;

typedef struct      s_view
{
    t_point_d  pos;
    t_point_d  dir;
    t_point_d  cam_plane;
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
    t_point_d  dir;
    t_point_i     pos;
    t_point_d  cur_dist;
    t_point_d  delt_dist;
    t_point_i     step;
    double          dist;
    int             hit_side;
    int             wall_start;
    int             wall_end;
    int             wall_height;
}                   t_ray;

typedef struct			s_image
{
	int					size_line;
	int					bpp;
	int					endian;
	void				*img_ptr;
	int	                *data;
    int                 width;
    int                 height;
}						t_image;

typedef struct s_textr
{
    t_image n_side;
    t_image s_side;
    t_image e_side;
    t_image w_side;
    t_image sprite;
}               t_textr;

typedef struct  s_inf
{
    t_view          view;
    t_ray           ray;
    t_turn_sin_cos  turn;
    void            *mlx_ptr;
    void            *win_ptr;
    t_image         img;
    // t_image         img_buf;
    int             **map;
    t_event         event;
    t_textr         textr;
    t_image         test;
    t_color         color;
    t_image         curr_textr;
    
}                   t_inf;

void    put_color(t_image *img, int x, int y, int color)
{
    img->data[y * img->size_line / 4 + x] = color;
}

int     get_color(t_image *img, int x, int y)
{
    return (img->data[y * img->size_line / 4 + x]);
}

void set_curr_textr(t_inf *inf)
{
    int hit_side;
    double ray_dir_x;
    double ray_dir_y;

    hit_side = inf->ray.hit_side;
    ray_dir_x = inf->ray.dir.x;
    ray_dir_y = inf->ray.dir.y;
    if (ray_dir_x <= 0 && ray_dir_y <= 0)
        inf->curr_textr = !hit_side ? inf->textr.n_side : inf->textr.w_side;
    else if (ray_dir_x >= 0 && ray_dir_y >= 0)
        inf->curr_textr = !hit_side ? inf->textr.s_side : inf->textr.e_side;
    else if (ray_dir_x >= 0 && ray_dir_y <= 0)
        inf->curr_textr = !hit_side ? inf->textr.s_side : inf->textr.w_side;       
    else
        inf->curr_textr = !hit_side ? inf->textr.n_side : inf->textr.e_side;
    
}

double get_wall_x(t_inf *inf)
{
    double wall_x;

    if (inf->ray.hit_side == 1)
        wall_x = inf->view.pos.x + inf->ray.dist * inf->ray.dir.x;
    else
        wall_x = inf->view.pos.y + inf->ray.dist * inf->ray.dir.y;
    wall_x -= floor(wall_x);
    return (wall_x);
}

int get_textr_x(t_inf *inf, double wall_x)
{
    int textr_x;

    textr_x = (int)(wall_x * (double)inf->curr_textr.width);
    if (inf->ray.hit_side == 1 && inf->ray.dir.y < 0)
        textr_x = inf->curr_textr.width - textr_x - 1;
    else if (inf->ray.hit_side == 0 && inf->ray.dir.x > 0)
        textr_x = inf->curr_textr.width - textr_x - 1;
    return (textr_x);
}

void draw_wall(t_inf *inf, int screen_x)
{
    double          wall_x;
    int             textr_x;
    double          textr_step;
    double          textr_y_pos;
    int             textr_y;
    int             screen_y;
    int             color;

    set_curr_textr(inf);
    wall_x = get_wall_x(inf);
    textr_x = get_textr_x(inf, wall_x);
    textr_step = 1.0 * inf->curr_textr.height / inf->ray.wall_height;
    textr_y_pos = (inf->ray.wall_start - HEIGHT / 2 + inf->ray.wall_height / 2) * textr_step;
    screen_y = inf->ray.wall_start;
    while (screen_y < inf->ray.wall_end)
    {
        textr_y = (int)textr_y_pos & (inf->curr_textr.height - 1);
        color = get_color(&inf->curr_textr, textr_x, textr_y);
        put_color(&inf->img, screen_x, screen_y, color);
        textr_y_pos += textr_step;
        screen_y++;
    }
}

void init_ray(t_inf *inf, int screen_x)
{
    double pos_on_plane;

    pos_on_plane = 2 * screen_x / (double)WIDTH - 1;
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
}

void calculate_step(t_inf *inf)
{
    if (inf->ray.dir.x < 0)
    {
        inf->ray.step.x = -1;
        inf->ray.cur_dist.x = (inf->view.pos.x - inf->ray.pos.x) *inf->ray.delt_dist.x;
    }
    else
    {
        inf->ray.step.x = 1;
        inf->ray.cur_dist.x = (inf->ray.pos.x + 1.0 - inf->view.pos.x) * inf->ray.delt_dist.x;
    }
    if (inf->ray.dir.y < 0)
    {
        inf->ray.step.y = -1;
        inf->ray.cur_dist.y = (inf->view.pos.y - inf->ray.pos.y) *  inf->ray.delt_dist.y;
    }
    else
    {
        inf->ray.step.y = 1;
        inf->ray.cur_dist.y = (inf->ray.pos.y + 1.0 - inf->view.pos.y) *  inf->ray.delt_dist.y;
    }
}

void calculate_dist_to_wall(t_inf *inf)
{
    int hit;
    
    hit = 0;
    while (hit == 0)
    {
        if (inf->ray.cur_dist.x < inf->ray.cur_dist.y)
        {
            inf->ray.cur_dist.x += inf->ray.delt_dist.x;
            inf->ray.pos.x += inf->ray.step.x;
            inf->ray.hit_side = 0;
        }
        else
        {
            inf->ray.cur_dist.y += inf->ray.delt_dist.y;
            inf->ray.pos.y += inf->ray.step.y;
            inf->ray.hit_side = 1;
        }
        if (inf->map[inf->ray.pos.x][inf->ray.pos.y] == 1)
            hit = 1;
    }
    if (inf->ray.hit_side == 0)
        inf->ray.dist = (inf->ray.pos.x - inf->view.pos.x +(1 - inf->ray.step.x) / 2) / inf->ray.dir.x;
    else
        inf->ray.dist = (inf->ray.pos.y - inf->view.pos.y + (1 - inf->ray.step.y) / 2) / inf->ray.dir.y;
}

void calculate_wall_range(t_inf *inf)
{
    int height;
    int start;
    int end;

    height = (int)(HEIGHT / inf->ray.dist);
    if ((start = -height / 2 + HEIGHT / 2) < 0)
        start = 0;
    if ((end = height / 2 + HEIGHT / 2) > HEIGHT)
        end = HEIGHT - 1;
    inf->ray.wall_height = height;
    inf->ray.wall_start = start;
    inf->ray.wall_end = end;
}

void draw_color_range(t_inf *inf, int x, int start_y, int end_y, unsigned int color)
{
    while (start_y <= end_y)
    {
        put_color(&inf->img, x, start_y, color);
        start_y++;
    }
}

void draw_stripe(t_inf *inf, int screen_x)
{
    draw_color_range(inf, screen_x, 0, inf->ray.wall_start - 1, inf->color.ceil);
    draw_wall(inf, screen_x);
    draw_color_range(inf, screen_x, inf->ray.wall_end, HEIGHT - 1, inf->color.floor);
}

int raycast(t_inf *inf)
{
    int screen_x;

    screen_x = 0;
    while (screen_x < WIDTH)
    {
        init_ray(inf, screen_x);
        calculate_step(inf);
        calculate_dist_to_wall(inf);
        calculate_wall_range(inf);
        draw_stripe(inf, screen_x);
        screen_x++;
    }
    return (1);
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

// void change_img_ptrs(t_inf *inf)
// {
//     t_image tmp;

//     tmp = inf->img;
//     inf->img = inf->img_buf;
//     inf->img_buf = tmp;
// }

int event_nadling(t_inf *inf)
{
    raycast(inf);
    mlx_put_image_to_window(inf->mlx_ptr, inf->win_ptr, inf->img.img_ptr, 0 , 0);
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
    return (0);
}

int button_release(int event_button, t_inf *inf)
{
    if (event_button == MOVE_FORWARD)
       inf->event.move_forawrd = 0;
    else if (event_button == MOVE_BACKWARD)
       inf->event.move_backword = 0;
    else if (event_button == MOVE_LEFT)
        inf->event.move_left = 0;
    else if (event_button == MOVE_RIGHT)
        inf->event.move_right = 0;
    else if (event_button == TURN_LEFT)
        inf->event.turn_left = 0;
    else if (event_button == TURN_RIGHT)
        inf->event.turn_right = 0;
    return (0);
}

int button_press(int event_button, t_inf *inf)
{
    if (event_button == MOVE_FORWARD)
        inf->event.move_forawrd = 1;
    else if (event_button == MOVE_BACKWARD)
        inf->event.move_backword = 1;
    else if (event_button == MOVE_LEFT)
        inf->event.move_left = 1;
    else if (event_button == MOVE_RIGHT)
        inf->event.move_right = 1;
    else if (event_button == TURN_LEFT)
        inf->event.turn_left = 1;
    else if (event_button == TURN_RIGHT)
        inf->event.turn_right = 1;
    else if (event_button == EXIT)
        close_game(inf);
    return (0);
}

int add_textr(void *mlx_ptr, t_image *textr, char *filename)
{
    if (!(textr->img_ptr = mlx_xpm_file_to_image(mlx_ptr, filename, &textr->width, &textr->height)))
    {
        return (KO);
    }
    textr->data = (int *)mlx_get_data_addr(textr->img_ptr,
                                            &textr->bpp,
                                            &textr->size_line,
                                            &textr->endian);
    return (OK);
}

int main(void)
{
    t_inf *inf;
    if (!(inf = (t_inf *)malloc(sizeof(t_inf))))
        return (0);

    inf->mlx_ptr = mlx_init();
    inf->win_ptr = mlx_new_window(inf->mlx_ptr, WIDTH, HEIGHT, "Test");

    if (add_textr(inf->mlx_ptr, &inf->textr.e_side, "../txtrs/east.xpm") == KO)
        close_game(inf);
    if (add_textr(inf->mlx_ptr, &inf->textr.w_side, "../txtrs/west.xpm") == KO)
        close_game(inf);
    if (add_textr(inf->mlx_ptr, &inf->textr.n_side, "../txtrs/nord.xpm") == KO)
        close_game(inf);
    if (add_textr(inf->mlx_ptr, &inf->textr.s_side, "../txtrs/south.xpm") == KO)
        close_game(inf);
    if (add_textr(inf->mlx_ptr, &inf->textr.sprite, "../txtrs/barrel.xpm") == KO)
        close_game(inf);

    inf->img.img_ptr = mlx_new_image(inf->mlx_ptr, WIDTH, HEIGHT);
    inf->img.data = (int *)mlx_get_data_addr(inf->img.img_ptr,
                                        &inf->img.bpp,
                                        &inf->img.size_line,
                                        &inf->img.endian);
    
    inf->color.ceil = 11656444;
    inf->color.floor = 8421504;

    int	map[MAP_HEIGHT][MAP_WIDTH] =
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 2, 2, 2, 2, 2, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };

    inf->map = fill_map(map);

    t_view view;
    view.dir.x = 0.0;
    view.dir.y = -1.0;
    view.pos.x = 8.9;
    view.pos.y = 8.9;
    view.cam_plane.x = -0.66;
    view.cam_plane.y = 0;

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
    
    mlx_hook(inf->win_ptr, 17, 1L << 17, close_game, inf);
	mlx_hook(inf->win_ptr, 2, 1L << 0, button_press, inf);
    mlx_loop_hook(inf->mlx_ptr, event_nadling, inf);
	mlx_hook(inf->win_ptr, 3, 1L << 1, button_release, inf);
    mlx_loop(inf->mlx_ptr);

    return 0;
}
