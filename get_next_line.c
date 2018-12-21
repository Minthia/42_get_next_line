/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdo-minh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/21 14:32:26 by hdo-minh          #+#    #+#             */
/*   Updated: 2018/12/21 14:32:28 by hdo-minh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static t_list	*ft_get_fd(t_list **new, int fd)
{
	t_list	*list;

	list = *new;
	if (list)
	{
		while (list)
		{
			if ((int)list->content_size == fd)
				return (list);
			list = list->next;
		}
	}
	list = ft_lstnew("\0", fd);
	list->content_size = fd;
	ft_lstadd(new, list);
	return (list);
}

static char		*ft_strnjoin2(const char *s1, const char *s2, size_t n)
{
	char	*join;
	int		diff;

	diff = ft_strlen(s2) - n;
	if (!s1 || !s2 || !(join = ft_strnew(ft_strlen(s1) + ft_strlen(s2) - diff)))
		return (NULL);
	ft_strcpy(join, s1);
	ft_strncat(join, s2, n);
	return (join);
}

static char		*ft_strnjoin_free2(char *content, char *buf, int ret)
{
	char	*tmp;

	tmp = content;
	content = ft_strnjoin2(content, buf, ret);
	free(tmp);
	return (content);
}

static size_t	ft_strclen2(const char *str, int c)
{
	size_t	count;

	count = 0;
	while (str[count] && str[count] != (unsigned char)c)
		count++;
	return (count);
}

static char		*ft_strcdup2(const char *src, int c)
{
	char	*dst;
	int 	i;

	i = 0;
	if (!(dst = ft_memalloc(ft_strclen2(src, c) - 1)))
		return (NULL);
	while (src[i] && src[i] != (unsigned char)c)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

int				get_next_line(int const fd, char **line)
{
	char			buf[BUFF_SIZE + 1];
	static t_list	*new;
	t_list			*list;
	int				ret;
	char 			*tmp;

	if (fd < 0 || !line || read(fd, buf, 0) < 0 || BUFF_SIZE < 0)
		return (-1);
	list = ft_get_fd(&new, fd);
	while (!ft_strchr(list->content, EOL) && (ret = read(fd, buf, BUFF_SIZE)))
		list->content = ft_strnjoin_free2(list->content, buf, ret);
	*line = ft_strcdup2(list->content, (int)EOL);
	tmp = list->content;
	ret = ft_strclen2(list->content, (int)EOL);
	tmp[ret] == EOL ? ret++ : ret;
	list->content = ft_strsub(tmp, (unsigned int)ret, ft_strlen(tmp) - ret);
	ft_strdel(&tmp);
	return (ret ? 1 : 0);
}
