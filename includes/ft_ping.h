#ifndef FT_PING_H
# define FT_PING_H

# include "parser.h"
# include <stdint.h>
# include <stddef.h>
# include <limits.h>
# include <netinet/ip_icmp.h>

# define ICMP_DEFAULT_PACKET_SIZE	64
# define ICMP_MAX_PACKET_SIZE		65400

# define echo_id		un.echo.id
# define echo_sequence	un.echo.sequence

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

typedef struct	s_packet
{
	struct iphdr	iphdr;
	struct icmphdr	icmphdr;
	union
	{
		char			data[ICMP_MAX_PACKET_SIZE];
		struct
		{
			struct iphdr	iphdr;
			struct icmphdr	icmphdr;
		} error;
	} un;
# define req_icmphdr	un.error.icmphdr
# define icmpdata		un.data
}				t_packet;

typedef struct	s_stat
{
	size_t	recieved;
	double	sum;
	double	sum_sq;
	double	min;
	double	max;
}				t_stat;

typedef struct	s_ping
{
	struct timeval	tv_request;
	struct addrinfo	*addr;
	t_stat			stats;
	char			*req_host;
	char			req_ip[INET_ADDRSTRLEN];
	int				len;
	size_t			num_xmit;
	int				ident;
}				t_ping;

typedef struct	s_reply_data
{
	int				type;
	int				len;
	char			ip[INET_ADDRSTRLEN];
	char			host[HOST_NAME_MAX];
	struct timeval	tv_reply;
	double			triptime;
	int				sequence;
	int				ttl;
}				t_reply_data;

extern e_status	g_status;

void	parse_args(t_args *args, int ac, char **av);
void	print_args(t_args args);
void	init(t_ping *ping, t_args *args);

void	handle_request(int sfd, t_ping *ping);
void	handle_reply(int sfd, t_ping *ping);

void	icmp_set_icmphdr(char *msg, int ident, int seqno);
void	icmp_add_timestamp(char *msg);
void	icmp_set_data(char *msg, size_t total_len);
void	icmp_add_checksum(char *msg, size_t len);

struct addrinfo	*host_to_addrinfo(char const *hostname);
int				create_raw_socket(void);
void			cleanup(struct addrinfo *addr, int sfd);

#endif /* FT_PING_H */
