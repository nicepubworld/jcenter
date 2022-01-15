//#include "modconfig.h"
#include <stdio.h>
#include <stdarg.h>
#include "jprint.h"
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>

MOD_JPRINT_CONF_T g_jprint_conf[MOD_NUM];
static FILE* g_debuglog_fp;

void jprint_init_conf()
{
    g_debuglog_fp = NULL;

    g_jprint_conf[MOD_JPRINT_DEBUG_LEVEL].mod = MOD_JPRINT_DEBUG_LEVEL;
    g_jprint_conf[MOD_JPRINT_DEBUG_LEVEL].desc = "debug_level";
    g_jprint_conf[MOD_JPRINT_DEBUG_LEVEL].val = 0;

    g_jprint_conf[MOD_JPRINT_MAIN].mod = MOD_JPRINT_MAIN;
    g_jprint_conf[MOD_JPRINT_MAIN].desc = "main";
    g_jprint_conf[MOD_JPRINT_MAIN].val = 0;

    g_jprint_conf[MOD_LOG_FILE].mod = MOD_LOG_FILE;
    g_jprint_conf[MOD_LOG_FILE].desc = "logfile";
    g_jprint_conf[MOD_LOG_FILE].val = 0;

}

void jprint_print_conf()
{
    int i;

    for (i=0;i<MOD_NUM;i++){
        if (g_jprint_conf[i].desc)
            printf("conf %s = %d\n",g_jprint_conf[i].desc,g_jprint_conf->val);
    }
}

void jprint_init()
{
	char buffOri[2048]={0};
	char buffName[2048]={0};
	char buffData[2048]={0};	
	char* endp = NULL;
    int i;
	
    jprint_init_conf();

	FILE* fp = fopen("/usr/jcenter/jconfig.conf","r");
	
    if (NULL != fp){
        while (fgets(buffOri,sizeof(buffOri),fp)>0){
			int buflen = strlen(buffOri);
			int index = 0;
			int parseData = 0;//0为属性字段，1为数据字段
			int valIndex = 0;
			
			printf("jimtest get buff:%s",buffOri);

            memset(buffName,sizeof(buffName),0);
            memset(buffData,sizeof(buffData),0);

			for (index=0;index<buflen;index++){
				if ('#' == buffOri[index])
					break;
				if ('=' == buffOri[index]){
					parseData = 1;
					valIndex = 0;
					continue;
				}
				if (' ' != buffOri[index] && '\n' != buffOri[index]){
					if (0==parseData){
						buffName[valIndex] = buffOri[index];
						valIndex++;
					}
					else{
						buffData[valIndex] = buffOri[index];
						valIndex++;
					}				
				}
			}
			printf("jimtest get no comment name:%s\n",buffName);
			printf("jimtest get no comment data:%s\n",buffData);
			
            if (1==parseData){
                for (i=0;i<MOD_NUM;i++){
                    if (strcmp(g_jprint_conf[i].desc,buffName) == 0){
                        g_jprint_conf[i].val = atoi(buffData);
                    }
                }

                if (strcmp("logpath",buffName) == 0){
                    if (NULL !=g_debuglog_fp)
                        fclose(g_debuglog_fp);
                    g_debuglog_fp = fopen(buffData,"a+");

                    if (NULL == g_debuglog_fp){
                        printf("cant open log file:%s\n",buffData);
                    }
                }
            }

			memset(buffData,0,sizeof(buffData));
			memset(buffName,0,sizeof(buffName));
        }

		fclose(fp);
    }

    jprint_print_conf();
}

void jprint(MOD_JPRINT_T mod,MOD_DEBUG_LVL_T lvl,const char* format,...)
{   
    va_list args;
    time_t now;
    struct tm * ptm;
    char buf[2048]={0};
    int slen = 0;
    struct timeval tv;

    gettimeofday(&tv, NULL);

    time(&now);

    ptm=localtime(&now);

    if (0==g_jprint_conf[mod].val||lvl < g_jprint_conf[MOD_JPRINT_DEBUG_LEVEL].val)
        return;

    switch (lvl)
    {
        case MOD_DEBUG_INFO:
            printf(JPRINT_GRAY"[info]");
            slen=sprintf(buf,"[info]");
            break;
        case MOD_DEBUG_TRACE:
            printf(JPRINT_WHITE"[trace]");
            slen=sprintf(buf,"[trace]");
            break;
        case MOD_DEBUG_WARN:
            printf(JPRINT_YELLOW"[warn]");
            slen=sprintf(buf,"[warn]");
            break;
        case MOD_DEBUG_ERR:
            printf(JPRINT_RED"[err]");
            slen=sprintf(buf,"[err]");
            break;
        case MOD_DEBUG_FATAL:
            printf(JPRINT_L_RED"[fatal]");
            slen=sprintf(buf,"[fatal]");
            break;
        default:
            printf(JPRINT_WHITE"[trace]");
            slen=sprintf(buf,"[trace]");
            break;
    }
    
    printf("%s[%04d-%02d-%02d %02d:%02d:%02d ms:%lu]",g_jprint_conf[mod].desc,ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,ptm->tm_hour, ptm->tm_sec, ptm->tm_min, (unsigned long)(tv.tv_sec * 1000UL + tv.tv_usec / 1000UL));
    slen+=snprintf(buf+slen,sizeof(buf),"%s[%04d-%02d-%02d %02d:%02d:%02d ms:%lu]",g_jprint_conf[mod].desc,ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,ptm->tm_hour, ptm->tm_min, ptm->tm_sec,(unsigned long)(tv.tv_sec * 1000UL + tv.tv_usec / 1000UL));

    va_start(args, format);
    vprintf(format, args);

    if (NULL != g_debuglog_fp && 1==g_jprint_conf[MOD_LOG_FILE].val)
    {
        slen+=vsnprintf(buf+slen,sizeof(buf)-slen,format,args);
        fprintf(g_debuglog_fp,"%s",buf);
        fflush(g_debuglog_fp);
    }

    va_end(args);
    
    printf(JPRINT_NONE);
}


