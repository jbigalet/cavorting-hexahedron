typedef char i8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef short i16;
typedef int i32;
typedef unsigned int u32;
#ifdef OS_WINDOWS
typedef unsigned long long u64;
typedef long long i64;
#else
typedef unsigned long u64;
typedef long i64;
#endif
typedef float f32;
typedef double f64;
typedef char* cstring;
typedef const char *ccstring;

#define array_size(a) (sizeof(a)/sizeof(a[0]))

#define DEG_TO_RAD ((f32)(M_PI/180))
static inline void assert_impl(bool cond, const i8 *expr,
                               const i8 *file, i32 line) {
    if (!cond) {
        fprintf(stderr, "assertion failed : '%s'\n", expr);
        fprintf(stderr, "  at %s:%d\n", file, line);
        abort();
    }
}
#define check(c) assert_impl((c), #c, __FILE__, __LINE__)
#ifdef assert
#undef assert
#endif
#define assert check
//#define assert(...)
static inline void unreachable_impl(ccstring file, i32 line) {
    fprintf(stderr, "reached unreachable statement\n");
    fprintf(stderr, "  at %s:%d\n", file, line);
    abort();
}
#ifdef OS_WINDOWS
#define unreachable() { unreachable_impl(__FILE__,__LINE__); }
#else
#define unreachable() { unreachable_impl(__FILE__,__LINE__); __builtin_unreachable(); }
#endif



using std::min;
using std::max;
using std::isnan;
using std::cos;
using std::sin;
using std::atan2;
using std::abs;
using std::round;
using std::copysign;
using std::asin;
using std::trunc;



template<typename K, typename V>
static V get_default(std::map<K, V> m, K key, V def) {
    auto it = m.find(key);
    if(it == m.end())
        return def;
    return it->second;
}

template<typename T>
static bool from_string(std::string s, T* out) {
    std::stringstream ss(s);
    T res;
    if((ss >> res).fail())
        return false;
    if(ss.rdbuf()->in_avail() != 0)  // there are leftovers (123xy -> xy)
        return false;
    *out = res;
    return true;
}

#if defined(OS_LINUX)
#define WEAK __attribute__((weak))
#define EXPORT
#elif defined(OS_WINDOWS)
#define WEAK __declspec(selectany)
#define EXPORT __declspec(dllexport)
#else
#error
#endif

#define TRANSFORM_BLUR_COUNT 32 // :(:(:(:(
#define NOISE_MIPMAP_COUNT 3
#define TRC 64 // TODO REMOVE ME
#define RENDER_PARAM_COUNT 256

// TODO replace glfwGetTIme & uncoment all this

/* struct log_runtime { */
/*     std::string name; */
/*     f64 t0; */
/*     log_runtime(std::string n): name(n) { */
/*         t0 = glfwGetTime(); */
/*     } */
/*     ~log_runtime() { */
/*         f64 t = glfwGetTime() - t0; */
/*         ccstring unit = "s"; */
/*         if (t < 1.0) { */
/*             t *= 100.0; */
/*             unit = "ms"; */
/*         } */
/*         printf("%s | done in %.2f %s\n", name.c_str(), t, unit); */
/*     } */
/* }; */


/* #define CPU_TIMER_AVG_COUNT 50 */
/* struct cpu_timer; */
/* extern std::vector<cpu_timer*> cpu_timers; */

/* // WARNING a cpu_timer must always either be static or on the heap. */
/* // also, you cant copy it */
/* struct cpu_timer { */
/*     std::string name; */
/*     f64 t0; */
/*     f64 results[CPU_TIMER_AVG_COUNT] = {0}; */
/*     u32 result_idx = 0; */
/*     bool full_results = false; */
/*     f64 total = 0; */
/*     bool active = false; */

/*     cpu_timer(std::string n): name(n) { */
/*         cpu_timers.push_back(this);  // TODO */ 
/*     } */

/*     void begin() { */
/*         active = true; */
/*         t0 = glfwGetTime(); */
/*     } */

/*     void end() { */
/*         f64 t = glfwGetTime() - t0; */
/*         f64 to_remove = results[result_idx]; */
/*         results[result_idx] = t; */
/*         result_idx = (result_idx+1)%CPU_TIMER_AVG_COUNT; */
/*         if(result_idx == 0) */
/*             full_results = true; */
/*         total = total - to_remove + t; */
/*     } */

/*     f32 avg() { */
/*         return total/(full_results ? CPU_TIMER_AVG_COUNT : result_idx)*1000;  // res in ms */
/*     } */
/* }; */

