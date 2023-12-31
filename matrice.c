#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define M 5 // Size of the matrices
#define N (M * M)  // Size of the buffer
#define NUM_THREADS M

// Synchronization variables
pthread_mutex_t mutex;
sem_t empty, full;

// Matrices B, C, and A
int B[M][M];
int C[M][M];
int A[M][M];

// Buffer and counter
int buffer[N];
int count = 0;

// Producer
void* producer(void* arg) {
    intptr_t row = (intptr_t)arg;//effectue une conversion du pointeur arg vers un entier de type intptr_t. Cela permet de stocker la valeur du pointeur sous forme d'entier.

    for (int j = 0; j < M; ++j) {
        int result = 0;
        for (int k = 0; k < M; ++k) {
            result += B[row][k] * C[k][j];
        }

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

         //S'assure que le nombre ne dépasse pas la taille du tampon
        if (count < N) {
            buffer[count] = result;
            count++;
        } else {
            // Handle buffer overflow
            fprintf(stderr, "Buffer overflow in producer\n");
            sem_post(&full);
            pthread_exit(NULL); // Exit upon buffer overflow
        }

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }

    return NULL;
}

// Consumer
void* consumer(void* arg) {
    intptr_t col = (intptr_t)arg;

    for (int i = 0; i < M; ++i) {
        int y;

        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        y = buffer[count - 1];  // recuperer la valeur du buffer
        count--;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);

// affichage la valeur recuperer elle va pas etre en ordre comme le (single process) car elle va recuperer les valeurs depuis la fin vers le debut du buffer
      
        printf("%d ", y);
    }

    printf("\n");
    return NULL;
}

int main() {
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, N);
    sem_init(&full, 0, 0);

    // Initialize matrices B and C with random values
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            //B[i][j] = rand() % 11;
            //C[i][j] = rand() % 11;
            B[i][j] = 1;
            C[i][j] = 1;
        }
    }

    // Single process calculation
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < M; ++j) {
            A[i][j] = 0;
            for (int k = 0; k < M; ++k) {
                A[i][j] += B[i][k] * C[k][j];
            }
        }
    }

    // Print the result of the single process calculation
    printf("Result using a single process:\n");
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < M; j++) {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }
    
    
    printf("\n");
      printf("Result using a producer and consumer methode:\n");
       // Create producer threads
   pthread_t th[NUM_THREADS];
   
    for (intptr_t i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&th[i], NULL, &producer, (void*)i) != 0) {    
        //  Ici, i est converti en un pointeur de type void* pour être passé en tant qu'argument générique. l'expression != 0 vérifie si la création du thread a réussi.
            //Si la valeur retournée est différente de zéro, cela indique une erreur lors de la création du thread.
            fprintf(stderr, "Error: Failed to create producer thread %ld.\n", i);
            perror(NULL);// Autres actions à entreprendre en cas d'échec de création du thread
        }
    }

    // Wait for producer threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            fprintf(stderr, "Error: Failed to join producer thread ");
        perror(NULL);
        // Autres actions à entreprendre en cas d'échec de jointure du thread
        }
    }

    // Create consumer threads
    
    for (intptr_t i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&th[i], NULL, &consumer, (void*)i) != 0) {

            perror("failed to create consumer thread. ");
        }
    }

    // Wait for consumer threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join consumer thread. ");
        }
    }

    // Clean up
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
