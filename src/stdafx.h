#ifndef _SRC_STDAFX_H
#define _SRC_STDAFX_H

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <time.h>

#ifdef _WIN32
#include <tchar.h>
#include <string>
#include <Windows.h>
#elif __linux__
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#endif

#endif
