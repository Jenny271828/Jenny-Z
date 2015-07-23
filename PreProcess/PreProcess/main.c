#include <stdio.h>
#include <stdlib.h>

#include "year.h"
#include "preprocess_data.h"
#include "preprocess_attributes.h"


int main(int argc, const char * argv[])
{
    preprocess_data(YEAR_08, YEAR_10);
    preprocess_attributes(YEAR_08, YEAR_10);
    return 0;
}
