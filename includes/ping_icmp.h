#ifndef PING_ICMP_H
# define PING_ICMP_H

# include <stddef.h>

bool	icmp_is_correct_checksum(struct icmphdr *icmphdr, size_t icmplen);
void	icmp_set_icmphdr(char *msg, int ident, int seqno);
void	icmp_add_timestamp(char *msg);
void	icmp_set_data(char *msg, size_t total_len);
void	icmp_add_checksum(char *msg, size_t len);

#endif /* PING_ICMP_H */