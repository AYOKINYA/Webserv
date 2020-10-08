#ifndef UTILS_HPP
# define UTILS_HPP

# include <sys/wait.h>
# include <sys/time.h>
# include <string.h>
# include <iostream>
# include <fcntl.h>
# include <unistd.h>
# include <sys/select.h>
# include <arpa/inet.h>
# include <string>
# include <map>
# include <algorithm>
# include <vector>
# include <errno.h>
# include <stdio.h>
# include <string.h>
# include <sys/stat.h>
# include <iomanip>
# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/ip.h>
# include <exception>
# include <signal.h>
# include "libft/libft.h"



int     ft_is_space(char c);
int     ft_getline(std::string &b, std::string &line);
std::vector<std::string> split(const std::string& str, char delim);

std::string trim(const std::string& str);
std::string trim_url(const std::string& str);
std::string trim_url_2(const std::string& str);
std::string trim_extension(const std::string& str);
void        ft_free(char **arr);
uint16_t    ft_htons(uint16_t port);
std::string ft_inet_ntoa(unsigned int addr);
long get_time(void);

#ifndef FT_FD_SETSIZE
#define FT_FD_SETSIZE 1024
#endif

#define FT_NBBY 8
typedef uint32_t ft_fd_mask;
#define FT_NFDBITS ((unsigned)(sizeof(ft_fd_mask) * FT_NBBY))
#define HOWMANY(x, y) (((x) + (y) - 1) / (y))

// typedef struct ft_fd_set {
//     ft_fd_mask fds_bits[HOWMANY(FT_FD_SETSIZE, FT_NFDBITS)];
// } ft_fd_set;

/* FD_SET */
static __inline void
__fd_set(int fd, fd_set *p)
{
    p->fds_bits[fd / FT_NFDBITS] |= (1U << (fd % FT_NFDBITS));
}

#define FT_FD_SET(n, p) __fd_set((n), (p))

/* FD_CLR */
static __inline void
__fd_clr(int fd, fd_set *p)
{
	p->fds_bits[fd / FT_NFDBITS] &= ~(1U << (fd % FT_NFDBITS));
}
#define FT_FD_CLR(n, p)	__fd_clr((n), (p))

/* FD_ISSET */
static __inline int
__fd_isset(int fd, const fd_set *p)
{
	return (p->fds_bits[fd / FT_NFDBITS] & (1U << (fd % FT_NFDBITS)));
}
#define FT_FD_ISSET(n, p)	__fd_isset((n), (p))

/* FD_ZERO */
#define	FT_FD_ZERO(p) do {					\
	fd_set *_p = (p);				\
	size_t _n = HOWMANY(FD_SETSIZE, FT_NFDBITS);	\
							\
	while (_n > 0)					\
		_p->fds_bits[--_n] = 0;			\
} while (0)

#define        FT_S_ISTYPE(mode, mask)        (((mode) & S_IFMT) == (mask))
#define        FT_S_ISDIR(mode)         FT_S_ISTYPE((mode), S_IFDIR)


#endif
