#include "modconfig.h"
#include "jprint.h"
#include <string.h>
#include <stdlib.h>

void jprint_init()
{
	char buffOri[2048]={0};
	char buffName[2048]={0};
	char buffData[2048]={0};	
	char* endp = NULL;

    int i;

    FILE* logfp = NULL;
    FILE* fp = NULL;
    MOD_JPRINT_CONF_T* jconf = NULL;
    
    jprint_init_conf();

    logfp = get_g_debuglog_fp();
    jconf = get_g_jprint_conf();

	fp = fopen("/usr/jcenter/jconfig.conf","r");
	
    if (NULL != fp){
        while (fgets(buffOri,sizeof(buffOri),fp)>0){
			int buflen = strlen(buffOri);
			int index = 0;
			int parseData = 0;//0为属性字段，1为数据字段
			int valIndex = 0;
			
			//printf("jimtest get buff:%s",buffOri);

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
				if (' ' != buffOri[index] &&'\r' != buffOri[index] && '\n' != buffOri[index]){
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
			//printf("jimtest get no comment name:%s\n",buffName);
			//printf("jimtest get no comment data:%s\n",buffData);
			
            if (1==parseData){
                for (i=0;i<MOD_NUM;i++){
                    if (strcmp(jconf[i].desc,buffName) == 0){
                        jconf[i].val = atoi(buffData);
                    }
                }

                if (strcmp("logpath",buffName) == 0){
                    if (NULL !=logfp)
                        fclose(logfp);
                    logfp = fopen(buffData,"a+");

                    if (NULL == logfp){
                        printf("cant open log file:%s\n",buffData);
                    }

                    set_g_debuglog_fp(logfp);
                }
            }

			memset(buffData,0,sizeof(buffData));
			memset(buffName,0,sizeof(buffName));
        }

		fclose(fp);
    }

    //jprint_print_conf();
}
