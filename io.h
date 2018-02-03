struct buff {
    u8 *data;
    u32 size;

    static buff alloc_from_string(std::string str);
    // does _not_ include the null terminator
    static buff alloc_from_string(ccstring str);
    static buff share_from_string(ccstring str);

    std::string to_string() {
        u32 _size = size;
        if(data[size-1] == '\0')
            _size -= 1;
        std::string s((i8*)data, _size);  // TODO bad cast
        return s;
    }
};

template<typename T> struct array {
    T *data = NULL;
    u32 count = 0;
    T& operator[](u32 i)
    {
        assert(i < count);
        assert((u64)data % alignof(T) == 0);
        return data[i];
    }

    T operator[](u32 i) const {
        assert(i < count);
        assert((u64)data % alignof(T) == 0);
        return data[i];
    }

    operator buff() {
        return buff { (u8*)data, (u32) (sizeof(T) * count) };
    }

    operator buff() const {
        return buff { (u8*)data, (u32) (sizeof(T) * count) };
    }
    operator std::vector<T>()
    {
        return std::vector<T>(data, data + count);
    }

    T* begin() {
        return data;
    }

    T* end() {
        return &data[count];
    }

    const T* begin() const {
        return data;
    }

    const T* end() const {
        return &data[count];
    }
};

template <typename T> array<T> alloc_array(u32 count)
{
    return array<T> { count ? (T*)malloc(sizeof(T)*count) : NULL, count };
}

template <typename T> array<T> make_array(std::initializer_list<T> l) {
    array<T> r;
    r.data = (T*)malloc(l.size() * sizeof(T));
    r.count = l.size();
    memcpy(r.data, l.begin(), l.size()*sizeof(T));
    return r;
}

template <typename T> void free_array(array<T> &s)
{
    free(s.data);
    s.data = NULL;
}

// Warning, this obviously invalidates copies of the array
// also we don't perform any amortized resizing
// so client code should do it instead
template <typename T> void resize(array<T> *a, u32 new_count)
{
    //check(new_count > a->count);
    a->data = (T*)realloc(a->data, new_count * sizeof(T));
    a->count = new_count;
}

template <typename T>
void remove_element(array<T> *a, u32 index)
{
    assert(index < a->count);
    if (index < a->count-1) {
        memmove(&((*a)[index]), &((*a)[index+1]), (a->count - (index+1))*sizeof(T));
    }
    resize(a, a->count-1);
}

template <typename T>
void insert_element(array<T> *a, u32 index, T v)
{
    assert(index <= a->count);
    u32 count = a->count;
    resize(a, a->count+1);
    if (index < count) {
        memmove(&((*a)[index+1]), &((*a)[index]), (count - index)*sizeof(T));
    }
    (*a)[index] = v;
}

template <typename T> void push(array<T> *a, T v)
{
    resize(a, a->count+1);
    (*a)[a->count-1] = v;
}

template <typename T> void grow(array<T> *a, u32 n)
{
    resize(a, a->count+n);
}

template <typename T> void append(array<T> *a, array<T> b)
{
    u32 n = a->count;
    grow(a, b.count);
    for (u32 i = 0; i < b.count; i++) {
        (*a)[i+n] = b[i];
    }
}

template <typename T> array<T> copy(array<T> a)
{
    array<T> r = {};
    r.data = (T*)malloc(a.count * sizeof(T));
    r.count = a.count;
    if (a.data && r.data) {
        memcpy(r.data, a.data, sizeof(T) * a.count);
    } else {
        assert(a.count == 0);
    }
    return r;
}

template <typename T> array<T> typed(array<u8> s)
{
    assert(s.count % sizeof(T) == 0);
    return array<T> { (T*)s.data, (u32)(s.count / sizeof(T)) };
}

template <typename T> array<T> alloc_copy_array(buff mem)
{
    assert(mem.size % sizeof(T) == 0);
    array<T> s = alloc_array<T>(mem.size / sizeof(T));
    memcpy(s.data, mem.data, mem.size);
    return s;
}

template<typename T>
void shuffle(array<T>* a) {
    for(u32 i=0 ; i<a->count-1 ; i++) {
        u32 j = (u32)random_i32(i, a->count);
        T tmp = a->data[i];
        a->data[i] = a->data[j];
        a->data[j] = tmp;
    }
}

template <typename T>
void dedup(array<T>* a) {
    // slowest way possible?
    for(i32 i=a->count-1 ; i>=0 ; i--)
        for(i32 j=0 ; j<i ; j++)
            if((*a)[i] == (*a)[j]) {
                remove_element(a, i);
                break;
            }
}

struct byte_stream {
    buff out;
    u32 index;
};

EXPORT buff alloc_buff(u32);
void free(buff&);
void free(byte_stream&);

void seek(byte_stream *s, u32 index);
void skip(byte_stream *s, i32 index);
u32 remaining_size(byte_stream *s);
u8 *pointer(byte_stream *s);
void read_check(byte_stream *s, ccstring ref);
void read_check(byte_stream *s, std::initializer_list<u8> bytes);

bool is_whitespace_char(u8);
u32 skip_whitespaces(byte_stream*);
u32 rewind_whitespaces(byte_stream*);
buff read_white_separated_string(byte_stream *);
bool read_const_str(byte_stream*, ccstring, bool allow_whitespaces=false);

u8 read_u8(byte_stream*);
i8 read_i8(byte_stream*);
u16 read_u16(byte_stream*);
i16 read_i16(byte_stream*);
u32 read_u32(byte_stream*);
i32 read_i32(byte_stream*);
u64 read_u64(byte_stream*);
i64 read_i64(byte_stream*);
f32 read_f32(byte_stream*);
f64 read_f64(byte_stream*);

void write_u8(byte_stream*,u8);
void write_i8(byte_stream*,i8);
void write_u16(byte_stream*,u16);
void write_i16(byte_stream*,i16);
void write_u32(byte_stream*,u32);
void write_i32(byte_stream*,i32);
void write_u64(byte_stream*,u64);
void write_i64(byte_stream*,i64);
void write_f32(byte_stream*,f32);
void write_f64(byte_stream*,f64);


buff read_whole_file(ccstring name, bool crash_if_not_exists=true);
buff read_whole_file(std::string name, bool crash_if_not_exists=true);
void write_whole_file(ccstring name, buff out);

std::string working_directory();
std::string get_formatted_time();
// caller must free each string when done
array<cstring> list_directory(ccstring directory);
bool remove_directory(ccstring dir);
std::string rename_to_temporary(ccstring path);
std::string base_name(std::string path);
bool file_exists(std::string);
bool string_lower_case_equal(cstring a, cstring b);
bool string_lower_case_fuzzy_match(ccstring a, ccstring b);
bool case_insensitive_compare(cstring a, cstring b);

std::string get_full_path(std::string path);
bool path_equals(const std::string&, const std::string&);
void print_string(buff s);
void print_bytes(buff s);

u8 reverse_bits(u8);
u16 reverse_bits(u16);
u32 reverse_bits(u32);

std::string stringf(ccstring format, ...);
std::string path_join(std::initializer_list<ccstring> list);

#define sformat_stack(fmt, ...) \
    (_sformat_stack(fmt, __VA_ARGS__), _sformat_stack_finish(alloca(_sformat_stack_buffer.size)))
void _sformat_stack(ccstring cfmt, ...);
buff _sformat_stack_finish(void *stk_buff);
extern buff _sformat_stack_buffer;


#define check_errno(e)                              \
    errno_check_impl((e), #e, __FILE__, __LINE__)
#define check_w32(e) \
    w32_check_impl((e), #e, __FILE__, __LINE__)
void errno_check_impl(i32 ret, ccstring expr, ccstring file, i32 line);
#ifdef OS_WINDOWS
void w32_check_impl(i32 ret, ccstring expr, ccstring file, i32 line);
void w32_print_last_error();
#endif




// TODO handle multiple types

#if defined(OS_LINUX)
#define WATCH_DESCRIPTOR_TYPE i32
#define WATCH_EVENT_TYPE_MODIFY IN_MODIFY
#elif defined(OS_WINDOWS)
#define WATCH_DESCRIPTOR_TYPE HANDLE
#define WATCH_EVENT_TYPE_MODIFY FILE_NOTIFY_CHANGE_LAST_WRITE
#define WATCH_MAX_EVENTS   1000
#else
#error
#endif
struct watcher {
    WATCH_DESCRIPTOR_TYPE wd;  // watch descriptor
    std::string path;
    u32 flags;
#if defined(OS_WINDOWS)
    OVERLAPPED overlapped;
    i8 read_directory_change_buffer[WATCH_MAX_EVENTS*(sizeof(FILE_NOTIFY_INFORMATION)+MAX_PATH)] = {0};
    bool read_directory_wait = false;
#endif
    bool active = false;
};

struct watch_event {
    u32 type;
    std::string filename;
    std::string fullpath;
};

void add_watcher(watcher *, std::string path, u32 flags);
void enable_watcher(watcher*);
void disable_watcher(watcher*);
std::vector<watch_event> get_watcher_events(watcher* w);

u32 crc32(buff);

/* png stuff */

enum class png_pixel_format {
    R8G8_UNORM,
    R8G8B8_UNORM,
    R8G8B8A8_UNORM,
};

struct png_data {
    u32 width;
    u32 height;
    u8* data;
    png_pixel_format format;
};

png_data read_png(byte_stream *s);
png_data read_png(ccstring file);


/* inflate */

u32 inflate(buff in, buff out, u32 *pconsumed_size);
