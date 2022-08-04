//
// Created by Peer De bakker on 3/22/22.
//

#ifndef GOMOKUBOT_GOMOKU_HPP
#define GOMOKUBOT_GOMOKU_HPP

#ifndef LOG
#define LOG 1
#endif

#ifndef THREADED
#define THREADED 0
#endif

#define FLAG_HISTORY 0b1000

#include <mutex>

extern std::mutex cerrMutex;
extern std::mutex fsMutex;
extern bool g_log; // Used to turn logging off and on, on short intervals to
                   // discover bugs

#endif // GOMOKUBOT_GOMOKU_HPP
