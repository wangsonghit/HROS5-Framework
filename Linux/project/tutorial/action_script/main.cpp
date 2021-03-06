/*
 * main.cpp
 *
 *  Created on: 2011. 1. 4.
 *      Author: robotis
 */

#include <unistd.h>
#include <string.h>
#include <libgen.h>

#include "Camera.h"
#include "Point.h"
#include "mjpg_streamer.h"
#include "minIni.h"
#include "LinuxCamera.h"
#include "ColorFinder.h"

#include "Action.h"
#include "Head.h"
#include "Walking.h"
#include "MXDXL.h"
#include "MotionManager.h"
#include "LinuxMotionTimer.h"
#include "LinuxArbotixPro.h"
#include "LinuxActionScript.h"

#ifdef MXDXL_1024
#define MOTION_FILE_PATH    "../../../../Data/motion_1024.bin"
#else
#define MOTION_FILE_PATH    "../../../../Data/motion_4096.bin"
#endif

void change_current_dir()
{
    char exepath[1024] = {0};
    if (readlink("/proc/self/exe", exepath, sizeof(exepath)) != -1)
        chdir(dirname(exepath));
}

int main(void)
{
    printf( "\n===== Action script Tutorial for DARwIn =====\n\n");

    change_current_dir();

    Action::GetInstance()->LoadFile(MOTION_FILE_PATH);

    //////////////////// Framework Initialize ////////////////////////////
    LinuxArbotixPro linux_arbotixpro("/dev/ttyUSB0");
    ArbotixPro arbotixpro(&linux_arbotixpro);
    if (MotionManager::GetInstance()->Initialize(&arbotixpro) == false)
        {
            printf("Fail to initialize Motion Manager!\n");
            return 0;
        }
    MotionManager::GetInstance()->AddModule((MotionModule*)Action::GetInstance());
    LinuxMotionTimer::Initialize(MotionManager::GetInstance());
    /////////////////////////////////////////////////////////////////////

    MotionManager::GetInstance()->SetEnable(true);

    Action::GetInstance()->Start(1);    /* Init(stand up) pose */
    while (Action::GetInstance()->IsRunning()) usleep(8 * 1000);

    printf("Press the ENTER key to begin!\n");
    getchar();

    LinuxActionScript::ScriptStart("script.asc");
    while (LinuxActionScript::m_is_running == 1) sleep(10);

    return 0;
}
