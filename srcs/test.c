/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@kiborroq.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/11 10:59:31 by kiborroq          #+#    #+#             */
/*   Updated: 2020/12/29 16:23:03 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define OK 1
#define KO -1

#define WIDTH 1820
#define HEIGHT 1000
#define MAP_HEIGHT 24
#define MAP_WIDTH 24
#define textr_HEIGHT 64
#define textr_WIDTH 64

#define WALL 1
#define SPRITE 2

#define COLORLESS -16777216

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

typedef struct  s_spr
{
    t_point_d   abs_pos;
    double      dist;
	t_point_d	transform;
	int			screen_x;
	t_point_i	start;
	t_point_i	end;
	int			width;
	int			height;
}               t_spr;

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
    t_point_d  		dir;
    t_point_i       pos;
    t_point_d       cur_dist;
    t_point_d       delt_dist;
    t_point_i       step;
    double          dist;
    int             hit_side;
    int             wall_start;
    int             wall_end;
    int             wall_height;
}                   t_ray;

typedef struct			s_image
{
	int					sl;
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
    t_image spr;
}               t_textr;

typedef struct  s_inf
{
    t_view          view;
    t_ray           ray;
    t_turn_sin_cos  turn;
    void            *mlx_ptr;
    void            *win_ptr;
    t_image         img;
    int             **map;
    t_event         event;
    t_textr         textr;
    t_image         test;
    t_color         color;
    t_image         curr_textr;
    t_spr           *sprs;
    int             num_sprs;
	double			*dist_buf;
}                   t_inf;

void    put_color(t_image *img, int x, int y, int color)
{
    img->data[y * img->sl / 4 + x] = color;
}

int     get_color(t_image *img, int x, int y)
{
    return (img->data[y * img->sl / 4 + x]);
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

void reset_sprs(t_spr *sprs, int *num_actual_sprs)
{
	int i;

	i = 0;
	while (i < *num_actual_sprs)
	{
		sprs[i].dist = -1;	
		i++;
	}
	*num_actual_sprs = 0;
}

void count_dist_to_spr(t_inf *inf)
{
	int	spr_x;
	int	spr_y;
	int view_x;
	int view_y;
	int	i;
	int	delta_x;
	int delta_y;


	view_x = inf->view.pos.x;
	view_y = inf->view.pos.y;
	i = 0;
	while (i < inf->num_sprs)
	{
		spr_x = inf->sprs[i].abs_pos.x;
		spr_y = inf->sprs[i].abs_pos.y;
		delta_x = abs(spr_x - view_x);
		delta_y = abs(spr_y - view_y);
		inf->sprs[i].dist = sqrt(delta_y * delta_y + delta_x * delta_x);
		i++;
	}
}

void calculate_dist_to_wall(t_inf *inf, int screen_x)
{
    while (1)
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
        // if (inf->map[inf->ray.pos.x][inf->ray.pos.y] == SPRITE)
        //     count_dist_to_spr(inf);
        if (inf->map[inf->ray.pos.x][inf->ray.pos.y] == WALL)
            break;
    }
    if (inf->ray.hit_side == 0)
        inf->ray.dist = (inf->ray.pos.x - inf->view.pos.x +
                        (1 - inf->ray.step.x) / 2) / inf->ray.dir.x;
    else
	{
        inf->ray.dist = (inf->ray.pos.y - inf->view.pos.y +
                        (1 - inf->ray.step.y) / 2) / inf->ray.dir.y;
	}
	inf->dist_buf[screen_x] = inf->ray.dist;
}

void calculate_wall_range(t_inf *inf)
{
    int height;
    int start;
    int end;

    height = (int)(HEIGHT / inf->ray.dist);
    if ((start = -height / 2 + HEIGHT / 2) < 0)
        start = 0;
    if ((end = height / 2 + HEIGHT / 2) >= HEIGHT)
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

void swap_sprs(t_spr *a, t_spr *b)
{
	t_spr tmp;

	tmp.dist = a->dist;
	tmp.abs_pos.x = a->abs_pos.x;
	tmp.abs_pos.y = a->abs_pos.y;
	a->dist = b->dist;
	a->abs_pos.x = b->abs_pos.x;
	a->abs_pos.y = b->abs_pos.y;
	b->dist = tmp.dist;
	b->abs_pos.x = tmp.abs_pos.x;
	b->abs_pos.y = tmp.abs_pos.y;
}

void sort_sprs(t_spr *sprs, int num_sprs)
{
	int i;
	int j;


	i = 0;
	while (i + 1 < num_sprs)
	{
		j = 0;
		while (j + 1 < num_sprs - j)
		{
			if (sprs[i].dist < sprs[i + 1].dist)
				swap_sprs(&sprs[i], &sprs[i + 1]);
			j++;
		}
		i++;
	}
}

void transform_spr(t_spr *spr, t_inf *inf)
{
	t_point_d rel_pos;
	double inv_det;
	
	rel_pos.x = spr->abs_pos.x - inf->view.pos.x;
	rel_pos.y = spr->abs_pos.y - inf->view.pos.y;
	inv_det = 1.0 / (inf->view.cam_plane.x * inf->view.dir.y -
					inf->view.cam_plane.y * inf->view.dir.x);
	spr->transform.x = inv_det * (inf->view.dir.y * rel_pos.x -
								inf->view.dir.x * rel_pos.y);
	spr->transform.y = inv_det * (-inf->view.cam_plane.y * rel_pos.x +
								inf->view.cam_plane.x * rel_pos.y);
	spr->screen_x = (int)((WIDTH / 2) *
					(1 + spr->transform.x / spr->transform.y));
}

void calculate_spr_range(t_spr *spr)
{
	spr->height = abs((int)(HEIGHT / spr->transform.y));
	if ((spr->start.y = -spr->height / 2 + HEIGHT / 2) < 0)
		spr->start.y = 0;
	if ((spr->end.y = spr->height / 2 + HEIGHT / 2) >= HEIGHT)
		spr->end.y = HEIGHT - 1;
	spr->width = spr->height;
	if ((spr->start.x = -spr->width / 2 + spr->screen_x) < 0)
		spr->start.x = 0;
	if ((spr->end.x = spr->width / 2 + spr->screen_x) >= WIDTH)
		spr->end.x = WIDTH - 1;	
}

void draw_curr_spr(t_spr *spr, t_image *spr_img, t_inf *inf)
{
	int x;
	int y;
	double textr_step;
	int textr_x;
	int textr_y;
	int color;

	x = spr->start.x;
    textr_step = 1.0 * spr_img->height / spr->height;
	while (x < spr->end.x)
	{
		textr_x = (int)(spr_img->sl * (x - (-spr->width / 2 + spr->screen_x)) *
						spr_img->width / spr->width) / spr_img->sl;
		y = spr->start.y;
        if (spr->transform.y > 0 && x > 0 && x < WIDTH && spr->transform.y < inf->dist_buf[x])
        {
            while (y < spr->end.y)
		    {
		    	textr_y = (y - HEIGHT / 2 + spr->height / 2) * textr_step;
		    	color = get_color(spr_img, textr_x, textr_y);
		    	if (color != COLORLESS)
		    		put_color(&inf->img, x, y, color);
		    	y++;
		    }
        }
		x++;
	}
}

void print_sprs(t_spr *sprs, int num)
{
	int i;
	
	i = 0;
	while (i < num)
	{
		ft_putnbr_fd((sprs + i)->abs_pos.x, 1);
		ft_putstr_fd(" ", 1);
		ft_putnbr_fd(sprs[i].abs_pos.y, 1);
		ft_putstr_fd(" ", 1);
		ft_putnbr_fd((int)sprs[i].dist, 1);
		ft_putstr_fd("\n", 1);
		i++;
	}
	ft_putstr_fd("\n\n", 1);
}

void draw_sprs(t_inf *inf)
{
	int i;

	i = 0;
	count_dist_to_spr(inf);
	sort_sprs(inf->sprs, inf->num_sprs);
	while (i < inf->num_sprs)
	{
		transform_spr(inf->sprs + i, inf);
		calculate_spr_range(inf->sprs + i);
		draw_curr_spr(inf->sprs + i, &inf->textr.spr, inf);
		i++;
	}
}

int raycast(t_inf *inf)
{
    int screen_x;

    screen_x = 0;
    while (screen_x < WIDTH)
    {
        init_ray(inf, screen_x);
        calculate_step(inf);
        calculate_dist_to_wall(inf, screen_x);
        calculate_wall_range(inf);
        draw_stripe(inf, screen_x);
        screen_x++;
    }
	draw_sprs(inf);
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

void free_array(void **array, int height)
{
	int i;

	i = 0;
	while (i < height)
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void destroy_mlx_elems(t_inf *inf)
{
    if (inf->textr.e_side.img_ptr)
        mlx_destroy_image(inf->mlx_ptr, inf->textr.e_side.img_ptr);
    if (inf->textr.w_side.img_ptr)
        mlx_destroy_image(inf->mlx_ptr, inf->textr.w_side.img_ptr);
    if (inf->textr.s_side.img_ptr)
        mlx_destroy_image(inf->mlx_ptr, inf->textr.s_side.img_ptr);
    if (inf->textr.n_side.img_ptr)
        mlx_destroy_image(inf->mlx_ptr, inf->textr.n_side.img_ptr);
    if (inf->textr.spr.img_ptr)
        mlx_destroy_image(inf->mlx_ptr, inf->textr.spr.img_ptr);
    if (inf->img.img_ptr)
        mlx_destroy_image(inf->mlx_ptr, inf->img.img_ptr);
    if (inf->win_ptr)
        mlx_destroy_window(inf->mlx_ptr, inf->win_ptr);
    if (inf->mlx_ptr)
        free(inf->mlx_ptr);
}

int close_game(t_inf *inf)
{
    destroy_mlx_elems(inf);
    free_array((void **)inf->map, MAP_WIDTH);
    free(inf->sprs);
    free(inf->dist_buf);
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

int num_digits(int **array, int array_height, int array_width, int dig)
{
    int i;
    int j;
    int num;

    i = 0;
    num = 0;
    while (i < array_height)
    {
		j = 0;
        while (j < array_width)
        {
            if (array[i][j] == dig)
                num++;
            j++;
        }
        i++;
    }
    return (num);
}

int init_sprs(t_inf *inf)
{
    int         x;
    int         y;
	int			i;

    inf->num_sprs = num_digits(inf->map, MAP_HEIGHT, MAP_WIDTH, SPRITE);
    if (!(inf->sprs = (t_spr *)malloc(inf->num_sprs * sizeof(t_spr))))
		return (KO);
	x = 0;
	i = 0;
	while (x < MAP_HEIGHT && i < inf->num_sprs)
	{
		y = 0;
		while (y < MAP_WIDTH && i < inf->num_sprs)
        {
            if (inf->map[x][y] == SPRITE)
			{
				inf->sprs[i].abs_pos.x = x + .5;
				inf->sprs[i].abs_pos.y = y + .5;
				inf->sprs[i].dist = .0;
				i++;
			}
            y++;
        }
		x++;
	}
	return (OK);
}

int add_textr(void *mlx_ptr, t_image *textr, char *filename)
{
    if (!(textr->img_ptr = mlx_xpm_file_to_image(mlx_ptr, filename, &textr->width, &textr->height)))
    {
        return (KO);
    }
    textr->data = (int *)mlx_get_data_addr(textr->img_ptr,
                                            &textr->bpp,
                                            &textr->sl,
                                            &textr->endian);
    return (OK);
}

// void save_screen(t_inf *inf)
// {
    
// }

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
    if (add_textr(inf->mlx_ptr, &inf->textr.spr, "../txtrs/barrel.xpm") == KO)
        close_game(inf);

    inf->img.img_ptr = mlx_new_image(inf->mlx_ptr, WIDTH, HEIGHT);
    inf->img.data = (int *)mlx_get_data_addr(inf->img.img_ptr,
                                        &inf->img.bpp,
                                        &inf->img.sl,
                                        &inf->img.endian);
    
    inf->color.ceil = 11656444;
    inf->color.floor = 8421504;

	inf->dist_buf = (double *)ft_calloc(WIDTH, sizeof(double));

    int	map[MAP_HEIGHT][MAP_WIDTH] =
    {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,2,2,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,2,2,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,0,1,1,1},
        {1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1},
        {1,1,1,1,0,1,1,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,0,1},
        {1,1,0,0,0,0,0,0,1,1,0,1,0,1,0,1,1,1,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,1,0,1},
        {1,1,0,0,0,0,0,0,1,1,0,1,0,1,0,1,1,1,1,1,0,1,1,1},
        {1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,2,2,2,0,2,2,1},
        {1,1,1,1,0,1,1,1,1,1,1,1,0,0,1,0,1,2,0,0,0,0,0,1},
        {1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1,2,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,1,0,0,0,1,1},
        {1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,1,0,1,0,1},
        {1,1,0,0,0,0,0,1,1,1,0,0,0,1,1,0,1,0,1,0,0,0,1,1},
        {1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0,1,0,1,0,1,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0,1,0,1,0,1,0,1},
        {1,1,0,0,0,0,0,1,1,1,0,0,0,1,1,0,1,0,1,0,0,0,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };

    inf->map = fill_map(map);
    init_sprs(inf);
    
    t_view view;
    view.dir.x = 0.0;
    view.dir.y = -1.0;
    view.pos.x = 6.5;
    view.pos.y = 1.5;
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

// gcc -Werror -Wextra -Wall test.c -L ../minilibx/ -lmlx -L ../libft/ -lft -lm -lX11 -lbsd -lXext