//#include "modconfig.h"
#include <stdio.h>
#include <stdarg.h>
#include "jprint.h"
#include <time.h>
#include <sys/time.h>
#include <string.h>

void jprint_init()
{
	char buffOri[2048]={0};
	char buffName[2048]={0};
	char buffData[2048]={0};	
	char* endp = NULL;
	
	FILE* fp = fopen("/usr/jcenter/jconfig.conf","r");

	
    if (NULL != fp){
        while (fgets(buffOri,sizeof(buffOri),fp)>0){
			int buflen = strlen(buffOri);
			int index = 0;
			int parseData = 0;//0为属性字段，1为数据字段
			int valIndex = 0;
			
			printf("jimtest get buff:%s",buffOri);

			for (index=0;index<buflen;index++){
				if ('#' == buffOri[index])
					break;
				if ('=' == buffOri[index]){
					parseData = 1;
					valIndex = 0;
					continue;
				}
				if (' ' != buffOri[index]){
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
			
			memset(buffData,0,sizeof(buffData));
			memset(buffName,0,sizeof(buffName));
        }

		fclose(fp);
    }
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

    //if (0==mod_need_print[mod]||lvl<gs_current_level)
    //    return;

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
    
    //printf("%s[%04d-%02d-%02d %02d:%02d:%02d ms:%lu]",mod_string[mod],ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,ptm->tm_hour, ptm->tm_min, ptm->tm_sec,tv.tv_sec * 1000 + tv.tv_usec / 1000);
    //slen+=snprintf(buf+slen,sizeof(buf),"%s[%04d-%02d-%02d %02d:%02d:%02d ms:%lu]",mod_string[mod],ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,ptm->tm_hour, ptm->tm_min, ptm->tm_sec,ptm->tm_sec,tv.tv_sec * 1000 + tv.tv_usec / 1000);

    va_start(args, format);
    vprintf(format, args);

#if 0
    if (NULL != g_debuglog_fp)
    {
        slen+=vsnprintf(buf+slen,sizeof(buf)-slen,format,args);
        fprintf(g_debuglog_fp,"%s",buf);
        fflush(g_debuglog_fp);
    }
#endif

    va_end(args);
    
    printf(JPRINT_NONE);
}


