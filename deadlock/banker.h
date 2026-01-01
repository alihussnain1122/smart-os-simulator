#ifndef BANKER_H
#define BANKER_H

void bankers_algorithm(int n, int m,
                        int alloc[n][m],
                        int max[n][m],
                        int avail[m]);

#endif
