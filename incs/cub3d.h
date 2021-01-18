/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@kiborroq.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/11 10:59:31 by kiborroq          #+#    #+#             */
/*   Updated: 2021/01/15 13:13:01 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "stdio.h"
# include "stdlib.h"
# include "unistd.h"
# include "math.h"
# include "mlx.h"
# include "../libft/libft.h"
# include "fcntl.h"

# define OK 1
# define KO -1

# define BUFFER_SIZE 100

# define NUM_ELEMS 8
# define SPACES " \n\t\v\r\f"

# define ARG_ERROR "Invalid program arguments"
# define NO_FILE_DIR "No such file or directory"
# define READEING_ERROR "Eror during file reading"
# define NOT_ENOUGH_MEMORY "Not enough memory for allocation"
# define NOT_ENOUGH_CONFIG "Map start but not all config elems are defined"
# define COLOR_ERROR "Error in color elem"
# define TEXTURE_ERROR "Error in texture or sprite elem"
# define RESOLUTION_ERROR "Error in resolution elem"
# define UNKNOWN_ELEM "Unknown config elem"
# define ELEM_REPAET "Config elem repeat"
# define INVALID_MAP "Invalid map"
# define SAVE_ERROR "Error during screenshot save"

# define NAME "cub3D"
# define SAVE_FLAG "--save"
# define EXTENTN ".cub"
# define SCREENSHOT "screenshot.bmp"

# define FIRST_LAST_LINE " 1"
# define OTHER_LINE " 012NSWE"
# define START_POS "NSWE"

# define WALL 1
# define SPR 2
# define SPACE -1

# define TRANSPARENT -16777216

# define EXIT 65307
# define MOVE_FORWARD 119
# define MOVE_BACKWARD 115
# define MOVE_LEFT 97
# define MOVE_RIGHT 100
# define TURN_LEFT 65361
# define TURN_RIGHT 65363

# define MOVE_SPEED_DEL 31000000
# define TURN_SPEED_DEL 38000000

typedef struct		s_color
{
	int				ceil;
	int				floor;
	int				tmp;
}					t_color;

typedef struct		s_header
{
	char			*sign;
	int				file_size;
	int				res_byte;
	int				offset;
}					t_header;

typedef struct		s_info_header
{
	int				size;
	int				width;
	int				height;
	int				planes;
	int				bpp;
	int				compres;
	int				img_size;
	int				hor_resol;
	int				ver_resol;
	int				color_used;
	int				color_important;
}					t_info_header;

typedef struct		s_bitmap
{
	t_header		h;
	t_info_header	i_h;
	int				*data;
}					t_bitmap;

typedef struct		s_point_d
{
	double			x;
	double			y;
}					t_point_d;

typedef struct		s_point_i
{
	int				x;
	int				y;
}					t_point_i;

typedef struct		s_spr
{
	t_point_d		abs_pos;
	double			dist;
	t_point_d		transform;
	int				screen_x;
	t_point_i		start;
	t_point_i		end;
	int				width;
	int				height;
}					t_spr;

typedef struct		s_view
{
	t_point_d		pos;
	t_point_d		dir;
	t_point_d		cam;
}					t_view;

typedef struct		s_turn_sin_cos
{
	double			sin_pozotive;
	double			sin_negative;
	double			cos_pozitive;
	double			cos_negative;
}					t_turn_sin_cos;

typedef struct		s_event
{
	int				move_forawrd;
	int				move_backword;
	int				move_left;
	int				move_right;
	int				turn_left;
	int				turn_right;
}					t_event;

typedef	struct		s_speed
{
	double			move;
	double			turn;
}					t_speed;

typedef struct		s_ray
{
	t_point_d		dir;
	t_point_i		pos;
	t_point_d		cur_dist;
	t_point_d		delt_dist;
	t_point_i		step;
	double			dist;
	int				hit_side;
	int				wall_start;
	int				wall_end;
	int				wall_height;
}					t_ray;

typedef struct		s_image
{
	int				sl;
	int				bpp;
	int				endian;
	void			*img_ptr;
	int				*data;
	int				width;
	int				height;
}					t_image;

typedef struct		s_textr
{
	t_image			n_side;
	t_image			s_side;
	t_image			e_side;
	t_image			w_side;
	t_image			spr;
}					t_textr;

typedef struct		s_inf
{
	t_view			view;
	t_ray			ray;
	t_turn_sin_cos	turn;
	void			*mlx;
	void			*win;
	t_image			img;
	int				**map;
	t_event			event;
	t_speed			speed;
	t_textr			textr;
	t_color			color;
	t_image			curr_textr;
	t_spr			*sprs;
	int				num_sprs;
	double			*dist_buf;
	int				num_line;
	int				start_map;
	char			*message;
	int				width;
	int				height;
	int				max_width;
	int				max_height;
	int				map_height;
	int				map_width;
	char			*line;
	t_list			*maplst;
}					t_inf;

/*
**main.c - program structure
*/

int					main(int argc, char **argv);
int					init_game(char *map_path, t_inf **inf);
int					raycast(t_inf *inf);
int					save_screen(t_inf *inf);
int					close_game(t_inf *inf);

/*
**init.c - funcs for game initialisation
*/

int					init_inf_mlx(t_inf **inf);
int					init_img(t_inf *inf);
int					init_sprs(t_inf *inf);
void				init_speed(t_inf *inf);
void				init_view(t_inf *inf);

/*
**init_utils.c - help funcs for game initialisation
*/

void				set_pos_dir_cam(t_inf *inf, double p_x,
												double p_y, char dir);
t_view				set_dir_cam(double d_x, double d_y,
								double c_x, double c_y);

/*
**parser.c - parser structure
*/

int					config_map_parser(t_inf *inf, char *map_path);
int					map_parser(int fd, t_inf *inf);
int					config_parser(int fd, t_inf *inf);
int					init_textr_rgb_resltn(char *line, t_inf *inf);

/*
**parser_utils.c - help funcs for parsing
*/

int					get_next_line_wrap(int fd, t_inf *inf);
int					skip_empty_lines(int fd, t_inf *inf);

/*
**map_parser.c - funcs for map parsing
*/

int					add_first_mapline(int fd, t_inf *inf);
int					add_remain_maplines(int fd, t_inf *inf);
int					maplst_to_table(t_inf *inf);
int					check_map(t_inf *inf);

/*
**map_parser_utils.c - help funcs for map parsing
*/

int					ismap(char *elem, char *set);
int					add_mapline_wrap(t_inf *inf, char *set);
int					add_mapline(t_list **maplst, char *line);
void				fill_line(int *line, int line_len, char *src);
int					check_mapline(int **map, int i, int width, int *num_pos);

/*
**config_parser.c - funcs for configurations parsing
*/

int					init_textr(t_inf *inf, char *elem);
int					init_rgb(t_inf *inf, char *elem);
int					init_resltn(t_inf *inf, char *elem);
int					check_elem(t_inf *inf, char *elem);

/*
**config_parser_utils.c - help funcs for configurations parsing
*/

int					add_textr_wrap(t_inf *inf, t_image *textr, char *elem);
int					add_textr(void *mlx, t_image *textr, char *path);
int					add_rgb_wrap(t_inf *inf, int *color, char *elem);
int					add_rgb(int *color, char *rgb);
int					add_resltn(t_inf *inf, int *width,
								int *height, char *resltn);

/*
**save.c - funcs for screenshot.bmp create
*/

void				init_bmp(t_bitmap *bmp, t_inf *inf);
void				write_bmp(t_bitmap bmp, t_inf *inf, int fd);
void				write_data(t_bitmap bmp, int sl, int fd);

/*
**hook_handl.c - funcs for hook handling in mlx loop
*/

int					button_press(int event_button, t_inf *inf);
int					button_release(int event_button, t_inf *inf);
int					event_nadling(t_inf *inf);

/*
**hook_handl_utils.c - help funcs for hook handling in mlx loop
*/

void				move_for_back(t_inf *inf, double dir);
void				move_left_right(t_inf *inf, double dir);
void				turn_left_right(t_inf *inf, double turn_sin,
												double turn_cos);

/*
**raycast.c - funcs for only raycasting
*/

void				init_ray(t_inf *inf, int screen_x);
void				calculate_step(t_inf *inf);
void				calculate_dist_to_wall(t_inf *inf, int screen_x);
void				calculate_wall_range(t_inf *inf);
void				draw_stripe(t_inf *inf, int screen_x);

/*
**draw_wall_ceil_floor.c - funcs for ceil, floor, and wall drawing in image
*/

void				draw_color_range(t_inf *inf, int x,
									int start_y, int end_y);
void				draw_wall(t_inf *inf, int screen_x);
void				set_curr_textr(t_inf *inf);
int					get_textr_x(t_inf *inf, double wall_x);
double				get_wall_x(t_inf *inf);

/*
**draw_sprites.c - funcs for sprites drawing in image
*/

void				draw_sprs(t_inf *inf);
void				count_dist_to_spr(t_inf *inf);
void				transform_spr(t_spr *spr, t_inf *inf);
void				calculate_spr_range(t_spr *spr, int width, int height);
void				draw_curr_spr(t_spr *spr, t_image *spr_img, t_inf *inf);

/*
**draw_utils.c - help funcs for drawing all elems in image
*/

void				sort_sprs(t_spr *sprs, int num_sprs);
void				swap_sprs(t_spr *a, t_spr *b);
int					get_color(t_image *img, int x, int y);
void				put_color(t_image *img, int x, int y, int color);

/*
**close.c - funcs for normal game closing
*/

void				destroy_mlx_elems(t_inf *inf);
void				print_error_message(t_inf *inf);

/*
**get_next_line.c - func that get every line in current fd
*/

int					get_next_line(int fd, char **line);

#endif
