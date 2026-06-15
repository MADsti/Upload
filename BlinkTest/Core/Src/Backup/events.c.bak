/*
 * events.c
 *
 *  Created on: 05.06.2026
 *      Author: nalle
 */
#include "events.h"
#include <stdint.h>

#define EVENT_QUEUE_SIZE 10

static Event_t eventQueue[EVENT_QUEUE_SIZE];

static uint8_t head = 0;
static uint8_t tail = 0;

void Event_Push(Event_t event)
{
    uint8_t nextHead =
        (head + 1) % EVENT_QUEUE_SIZE;

    if(nextHead != tail)
    {
        eventQueue[head] = event;
        head = nextHead;
    }
}

bool Event_Get(Event_t* event)
{
    if(head == tail)
    {
        return false;
    }

    *event = eventQueue[tail];

    tail =
        (tail + 1) % EVENT_QUEUE_SIZE;

    return true;
}
