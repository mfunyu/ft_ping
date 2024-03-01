#ifndef PING_STRUCT_H
# define PING_STRUCT_H

# include <netinet/ip_icmp.h>

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
# define echo_id		un.echo.id
# define echo_sequence	un.echo.sequence

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
	int				sfd;
	struct sockaddr	dst_addr;
	char			*dst_hostname;
	char			dst_ip[INET_ADDRSTRLEN];
	t_stat			stats;
	int				len;
	size_t			num_xmit;
	int				ident;
	struct timeval	interval;
}				t_ping;

typedef struct	s_echo_data
{
	int				type;
	int				len;
	char			ip[INET_ADDRSTRLEN];
	char			host[HOST_NAME_MAX];
	double			triptime;
	int				sequence;
	int				ttl;
}				t_echo_data;

#endif /* PING_STRUCT_H */