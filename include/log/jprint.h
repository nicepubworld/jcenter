#ifndef __HEADER_JPRINT_H__
#define __HEADER_JPRINT_H__

void Assert(char* filename, unsigned int lineno);

#ifdef DEBUG_VERSION
#define JC_ASSERT(condition)    \
do{                          \
if(condition)                \
NULL;                        \
else                         \
Assert((char*)__FILE__, __LINE__);  \
}while(0)
#else
#define JC_ASSERT(condition) {}
#endif


#define JPRINT_NONE                 "\e[0m"
#define JPRINT_BLACK                "\e[0;30m"
#define JPRINT_L_BLACK              "\e[1;30m"
#define JPRINT_RED                  "\e[0;31m"
#define JPRINT_L_RED                "\e[1;31m"
#define JPRINT_GREEN                "\e[0;32m"
#define JPRINT_L_GREEN              "\e[1;32m"
#define JPRINT_BROWN                "\e[0;33m"
#define JPRINT_YELLOW               "\e[1;33m"
#define JPRINT_BLUE                 "\e[0;34m"
#define JPRINT_L_BLUE               "\e[1;34m"
#define JPRINT_PURPLE               "\e[0;35m"
#define JPRINT_L_PURPLE             "\e[1;35m"
#define JPRINT_CYAN                 "\e[0;36m"
#define JPRINT_L_CYAN               "\e[1;36m"
#define JPRINT_GRAY                 "\e[0;37m"
#define JPRINT_WHITE                "\e[1;37m"

#define JPRINT_BOLD                 "\e[1m"
#define JPRINT_UNDERLINE            "\e[4m"
#define JPRINT_BLINK                "\e[5m"
#define JPRINT_REVERSE              "\e[7m"
#define JPRINT_HIDE                 "\e[8m"
#define JPRINT_CLEAR                "\e[2J"
#define JPRINT_CLRLINE              "\r\e[K"

typedef enum MOD_JPRINT_COLOR_E{
    JPRINT_COLOR_BLACK,
    JPRINT_COLOR_L_BLACK,
    JPRINT_COLOR_RED,
    JPRINT_COLOR_L_RED,
    JPRINT_COLOR_GREEN,
    JPRINT_COLOR_L_GREEN,
    JPRINT_COLOR_BROWN,
    JPRINT_COLOR_YELLOW,
    JPRINT_COLOR_BLUE,
    JPRINT_COLOR_L_BLUE,
    JPRINT_COLOR_PURPLE,
    JPRINT_COLOR_L_PURPLE,
    JPRINT_COLOR_CYAN,
    JPRINT_COLOR_L_CYAN,
    JPRINT_COLOR_GRAY,
    JPRINT_COLOR_WHITE
}MOD_JPRINT_COLOR_T;


typedef enum MOD_JPRINT_E{
    MOD_JPRINT_INIT,
    MOD_JPRINT_MAIN,
    MOD_NUM
}MOD_JPRINT_T;

typedef enum MOD_DEBUG_LVL_E{
    MOD_DEBUG_INFO,
    MOD_DEBUG_TRACE,
    MOD_DEBUG_WARN,
    MOD_DEBUG_ERR,
    MOD_DEBUG_FATAL,
}MOD_DEBUG_LVL_T;


void jprint_init();
void jprint(MOD_JPRINT_T mod,MOD_DEBUG_LVL_T lvl,const char* format,...);
void jprintc(MOD_JPRINT_T mod,MOD_DEBUG_LVL_T lvl,MOD_JPRINT_COLOR_T color,const char* format,...);
void jprint_hex(MOD_JPRINT_T mod,MOD_DEBUG_LVL_T lvl,const unsigned char* buff,int len);

#endif