/// Some times Windows Security automatically Blocks the "exe file"
/// because of the color code in Code Blocks.Please Allow it.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t italian, german;
int german_player, italian_player, german_player_count = 0, italian_player_count = 0,
                                   tram_car = 1, german_player_waiting =0, italian_player_waiting =0;

/// This print function prints the total number of Germans and Italians waiting to board the tram.
void print()
{
    printf("Total number of Italian waiting to board the tram is :  %d\n",italian_player_waiting);
    printf("Total number of German waiting to board the tram is :  %d\n\n",german_player_waiting);

}

void *italian_players_arrived(void *arg)
{
    italian_player_waiting++;
    printf("Italian player arrived. \n");
    print();

    sem_wait(&italian); /// Decrements the value of semaphore.

    italian_player_waiting--;
    int a=1;

    while(a!=0)
    {
        if((italian_player_count == 2 && german_player_count == 1)||(german_player_count == 3 && italian_player_count == 0) ) /// Checks Unsafe Combination .
        {
            italian_player_waiting++;
            if(italian_player_count == 2 && german_player_count == 1)
            {
                printf("\033[1;33m"); //Yellow Color for this print.
                printf("A Italian Player Trying to enter in the tram car. But there are 2 Italian Player and 1 German Player in the tram car. So no more Italian Player Can enter in the tram car. Italian Player must wait. \n\n");
                printf("\033[0m");
            }
            if(german_player_count == 3)
            {
                printf("\033[1;33m"); //Yellow Color for this print.
                printf("A Italian Player Trying to enter in the tram car. But there are 3 German Player in the tram car. So Italian Player Can not enter in the tram car. Italian Player must wait. \n\n");
                printf("\033[0m");
            }
            print();

            sem_wait(&italian); /// If Unsafe italian_players_arrived thread have to wait.So, So, italian_players_arrived have to be locked
            /// So, Decrements the value of semaphore.
            /// If free from wait then While loop checks again Player should enter or not.
            /// If still unsafe italian_players_arrived have to thread wait again and italian_players_arrived have to be locked again.

            italian_player_waiting--;
        }
        else
        {
            break; /// If safe While loop Ends.
        }
    }
    italian_player_count++; /// italian_player_count counts how many Italian players Enter the tram car. So, Increasing  italian_player_count.

    printf("\nItalian player enter the tram car. \n\n");

    if((italian_player_count + german_player_count) == 4) /// If safe Combination and Total 4 players Tram Leaves.
    {
        printf("\033[1;31m"); //Red Color for this print.
        printf("\nTram car is fully loaded. Tram car number %d is leaving. %d German Player and %d Italian Player in the Tram car.\n", tram_car,german_player_count,italian_player_count);
        printf("\033[0m");

        german_player_count = 0; /// As Tram car left, german_player_count again 0 meaning no German player in tram car.

        italian_player_count = 0; /// As Tram car left, italian_player_count again 0 meaning no Italian player in tram car.

        print(); //Prints Number of Germans and Italians waiting to board the tram.

        tram_car++; //Increasing Tram Car Number.

        sem_post(&german); /// When Combination is Unsafe in german_players_arrived thread and (sem_wait(&german);) called in german_players_arrived thread.
        /// And we Decrements the value of semaphore (german) and locked the italian_players_arrived thread
        /// but as now Safe Combination found and tram car leaves. That value of semaphore (german) need to be Increased
        /// and then italian_players_arrived thread will be unlocked.
    }
    sem_post(&italian); /// Increased the value of semaphore.
}

void *german_players_arrived(void *arg)
{
    german_player_waiting++;
    printf("German player arrived. \n");
    print();

    sem_wait(&german);  /// Decrements the value of semaphore.

    german_player_waiting--;
    int a=1;

    while(a!=0)
    {
        if((german_player_count == 2 && italian_player_count == 1)||(italian_player_count == 3 && german_player_count == 0)) /// Checks Unsafe Combination .
        {
            german_player_waiting++;
            if(german_player_count == 2 && italian_player_count == 1)
            {
                printf("\033[1;33m"); //Yellow Color for this print.
                printf("A German Player Trying to enter in the tram car. But there are 2 German Player and 1 Italian Player in the tram car. So no more German Player Can enter in the tram car. German Player must wait. \n\n");
                printf("\033[0m");
            }
            if(italian_player_count == 3)
            {
                printf("\033[1;33m"); //Yellow Color for this print.
                printf("A German Player Trying to enter in the tram car. But there are 3 Italian Player in the tram car. So German Player Can not enter in the tram car. German Player must wait. \n\n");
                printf("\033[0m");
            }
            print();
            sem_wait(&german); /// If Unsafe german_players_arrived thread have to wait.So, So, german_players_arrivedhave to be locked
            /// So, Decrements the value of semaphore.
            /// If free from wait then While loop checks again Player should enter or not.
            /// If still unsafe german_players_arrived have to thread wait again and german_players_arrived have to be locked again.
            german_player_waiting--;
        }
        else
        {
            break; /// If safe While loop Ends.
        }
    }
    german_player_count++; // german_player_count counts how many German players Enter the tram car. So, Increasing german_player_count.

    printf("\nGerman player enter the tram car. \n\n");

    if((german_player_count + italian_player_count) == 4) //If safe Combination and Total 4 players Tram Leaves.
    {
        printf("\033[1;31m"); //Red Color for this print.
        printf("\nTram car is fully loaded. Tram car number %d is leaving. %d German Player and %d Italian Player in the Tram car.\n", tram_car,german_player_count,italian_player_count);
        printf("\033[0m");
        german_player_count=0;  //As Tram car left, german_player_count again 0 meaning no German player in tram car.

        italian_player_count=0; //As Tram car left, italian_player_count again 0 meaning no Italian player in tram car.

        print(); //Prints Number of Germans and Italians waiting to board the tram.

        tram_car++; //Increasing Tram Car Number.

        sem_post(&italian); /// When Combination is Unsafe in italian_players_arrived thread and (sem_wait(&german);) called in italian_players_arrived thread.
        /// And we Decrements the value of semaphore (italian) and locked the italian_players_arrived thread
        /// but as now Safe Combination found and tram car leaves. That value of semaphore (italian) need to be Increased
        /// and then italian_players_arrived thread will be unlocked.
    }
    sem_post(&german); /// Increased the value of semaphore.
}

void main ()
{
    int enter, i, t, t1, t2, ip, gp,tp, total_player = 0, it, g, x = 1;
    pthread_t thread1[500];
    system("color 0");
    if (sem_init(&italian, 0, 1) == -1) /// Initializing Semaphores
    {
        perror("Could not initialize mylock semaphore");
        exit(2);
    }
    if (sem_init(&german, 0, 1) == -1) //Initializing Semaphores
    {
        perror("Could not initialize mylock semaphore");
        exit(2);
    }
    printf("\033[1;31m");
    printf("\n=================================================================================================================\n");
    printf("\n\t\t--------Welcome To Our Project--------\n");
    printf("\n\t\t    --------FIFA World Cup--------\n");
    printf("\n=================================================================================================================\n");
    printf("\033[0m");
    while(x!=0)
    {

        it = 0; /// Variable for German Player.
        g = 0;  /// Variable for Italian Player.
        printf("\033[1;34m"); //Blue Color for this Menu.
        printf("\n\t\tWhat would you like to do?\n");
        printf("\nEnter 1 for 0 German Player and 4 Italian Player (Which is Safe Combination)");
        printf("\nEnter 2 for 4 German Player and 0 Italian Player (Which is Safe Combination)");
        printf("\nEnter 3 for 2 German Player and 2 Italian Player (Which is Safe Combination)");
        printf("\nEnter 4 for 1 German Player and 3 Italian Player (Which is Unsafe Combination)");
        printf("\nEnter 5 for 3 German Player and 1 Italian Player (Which is Unsafe Combination)");
        printf("\nEnter 6 for Any User Input");
        printf("\nEnter 0. Exit");
        printf("\n\nEnter a choice. Please Do Not Enter Characters : ");
        printf("\033[0m");
        scanf("%d",&enter);
        printf("\n");

        if(enter == 1)
        {
            for(i = total_player ; i < (total_player + 4); i++) /// total_player keeps truck of Previous total (German + Italian) Players.
            {
                t2 = pthread_create (&thread1[i], NULL, italian_players_arrived, (void *) t); //Creating 4 thread for Italian Players.
                if (t2)
                {
                    printf("ERROR; return code from pthread_create() is %d\n", t2);
                    exit(-1);
                }
            }
            total_player = total_player + 4; /// Increasing Total Player.
        }
        else if(enter == 2)
        {
            for(i = total_player ; i < (total_player + 4); i++) //total_player keeps truck of Previous total (German + Italian) Players.
            {
                t1 = pthread_create (&thread1[1], NULL, german_players_arrived, (void *) t); //Creating 4 thread for German Players.
                if (t1)
                {
                    printf("ERROR; return code from pthread_create() is %d\n", t1);
                    exit(-1);
                }
            }
            total_player = total_player + 4; //Increasing Total Player.
        }
        else if(enter == 3)
        {
            g = 2; //Variable for German Player.
            it = 2; //Variable for Italian Player
            for(i = total_player ; i < (total_player + 4); i++) // total_player keeps truck of Previous total (German + Italian) Players.
            {
                if(g!=0)
                {
                    t1 = pthread_create (&thread1[i], NULL, german_players_arrived, (void *) t); /// Creating 2 thread for German Players.
                    if (t1)
                    {
                        printf("ERROR; return code from pthread_create() is %d\n", t1);
                        exit(-1);
                    }
                    g--; /// Decreasing Variable for German Player
                }
                if(it!=0)
                {
                    t2 = pthread_create (&thread1[i], NULL, italian_players_arrived, (void *) t); ///Creating 2 thread for Italian Players.
                    if (t2)
                    {
                        printf("ERROR; return code from pthread_create() is %d\n", t2);
                        exit(-1);
                    }
                    it--; /// Decreasing Variable for ItalianPlayer.
                }
            }
            total_player = total_player + 4; /// Increasing Total Player.
        }
        else if(enter == 4)
        {
            g = 1; //Variable for German Player.
            it = 3; //Variable for Italian Player.
            for(i = total_player ; i < (total_player + 4); i++) //total_player keeps truck of Previous total (German + Italian) Players.
            {
                if(g!=0)
                {
                    t1 = pthread_create (&thread1[i], NULL, german_players_arrived, (void *) t); /// Creating 1 thread for German Players.
                    if (t1)
                    {
                        printf("ERROR; return code from pthread_create() is %d\n", t1);
                        exit(-1);
                    }
                    g--; //Decreasing Variable for German Player.
                }
                if(it!=0)
                {
                    t2 = pthread_create (&thread1[i], NULL, italian_players_arrived, (void *) t); /// Creating 3 thread for Italian Players.
                    if (t2)
                    {
                        printf("ERROR; return code from pthread_create() is %d\n", t2);
                        exit(-1);
                    }
                    it--; /// Decreasing Variable for ItalianPlayer.
                }
            }
            total_player = total_player + 4; /// Increasing Total Player.
        }
        else if(enter == 5)
        {
            g = 3; //Variable for German Player.
            it = 1; //Variable for Italian Player.
            for(i = total_player ; i < (total_player + 4); i++) //total_player keeps truck of Previous total (German + Italian) Players.
            {
                if(g!=0)
                {
                    t1 = pthread_create (&thread1[i], NULL, german_players_arrived, (void *) t); /// Creating 3 thread for German Players.
                    if (t1)
                    {
                        printf("ERROR; return code from pthread_create() is %d\n", t1);
                        exit(-1);
                    }
                    g--; /// Decreasing Variable for German Player.
                }
                if(it!=0)
                {
                    t2 = pthread_create (&thread1[i], NULL, italian_players_arrived, (void *) t); /// Creating 1 thread for Italian Players.
                    if (t2)
                    {
                        printf("ERROR; return code from pthread_create() is %d\n", t2);
                        exit(-1);
                    }
                    it--; /// Decreasing Variable for ItalianPlayer.
                }
            }
            total_player = total_player + 4; /// Increasing Total Player.
        }
        else if(enter==6)
        {
            printf("\033[1;32m"); //Green Color for this print.
            printf("How many Italian Players? ");
            printf("\033[0m");
            scanf("%d",&ip); /// User Input of Italian Players.
            printf("\033[1;32m"); //Green Color for this print.
            printf("How many German Players? ");
            scanf("%d",&gp); /// User Input of German Players.
            printf("\033[0m");
            printf("\n");
            tp= ip + gp; /// tp is total (German + Italian) Players given by user.
            int f;
            f = total_player + tp; /// f is (Previous Total Player of (German + Italian) Players) + (total (German + Italian) Players given by user).
            for(i = total_player ; i < f; i++)
            {
                if(gp!=0)
                {
                    t1 = pthread_create (&thread1[i], NULL, german_players_arrived, (void *) t); /// Creating needed threads for German Player as user input.
                    if (t1)
                    {
                        printf("ERROR; return code from pthread_create() is %d\n", t1);
                        exit(-1);
                    }
                    gp--; /// Decreasing Variable for German Player.
                }
                if(ip!=0)
                {
                    t2 = pthread_create (&thread1[i], NULL, italian_players_arrived, (void *) t); /// Creating needed threads for ItalianPlayer as user input
                    if (t2)
                    {
                        printf("ERROR; return code from pthread_create() is %d\n", t2);
                        exit(-1);
                    }
                    ip--; /// Decreasing Variable for ItalianPlayer.
                }
            }
            total_player = total_player + tp; /// Increasing Total Player.
        }
        else if(enter==0)
        {
            sem_destroy(&italian); /// Destroy Semaphores.
            sem_destroy(&german); /// Destroy Semaphores.
            printf("\n\t\t    -------- Thank You!--------\n");
            exit(0); /// If User Enters 0 Program Exits.
        }
        else
        {
            printf("\nWrong Input. Try Again"); /// If User Enters less then 0 and greater than 6. User needs to Try Again.
        }
    }
    for(i = 0 ; i < total_player; i++)
    {
        pthread_join (thread1[i],NULL); /// Join All The Thread.
    }

    sem_destroy(&italian); /// Destroy Semaphores.
    sem_destroy(&german); /// Destroy Semaphores.
    printf("\n\t\t    -------- Thank You!--------\n");

    exit(0);
}
