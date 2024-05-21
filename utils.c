//detects single quotes closed
int closed_quotes_detector(char **argv)
{
    int sc=0;
    int dc =0;
    int i;
    
    while ()
    
    while (*str)
    {
        if (*str == "'")
            sc++;
        if (*str == '"')
            dc++;
        str++;
    }


    if (sc !=0 && sc % 2 == 0)
        return 1;
    else
        return 3;
    
    if (sc !=0 && dc % 2 == 0)
        return 2;
    else
        return 3;
    
    return 0;
}

int main (int argc, char **argv, char **envp)
{

    return 0;
}