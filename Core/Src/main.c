#include "main.h"
#include "ek_export.h"
#include "ek_io.h"

int main(void)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);
    systick_init();
    ek_io_init();
    ek_export_init();

    
    while (1)
    {
    }
}

void Error_Handler(void)
{
    while (1)
    {
    }
}
