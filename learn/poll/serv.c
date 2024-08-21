#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
int maxSock;
char *msg = NULL;
int gclid[5000];
char *cliBuff[5000];

char buff_sd[1001];
char buff_rd[1001];

fd_set rs, ws, as;

void error(char *s)
{
    write(2,s,strlen(s));
    exit(1);
}

int extract_msg(char **buff, char **msg)
{
	int i = 0;
	char *newbuff;

	*msg = 0;
	if (*buff == 0)
		return (0);

	while ((*buff)[i])
	{
		if ((*buff)[i] == '\n')
		{
			newbuff = calloc(strlen(*buff + i + 1) + 1, sizeof(*newbuff));
			if (!newbuff)
				return (-1);
			strcpy(newbuff, (*buff + i + 1));
			*msg = *buff;
			(*msg)[i + 1] = 0;
			*buff = newbuff;
			return (1);
		}
		++i;
	}
	return (0);
}

char *str_join(char *buf, char *add)
{
	char	*newbuf;
	int		len;

	if (buf == 0)
		len = 0;
	else
		len = strlen(buf);
	newbuf = malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
	if (newbuf == 0)
		return (0);
	newbuf[0] = 0;
	if (buf != 0)
		strcat(newbuf, buf);
	free(buf);
	strcat(newbuf, add);
	return (newbuf);
}

int main (int ac, char **av)
{
    if(ac != 2)
        error("Wrong number of arguments\n");
    
    int sokfd, confd, cliId;
    cliId = 0;
    struct sockaddr_in serv_addr, cli_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    socklen_t len = sizeof(cli_addr);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    serv_addr.sin_port = htons(atoi(av[1]));

    sokfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sokfd == -1)
        error("");
    if (bind(sokfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("");
    if (listen(sokfd, SOMAXCONN) < 0)
        error("");
    
    maxSock = sokfd;
    FD_ZERO(&atv_set);
    FD_SET(sokfd, &atv_set);

    while (1)
    {
        rd_set = wrt_set = atv_set;
        if(select(maxSock + 1, &rd_set, &wrt_set, 0, 0) <= 0)
            continue;
        if(FD_ISSET(sokfd, &rd_set))
        {
            confd = accept(sokfd, (struct sockaddr *)&cli_addr, &len);
            if(confd < 0)
                error("");
            FD_SET(confd, &atv_set);
            if (confd > maxSock)
                maxSock = confd;
        }
        else
        {
            if (read_msg(i) == 1)
                send_msg(i);
        }


    }
    
}