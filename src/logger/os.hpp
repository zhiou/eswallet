//
//  os.h
//  transmit
//
//  Created by zhiou on 2019/8/19.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

namespace excelsecu {
    namespace os {
#ifdef _OS_ANDROID_
        constexpr android_max_path_len = 260;
        
        static std::string package_name(pid_t pid)
        {
            char name[android_max_path_len] = {0};
            char proc_pid_path[android_max_path_len]={0};
            char buf[android_max_path_len]={0};
            char* find=NULL;
            int name_len = android_max_path_len;
            
            sprintf(proc_pid_path,"/proc/%d/status",pid);
            //    EsStrPrintf(proc_pid_path, MAX_PATH, "/proc/%d/status",pid);
            FILE *fp=fopen(proc_pid_path,"r");
            if(NULL != fp)
            {
                if(fgets(buf,MAX_PATH-1,fp) == NULL)
                {
                    fclose(fp);
                    return 0;
                }
                find = strchr(buf,'\n');
                if(find)
                    *find = '\0';
                sscanf(buf,"%*s %s",name);
                name_len  = strlen(szExePath);
                *(name+name_len) = '\0';
                
                fclose(fp);
                return std::string(szExePath);
            }
            return "";
        }
        
        static std::string log_base_dir()
        {
            pid_t pid = getpid();
            
            auto package_name = ESTransmitGetAppPackageID(pid, packageName, PATH_MAX);
            
            char *dir = getenv("EXTERNAL_STORAGE");
            char *logDir = "EsLogs";
            
            size_t len = strlen(dir) * strlen(logDir) + package_name.length() + 8;
            
            std::unique_ptr<char> buff(new char[len]);
            
            sprintf(buff.get(), "%s/%s",dir,logDir);
            
            mkdir(buff.get(), S_IRWXU | S_IRWXG | S_IRWXO);
            
            memset((void *)buff.get(), 0, len);
            sprintf(buff.get(), "%s/%s/%s/",dir,logDir,package_name.c_str());
            //    EsStrPrintf(buff, len, "%s/%s/%s",dir,logDir,packageName)
            
            std::string path(buff.get());
            mkdir(buff, S_IRWXU | S_IRWXG | S_IRWXO);
            return path;
        }
#elif _OS_IOS_
        static std::string log_base_dir() {
            std::string path = "";
#if TARGET_OS_SIMULATOR
            path = "";
#else
            char *dir = getenv("HOME");
            const char* doc = "Documents";
            size_t len = strlen(dir) * strlen(doc) + 8;
            
            std::unique_ptr<char> buff(new char[len]);
            sprintf(buff.get(), "%s/%s/",dir, doc);
            
            path = std::string(buff.get());
#endif
            return path;
        }
#endif
        
    }
}
