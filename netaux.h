#pragma once

#include <netdb.h>
#include <stdio.h>
#include <errno.h>

#define ERROR(msg) { perror((msg)); exit(errno); }

char *addrtostr(struct sockaddr *sa);
