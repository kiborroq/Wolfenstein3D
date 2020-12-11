/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/07 12:39:08 by kiborroq          #+#    #+#             */
/*   Updated: 2020/12/11 10:59:11 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <unistd.h>
#include <stdlib.h>

#define WIDTH 1920
#define HEIGHT 1080 

int main(int argc, char *argv[])
{
    int width[1];
    int height[1];

    width[0] = 64;
    height[0] = 64;
    void *new_mlx = mlx_init();
    void *new_win = mlx_new_window(new_mlx, WIDTH, HEIGHT, "Test");
    void *new_img = mlx_xpm_file_to_image(new_mlx, argv[1], width, height);
    if (!new_img)
    {
        write(1, "ERROR\n", 6);
        exit(0);
    }
    int x;
    int y = 0;

    while (y < HEIGHT + 63)
    {
        x = 0;
        while (x < WIDTH + 63)
        {
            mlx_put_image_to_window(new_mlx, new_win, new_img, x, y);
            x += 64;
        }
        y += 64;
    }
    mlx_loop(new_mlx);
    free(new_win);
    free(new_mlx);
    mlx_destroy_image(new_mlx, new_img);
    mlx_clear_window(new_mlx, new_win);
    mlx_destroy_window(new_mlx, new_win);
    return 0;
}
