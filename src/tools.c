long long myAtoI(char* a)
{
    long long ret = 0;
    int i = 0;
    while(a[i] != '\0')
    {
        ret *= 10;
        ret += a[i] - '0';
        i++;
    }

    return ret;
}