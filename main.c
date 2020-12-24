/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq@student.42.fr <kiborroq>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/07 12:39:08 by kiborroq          #+#    #+#             */
/*   Updated: 2020/12/23 23:02:54 by kiborroq@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minilibx/mlx.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct  s_data {
    void        *img;
    char        *addr;
    int         bits_per_pixel;
    int         line_length;
    int         endian;
}               t_data;

void            my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
    char    *dst;

    dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
    *(unsigned int*)dst = color;
}

int             main(void)
{
    void    *mlx;
    void    *mlx_win;
    t_data  img;

    mlx = mlx_init();
    mlx_win = mlx_new_window(mlx, 1920, 1080, "Hello world!");
    img.img = mlx_new_image(mlx, 1920, 1080);
    img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
                                 &img.endian);
    for (int i = 200; i < 1720; i++) {
        for (int j = 200; j < 880; j++) {
            my_mlx_pixel_put(&img, i, j, 0x00FF0000);
        }
    }
    mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);
    mlx_loop(mlx);
}

// #define WIDTH 1920
// #define HEIGHT 1080 

// int main(int argc, char *argv[])
// {
//     int width[1];
//     int height[1];

//     width[0] = 64;
//     height[0] = 64;
//     void *new_mlx = mlx_init();
//     void *new_win = mlx_new_window(new_mlx, WIDTH, HEIGHT, "Test");
//     void *new_img = mlx_xpm_file_to_image(new_mlx, argv[1], width, height);
//     if (!new_img)
//     {
//         write(1, "ERROR\n", 6);
//         exit(0);
//     }
//     int x;
//     int y = 0;



//     while (y < HEIGHT + 63)
//     {
//         x = 0;
//         while (x < WIDTH + 63)
//         {
//             mlx_put_image_to_window(new_mlx, new_win, new_img, x, y);
//             x += 64;
//         }
//         y += 64;
//     }

//     printf("%s\n", mlx_get_data_addr(new_img, 0, 0, 0));

//     mlx_loop(new_mlx);
//     mlx_destroy_image(new_mlx, new_img);
//     mlx_clear_window(new_mlx, new_win);
//     mlx_destroy_window(new_mlx, new_win);
//     return 0;
// }


// gcc -Werror -Wextra -Wall main.c -L minilibx/ -lmlx -lm -lbsd -lX11 -lXext