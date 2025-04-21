

#include "../../includes/cub3d.h"

static void	free_all(char **tmp0, char **tmp1, char **tmp2)
{
	free_ptr(tmp0);
	free_ptr(tmp1);
	free_ptr(tmp2);
}

static int	get_color(int *color, char *line)
{
	t_get_color	c;

	ft_bzero (&c, sizeof (t_get_color));
	c.ret = 1;
	c.start = 2;
	c.len = ft_strchr (line + c.start, ',') - line - c.start;
	c.temp[0] = ft_substr (line, c.start, c.len);
	*color = ft_atoi (c.temp[0]) << 16;
	c.start += c.len + 1;
	c.len = ft_strchr (line + c.start, ',') - line - c.start;
	c.temp[1] = ft_substr (line, c.start, c.len);
	*color |= ((unsigned char) ft_atoi (c.temp[1])) << 8;
	c.start += c.len + 1;
	c.len = ft_strchr (line + c.start, '\0') - line - c.start;
	c.temp[2] = ft_substr (line, c.start, c.len - 1);
	*color |= ((unsigned char) ft_atoi (c.temp[2]));
	check_nbr(&c, &c.ret);
	free_all(&c.temp[0], &c.temp[1], &c.temp[2]);
	return (c.ret);
}

static void	check_element( char *line, t_data *data)
{
	if (!ft_strncmp(line, "NO ", 3) && !data->map_counter)
		data->no_counter++;
	else if (!ft_strncmp(line, "SO ", 3) && !data->map_counter)
		data->so_counter++;
	else if (!ft_strncmp(line, "EA ", 3) && !data->map_counter)
		data->ea_counter++;
	else if (!ft_strncmp(line, "WE ", 3) && !data->map_counter)
		data->we_counter++;
	else if (!ft_strncmp(line, "C ", 2) && !data->map_counter)
		data->c_counter++;
	else if (!ft_strncmp(line, "F ", 2) && !data->map_counter)
		data->f_counter++;
	else if (ft_strchr("NWES10", line[0]) || data->map_counter)
		data->map_counter++;
}

static int	set_data(char *line, t_data *data)
{
	static int	s;
	size_t		i;
	int			color;

	i = 0;
	color = 0;
	while (line[i] && line[i] != '\n')
		i++;
	check_element(line, data);
	if (!ft_strncmp(line, "NO ", 3) && !s)
		data->textures.no_texture = ft_strtrim(line, "NO \n\r");
	else if (!ft_strncmp(line, "SO ", 3) && !s)
		data->textures.so_texture = ft_strtrim(line, "SO \n\r");
	else if (!ft_strncmp(line, "EA ", 3) && !s)
		data->textures.ea_texture = ft_strtrim(line, "EA \n\r");
	else if (!ft_strncmp(line, "WE ", 3) && !s)
		data->textures.we_texture = ft_strtrim(line, "WE \n\r");
	else if (!ft_strncmp(line, "C ", 2) && !s)
		color = get_color(&data->colors.celling, line);
	else if (!ft_strncmp(line, "F ", 2) && !s)
		color = get_color(&data->colors.floor, line);
	else if (ft_strchr("NWES10 ", line[0]) || s)
		s = get_map(data, line);
	return (color);
}

void	get_data(t_data *data)
{
	char	*line;
	int		fd;
	int		err;

	fd = open(data->path, O_RDONLY);
	if (fd < 0)
		ft_exit(data, "cannot open this file discriptor", 1);
	line = get_next_line(fd);
	err = 0;
	while (line && err >= 0)
	{
		err += set_data(line, data);
		free_ptr(&line);
		line = get_next_line(fd);
	}
	if (err < 2)
	{
		while (line)
		{
			free_ptr(&line);
			line = get_next_line(fd);
		}
	}
	if (err < 2)
		ft_exit(data, "invalide information", 1);
}
