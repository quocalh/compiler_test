#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <stdlib.h>

// argc can be ommited since C23
#define PORT_ARG_CONFIG void* station, void* self, ... // variadic
#define STATION_ARG_CONFIG void *station, void* self, va_list arg // va_list

// visitor design pattern but with different terminology
// use tablet to determine the struct

typedef struct{
    void* (*connect)(PORT_ARG_CONFIG);
    const char* name;
    int age;
    int level;
} Player;

typedef struct{
    void* (*human_port)(STATION_ARG_CONFIG); 
    void* (*monster_port)(STATION_ARG_CONFIG);
} Station;

// passing va_list into another func: https://stackoverflow.com/questions/36881533/passing-va-list-to-other-functions
// count can be ommited since C23 for va_start
// each va_start va_end must be called at the same function
void* human_port(PORT_ARG_CONFIG)
{
    Station* _station = station;
    va_list ap, passing_ap;
    // duplicate the ap to pass it down
    va_start(ap); 

    va_copy(passing_ap, ap);
    void* ptr = _station->human_port(station, self, passing_ap);
    va_end(passing_ap);

    va_end(ap);
    return ptr;
}
void* monster_port(PORT_ARG_CONFIG)
{
    Station* _station = station;
    va_list ap, passing_ap;
    va_start(ap);

    va_copy(passing_ap, ap);
    void* ptr = _station->monster_port(station, self, passing_ap);
    va_end(passing_ap);

    va_end(ap);
    return ptr;
}

Player* PlayerInit(int age, int level, const char* name)
{
    Player* ptr = malloc(sizeof(*ptr));
    if (!ptr) {assert(0); return NULL;}
    
    *ptr = (Player){
        .age = age, .level = level, .name = name, .connect = human_port
    };
    return ptr;
}


void* StationHumanLoadSayHello(void* station, void* self, va_list ap)
{
    printf("hello: %d, %d, %d, %d\n",
        va_arg(ap, int), 
        va_arg(ap, int), 
        va_arg(ap, int),
        va_arg(ap, int)
    );
    va_end(ap);
}

void* StationMonsterLoadSayHello(void* station, void* self, va_list ap)
{
    printf("hello: %d, %d, %d, %d\n",
        va_arg(ap, int) * 2, 
        va_arg(ap, int) * 2, 
        va_arg(ap, int) * 2,
        va_arg(ap, int) * 2
    );
    va_end(ap);
}

void StationLoad(Station* station)
{
    station->human_port = StationHumanLoadSayHello;
    station->monster_port = StationMonsterLoadSayHello;
}







void example1(void* station, void* self, ...)
{
    va_list ap; // arg pointer
    va_start(ap);

    int tmp = va_arg(ap, int); // get the int value
    const char* tmp1 = va_arg(ap, const char*); // get the str pointer value
    
    // DONT START IN THE MIDDLE OF ANOTHER VA_LIST QUERY INTERVAl
    va_start(ap);
    int tmp3 = va_arg(ap, int);
    const char* tmp4 = va_arg(ap, const char*);
    double tmp5 = va_arg(ap, double);
    va_end(ap);
    
    // GUARANTEE GARBAGE VALUE
    double tmp2 = va_arg(ap, double); // get the double value

    printf("tmp\n");
    va_end(ap);
}

int main_tmp()
{
    printf("hello world\n");
    
    Player* player = PlayerInit(23, 56, "John");
    Player* monster = PlayerInit(3434, 586, "Pork");
    monster->connect = monster_port;
    
    Station station;
    StationLoad(&station);
    
    player->connect(&station, player, 1, 2, 3, 4);
    monster->connect(&station, monster, 1, 2, 3, 4);
    
    example1(&station, NULL, 1, "we are charlie kirk", 6.67);
    
    free(player);
    free(monster);
    
}
