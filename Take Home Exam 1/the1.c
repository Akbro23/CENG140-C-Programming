#include <stdio.h>
#include <math.h>
#define max(a, b) a > b ? (a) : (b)


int main(){
    static int wall[100000];
    long long length_of_wall;
    int number_of_tribes;
    long long t;
    long long width;
    int height;
    int time_of_first_attack;
    long long position;
    int speed;
    long long attack_interval;
    int number_of_attacks;
    int attack;
    long long current_position;
    long long dl;
    long long attacked_position;

    scanf("%lld %d", &length_of_wall, &number_of_tribes);
    
    for (t = 0; t <= length_of_wall; t++){
        wall[t] = 1;      
    }

    for (t = 0; t < number_of_tribes; t++){
        scanf("%lld %d %d %lld %d %lld %d", &width, &height, &time_of_first_attack, &position, &speed, &attack_interval, &number_of_attacks);
        if (speed > 0){        
            for (attack = 0; attack < number_of_attacks; attack++){
                current_position = position + attack * speed * attack_interval;
                if (current_position < 0)
                    continue;
                if (current_position > length_of_wall)
                    break;
                for (dl = 0; dl < width; dl++){
                    attacked_position = current_position + dl;
                    if (attacked_position > length_of_wall)
                        break;
                    wall[attacked_position] = max(wall[attacked_position], height); 
                }              
            }
        } else {
            position = position + width - 1;
            for (attack = 0; attack < number_of_attacks; attack++){
                current_position = position + attack * speed * attack_interval;            
                if (current_position > length_of_wall + width)             
                    continue;
                if (current_position < 0)
                    break;
                for (dl = 0; dl < width; dl++){
                    attacked_position = current_position - dl;
                    if (attacked_position < 0)
                        break;
                    wall[attacked_position] = max(wall[attacked_position], height); 
                }          
            }
        }
    }

    for (t = 0; t < length_of_wall; t++){
        printf("%d ", wall[t]);      
    }
    printf("%d\n", wall[t]);

    return 0;
}