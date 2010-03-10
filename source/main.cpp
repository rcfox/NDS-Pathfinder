#ifndef MAIN_CPP___
#define MAIN_CPP___

#include <nds.h>
#include <queue>
#include <stdio.h>
#include <cassert>

#include "pathfinding.h"
#include "robot.h"
#include "draw.h"
#include "screenshot.h"

#include <boost/foreach.hpp>

void ClearScreen(void)
{
    for(int i = 0; i < 256 * 192; i++)
    {
        VRAM_A[i] = RGB15(0,0,0);
    }

    draw::line(0,0,0,SCREEN_HEIGHT,RGB15(31,31,31));
    draw::line(SCREEN_WIDTH,0,SCREEN_WIDTH,SCREEN_HEIGHT,RGB15(31,31,31));
    draw::line(0,0,SCREEN_WIDTH,0,RGB15(31,31,31));
    draw::line(0,SCREEN_HEIGHT,SCREEN_WIDTH,SCREEN_HEIGHT,RGB15(31,31,31));
}

int main(void)
{
    touchPosition touch;

    defaultExceptionHandler();

    lcdMainOnBottom();
    irqEnable(IRQ_VBLANK);

    videoSetMode(MODE_FB0);
    vramSetBankA(VRAM_A_LCD);

    consoleDemoInit();

    ClearScreen();

    touchPosition firstTouch = { 0, 0 };
    touchPosition lastTouch = { 0, 0 };

    int colours[3] = { 31, 31, 31 };
    int colourIndex = 0;

    printf("RGB15(%d,%d,%d)\n",colours[0],colours[1],colours[2]);

    std::stack<node*>* path = NULL;
    bool searched = false;

    node start(10,10,NULL);
    node target(244,180,NULL);

    Robot robot;
    robot.setPosition(start.x,start.y);

    int nextTouch = 0;

    //we like infinite loops in console dev!
    while(1)
    {
        swiWaitForVBlank();

        scanKeys();
        touchRead(&touch);

        int kd = keysDown();
        if (kd & KEY_A)
        {
            ClearScreen();
        }

        if (kd & KEY_UP)
        {
            ++colours[colourIndex];
            if (colours[colourIndex] > 31) colours[colourIndex] = 0;
            printf("RGB15(%d,%d,%d)\n",colours[0],colours[1],colours[2]);
        }

        if (kd & KEY_DOWN)
        {
            --colours[colourIndex];
            if (colours[colourIndex] < 0) colours[colourIndex] = 31;
            printf("RGB15(%d,%d,%d)\n",colours[0],colours[1],colours[2]);
        }

        if (kd & KEY_LEFT)
        {
            --colourIndex;
            if (colourIndex < 0) colourIndex = 2;
        }

        if (kd & KEY_RIGHT)
        {
            ++colourIndex;
            if (colourIndex > 2) colourIndex = 0;
        }

        if (kd & KEY_Y)
        {
            robot.setPosition(start.x,start.y);
            path = NULL;
            node::cleanup();
        }

        if(kd & KEY_B)
        {
            printf("Working...\n");
            searched = true;
            node current(robot.getPosition().px, robot.getPosition().py,NULL);
            path = astar::generate_path( astar::search(&current,&target) );
            printf("Path found!\n");
        }

        if (kd & KEY_R)
        {
            nextTouch = 2;
        }

        if (kd & KEY_L)
        {
            nextTouch = 1;
        }

        if(kd & KEY_TOUCH)
        {
            switch (nextTouch)
            {
            case 0:
                firstTouch = touch;
                draw::plot(touch.px,touch.py,RGB15(colours[0],colours[1],colours[2]));
                break;
            case 1: // Set start
                start.x = touch.px;
                start.y = touch.py;
                robot.setPosition(touch);
                break;
            case 2: // Set target
                target.x = touch.px;
                target.y = touch.py;
                break;
            default:
                break;
            }
        }

        if(keysHeld() & KEY_TOUCH)
        {
            if (nextTouch == 0)
            {
                lastTouch = touch;
                draw::line(firstTouch.px, firstTouch.py, lastTouch.px, lastTouch.py, RGB15(colours[0],colours[1],colours[2]));
                firstTouch = touch;
                //q.push(touch);
            }
        }

        if (keysUp() & KEY_TOUCH)
        {
            nextTouch = 0;
        }

        if (path)
        {
            if (searched && !path->empty())
            {
                assert(path != NULL);
                assert(path->top() != NULL);
                robot.move(path->top()->x, path->top()->y);

                if (robot.getPosition().px == path->top()->x && robot.getPosition().py == path->top()->y) path->pop();
            }
            else if (path->empty())
            {
                node::cleanup();
                delete path;
                path = NULL;
                searched = false;
            }
        }
        robot.draw();

        draw::plot(target.x, target.y, RGB15(31,0,0));
        draw::plot(target.x+1, target.y, RGB15(31,0,0));
        draw::plot(target.x+2, target.y, RGB15(31,0,0));
        draw::plot(target.x-1, target.y, RGB15(31,0,0));
        draw::plot(target.x-2, target.y, RGB15(31,0,0));
        draw::plot(target.x, target.y+1, RGB15(31,0,0));
        draw::plot(target.x, target.y+2, RGB15(31,0,0));
        draw::plot(target.x, target.y-1, RGB15(31,0,0));
        draw::plot(target.x, target.y-2, RGB15(31,0,0));
        draw::plot(target.x+1, target.y+1, RGB15(31,0,0));
        draw::plot(target.x+1, target.y-1, RGB15(31,0,0));
        draw::plot(target.x-1, target.y+1, RGB15(31,0,0));
        draw::plot(target.x-1, target.y-1, RGB15(31,0,0));

    }

    return 0;
}

#endif // #ifndef MAIN_CPP___
