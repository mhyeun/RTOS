/**
 * Implement printOverlaps().
 * @author Matthew Hyeun
 */

#include "day.h"
#include <stdio.h>
#include <stdbool.h>

int compare(timeofday_t a, timeofday_t b) {
	return a.hour * 60 - b.hour * 60 + a.minute - b.minute;
}

void sortDay(event_t events[], uint32_t n) {
	for(int i=0; i<n-1; i++) {
		int min = i;
		for(int j=i+1; j<n; j++)
			if(compare(events[j].start, events[min].start) < 0)
				min = j;
		event_t temp = events[i];
		events[i] = events[min];
		events[min] = temp;
	}
}

uint32_t endAt(event_t e) {
	return e.start.hour*60 + e.start.minute + e.duration;
}

void print(event_t event) {
	uint32_t end = endAt(event);
	printf("%s from %02d:%02d to %02d:%02d\n", event.name,
			event.start.hour, event.start.minute,
			end / 60 % 24, end % 60);
}

// Helper function to convert time to minutes
uint32_t convertToMins(event_t e) {
  return e.start.hour*60 + e.start.minute;
}

// Helper to calculate the difference between start and end times between consecutive events
int calculateDifference(uint32_t index, uint32_t n, event_t events[]) {
  if(index == n-1) {
    return compare(events[0].start, events[index].start) - events[index].duration;
  }
  return compare(events[index+1].start, events[index].start) - events[index].duration;
}

// Helper to print result
void printResult(event_t a, event_t b, bool c) {
  if (c) {
    uint32_t end = endAt(b);
    printf("%s overlaps %s from %02d:%02d to %02d:%02d\n", a.name, b.name, a.start.hour, a.start.minute, end / 60 % 24, end % 60);
  } else {
    uint32_t end = endAt(a);
    printf("%s overlaps %s from %02d:%02d to %02d:%02d\n", a.name, b.name, a.start.hour, a.start.minute, end / 60 % 24, end % 60);
  }
}

void printOverlaps(event_t events[], uint32_t n) {
  for(int i=0; i < n; i++) {
    int difference = calculateDifference(i, n, events);

    // When at end of list, to compare last to first element
    if (i == n-1) {
      if(difference < 0) {
        if(endAt(events[i]) > convertToMins(events[i+1])) {
          printResult(events[0], events[i], true);
        } else {
          printResult(events[0], events[i], false);
        }
      }
    }

    // When not at end of list 
    else {
      if(difference < 0) {      
        if(endAt(events[i]) > convertToMins(events[i+1])) {
          printResult(events[i+1], events[i], true);
        } else {
          printResult(events[i+1], events[i], false);
        }
      }
    }
  }
}



