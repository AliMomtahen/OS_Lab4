#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

void help()
{
    
    printf(1, "  set_queue <pid> <new_queue>\n");
    printf(1, "  set_one <pid> <priority_ratio> <arrival_time_ratio> <executed_cycle_ratio>\n");
    printf(1, "  set_all <priority_ratio> <arrival_time_ratio> <executed_cycle_ratio>\n");
}



void set_queue(int pid, int new_queue)
{
    if (pid < 1)
    {
        printf(1, "Invalid pid\n");
        return;
    }
    if (new_queue < 1 || new_queue > 3)
    {
        printf(1, "Invalid queue\n");
        return;
    }
    int res = change_proc_queue(pid, new_queue);
    if (res < 0)
        printf(1, "Error changing queue\n");
    else
        printf(1, "Queue changed successfully\n");
}


void set_one(int pid, int priority_ratio, int arrival_time_ratio, int executed_cycle_ratio
        , int process_size_ratio)
{
    if (pid < 1)
    {
        printf(1, "Invalid pid\n");
        return;
    }
    if (priority_ratio < 0 || arrival_time_ratio < 0 || executed_cycle_ratio < 0 || process_size_ratio < 0)
    {
        printf(1, "Invalid ratios\n");
        return;
    }
    // printf(1 ,"%d %d %d %d %d\n" , pid , priority_ratio , arrival_time_ratio , executed_cycle_ratio
    //     ,process_size_ratio);
    int res = change_param_bjf(pid , (float)priority_ratio,(float) arrival_time_ratio,(float) executed_cycle_ratio
        ,(float) process_size_ratio);
    if (res < 0)
        printf(1, "Error setting BJF params\n");
    else
        printf(1, "BJF params set successfully\n");
}

void set_all(int priority_ratio, int arrival_time_ratio, int executed_cycle_ratio , int process_size_ratio)
{
    if (priority_ratio < 0 || arrival_time_ratio < 0 || executed_cycle_ratio < 0 || process_size_ratio < 0)
    {
        printf(1, "Invalid ratios\n");
        return;
    }
    // printf(1 ,"%d %d %d %d %d\n" , 144 , priority_ratio , arrival_time_ratio , executed_cycle_ratio
    //     ,process_size_ratio);
    change_param_bjf_all(priority_ratio, arrival_time_ratio, executed_cycle_ratio , process_size_ratio);
    printf(1, "BJF params set successfully\n");
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        help();
        exit();
    }
    if (!strcmp(argv[1], "set_queue"))
    {
        if (argc < 4)
        {
            help();
            exit();
        }
        set_queue(atoi(argv[2]), atoi(argv[3]));
    }
    
    else if (!strcmp(argv[1], "set_one"))
    {
        if (argc < 6)
        {
            help();
            exit();
        }
        set_one(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]) , atoi(argv[6]));
    }
    else if (!strcmp(argv[1], "set_all"))
    {
        if (argc < 5)
        {
            help();
            exit();
        }
        set_all(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]) , atoi(argv[5]));
    }
    
    exit();
}
