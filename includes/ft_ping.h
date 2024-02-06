#ifndef FT_PING_H
# define FT_PING_H

# include "parser.h"
# include <stdint.h>
# include <stddef.h>

# define ICMP_ECHO_REQUEST		8
# define ICMP_ECHO_REPLY		0

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

typedef struct  s_icmp_header
{
	uint8_t		type; // 8
	uint8_t		code; // 16
	uint16_t	checksum; // 32
	uint16_t	identifier; // 48
	uint16_t	sequence_number; // 64 ( 8 bytes )
}				t_icmp_header;

void	parse_args(t_args *args, int ac, char **av);
void	print_args(t_args args);

void	icmp_echo_request_message(char *msg, size_t len);

struct addrinfo	*host_to_addrinfo(char const *hostname);
int				create_raw_socket(void);
void			send_packet(struct addrinfo *addr, int sfd, char *msg, size_t len);
void			receive_packet(int sfd);
void			cleanup(struct addrinfo *addr, int sfd);

#endif /* FT_PING_H */
