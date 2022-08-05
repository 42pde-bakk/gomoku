//
// Created by Peer De bakker on 3/22/22.
//

#ifndef GOMOKUBOT_GOMOKU_HPP
#define GOMOKUBOT_GOMOKU_HPP

#ifndef LOG
# define LOG 1
#endif

#ifndef THREADED
# define THREADED 0
#endif

#define FLAG_HISTORY		0b1000
#define FLAG_LOOKUPTABLE	0b0100
#define FLAG_MAX_CHILDREN	0b0010
extern unsigned int g_max_children;
extern bool	g_uses_lookuptable;


#include <mutex>

extern std::mutex cerrMutex;
extern std::mutex fsMutex;
extern bool g_log; // Used to turn logging off and on, on short intervals to discover bugs

#endif //GOMOKUBOT_GOMOKU_HPP
