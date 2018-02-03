
f32 signed_angle_between(vec3f e1, vec3f e2, vec3f n)
{
    e1 = normalized(e1);
    e2 = normalized(e2);
    return atan2(dot(cross(e1, e2), n), dot(e1, e2));
}


vec3f perspective_lerp(vec3f A, vec3f B, f32 Aw, f32 Bw, f32 t) {
    vec3f X = (1.f - t) * (A / Aw) + t*(B / Bw);
    f32 invw = (1.f - t) * (1.f / Aw) + t * (1.f / Bw);
    return X / invw;
}


f32 srgb_to_lin(f32 s)
{
    if (s <= 0.04045f)
        return s*(1.f/12.92f);
    else
        return pow((s+0.055f)*(1.f/1.055f), 2.4f);
}

vec3f hsl_to_rgb(vec3f hsl)
{
    f32 h = hsl.x, s = hsl.y, l = hsl.z;
    f32 ch = (1 - abs(2*l - 1)) * s;
    f32 hnorm = h / 60.f; // degrees
    f32 x = ch * (1.f - abs(fmodf(hnorm, 2) - 1));
    vec3f rgb;
    switch ((i32)trunc(hnorm) % 6) {
    case 0: rgb = {ch, x, 0.f}; break;
    case 1: rgb = {x, ch, 0.f}; break;
    case 2: rgb = {0.f, ch, x}; break;
    case 3: rgb = {0.f, x, ch}; break;
    case 4: rgb = {x, 0.f, ch}; break;
    case 5: rgb = {ch, 0.f, x}; break;
    default: unreachable();
    }
    f32 mean = l - 0.5f*ch;
    rgb += vec3f { mean, mean, mean };
    return rgb;
}

vec3f rgb_to_hsl(vec3f rgb) {
    f32 r = rgb.x, g = rgb.y, b = rgb.z;
    f32 _max = max(max(r, g), b);
    f32 _min = min(min(r, g), b);
    f32 h, s;
    f32 l = (_max+_min)/2.f;

    if(_min == _max) {  // achromatic
        h = 0;
        s = 0;
    } else {
        f32 d = _max-_min;
        s = l > 0.5f ? d/(2-_min-_max) : d/(_min+_max);
        if(_max == r)      h = (g-b)/d + (g < b ? 6.f : 0.f);
        else if(_max == g) h = (b-r)/d + 2.f;
        else if(_max == b) h = (r-g)/d + 4.f;
        else
            unreachable();

        h *= 60.f;
    }

    return vec3f { h, s, l };
}

/*f32 abs(f32 f)
{
    return std::abs(f);
    }*/
/*
f32 cos(f32 f)
{
    return std::cos(f);
}

f32 sin(f32 f)
{
    return std::sin(f);
}
*/

// TODO implement a generic printf
void print_mat(mat4 m)
{
    for (u32 i = 0; i < 4; i++) {
        for (u32 j = 0; j < 4; j++) {
            printf("%.8f ", m.data[i + 4*j]);
        }
        printf("\n");
    }
}

mat4 operator*(mat4,mat4);
mat4 operator*(mat4 m, rotate_axis r)
{
    f32 u = r.axis.x,
        v = r.axis.y,
        w = r.axis.z;
    f32 c = cos(r.angle),
        s = sin(r.angle);
    mat4 rm = { u*u + (1 - u*u)*c,  u*v*(1-c) - w*s, u*w*(1 - c) + v*s, 0,
                u*v*(1 - c) + w*s,  v*v + (1 - v*v)*c, v*w*(1 - c) - u*s, 0,
                u*w*(1 - c) - v*s,  v*w*(1 - c) + u*s, w*w + (1 - w*w)*c, 0,
                0, 0, 0, 1 };
    return m * rm;
}




#if 0
vec2f line_line_intersection(vec2f v0, vec2f v1, vec2f u0, vec2f u1, f32 *vp, f32 *up) {
    vec2f vmid = 0.5f*(v0+v1),
          umid = 0.5f*(u0+u1);
    vec2f vdir = v1-v0,
          udir = u1-u0;
    f32 va = odot(umid - vmid, udir)/odot(vdir,udir),
        ua = odot(vmid - umid, vdir)/odot(udir,vdir);
    vec2f res = vmid + va*vdir;
    *vp = va;
    *up = ua;
    assert(!isnan(va) && !isnan(ua) && !isnan(res));
    return res;
}

f32 line_line_parameter(vec2f v, vec2f vdir, vec2f u0, vec2f u1) {
    vec2f udir = u1 - u0;
    // v + (odot(umid - v, udir)/odot(vdir,udir))*vdir;
    return odot(u0 - v, udir)/odot(vdir,udir);
}
#endif

f32 det(vec2f a, vec2f b)
{
    return a.x*b.y - b.x*a.y;
}


Ray::Ray(vec3f O, vec3f _dir): orig(O), dir(normalized(_dir)) {
    /* assert(all(std::abs(dir) > vec3f { 0.00001f, 0.00001f, 0.00001f })); */
    invdir = vec3f { 1.f/dir.x, 1.f/dir.y, 1.f/dir.z };
    sign = invdir < vec3f::O;
}


// return CPA on the first line
f32 line_line_cpa_parameter(vec3f o1, vec3f d1, vec3f o2, vec3f d2)
{
    f32 a = dot(d1, d1),
        b = dot(d1, d2),
        c = dot(d2, d2),
        d = dot(d1, o1 - o2),
        e = dot(d2, o1 - o2);

    f32 s = (b*e - c*d) / (a*c - b*b);
    return s;
}

vec3f line_line_cpa(vec3f o1, vec3f d1, vec3f o2, vec3f d2)
{
    return o1 + line_line_cpa_parameter(o1,d1,o2,d2)*d1;
}

vec3f line_ray_cpa(vec3f o, vec3f d, Ray ray) {
    return line_line_cpa(o, d, ray.orig, ray.dir);
}


scale extract_scale(mat4 m)
{
    vec3f c1 = vec3f { m.data[0 + 4*0], m.data[1 + 4*0], m.data[2 + 4*0] };
    vec3f c2 = vec3f { m.data[0 + 4*1], m.data[1 + 4*1], m.data[2 + 4*1] };
    vec3f c3 = vec3f { m.data[0 + 4*2], m.data[1 + 4*2], m.data[2 + 4*2] };
    return scale { vec3f { norm(c1), norm(c2), norm(c3) } };
}

#ifdef OS_WINDOWS
#include <intrin.h>
#endif
// undefined for input = 0
u32 count_leading_zeros(u32 n)
{
#ifdef OS_WINDOWS
    unsigned long r = 0;
    _BitScanReverse(&r, n);
    return 31 - r;
#endif
#ifdef OS_LINUX
    return __builtin_clz(n);
#endif
}

u32 ilog2(u32 n)
{
    assert(n != 0);
    return 32 - count_leading_zeros(n);
}

// roughly from stackoverflow
u32 ilog10(u32 n)
{
    // log10(n) is in (log(2)/log(10)) * [ ilog2(n), ilog2(n)+1 [
    // so there is at most two possible values once ilog2(n) is known
    // this is floor(i*log(2)/log(n)) :
    u32 log2to10[] = {
        0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 5, 5, 5,
        6, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9
    };
    u32 pow10[] = {
        1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000,
    };
    assert(n != 0);
    u32 lower = log2to10[ilog2(n)];
    if (pow10[lower] <= n) {
        lower++;
    }
    return lower;
}

// from wikipedia
void to_euler(quat q, f32 *px, f32 *py, f32 *pz)
{

    // WARNING indices are wrong
    f32 r11 = 2*(q.p.x*q.p.z + q.w*q.p.y);
    f32 r12 = q.w*q.w - q.p.x*q.p.x - q.p.y*q.p.y + q.p.z*q.p.z;
    f32 r21 = -2*(q.p.y*q.p.z - q.w*q.p.x);
    f32 r31 = 2*(q.p.x*q.p.y + q.w*q.p.z);
    f32 r32 = q.w*q.w - q.p.x*q.p.x + q.p.y*q.p.y - q.p.z*q.p.z;

    // TODO check better
    if (abs(r31) <= 1e-3 && abs(r32) <= 1e-3) {
#if 0
        *py = 0.f;
        *px = M_PI*0.5f;
        f32 R31 = 2*(q.p.x*q.p.z - q.p.y*q.w);
        f32 R32 = 2*(q.p.y*q.p.z + q.p.x*q.w);
        *pz = atan2(R31, R32);
        if (r21 < 0.f) {
            *px *= -1.f;
            *pz += M_PI;
        }
#endif
        *pz = 0.f;
        *px = M_PI*0.5f;
        f32 R31 = 2*(q.p.x*q.p.z - q.p.y*q.w);
        f32 R32 = 2*(q.p.y*q.p.z + q.p.x*q.w);
        *py = atan2(R31, R32);
        if (r21 < 0.f) {
            *px *= -1.f;
            *py += M_PI;
        }

    } else {
        *pz = atan2( r31, r32 );
        *px = asin ( r21 );
        *py = atan2( r11, r12 );
    }
}

quat from_euler(f32 x, f32 y, f32 z) {
    return rotate(rotate_axis { vec3f::Y, y }) *
        rotate(rotate_axis { vec3f::X, x }) *
        rotate(rotate_axis { vec3f::Z, z });
}


quat quat_from_src_dst(vec3f src, vec3f dst)
{
    src = normalized(src);
    dst = normalized(dst);
    f32 s = dot(src, dst);
    if (abs(s) > 1.f - 1e-5f) {
        if (s > 0.f)
            return quat { vec3f::O, 1.f };
        return rotate(rotate_axis { vec3f::Y, (f32)M_PI });
    }
    quat q;
    vec3f a = cross(src, dst);
    q.p = a;
    q.w = sqrt(norm2(src) * norm2(dst)) + s;
    return q;
}

quat quat_from_basis(vec3f X, vec3f Y, vec3f Z)
{
    quat q;
    q.w = sqrt(max(0.f, 1.f + X.x + Y.y + Z.z)) * 0.5f;
    q.p.x = sqrt(max(0.f, 1.f + X.x - Y.y - Z.z)) * 0.5f;
    q.p.y = sqrt(max(0.f, 1.f - X.x + Y.y - Z.z)) * 0.5f;
    q.p.z = sqrt(max(0.f, 1.f - X.x - Y.y + Z.z)) * 0.5f;
    q.p.x = copysign(q.p.x, Y.z - Z.y);
    q.p.y = copysign(q.p.y, Z.x - X.z);
    q.p.z = copysign(q.p.z, X.y - Y.x);
    return q;
}

quat inv(quat q)
{
    q.p = -q.p;
    q = q/norm2(q);
    return q;
}

// TODO replace this garbage by something known

// random in [0,1]
f32 random_f01()
{
    f32 r = ((f32)rand()) / (f32)RAND_MAX;
    //f32 r = drand48();
    return r;
}

// random in [a, b[
i32 random_i32(i32 a, i32 b) {
    return a + rand() % (b-a);
}

template<typename T>
vec3f barycentric_coordinates(T p, T a, T b, T c) {
    T v0 = b - a;
    T v1 = c - a;
    T v2 = p - a;

    f32 d00 = dot(v0, v0);
    f32 d01 = dot(v0, v1);
    f32 d11 = dot(v1, v1);
    f32 d20 = dot(v2, v0);
    f32 d21 = dot(v2, v1);

    f32 denom = d00 * d11 - d01 * d01;

    f32 aw = (d11 * d20 - d01 * d21) / denom;
    f32 bw = (d00 * d21 - d01 * d20) / denom;
    return vec3f{1.0f - aw - bw, aw, bw};
}

template vec3f barycentric_coordinates<vec3f>(vec3f p, vec3f a, vec3f b, vec3f c);

template<typename T>
bool is_point_inside_triangle(T p, T a, T b, T c, bool inclusive) {
    vec3f bar = barycentric_coordinates(p, a, b, c);
    if(inclusive)
        return bar.x >= 0 && bar.y >= 0 && bar.z >= 0;
    else
        return bar.x > 0 && bar.y > 0 && bar.z > 0;
}


template bool is_point_inside_triangle<vec3f>(vec3f p, vec3f a, vec3f b, vec3f c, bool inclusive);


mat4 lookat(vec3f pos, vec3f target, vec3f up) {
    vec3f zaxis = normalized(pos-target);
    vec3f xaxis = normalized(cross(up, zaxis));
    vec3f yaxis = cross(zaxis, xaxis);

    return mat4 {
            xaxis.x, xaxis.y, xaxis.z, -dot(xaxis, pos),

            yaxis.x, yaxis.y, yaxis.z, -dot(yaxis, pos),

            zaxis.x, zaxis.y, zaxis.z, -dot(zaxis, pos),

            0, 0, 0, 1
    };
}
