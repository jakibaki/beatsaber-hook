#include <android/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <linux/limits.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <dlfcn.h>


#include "../shared/inline-hook/inlineHook.h"
#include "../shared/utils/utils.h"

#define MOD_PATH "/sdcard/Android/data/com.beatgames.beatsaber/files/mods/"
#define MOD_TEMP_PATH "/data/data/com.beatgames.beatsaber/cache/curmod.so"

void load_mods()
{
    __android_log_write(ANDROID_LOG_INFO, "QuestHook", "Loading mods!");
    mkdir(MOD_PATH, 0);

    struct dirent *dp;
    DIR *dir = opendir(MOD_PATH);

    while ((dp = readdir(dir)) != NULL)
    {
        if (strlen(dp->d_name) > 3 && !strcmp(dp->d_name + strlen(dp->d_name) - 3, ".so"))
        {
            char full_path[PATH_MAX] = MOD_PATH;
            strcat(full_path, dp->d_name);
            __android_log_print(ANDROID_LOG_INFO, "QuestHook", "Loading mod: %s", full_path);
            int infile = open(full_path, O_RDONLY);
            off_t filesize = lseek(infile, 0, SEEK_END);
            lseek(infile, 0, SEEK_SET);

            int outfile = open(MOD_TEMP_PATH, O_CREAT | O_WRONLY);
            sendfile(outfile, infile, 0, filesize);
            close(infile);
            close(outfile);
            chmod(MOD_TEMP_PATH, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP);
            dlopen(MOD_TEMP_PATH, RTLD_NOW);
        }
    }
    closedir(dir);

    __android_log_write(ANDROID_LOG_INFO, "QuestHook", "Done loading mods!");
}

__attribute__((constructor)) void lib_main()
{
    __android_log_write(ANDROID_LOG_INFO, "QuestHook", "Welcome!");
    load_mods();
}