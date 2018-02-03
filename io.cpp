#ifndef STD_PCH
#include "std.h"
#endif
#pragma hdrstop ( "build/std.h.pch" )

#define GLOBAL_STATE __declspec(dllexport)

#include "io.h"



void errno_check_impl(i32 ret, ccstring expr, ccstring file, i32 line)
{
    if (ret != 0) {
        i32 err = errno;
        fprintf(stderr, "system call failed : '%s'\nerror : %s (%d)\n", expr, strerror(err), err);
        fprintf(stderr, "  at %s:%d\n", file, line);
        abort();
    }
}

#ifdef OS_WINDOWS
void w32_print_last_error()
{
	DWORD err = GetLastError();
	LPSTR msg = nullptr;
	u32 size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
							   NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&msg, 0, NULL);


	fprintf(stderr, "error : %s (%u)\n", msg, err);
}

void w32_check_impl(i32 ret, ccstring expr, ccstring file, i32 line)
{
    if (ret == 0) {
        w32_print_last_error();
		fprintf(stderr, "expr : '%s'\n", expr);
		fprintf(stderr, "  at %s:%d\n", file, line);
        abort();
    }
}
#endif

bool rename_to_numbered(ccstring path, u32 n, std::string *res)
{
#ifdef OS_WINDOWS
    std::string alt(path);
    alt += "_" + std::to_string(n);
    if (MoveFile(path, alt.c_str()) != 0) {
        *res = alt;
        return true;
    }
    // TODO check that the error code is reasonable
    return false;
#else
    unreachable();
#endif
}

std::string rename_to_temporary(ccstring path)
{
    u32 len = strlen(path);
    check(len >= 1);
#if defined(OS_WINDOWS)
    check(path[len-1] != '\\');
#elif defined(OS_LINUX)
    check(path[len-1] != '/');
#else
    unreachable();
#endif
    u32 temp_n = 0;
    while (temp_n < 200) {
        std::string result;
        if (rename_to_numbered(path, temp_n++, &result)) {
            return result;
        }
    }
    unreachable();
}

void write_to_stdout(buff data);

buff buff::alloc_from_string(std::string str) {
    u8* data = (u8*)malloc(str.size());
    std::copy(str.begin(), str.end(), data);
    // TODO checked conv ?
    return { data, (u32)str.size() };
}
// does _not_ include the null terminator
buff buff::alloc_from_string(ccstring str) {
    u32 n = strlen(str); // TODO same ?
    u8 *data = (u8*)malloc(n);
    memcpy(data, str, n);
    return { data, n };
}
buff buff::share_from_string(ccstring str) {
    u32 n = strlen(str);
    return { (u8*)str, n };
}



buff alloc_buff(u32 size)
{
    return buff { (u8*)malloc(size), size };
}

void free(buff& b) {
    free(b.data);
    // not strictly necessary but good to have at least in debug to fail fast
    // instead of double free / use after free
    // in release we can avoid that and maybe not have to take a ref
    b.data = NULL;
}

void free(byte_stream& s) {
    free(s.out);
}

u32 ilog10(u32);

u32 format_max_size(buff fmt)
{
    u8 *f = fmt.data;
    u32 max_size = 0;
    u32 i = 0;
    while (i < fmt.size) {
        u8 c = f[i++];
        if (c == '%') {
            check(i < fmt.size);
            u8 c2 = f[i++];
            if (c2 == '%') {
                max_size++;
                continue;
            }
            if (c2 == 'd') {
                max_size += 11; // log10(2^31) + 1
                continue;
            }
            if (c2 == 'u') {
                max_size += 10; // log10(2^32)
                continue;
            }
            unreachable();
        } else {
            max_size++;
        }
    }
    return max_size;
}

u32 format_max_size(ccstring fmt)
{
    return format_max_size(buff { (u8*)fmt, (u32)strlen(fmt) });
}

u32 vformat_size(buff fmt, va_list va)
{
    u8 *f = fmt.data;
    u32 size = 0;
    u32 i = 0;
    while (i < fmt.size) {
        u8 c = f[i++];
        if (c == '%') {
            check(i < fmt.size);
            u8 c2 = f[i++];
            if (c2 == '%') {
                size++;
                continue;
            }
            if (c2 == 'd') {
                i32 v = va_arg(va, i32);
                if (v == 0) size++;
                else if (v < 0) {
                    size += ilog10((u32)-v) + 1;
                } else {
                    size += ilog10((u32)v);
                }
                continue;
            }
            if (c2 == 'u') {
                u32 v = va_arg(va, u32);
                if (v == 0) size++;
                else size += ilog10(v);
                continue;
            }
            if (c2 == 's') {
                ccstring s = va_arg(va, ccstring);
                size += strlen(s);
                continue;
            }
            unreachable();
        } else {
            size++;
        }
    }
    return size;
}

std::string stringf(ccstring format, ...)
{
    va_list va;
    va_start(va, format);
    u32 n = vsnprintf(NULL, 0, format, va);
    va_end(va);
    std::string b(n+1, '\0');
    va_start(va, format);
    vsnprintf(&b[0], n+1, format, va);
    b.resize(n);
    va_end(va);
    return b;
}

#ifdef OS_LINUX
#define PATH_SEP '/'
#elif defined(OS_WINDOWS)
#define PATH_SEP '\\'
#else
#error
#endif

std::string path_join(std::initializer_list<ccstring> list) {
    u32 size = 0;
    for(ccstring s: list)
        size += strlen(s);
    size += list.size();

    std::string str(size, '\0');
    u32 i = 0;
    for(ccstring s: list) {
        if(i != 0)
            str[i++] = PATH_SEP;
        u32 l = strlen(s);
        memcpy(&str[i], &s[0], l);
        i += l;
    }

    return str;
}

u32 format_size(ccstring cfmt, ...)
{
    va_list va;
    va_start(va, cfmt);
    u32 n = vformat_size(buff {(u8*)cfmt, (u32)strlen(cfmt)}, va);
    va_end(va);
    return n;
}
    

u32 vsformat(buff out, buff fmt, va_list va)
{
    u8 *f = fmt.data;
    u8 *o = out.data;
    u32 i = 0, j = 0;
    while (i < fmt.size) {
        u8 c = f[i++];
        if (c == '%') {
            check(i < fmt.size);
            u8 c2 = f[i++];
            if (c2 == '%') {
                check(j < out.size);
                o[j++] = '%';
                continue;
            }
            if (c2 == 'd' || c2 == 'u') {
                u32 uv;
                if (c2 == 'u')
                    uv = va_arg(va, u32);
                else {
                    i32 v = va_arg(va, i32);
                    if (v < 0) {
                        check(j < out.size);
                        o[j++] = '-';
                        uv = (u32)-v;
                    } else {
                        uv = (u32)v;
                    }
                }
                if (uv == 0) o[j++] = '0';
                else {
                    u32 size = ilog10(uv);
                    check(j + size <= out.size);
                    for (i32 k = size-1; k >= 0; k--) {
                        o[j + k] = '0' + (uv%10);
                        uv = uv/10;
                    }
                    j += size;
                    assert(uv == 0);
                }
                continue;
            }
            if (c2 == 's') {
                ccstring s = va_arg(va, ccstring);
                u32 n = strlen(s);
                check(j + n <= out.size);
                memcpy(&o[j], s, n);
                j += n;
                continue;
            }
            if (c2 == '0') {
                o[j++] = 0;
                continue;
            }
            unreachable();
        } else {
            check(j < out.size);
            o[j++] = c;
        }
    }
    va_end(va);
    assert(j <= out.size);
    return j;
}

buff sformat_alloc(ccstring cfmt, ...)
{
    va_list va;
    va_start(va, cfmt);
    buff fmt = buff::share_from_string(cfmt);
    buff out = alloc_buff(vformat_size(fmt, va));
    va_end(va);
    va_start(va, cfmt);
    vsformat(out, fmt, va);
    va_end(va);
    return out;
}

#define sformat_stack(fmt, ...) \
    (_sformat_stack(fmt, __VA_ARGS__), _sformat_stack_finish(alloca(_sformat_stack_buffer.size)))


// TODO THREAD should be thread local
u8 _sformat_stack_data[2048];
buff _sformat_stack_buffer { _sformat_stack_data, 0 };

void _sformat_stack(ccstring cfmt, ...)
{
    va_list va;
    buff fmt = buff::share_from_string(cfmt);
    va_start(va, cfmt);
    check(_sformat_stack_buffer.size == 0);
    _sformat_stack_buffer.size = vsformat( buff {_sformat_stack_data, array_size(_sformat_stack_data) }, fmt, va);
    va_end(va);
}

buff _sformat_stack_finish(void *stk_buff)
{
    u32 n = _sformat_stack_buffer.size;
    memcpy(stk_buff, _sformat_stack_buffer.data, n);
    _sformat_stack_buffer.size = 0;
    return buff { (u8*)stk_buff, n };
}

void sformat(buff out, ccstring cfmt, ...)
{
    va_list va;
    buff fmt = buff::share_from_string(cfmt);
    va_start(va, cfmt);
    vsformat(out, fmt, va);
    va_end(va);
}

// TODO THREAD should be thread local
u8 _pformat_data[2048];
buff _pformat_buffer { _pformat_data, 0 };

void pformat(ccstring cfmt, ...)
{
    va_list va;
    buff fmt = buff::share_from_string(cfmt);
    va_start(va, cfmt);
    check(_pformat_buffer.size == 0);
    _pformat_buffer.size = vsformat(buff { _pformat_data, array_size(_pformat_data) }, fmt, va);
    va_end(va);
    // TODO use platform primitives
    // especially since this will stop at the first NUL
    write_to_stdout(_pformat_buffer);
    //printf("%.*s", _pformat_buffer.size, _pformat_buffer.data);
    _pformat_buffer.size = 0;
}

buff read_whole_file(std::string name, bool crash_if_not_exists) {
    return read_whole_file(name.c_str());
}


#ifdef OS_LINUX

buff read_whole_file(ccstring name, bool crash_if_not_exists)
{
    i32 fd = open(name, O_RDONLY);
    if (fd == -1) {
        if(crash_if_not_exists) {
            i32 err = errno;
            fprintf(stderr, "Could not open '%s' : %s.\n", name, strerror(err));
            unreachable();
        } else {
            return { NULL, 0 };
        }
    }
    i64 size = lseek(fd, 0, SEEK_END);
    check(size != -1); // TODO errno
    u8 *data = (u8*)malloc(size);
    i64 actually_read = pread(fd, data, size, 0);
    if (actually_read == -1) {
        i32 err = errno;
        fprintf(stderr, "Could not read '%s' : %s.\n", name, strerror(err));
        unreachable();
    }
    close(fd);
    check(actually_read == size); // TODO handle short reads
    return buff { data, (u32)size };
}


void write_whole_file(ccstring name, buff out)
{
    i32 fd = open(name, O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd == -1) {
        i32 err = errno;
        fprintf(stderr, "Could not open '%s' : %s.\n", name, strerror(err));
        unreachable();
    }
    i64 actually_write = write(fd, out.data, out.size);
    if (actually_write == -1) {
        i32 err = errno;
        fprintf(stderr, "Could not write '%s' : %s.\n", name, strerror(err));
        unreachable();
    }
    close(fd);
    check(actually_write == out.size); // TODO handle short writes
}

void write_to_stdout(buff data)
{
    i64 actually_wrote = write(1, data.data, data.size);
    check(actually_wrote == data.size); // TODO same
}

#endif


#ifdef OS_LINUX

struct linux_dirent {
   long           d_ino;
   off_t          d_off;
   unsigned short d_reclen;
   char           d_name[];
};



#endif

std::string base_name(std::string path)
{
    std::string res;
#if defined(OS_LINUX)
    // TODO this is completly gore
    cstring namedup = strdup(path.c_str());
    cstring base = basename(namedup);
    res = base;
    free(namedup);
#elif defined(OS_WINDOWS)
    char drive[_MAX_DRIVE];  
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME]; 
    char ext[_MAX_EXT];
    _splitpath(path.c_str(), drive, dir, fname, ext);
    res += fname;
    res += ext;
#else
#error
#endif
    return res;
}

// each string is mallocd :(
array<cstring> list_directory(ccstring directory)
{
    array<cstring> items = { NULL, 0 };
#if defined(OS_LINUX)
    DIR* dirp = opendir(directory);
    dirent* dp = NULL;
    while((dp = readdir(dirp)) != NULL) {
        if(dp->d_ino == 0
                || strcmp(dp->d_name, ".") == 0
                || strcmp(dp->d_name, "..") == 0)
            continue;

        push(&items, strdup(dp->d_name));
    }
    closedir(dirp);
#elif defined(OS_WINDOWS)
    char buff[1024];// TODO again :( :( :( :(
    sprintf(buff, "%s\\*", directory);
    WIN32_FIND_DATA ffd;
    HANDLE h = FindFirstFile(buff, &ffd);
    bool success = h != INVALID_HANDLE_VALUE;
    while(success)
    {
        if (strcmp(ffd.cFileName, ".") != 0 &&
                strcmp(ffd.cFileName, "..") != 0) {
        push(&items, strdup(ffd.cFileName));
        }
        success = FindNextFile(h, &ffd);
    }
    FindClose(h);
#else
#error
#endif
    return items;
}

bool remove_file(ccstring dir, ccstring file)
{
    // TODO actual path manip
#if defined(OS_LINUX)
    cstring path = (cstring)sformat_stack("%s/%s%0", dir, file).data;
    check_errno(unlink(path));
#elif defined(OS_WINDOWS)
    cstring path = (cstring)sformat_stack("%s\\%s%0", dir, file).data;
    printf("Deleteing : %s\n", path);
    check_w32(DeleteFile(path) != 0);
#endif
    return true; // TODO actual error handling
}

bool empty_directory(ccstring dir)
{
    // TODO recursive
    array<cstring> items = list_directory(dir);
    for (u32 i = 0; i < items.count; i++) {
        if (!remove_file(dir, items[i]))
            return false;
        free(items[i]);
    }
    free_array(items);
    // TODO actual error handling
    return true;
}

bool remove_directory(ccstring dir)
{
    empty_directory(dir);
#if defined(OS_LINUX)
    check_errno(rmdir(dir));
#elif defined(OS_WINDOWS)
    check_w32(RemoveDirectory(dir));
#else
#error
#endif
    // TODO ditto
    return true;
}

bool file_exists(std::string dir)
{
#if defined(OS_WINDOWS)
    return (bool)PathFileExists(dir.c_str());
#elif defined(OS_LINUX)
    return access(dir.c_str(), F_OK) != -1;
#else
#error
#endif
}


#if defined(OS_LINUX)
#define OS_PATH_SEPARATOR "/"
#elif defined(OS_WINDOWS)
#define OS_PATH_SEPARATOR "\\"
#else
#error
#endif

std::string get_full_path(std::string path) {
#if defined(OS_LINUX)
    char full_path[PATH_MAX];
    realpath(path.c_str(), full_path);
    return full_path;
#elif defined(OS_WINDOWS)
    TCHAR full_path[MAX_PATH];
    GetFullPathName(path.c_str(), MAX_PATH, full_path, NULL);
    return full_path;
#else
#error
#endif
}

bool case_insensitive_compare(cstring a, cstring b) {
    while(true) {
        if(tolower(*a) < tolower(*b))
            return true;
        if(tolower(*a) > tolower(*b))
            return false;
        if(*a == '\0')
            return true;
        a++;
        b++;
    }
}

bool string_lower_case_equal(cstring a, cstring b) {
    while(true) {
        if(tolower(*a) != tolower(*b))
            return false;

        if(*a == '\0')
            return true;

        a++;
        b++;
    }
}

bool string_lower_case_equal(const std::string& a, const std::string& b)
{
    u32 sz = a.size();
    if (b.size() != sz)
        return false;
    for (u32 i = 0; i < sz; ++i)
        if (tolower(a[i]) != tolower(b[i]))
            return false;
    return true;
}

bool string_lower_case_fuzzy_match(ccstring str, ccstring tosearch) {
    u32 str_sz = strlen(str);
    u32 tosearch_sz = strlen(tosearch);
    u32 str_idx = 0;
    for(u32 i=0 ; i<tosearch_sz ; i++) {
        char c = tosearch[i];
        if(c >= 'A' && c <= 'Z')
            c -= ('A'-'a');

        bool found_char = false;
        while(str_idx < str_sz) {
            char ch = str[str_idx];
            if(ch >= 'A' && ch <= 'Z')
                ch -= ('A'-'a');
            if(ch == c) {
                found_char = true;
                break;
            }
            str_idx++;
        }
        str_idx++;
        if(!found_char)
            return false;
    }
    return true;
}

bool path_equals(const std::string& a, const std::string& b)
{
#ifdef OS_WINDOWS
    return string_lower_case_equal(a, b);
#else
    return a == b;
#endif
}


#ifdef OS_WINDOWS

buff read_whole_file(ccstring name, bool crash_if_not_exists)
{
    HANDLE fd = CreateFile(name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (fd == INVALID_HANDLE_VALUE) {
        if(crash_if_not_exists) {
            printf("Failed to open '%s'\n", name);
            w32_print_last_error();
            unreachable(); // TODO print error reason
        } else {
            return { NULL, 0 };
        }
    }
    LARGE_INTEGER large_size;
    check(GetFileSizeEx(fd, &large_size));
    u32 size = large_size.QuadPart; // TODO check for overflo
    DWORD actually_read = 0;
    u8 *data = (u8*)malloc(size);
    check(ReadFile(fd, data, size, &actually_read, NULL));
    CloseHandle(fd);
    return { data, (u32)size };
}

void write_whole_file(ccstring name, buff out)
{
    HANDLE fd = CreateFile(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    check_w32(fd != INVALID_HANDLE_VALUE);
    DWORD actually_write = 0;
    check_w32(WriteFile(fd, out.data, out.size, &actually_write, 0));
    check(actually_write == out.size);
    CloseHandle(fd);
}

void write_to_stdout(buff data)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    check(h && h != INVALID_HANDLE_VALUE);
    DWORD actually_wrote = 0;
    WriteConsole(h, data.data, data.size, &actually_wrote, NULL);
    check(actually_wrote == data.size);
}

#endif

void check_available(byte_stream *s, u32 n)
{
    check(s->index + n <= s->out.size);
}

void read_check(byte_stream *s, const u8 *ref, u32 len)
{
    check_available(s, len);
    if (memcmp(s->out.data + s->index, ref, len) != 0) {
        fprintf(stderr, "Error while reading stream. Expected '%s', got '%.*s'.\n", ref, len, s->out.data + s->index);
        unreachable();
    }
    s->index += len;
}

void read_check(byte_stream *s, ccstring ref)
{
    read_check(s, (const u8*)ref, strlen(ref));
}

void read_check(byte_stream *s, std::initializer_list<u8> bytes)
{
    read_check(s, bytes.begin(), bytes.size());
}

u8 *pointer(byte_stream *s)
{
    return s->out.data + s->index;
}

#define READ_SIMPLE(typ)                                    \
    typ read_##typ(byte_stream *s)                          \
    {                                                       \
        check_available(s, sizeof(typ));                    \
        typ v;                                              \
        memcpy(&v, s->out.data + s->index, sizeof(typ));    \
        s->index += sizeof(typ);                            \
        return v;                                           \
    }                                                       \
                                                            \
    void write_##typ(byte_stream *s, typ v)                 \
    {                                                       \
        check_available(s, sizeof(typ));                    \
        memcpy(pointer(s), &v, sizeof(typ));                \
        s->index += sizeof(typ);                            \
    }

READ_SIMPLE(u8)
READ_SIMPLE(i8)
READ_SIMPLE(u16)
READ_SIMPLE(i16)
READ_SIMPLE(u32)
READ_SIMPLE(i32)
READ_SIMPLE(u64)
READ_SIMPLE(i64)
READ_SIMPLE(f32)
READ_SIMPLE(f64)


void seek(byte_stream *s, u32 index)
{
    check(index <= s->out.size);
    s->index = index;
}

void skip(byte_stream *s, i32 index)
{
    check(s->index + index <= s->out.size);
    s->index += index;
}

u32 remaining_size(byte_stream *s)
{
    return s->out.size - s->index;
}



bool is_whitespace_char(u8 c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

u32 skip_whitespaces(byte_stream* s) {
    u32 skipped = 0;
    while(remaining_size(s) && is_whitespace_char(*pointer(s))) {
        skip(s, 1);
        skipped++;
    }
    return skipped;
}

u32 rewind_whitespaces(byte_stream* s) {
    u32 skipped = 0;
    while(s->index > 0 && is_whitespace_char(*(pointer(s)-1))) {
        skip(s, -1);
        skipped++;
    }
    return skipped;
}


bool read_const_str(byte_stream* s, ccstring str, bool allow_whitespaces) {
    u32 whitespaces = 0;
    if(allow_whitespaces)
        whitespaces = skip_whitespaces(s);
    u32 n = strlen(str);
    if(remaining_size(s) >= n && memcmp(pointer(s), str, n) == 0) {
        skip(s, n);
        return true;
    }
    s->index -= whitespaces;
    return false;
}

buff read_white_separated_string(byte_stream *s)
{
    skip_whitespaces(s);
    if(!remaining_size(s)) return buff { NULL, 0 };
    u32 start = s->index;
    u8 c;
    do {
        c = read_u8(s);
    } while (remaining_size(s) && !is_whitespace_char(c));
    return buff { s->out.data + start, s->index - start - 1 };
}


u8 reverse_bits(u8 v)
{
    v = ((v >> 1) & 0x55) | ((v & 0x55) << 1);
    v = ((v >> 2) & 0x33) | ((v & 0x33) << 2);
    v = ((v >> 4) & 0x0F) | ((v & 0x0F) << 4);
    return v;
}

u16 reverse_bits(u16 v)
{
    return ((u16)reverse_bits((u8)(v&0xff)) << 8) |
        reverse_bits((u8)(v >> 8));
}

u32 reverse_bits(u32 v)
{
    return ((u32)reverse_bits((u16)(v & 0xffff)) << 16) |
        reverse_bits((u16)(v >> 16));
}

bool is_ascii_printable(u8 c)
{
    return 0x20 <= c && c <= 0x7e;
}

void print_string(buff s)
{
    for (u32 i = 0; i < s.size; i++) {
        u8 c = s.data[i];
        if (is_ascii_printable(c)) {
            printf("%c", c);
        } else {
            printf("\\x%.2x", c);
        }
    }
}

void print_bytes(buff s)
{
    for (u32 i = 0; i < s.size; i++) {
        u8 b = s.data[i];
        printf("%x", b);
        if (is_ascii_printable(b))
            printf("(%c)", b);
        printf(" ");
    }
}

void init_deflate_tables();


void sleep_sec(double dt)
{
#ifdef OS_LINUX
    struct timespec t0={0,0},t1={0,0};
    i32 s = (i32)dt;
    i64 ns = (i64)((dt - s) * 1e9);
    t0.tv_sec  = s;
    t0.tv_nsec = ns;
    nanosleep(&t0 , &t1);
#endif
#ifdef OS_WINDOWS
        Sleep((u32)(dt*1000));
#endif
}

std::string working_directory() {
#if defined(OS_LINUX)
    i8 cwd[PATH_MAX];
    check(getcwd(cwd, sizeof(cwd)));
    return std::string(cwd);
#elif defined(OS_WINDOWS)
    i8 cwd[MAX_PATH];
    check(_getcwd(cwd, sizeof(cwd)));
    return std::string(cwd);
#else
#error
#endif
}


std::string get_formatted_time() {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%H:%M:%S");
    return ss.str();
}


#if defined(OS_WINDOWS)
std::string wchar_to_string(WCHAR* wchar, i32 length=-1) {
    std::wstring w;
    if(length < 0)
        w = std::wstring(wchar);
    else
        w = std::wstring(wchar, 0, length/sizeof(u16));

    std::string s(w.begin(), w.end());
    return s;
}
#endif


#if defined(OS_LINUX)

#define INOTIFY_EVENT_SIZE        (sizeof(inotify_event))
#define INOTIFY_EVENT_BUFFER_SIZE (1024*(INOTIFY_EVENT_SIZE+NAME_MAX+1))
i32 inotify_fd;
u8 inotify_event_buffer[INOTIFY_EVENT_BUFFER_SIZE];
pollfd poll_fd;

#elif defined(OS_WINDOWS)

#else
#error
#endif


bool watcher_initialized = false;
void init_watcher() {
#if defined(OS_LINUX)
    inotify_fd = inotify_init1(IN_NONBLOCK);
    check(inotify_fd >= 0);
    poll_fd = { inotify_fd, POLLIN, 0 };

#elif defined(OS_WINDOWS)
    // nothing to do
#else
#error
#endif
}

void enable_watcher(watcher *v) {
    if(!watcher_initialized) {
        init_watcher();
        watcher_initialized = true;
    }
    check(!v->active);
#if defined(OS_LINUX)
    WATCH_DESCRIPTOR_TYPE wd = inotify_add_watch(inotify_fd, v->path.c_str(), v->flags);
    check(wd >= 0);
    v->wd = wd;
#elif defined(OS_WINDOWS)
    OVERLAPPED watcher_overlapped = {0};

    HANDLE wd = CreateFile(
            v->path.c_str(),                                        // lpFileName
            FILE_LIST_DIRECTORY,                                 // dwDesiredAccess
            FILE_SHARE_READ | FILE_SHARE_WRITE,                  // dwShareMode
            NULL,                                                // lpSecurityAttributes
            OPEN_EXISTING,                                       // dwCreationDisposition
            FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,   // dwFlagsAndAttributes - overlapped async mode
            NULL                                                 // hTemplateFile
            );
    check(wd != NULL && wd != INVALID_HANDLE_VALUE);

    watcher_overlapped.hEvent = CreateEvent(
            NULL,   // lpEventAttributes
            true,   // bManualReset - the auto reset doc is unclear, we'll reset it ourself
            false,  // bInitialState - generates an event with the initial state
            NULL    // lpName
            );

    check(watcher_overlapped.hEvent != NULL);
    v->wd = wd;
    v->overlapped = watcher_overlapped;
#else
#error
#endif
    v->active = true;
}

void add_watcher(watcher *v, std::string path, u32 flags) {
    //printf("installing watcher on %s\n", path.c_str());
    v->path = get_full_path(path);
    v->flags = flags;
    enable_watcher(v);
}

void disable_watcher(watcher *w)
{
    check(w->active);
    w->active = false;
#if defined(OS_LINUX)
    inotify_rm_watch(inotify_fd, w->wd);
#elif defined(OS_WINDOWS)
    CloseHandle(w->wd);
    CloseHandle(w->overlapped.hEvent);
#else
#error
#endif
}

std::map<WATCH_DESCRIPTOR_TYPE, std::vector<watch_event>> unhandled_watch_events;  // keep polled events the user didnt need (if get_watcher_events was called with another watcher). ordered by watch descriptor id

std::vector<watch_event> get_watcher_events(watcher* w) {
    if (!w->active) return {};
    
    std::vector<watch_event> res;

    for(watch_event ev: get_default(unhandled_watch_events, w->wd, {})) {
        // we filled in fullpath wrong at the time
        ev.fullpath = w->path + OS_PATH_SEPARATOR + ev.filename;
        res.push_back(ev);
    }
    unhandled_watch_events[w->wd] = {};

#if defined(OS_LINUX)
    while(true) {
        i32 poll_res = poll(&poll_fd, 1, 0);
        check(poll_res >= 0);
        if(poll_res == 0)
            break;

        i32 buf_length = read(
                              inotify_fd,
                              inotify_event_buffer,
                              INOTIFY_EVENT_BUFFER_SIZE);

        check(buf_length >= 0);
        i32 buf_pos = 0;
        while(buf_pos < buf_length) {
            inotify_event* ev = (inotify_event*)&inotify_event_buffer[buf_pos];
            if(ev->len) {
                std::string fullpath = w->path + OS_PATH_SEPARATOR + ev->name;
                printf("%s modified\n", fullpath.c_str());
                watch_event event = {
                    ev->mask, // type
                    ev->name, // filename
                    fullpath  // fullpath
                };

                if(ev->wd == w->wd) {
                    res.push_back(event);
                } else {
                    printf("... delayed\n");
                    unhandled_watch_events[ev->wd].push_back(event);
                    check(unhandled_watch_events[ev->wd].size() < 100000);
                }
            }
            buf_pos += INOTIFY_EVENT_SIZE + ev->len;
        }
    }

#elif defined(OS_WINDOWS)
    if(!w->read_directory_wait) {
        ResetEvent(w->overlapped.hEvent);  // as we have disabled bManualReset
        memset(&w->overlapped, 0, FIELD_OFFSET(OVERLAPPED, hEvent));  // reset overlapped, except hEvent
        bool read_res = ReadDirectoryChangesW(
                w->wd,                                 // hDirectory
                w->read_directory_change_buffer,         // lpBuffer
                sizeof(w->read_directory_change_buffer), // nBufferLength
                false,                                // bWatchSubtree
                w->flags,                              // dwNotifyFilter
                NULL,                                 // lpBytesReturned - undefined here in asynchronous mode
                &w->overlapped,                        // lpOverlapped
                NULL                                  // lpCompletionRoutine
                );
        check(read_res);
        w->read_directory_wait = true;
    }

    DWORD overlapped_res_size;
    bool overlapped_res = GetOverlappedResult(
            w->wd,
            &w->overlapped,
            &overlapped_res_size,
            false    // bWait - dont wait for an answer if something is still pending
            );

    if(overlapped_res) {
        i32 offset = 0;
        while(offset < overlapped_res_size) {
            FILE_NOTIFY_INFORMATION* fni = (FILE_NOTIFY_INFORMATION*)(w->read_directory_change_buffer + offset);

            if(fni->Action != 0) {
                std::string filename = wchar_to_string(fni->FileName, fni->FileNameLength);
                std::string fullpath = w->path + OS_PATH_SEPARATOR + filename;
                printf("%s modified\n", fullpath.c_str());
                watch_event event = {
                    fni->Action,                    // type
                    filename,                       // filename
                    fullpath                        // fullpath
                };
                res.push_back(event);
            }

            if(fni->NextEntryOffset == 0)
                break;

            offset += fni->NextEntryOffset;
        }

        w->read_directory_wait = false;
    }
#else
#error
#endif

    return res;
}

const u32 CRC_POLY = 0x04C11DB7;

u32 crc32(buff b)
{
    u32 crc = 0xffffffff;
    for (u32 i = 0; i < b.size; i++) {
        u32 s = reverse_bits((u32)b.data[i]);
        for (u32 j = 0; j < 8; j++) {
            if ((i32)(crc ^ s) < 0)
                crc = (crc << 1) ^ CRC_POLY;
            else
                crc = (crc << 1);
            s = s << 1;
        }
    }
    return reverse_bits(~crc);
}
