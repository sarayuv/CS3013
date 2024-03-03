#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

// Constants
#define MAX_BASEBALL_PLAYERS 18
#define MAX_FOOTBALL_PLAYERS 22
#define BASEBALL 1
#define FOOTBALL 2

// Prototypes
void* player_threads(void* arg);

// Other
int baseball_players[MAX_BASEBALL_PLAYERS];
int football_players[MAX_FOOTBALL_PLAYERS];
int baseball_count = 0;
int football_count = 0;
pthread_mutex_t baseball_lock;
pthread_mutex_t football_lock;
pthread_cond_t baseball_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t football_cond = PTHREAD_COND_INITIALIZER;

void* player_threads(void* arg) {
    int player_type = (int)arg;
    int player_num;
    pthread_mutex_t* lock;
    int* players;
    int* num_players;
    pthread_cond_t* cond;

    if (player_type == BASEBALL) {
        lock = &baseball_lock;
        players = baseball_players;
        num_players = &baseball_count;
        cond = &baseball_cond;
    } else if (player_type == FOOTBALL) {
        lock = &football_lock;
        players = football_players;
        num_players = &football_count;
        cond = &football_cond;
    } else {
        printf("Error\n");
        pthread_exit(NULL);
    }

    pthread_mutex_lock(lock);

    // Wait until the field is safe and enter
    if (player_type == BASEBALL) {
        if (*num_players >= MAX_BASEBALL_PLAYERS) {
            pthread_cond_wait(cond, lock);  
        }
    } else if (player_type == FOOTBALL) {
        if (*num_players >= MAX_FOOTBALL_PLAYERS) {
            pthread_cond_wait(cond, lock);
        }
    }

    player_num = ++(*num_players);
    players[*num_players - 1] = player_num;

    if (player_type == BASEBALL) {
        printf("Player %d entered the field for baseball\n", player_num);
    } else if (player_type == FOOTBALL) {
        printf("Player %d entered the field for football\n", player_num);
    }

    // Check if team is ready
    if (player_type == BASEBALL) {
        if (*num_players >= MAX_BASEBALL_PLAYERS) {
            printf("Team ready for baseball\n");
        }
    } else if (player_type == FOOTBALL) {
        if (*num_players >= MAX_FOOTBALL_PLAYERS) {
            printf("Team ready for football\n");
        }
    }

    // Check if game started
    if (player_type == BASEBALL) {
        if (*num_players >= MAX_BASEBALL_PLAYERS) {
            printf("Game started for baseball\n");
        }
    } else if (player_type == FOOTBALL) {
        if (*num_players >= MAX_FOOTBALL_PLAYERS) {
            printf("Game started for football\n");
        }
    }

    pthread_mutex_unlock(lock);

    // Play sport
    sleep(rand() % 10);

    // Leave field
    pthread_mutex_lock(lock);
    if (player_type == BASEBALL) {
        printf("Player %d left the field for baseball\n", player_num);
    } else if (player_type == FOOTBALL) {
        printf("Player %d left the field for football\n", player_num);
    }

    // Broadcast to other players
    pthread_cond_broadcast(cond);

    pthread_mutex_unlock(lock);
}

int main() {
    // Seed
    FILE *seedFile = fopen("seed.txt", "r");
    int seed;
    if (seedFile == NULL) {
        printf("Error with seed");
        exit(1);
    } else {
        fscanf(seedFile, "%d", &seed);
        fclose(seedFile);
    }
    srand(seed);

    pthread_t threads[MAX_BASEBALL_PLAYERS + MAX_FOOTBALL_PLAYERS];
    int i;

    // Initialize locks
    if (pthread_mutex_init(&baseball_lock, NULL) != 0 || pthread_mutex_init(&football_lock, NULL) != 0) {
        printf("Error initializing locks\n");
        exit(1);
    }

    // Create threads for players
    for (i = 0; i < MAX_BASEBALL_PLAYERS; i++) {
        pthread_create(&threads[i], NULL, player_threads, (void*)BASEBALL);
    }

    for (; i < MAX_BASEBALL_PLAYERS + MAX_FOOTBALL_PLAYERS; i++) {
        pthread_create(&threads[i], NULL, player_threads, (void*)FOOTBALL);
    }

    // Wait for all threads to finish
    for (i = 0; i < MAX_BASEBALL_PLAYERS + MAX_FOOTBALL_PLAYERS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy locks
    pthread_mutex_destroy(&baseball_lock);
    pthread_mutex_destroy(&football_lock);

    return 0;
}
