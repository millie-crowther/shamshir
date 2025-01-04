#define THREAD_COUNT 20
static int counts[THREAD_COUNT];

static void *handle_request(void *a)
{
    int rc, i, thread_id = (int)a;
    pid_t pid = getpid();
    FCGX_Request request;
    char *server_name;

    FCGX_InitRequest(&request, 0, 0);

    for (;;)
    {
        static pthread_mutex_t accept_mutex = PTHREAD_MUTEX_INITIALIZER;
        static pthread_mutex_t counts_mutex = PTHREAD_MUTEX_INITIALIZER;

        /* Some platforms require accept() serialization, some don't.. */
        pthread_mutex_lock(&accept_mutex);
        rc = FCGX_Accept_r(&request);
        pthread_mutex_unlock(&accept_mutex);

        if (rc < 0)
            break;

        server_name = FCGX_GetParam("SERVER_NAME", request.envp);

        FCGX_FPrintF(request.out, "Hi");

        FCGX_Finish_r(&request);
    }

    return NULL;
}

int main(void)
{
    int i;
    pthread_t id[THREAD_COUNT];

    FCGX_Init();

    for (i = 1; i < THREAD_COUNT; i++)
        pthread_create(&id[i], NULL, handle_request, (void*)i);

    handle_request(0);

    return 0;
}