#ifndef FT_PING_H
# define FT_PING_H

# include "parser.h"
# include <stdint.h>
# include <stddef.h>
# include <limits.h>
# include <netinet/ip_icmp.h>

# define ICMP_DEFAULT_PACKET_SIZE	64
# define ICMP_MAX_PACKET_SIZE		65400

/*
**  0                   1                   2                   3
**     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
**    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
**    |     Type      |     Code      |          Checksum             |
**    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
**    |           Identifier          |        Sequence Number        |
**    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
**    |     Data ...
*/

/*
** struct icmphdr
** {
**   u_int8_t type;                // message type
**   u_int8_t code;                // type sub-code
**   u_int16_t checksum;
**   union
**   {
**     struct
**     {
**       u_int16_t        id;
**       u_int16_t        sequence;
**     } echo;                        // echo datagram
**   } un;
** };
*/
typedef enum	s_status
{
	NO_STATUS,
	SEND,
	INTERRUPT,
}				e_status;

typedef struct	s_icmp_send
{
	int				len;
	struct addrinfo	*addr;
	struct timeval	tv;
	char			ip[INET_ADDRSTRLEN];
}				t_icmp_send;

typedef struct	s_icmp_recv
{
	char			host[HOST_NAME_MAX];
	char			ip[INET_ADDRSTRLEN];
	size_t			triptime;
	int				sequence;
	int				len;
	int				type;

}				t_icmp_recv;

extern e_status	g_status;

void	parse_args(t_args *args, int ac, char **av);
void	print_args(t_args args);
void	init_send(t_icmp_send *send, t_args *args);
void	init_recv(int sfd);

void	handle_send(int sfd, t_icmp_send *send);
void	handle_recv(int sfd, t_icmp_send *send);

void	icmp_echo_request_message(char *msg, size_t len);

struct addrinfo	*host_to_addrinfo(char const *hostname);
int				create_raw_socket(void);
void			cleanup(struct addrinfo *addr, int sfd);

#endif /* FT_PING_H */
