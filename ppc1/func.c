/*
 * Illustration of declaration and use of pointer to function
 */

#include <stdio.h>

int main()
{
  void func1(int);
  void func2(int);

  /* The following is the prototype of a function that has two arguments.
   * The first argument of dispatch() pointer to a function.
   * The second argument of dispatch() is an int.
   *
   * The first argument of dispatch() points to a function that is of
   * type void, and has one argument that is an int
   */
  void dispatch(void(*)(int), int);

    /* The following says that addr is a pointer to a function.
     *
     * The function that it will point to:
     *   has type void, and has a single argument of type int
     */
    void (*addr)(int);

    int x;

    /*
     * First, call the functions directly, using pointer to func
     */
    x = 19;
    addr = func1;

    printf("\n\n   Calling addr(%d), with addr = func1\n", x);
    addr(x);

    x = 41;
    addr = func2;

    printf("\n   Calling addr(%d), with addr = func2\n", x);
    addr(x);

    /*
     * Now, show how to use the dispatch() function with different arguments
     */
    x = 19;
    addr = func1;

    printf("\n\n   Calling dispatch(addr, %d) with addr = func1\n", x);
    dispatch(addr, x);

    x = 41;
    addr = func2;

    printf("\n\n   Calling dispatch(addr, %d) with addr = func2\n", x);
    dispatch(addr, x);

    return 0;
}

void func1(int n)
{
  printf("       This is func1(): n = %d\n", n);
}

void func2(int n)
{
  printf("       This is func2(): n = %d\n", n);
}

void dispatch(void(*func)(int), int arg)
{
  printf("     In dispatch(), arg = %d\n", arg);
  (*func)(arg);
}
