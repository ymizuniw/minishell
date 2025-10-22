



//plot1

#define unique_set "fdas4312ghkj5678"

char *gen_unique_name()
{
    char *name=NULL;
    int  rand=0;
    name=getenv("$PPID");
    rand=atoi(name);

    int mod=0;

    size_t i=0;
    char *tmp = malloc(sizeof(char)*(PATH_MAX+1));
    while (1)
    {
        //cpy mod idx from unique set to tmp,

        //check if it already exists
            rand -= 3;
            continue ;
        //if not exit
            return(tmp);
    }
}

//plot2
char *unique_filename(char *filename, size_t len, int *charset[], size_t times)
{
    char c;
    char n;
    char sign;

    size_t mod=0;
    size_t i=0;

    while (i<len)
    {
        mod = len%i;
        if (mod==0)
        {
            int cur = (int)filename[i];
            if (charset[cur]<times)
                filename[i]=
        }
        if (mod==1)
        {

        }
        if (mod==2)
        {

        }
    }

}


char *gen_filename(char *filename)
{
    while (1)
    {
        int fd = open(filename, O_CREAT | O_EXCL);
        if (fd < 0)
        {

        }
    }
}
