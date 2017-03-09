#ifndef FRED_H_INCLUDED
#define FRED_H_INCLUDED

#ifdef __cplusplus
class Fred {
public:
Fred();
void wilma(int);
private:
int a_;
};
#else
typedef
struct Fred
Fred;
#endif
#ifdef __cplusplus
extern "C" {
#endif
#if defined(__STDC__) || defined(__cplusplus)
extern void c_function(Fred*); /* ANSI C prototypes */
extern Fred* cplusplus_callback_function(Fred*);
#else
extern void c_function(); /* K&R style */
extern Fred* cplusplus_callback_function();
#endif
#ifdef __cplusplus
}
#endif

#endif // FRED_H_INCLUDED
