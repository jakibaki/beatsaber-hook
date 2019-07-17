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

    struct dirent **file_list;
    int no_files = scandir(MOD_PATH, &file_list, NULL, alphasort);

    // Goes through all files in the mods folder alphabetically
    for (int i = 0; i < no_files; i++)
    {
        // Only attempts to load .so files
        if (strlen(file_list[i]->d_name) > 3 && !strcmp(file_list[i]->d_name + strlen(file_list[i]->d_name) - 3, ".so"))
        {
            char full_path[PATH_MAX] = MOD_PATH;
            strcat(full_path, file_list[i]->d_name);

            __android_log_print(ANDROID_LOG_INFO, "QuestHook", "Loading mod: %s", full_path);

            // Get filesize of mod
            int infile = open(full_path, O_RDONLY);
            off_t filesize = lseek(infile, 0, SEEK_END);
            lseek(infile, 0, SEEK_SET);

            // Unlink old file
            unlink(MOD_TEMP_PATH);

            // Creates temporary copy (we can't execute stuff in /sdcard so we need to copy it over)
            int outfile = open(MOD_TEMP_PATH, O_CREAT | O_WRONLY);
            sendfile(outfile, infile, 0, filesize);
            close(infile);
            close(outfile);

            // Mark copy as executable
            chmod(MOD_TEMP_PATH, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP);
            // and load it
            dlopen(MOD_TEMP_PATH, RTLD_NOW);
        }
        free(file_list[i]);
    }
    free(file_list);

    __android_log_write(ANDROID_LOG_INFO, "QuestHook", "Done loading mods!");
}

__attribute__((constructor)) void lib_main()
{
    __android_log_write(ANDROID_LOG_INFO, "QuestHook", "Welcome!");
    load_mods();
}