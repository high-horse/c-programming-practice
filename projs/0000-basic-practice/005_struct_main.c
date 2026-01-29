#include <stdio.h>

struct car
{
    char *name;
    float price;
    int speed;
};
void set_car_price_pointer(struct car *c, float price);
void set_car_speed_arrow(struct car *c, int speed);

int main(void)
{
    // compare struct
    // you have to manually compare each fields instead of using memcmp()
}

void les_1_initialization(void)
{

    struct car ford;
    ford.name = "Ford f150";
    ford.price = 1000.99;
    ford.speed = 100;

    printf("Name %s\n", ford.name);
    printf("Price %f\n", ford.price);
    printf("Speed %d\n", ford.speed);

    struct car saturn = {"saturn sl/2", 999.99, 99};
    printf("Name %s\n", saturn.name);
    printf("Price %f\n", saturn.price);
    printf("Speed %d\n", saturn.speed);

    // price is not initialized, thus 0 is set
    struct car honda = {.price = 99.99, .name = "honda i5"};
    printf("Name %s\n", honda.name);
    printf("Price %f\n", honda.price);
    printf("Speed %d\n", honda.speed);
}

void les_2_pass_struct_to_func(void)
{
    struct car honda = {.name = "holda car"};
    set_car_price_pointer(&honda, 99.9);
    set_car_speed_arrow(&honda, 10);
    printf("Name %s\n", honda.name);
    printf("Price %f\n", honda.price);
    printf("Speed %d\n", honda.speed);
}
void set_car_price_pointer(struct car *c, float price)
{
    // c.price = price; this is mistake.
    // You have to deference the structbefore assigning / accesing properties.
    // you can to it  this way
    (*c).price = price;
}
void set_car_speed_arrow(struct car *c, int speed)
{
    c->speed = speed;
}

void les_3_copy_struct(void)
{
    struct car honda = {.name = "honda car", .price = 99.91, .speed = 10};
    struct car no_honda = honda;

    printf("Name honda => %s \t no_honda => %s\n", honda.name, no_honda.name);
    printf("Price honda => %f \t no_honda => %f\n", honda.price, no_honda.price);
    printf("Speed honda => %d \t no_honda => %d\n", honda.speed, no_honda.speed);

    // no change in honda, only change in no_honda.
    no_honda.name = "No nonda car";
    no_honda.price = 100.99;
    no_honda.speed = 1;
    printf("Name honda => %s \t no_honda => %s\n", honda.name, no_honda.name);
    printf("Price honda => %f \t no_honda => %f\n", honda.price, no_honda.price);
    printf("Speed honda => %d \t no_honda => %d\n", honda.speed, no_honda.speed);
}