{| number_types = [('f', 'f32'), ('i', 'i32'), ('d', 'f64')] |}
// forward declaration for the vec cast operators
{| for t, _ in number_types: |}
{| for i in xrange(2, 5): |}
struct vec{# "%s%s" % (i, t) #};
{| end |}
{| end |}

{| for t in ['f32', 'i32']: |}
static inline {# t #} clamp({# t #} v, {# t #} m, {# t #} M) {
    return max(min(v, M), m);
}
{| end |}


{| for t, tname in [('b', 'bool'), ('f', 'f32'), ('i', 'i32'), ('d', 'f64')]: |}
{| for i in xrange(2, 5): |}
    {| _type = "vec%s%s" % (i, t) |}
    {| l = list('xyzw')[:i] |}
struct {# _type #} {

    {| for v in l: |}
        {# tname #} {# v #};
    {| end |}

    {| for j in xrange(2, i): |}
        {| for c in itertools.combinations(l, j): |}
            vec{# j #}{# t #} {# ''.join(c) #}() const { return vec{# j #}{# t #} { {# ','.join(c) #} };}
        {| end |}
    {| end |}

    
    static const {# _type #} {# ','.join([v.upper() for v in l]) #},O;

    // forward declaration of cast operators
    {| for other_type in [abt[0] for abt in number_types if abt[0] != t and (t, tname) in number_types]: |}
    operator {# "vec%s%s" % (i, other_type) #}() const;
    {| end |}

    // in-place operators
    {| if t != 'b': |}
        {| for op in ['+', '-']: |}
        {# _type #}& operator{# op #}=(const {# _type #}& v) {
            {| for v in l: |}
                {# v #} {# op #}= v.{# v #};
            {| end |}
            return *this;
        }
        {| end |}

        {| for op in ['*', '/']: |}
        {# _type #}& operator{# op #}=(const f32 f) {
            {| for v in l: |}
            {# v #} {# op #}= f;
            {| end |}
            return *this;
        }
        {| end |}
    {| end |}

    {# tname #}& operator[](i32 i) {
        assert(i < {# i #});
        return (&x)[i];
    }
    {# tname #} operator[](i32 i) const {
        assert(i < {# i #});
        return (&x)[i];
    }

    std::string to_string() {
        std::ostringstream stream;
        stream << "vec(" << {# ' << "," << '.join(l) #} << ")";
        return stream.str();
    }

};  // end of struct {# _type #}


{| for j, v in enumerate(l+["O"]): |}
WEAK const {# _type #} {# _type #}::{# v.upper() #} { {# ', '.join([("1" if (j == jj) else "0") for jj in xrange(i)]) #} };
{| end |}


// not in-place operators of {# _type #}

static inline bool operator==(const {# _type #}& a, const {# _type #}& b) {
    return {# ' && '.join('(a.%s == b.%s)' % (v, v) for v in l) #};
}

static inline bool operator!=(const {# _type #}& a, const {# _type #}& b) {
    return !(a == b);
}

{| if t == 'b': |}
static inline bool all(const {# _type #}& v) {
    return {# ' && '.join('v.%s' % v for v in l) #};
}

static inline bool any(const {# _type #}& v) {
    return {# ' || '.join('v.%s' % v for v in l) #};
}

{| end |}
{| if t != 'b': |}

{| for op in ['+', '-']: |}
static inline {# _type #} operator{# op #}({# _type #} a, const {# _type #}& b) {
    a {# op #}= b;
    return a;
}
{| end |}

{| for op in ['*', '/']: |}
static inline {# _type #} operator{# op #}({# _type #} v, const f32 f) {
    v {# op #}= f;
    return v;
}
{| end |}

static inline {# _type #} operator*(const f32 f, {# _type #} v) {
    return v*f;
}

static inline {# _type #} operator-(const {# _type #}& v) {
    return {# _type #} { {# ', '.join('-v.%s' % v for v in l) #} };
}

{| for comparator in ['<', '<=', '>', '>=']: |}
    static inline vec{# i #}b operator{# comparator #}(const {# _type #}& a, const {# _type #}& b) {
        return vec{# i #}b{ {# ', '.join(['(a.%s %s b.%s)' % (v, comparator, v) for v in l]) #} };
    }
{| end |}

static inline {# _type #} hadamard(const {# _type #}& a, const {# _type #}& b) {
    return {# _type #} { {# ', '.join('a.%s*b.%s' % (v,v) for v in l) #} };
}

static inline f32 dot(const {# _type #}& a, const {# _type #}& b) {
    return {# ' + '.join('a.%s*b.%s' % (v,v) for v in l) #};
}

static inline f32 norm2(const {# _type #}& v) {
    return dot(v,v);
}

static inline f32 norm(const {# _type #}& v) {
    return sqrt(norm2(v));
}

inline f32 manhattan(const {# _type #}& v) {
    return {# ' + '.join('abs(v.%s)' % v for v in l) #};
}

static inline {# _type #} normalized(const {# _type #}& v) {
    f32 n = norm2(v);
    //assert(n > 1e-8);
    return v/sqrt(n);
}

{| if t == 'f': |}
static inline {# _type #} round(const {# _type #}& v) {
    return {# _type #} { {# ', '.join('round(v.%s)' % v for v in l) #} };
}


static inline {# _type #} lerp(const {# _type #}& a, const {# _type #}& b, f32 t) {
    return (1.f - t)*a + t*b;
}


static inline {# _type #} constant_speed_lerp(const {# _type #}& a, const {# _type #}& b, f32 dt) {
    return a + (b-a) * clamp(dt / norm(b-a), 0.f, 1.f);
}


static inline {# _type #} flush_to_zero({# _type #} v, f32 epsilon) {
    if(norm2(v) <= epsilon*epsilon)
        return {# _type #}::O;
    return v;
}

static inline bool hasnan(const {# _type #}& v) {
    return {# ' || '.join('isnan(v.%s)' % v for v in l) #};
}
{| end |}


namespace std {
    {| for func in ['min', 'max']: |}
    static inline {# _type #} {# func #}(const {# _type #} &a, const {# _type #} &b) {
        return {# _type #}{ {# ', '.join(['%s(a.%s, b.%s)' % (func, v, v) for v in l]) #} };
    }
    {| end |}

    static inline {# _type #} abs(const {# _type #} &v) {
        return {# _type #}{ {# ', '.join(['abs(v.%s)' % v for v in l]) #} };
    }
}

{| end |}


// constructors
// TODO clean me up =)
{| for p in _pseudo_partitions(i): |}
static inline {# _type #} vec({# ', '.join([(("vec%s%s" % (ii, t)) if ii != 1 else tname) + (" z%s" % j) for j, ii in enumerate(p)]) #}) { return {# _type #} { {# ', '.join(((', '.join("z%s.%s" % (j, "xyzw"[k]) for k in range(ii))) if ii != 1 else "z%s" % j) for j, ii in enumerate(p)) #} }; }
{| end |}


{| end |}


{| end |}


// vec cast operator implementations
{| for t, tname in number_types: |}
{| for i in xrange(2, 5): |}
    {| _type = "vec%s%s" % (i, t) |}
    {| l = list('xyzw')[:i] |}

    {| for other_type, other_type_name in [abt for abt in number_types if abt[0] != t]: |}
inline {# _type #}::operator {# "vec%s%s" % (i, other_type) #}() const {
        return vec{# "%s%s" % (i, other_type) #} { {# ', '.join(["(%s)%s" % (other_type_name, v) for v in l]) #} };
    }
    {| end |}
{| end |}
{| end |}






static inline vec3f cross(const vec3f& a, const vec3f& b) {
    return vec3f{
        a.y*b.z - b.y*a.z,
        b.x*a.z - a.x*b.z,
        a.x*b.y - b.x*a.y
    };
}

struct translate {
    vec3f v;
};

struct scale {
    vec3f v;
};

struct rotate_axis {
    vec3f axis; // normalized
    f32 angle;
};

struct quat {
    vec3f p = {}; // pure part
    f32 w = 1.f; // real part
};

struct isom {
    vec3f translation = vec3f::O;
    quat rotation = {{0, 0, 0}, 1};
};

// encodes T*R*S
struct aff {
    isom i = {};
    vec3f scale = {1, 1, 1};
};

// removes translation & inverse scaling
static inline aff vertex_to_normal_transform(aff a) {
    a.i.translation = vec3f::O;
    a.scale = vec3f{1.f/a.scale.x, 1.f/a.scale.y, 1.f/a.scale.z};
    return a;
}

{| mat_types = [('', 'f32', 'f'), ('d', 'f64', 'd')] |}
{| for t, et, vt in mat_types: |}
struct mat4{# t #};
{| end |}
{| for t, et, vt in mat_types: |}
struct mat4{# t #} {
    {# et #} data[16];
    mat4{# t #}() {}
    {| for ot, oet, ovt in mat_types: |}
    {| if ot != t: |}
    static mat4{# t #} cast(const mat4{# ot #}&);
    {| end |}
    {| end |}
    mat4{# t #}(std::initializer_list<{# et #}> fs);
    static mat4{# t #} id();
    static mat4{# t #} proj({# et #} n, {# et #} f, {# et #} fov, u32 w, u32 h);
    static mat4{# t #} ortho({# et #} n, {# et #} f, {# et #} e, u32 w, u32 h);
};
{| end |}



// AABBs

{| aabb_types = [('f', 'f32'), ('i', 'i32')] |}

// forward declaration for aabb cast constructor
{| for t, tname in aabb_types: |}
{| for i in xrange(2, 5): |}
class aabb{# "%s%s" % (i, t) #};
{| end |}
{| end |}

{| for t, tname in aabb_types: |}
{| for i in xrange(2, 5): |}
{| _type = "aabb%s%s" % (i, t) |}
{| vec_type = "vec%s%s" % (i, t) |}
class {# _type #} {
public:
    {# vec_type #} corner;
    {# vec_type #} dim;

    {# _type #}() : corner({# vec_type #}()), dim({# vec_type #}()) {}
    {# _type #}(const {# vec_type #} corner, const {# vec_type #} dim) : corner(corner), dim(dim) {}

    // forward declaration of cast constructors
    {| for other_type in [abt[0] for abt in aabb_types if abt[0] != t]: |}
    {# _type #}(const {# "aabb%s%s" % (i, other_type) #}& r);
    {| end |}

    bool contains(const {# vec_type #} p) const {
        {# vec_type #} diff = p-corner;
        return all(diff <= dim) && all({# vec_type #}() <= diff);
    }

    bool intersects(const {# _type #}& b) {
        return all(
                std::min(opposite_corner(), b.opposite_corner())
              > std::max(corner, b.corner)
        );
    }

    {# _type #}& operator&=(const {# _type #}& b) {
        {# vec_type #} opposite = std::min(opposite_corner(), b.opposite_corner());
        corner = std::max(corner, b.corner);
        if(any(opposite < corner))
            dim = {# vec_type #}::O;
        else
            dim = opposite-corner;
        return *this;
    }

    {# _type #}& operator|=(const {# _type #}& b) {
        {# vec_type #} opposite = std::max(opposite_corner(), b.opposite_corner());
        corner = std::min(corner, b.corner);
        dim = opposite-corner;
        return *this;
    }

    {# tname #} right() const {
        return corner.x + dim.x;
    }

    {# tname #} bottom() const {
        return corner.y + dim.y;
    }

    {# vec_type #} center() const {
        return corner + dim/2.f;
    };

    {# vec_type #} opposite_corner() const {
        return corner + dim;
    }

    {# _type #} with_margin({# vec_type #} margin) {
        return {# _type #}(
            corner + margin,
            dim - 2*margin
        );
    }

    {# tname #} volume() const {
        return {# '*'.join(("dim.%s" % comp) for comp in list("xyzw"[:i])) #};
    }

    // TODO for every size
    {| if i == 2: |}
    {# _type #} clip(const {# _type #}& scissor) {
        {# vec_type #} top_left = {# vec_type #} {
            clamp(corner.x, scissor.corner.x, scissor.right()),
            clamp(corner.y, scissor.corner.y, scissor.bottom())
        };
        {# vec_type #} bottom_right = {# vec_type #} {
            clamp(right(), scissor.corner.x, scissor.right()),
            clamp(bottom(), scissor.corner.y, scissor.bottom())
        };
        return {# _type #}(top_left, bottom_right-top_left);
    }
    {| end |}

    std::string to_string() {
        std::ostringstream stream;
        stream << "AABB(" << corner.to_string() << ", " << dim.to_string() << ")";
        return stream.str();
    }
};

static inline bool operator==(const {# _type #}& a, const {# _type #}& b) {
    return a.corner == b.corner && a.dim == b.dim;
}

static inline bool operator!=(const {# _type #}& a, const {# _type #}& b) {
    return !(a == b);
}

{| for op in ['&', '|']: |}
static inline {# _type #} operator{# op #}({# _type #} a, const {# _type #}& b) {
    a {# op #}= b;
    return a;
}
{| end |}

static inline bool hasnan(const {# _type #}& aabb) {
    return hasnan(aabb.dim) || hasnan(aabb.corner);
}

static inline {# _type #} bounding_box(std::vector<{# vec_type #}> points) {
    {# _type #} bb;
    if(points.size() > 0)
        bb.corner = points[0];
    for(u32 i=1 ; i<points.size() ; i++)
        bb |= {# _type #}(points[i], {# vec_type #}());
    return bb;
}

{| end |}


{| end |}


const vec4f white = { 1, 1, 1, 1 };
const vec4f black = { 0, 0, 0, 1 };
const vec4f grey = { 0.5, 0.5, 0.5, 1 };
const vec4f red = { 1, 0, 0, 1 };
const vec4f green = { 0, 1, 0, 1 };
const vec4f blue = { 0, 0, 1, 1 };
const vec4f yellow = red + green;
const vec4f cyan = green + blue;
const vec4f magenta = red + blue;
const vec4f light_blue = vec4f { 172, 206, 247, 255 } / 255.f;


// aabb cast constructor implementations
{| for t, tname in aabb_types: |}
{| for i in xrange(2, 5): |}
{| _type = "aabb%s%s" % (i, t) |}
{| vec_type = "vec%s%s" % (i, t) |}
    {| for other_type in [abt[0] for abt in aabb_types if abt[0] != t]: |}
    inline {# _type #}::{# _type #}(const {# "aabb%s%s" % (i, other_type) #}& r) {
        corner = ({# vec_type #})r.corner;
        dim = ({# vec_type #})r.dim;
    }
    {| end |}
{| end |}
{| end |}


// quaternion
static inline quat operator*(quat a, quat b)
{
    f32 x =  a.p.x * b.w + a.p.y * b.p.z - a.p.z * b.p.y + a.w * b.p.x;
    f32 y = -a.p.x * b.p.z + a.p.y * b.w + a.p.z * b.p.x + a.w * b.p.y;
    f32 z =  a.p.x * b.p.y - a.p.y * b.p.x + a.p.z * b.w + a.w * b.p.z;
    f32 w = -a.p.x * b.p.x - a.p.y * b.p.y - a.p.z * b.p.z + a.w * b.w;
    return quat { vec3f { x, y, z }, w };
}


static inline quat operator-(quat a) { return quat { -a.p, -a.w }; }
static inline quat operator-(quat a, quat b) { return quat { a.p - b.p, a.w - b.w }; }
static inline quat operator*(f32 s, quat a) { return quat { s*a.p, s*a.w }; }
static inline quat operator+(quat a, quat b) { return quat { a.p + b.p, a.w + b.w }; }


static inline f32 norm2(quat q)
{
    return norm2(q.p) + q.w*q.w;
}

static inline f32 norm(quat q)
{
    return sqrt(norm2(q));
}

static inline quat operator/(quat q, f32 s)
{
    return quat { q.p/s, q.w/s };
}

static inline quat conj(quat a)
{
    return { -a.p, a.w };
}

static inline vec3f operator*(quat q, vec3f v)
{
    vec3f t = 2 * cross(q.p, v);
    return v + q.w * t + cross(q.p, t);
}

static inline vec3f operator*(isom i, vec3f v)
{
    return i.translation + i.rotation * v;
}

static inline isom operator*(isom i, quat r)
{
    return {i.translation, i.rotation * r};
}


static inline quat rotate(rotate_axis r)
{
    // TODO PERF maybe don't normalize here and assume/assert it is
    quat q;
    q.p = sin(r.angle*0.5f) * normalized(r.axis);
    q.w = cos(r.angle*0.5f);
    return q;
}

static inline isom operator*(isom i, translate t)
{
    return isom { i.translation + i.rotation * t.v, i.rotation };
}

static inline isom operator*(isom i, rotate_axis r)
{
    return isom { i.translation, i.rotation * rotate(r) };
}

static inline vec3f operator*(aff a, vec3f v)
{
    return a.i * hadamard(a.scale, v);
}

static inline isom operator*(isom a, isom b)
{
    return { a.translation + a.rotation * b.translation, a.rotation * b.rotation };
}


static inline aff operator*(isom a, aff b)
{
    return { a*b.i, b.scale };
}


static inline quat operator*(quat a, rotate_axis r)
{
    return a * rotate(r);
}

static inline quat operator*(rotate_axis r, quat a)
{
    return rotate(r) * a;
}

static inline quat normalized(quat q)
{
    return q / norm(q);
}

static inline quat torque_minimal_rotation(vec3f u, vec3f v)
{
    quat q;
    q.w = dot(u, v) + sqrt(norm2(u) * norm2(v));
    q.p = cross(u, v);
    return normalized(q);
}

// TODO rename me perp
static inline f32 odot(vec2f a, vec2f b)
{
    vec2f orth = vec2f { -b.y, b.x };
    return dot(a, orth);
}


static inline i32 side(vec2f o, vec2f d, vec2f p)
{
    f32 s = odot(p - o, d);
    if (s == 0) return 0;
    else if (s > 0) return 1;
    else return -1;
}

static inline f32 lerp(f32 a, f32 b, f32 t)
{
    return (1.f - t)*a + t*b;
}


// TODO handle close-to-zero directions
struct Ray {
    vec3f orig;
    vec3f dir;
    // cached stuff, to optimize aabb intersection test
    vec3f invdir;
    vec3b sign;  // signs of dir

    Ray(vec3f O, vec3f _dir);
};

// TODO 2 version: one with only t (usual case), and one with everything
struct ray_hit {
    vec3f contact;
    //vec3f normal;
    f32 t;
};

static inline bool ray_cast_tri(Ray ray, vec3f tri[3], ray_hit* hit) {
    vec3f e1 = tri[1] - tri[0],
          e2 = tri[2] - tri[0];
    vec3f p = cross(ray.dir, e2);
    f32 det = dot(e1, p);
    if (abs(det) <= 1e-16f) {
        return false;
    }
    f32 invdet = 1.f/det;
    vec3f t = ray.orig - tri[0];
    f32 u = dot(t,p)*invdet;
    if (u < 0.f || u > 1.f) {
        return false;
    }
    vec3f q = cross(t, e1);
    f32 v = dot(ray.dir, q)*invdet;
    if (v < 0.f || u+v > 1.f) {
        return false;
    }
    f32 ti = dot(e2, q)*invdet;
    if (ti > 1e-16f && ti < hit->t) {
        *hit = { ray.orig + ti*ray.dir, ti };
        return true;
    }
    return false;
}



static inline f32 constant_speed_lerp(f32 a, f32 b, f32 dt)
{
    return a + (b-a) * clamp(dt / abs(b-a), 0.f, 1.f);
}

static inline f32 ease_in(f32 a, f32 b, f32 dt)
{
    if (abs(b-a) < 1e-6f) return b;
    f32 speed = clamp(abs(b-a)/4.f, 0.f, 0.25f);
    return a + (b-a) * clamp(speed * dt / abs(b-a), 0.f, 1.f);
}

static inline vec3f ease_in(vec3f a, vec3f b, f32 dt)
{
    if (norm(b-a) < 1e-6f) return b;
    f32 speed = clamp(norm2(b-a)*0.05f, 0.f, 0.25f);
    return a + (b-a) * clamp(speed * dt / norm(b-a), 0.f, 1.f);
}

static inline f32 ease_in_angle(f32 a, f32 b, f32 dt)
{
    f32 d = b-a;
    d = fmodf(fmodf(d, 2.f * M_PI) + 3.f * M_PI, 2.f * M_PI) - (f32)M_PI;
    if (abs(d) < 1e-6f) return b;
    f32 speed = clamp(abs(d)*0.05f, 0.f, 0.05f);
    return a + (d) * clamp(speed * dt / abs(d), 0.f, 1.f);
}


static inline quat slerp(quat a, quat b, f32 t)
{
    // TODO maybe just assert that they are unit
    // since we are rarely dealing with non-unit quats anywhere
    a = normalized(a);
    b = normalized(b);

    f32 s = dot(a.p, b.p) + a.w*b.w;

    if (s > 0.999f) {
        return normalized(a + t*(b - a));
    }

    if (s < 0.0f) {
        b = -b;
        s = -s;
    }

    s = clamp(s, -1.f, 1.f);
    f32 th = acos(s) * t;
    quat q = b - s*a;
    return cos(th)*a + sin(th)*normalized(q);
}



static inline quat constant_speed_slerp(quat a, quat b, f32 dt)
{
    // TODO maybe just assert that they are unit
    // since we are rarely dealing with non-unit quats anywhere
    a = normalized(a);
    b = normalized(b);

    f32 s = dot(a.p, b.p) + a.w*b.w;

    if (s > 0.9999f) {
        return normalized(b);// + t*(b - a));
    }

    if (s < 0.0f) {
        b = -b;
        s = -s;
    }

    s = clamp(s, -1.f, 1.f);
    f32 ph = acos(s);
    f32 th = copysign(clamp(dt, 0.f, abs(ph)), ph);
    quat q = b - s*a;
    return cos(th)*a + sin(th)*normalized(q);
}



static inline vec3f slerp(vec3f a, vec3f b, f32 t)
{
    f32 s = dot(a, b)/(norm(a) * norm(b));
    s = clamp(s, -1.f, 1.f);
    f32 theta = acos(s);

    if (theta == 0) return b;

    f32 sin_theta = sin(theta);
    f32 alpha = t*theta;
    return (a * (sin(theta - alpha) / sin_theta)) + (b * (sin(alpha) / sin_theta));
}


static inline vec3f constant_speed_slerp(vec3f a, vec3f b, f32 dtheta)
{
    f32 s = dot(a, b)/(norm(a) * norm(b));
    s = clamp(s, -1.f, 1.f);
    f32 theta = acos(s);

    if (theta == 0) return b;

    f32 sin_theta = sin(theta);
    f32 alpha = clamp(dtheta, 0.f, theta);
    return (a * (sin(theta - alpha) / sin_theta)) + (b * (sin(alpha) / sin_theta));
}


static inline vec3f slerp_axis(vec3f a, vec3f b, f32 t, vec3f axis)
{
    //assert(abs(dot(a, axis) < 1e-5f && abs(dot(b, axis)) < 1e-5f));
    f32 la = norm(a),
        lb = norm(b);
    vec3f an = a/la, bn = b/lb;
    if (dot(an,bn) >= 1.f - 1e-10f) {
        return lerp(a, b, t);
    }
    vec3f half;
    if (dot(an,bn) <= -1.f + 1e-5f) {
        half = cross(a, axis);
    }
    else {
        half = normalized(0.5f*(a + b));
    }
    half = half - dot(half, axis)*axis;
    if (t < 0.5f)
        return slerp(a, half, 2*t);
    else
        return slerp(half, b, 2*t - 1.f);
}

{| for t, et, vt in mat_types: |}

    {| for ot, oet, ovt in mat_types: |}
    {| if ot != t: |}
inline mat4{# t #} mat4{# t #}::cast(const mat4{# ot #}& other) {
    mat4{# t #} m;
    for (u32 i = 0; i < 16; i++) {
        m.data[i] = ({# et #})other.data[i];
    }
    return m;
}
    {| end |}
    {| end |}


static inline mat4{# t #} operator*(mat4{# t #} a, mat4{# t #} b)
{
    mat4{# t #} m;
    for (u32 i = 0; i < 4; i++) {
        for (u32 j = 0; j < 4; j++) {
            {# et #} dot = 0;
            for (u32 k = 0; k < 4; k++)
                dot += a.data[i + 4*k] * b.data[k + 4*j];
            m.data[i + 4*j] = dot;
        }
    }
    return m;
}

static inline mat4{# t #} operator*(mat4{# t #} m, quat q)
{
    mat4{# t #} qm = {
        1 - 2*q.p.y*q.p.y - 2*q.p.z*q.p.z, 2*q.p.x*q.p.y - 2*q.p.z*q.w, 2*q.p.x*q.p.z + 2*q.p.y*q.w, 0,
        2*q.p.x*q.p.y + 2*q.p.z*q.w, 1 - 2*q.p.x*q.p.x - 2*q.p.z*q.p.z, 2*q.p.y*q.p.z - 2*q.p.x*q.w, 0,
        2*q.p.x*q.p.z - 2*q.p.y*q.w, 2*q.p.y*q.p.z + 2*q.p.x*q.w, 1 - 2*q.p.x*q.p.x - 2*q.p.y*q.p.y, 0,
        0, 0, 0, 1
    };
    return m * qm;
    
}

static inline mat4{# t #} operator-(mat4{# t #} a, mat4{# t #} b)
{
    mat4{# t #} m;
    for (u32 i = 0; i < 4; i++)
        for (u32 j = 0; j < 4; j++)
            m.data[i+4*j] = a.data[i+4*j] - b.data[i+4*j];
    return m;
}


static inline vec3{# vt #} operator*(mat4{# t #} m, vec3{# vt #} b)
{
    vec3{# vt #} r;
    {# et #} w;
    r.x = b.x * m.data[0 + 4*0] +
          b.y * m.data[0 + 4*1] +
          b.z * m.data[0 + 4*2] +
                m.data[0 + 4*3];
    r.y = b.x * m.data[1 + 4*0] +
          b.y * m.data[1 + 4*1] +
          b.z * m.data[1 + 4*2] +
                m.data[1 + 4*3];
    r.z = b.x * m.data[2 + 4*0] +
          b.y * m.data[2 + 4*1] +
          b.z * m.data[2 + 4*2] +
                m.data[2 + 4*3];
    w =   b.x * m.data[3 + 4*0] +
          b.y * m.data[3 + 4*1] +
          b.z * m.data[3 + 4*2] +
                m.data[3 + 4*3];
    r.x /= w;
    r.y /= w;
    r.z /= w;
    return r;
}

static inline vec4{# vt #} operator*(mat4{# t #} m, vec4{# vt #} b)
{
    vec4{# vt #} r;
    r.x = b.x * m.data[0 + 4*0] +
          b.y * m.data[0 + 4*1] +
          b.z * m.data[0 + 4*2] +
          b.w * m.data[0 + 4*3];
    r.y = b.x * m.data[1 + 4*0] +
          b.y * m.data[1 + 4*1] +
          b.z * m.data[1 + 4*2] +
          b.w * m.data[1 + 4*3];
    r.z = b.x * m.data[2 + 4*0] +
          b.y * m.data[2 + 4*1] +
          b.z * m.data[2 + 4*2] +
          b.w * m.data[2 + 4*3];
    r.w = b.x * m.data[3 + 4*0] +
          b.y * m.data[3 + 4*1] +
          b.z * m.data[3 + 4*2] +
          b.w * m.data[3 + 4*3];
    return r;
}

static inline vec3{# vt #} trans_product(mat4{# t #} m, vec3{# vt #} b) {
    vec3{# vt #} r;
    {# et #} w;
    r.x = b.x * m.data[4*0 + 0] +
          b.y * m.data[4*0 + 1] +
          b.z * m.data[4*0 + 2] +
                m.data[4*0 + 3];
    r.y = b.x * m.data[4*1 + 0] +
          b.y * m.data[4*1 + 1] +
          b.z * m.data[4*1 + 2] +
                m.data[4*1 + 3];
    r.z = b.x * m.data[4*2 + 0] +
          b.y * m.data[4*2 + 1] +
          b.z * m.data[4*2 + 2] +
                m.data[4*2 + 3];
    w =   b.x * m.data[4*3 + 0] +
          b.y * m.data[4*3 + 1] +
          b.z * m.data[4*3 + 2] +
                m.data[4*3 + 3];
    r.x /= w;
    r.y /= w;
    r.z /= w;
    return r;
}

static inline vec4{# vt #} trans_product(mat4{# t #} m, vec4{# vt #} b) {
    vec4{# vt #} r;
    r.x = b.x * m.data[4*0 + 0] +
          b.y * m.data[4*0 + 1] +
          b.z * m.data[4*0 + 2] +
          b.w * m.data[4*0 + 3];
    r.y = b.x * m.data[4*1 + 0] +
          b.y * m.data[4*1 + 1] +
          b.z * m.data[4*1 + 2] +
          b.w * m.data[4*1 + 3];
    r.z = b.x * m.data[4*2 + 0] +
          b.y * m.data[4*2 + 1] +
          b.z * m.data[4*2 + 2] +
          b.w * m.data[4*2 + 3];
    r.w = b.x * m.data[4*3 + 0] +
          b.y * m.data[4*3 + 1] +
          b.z * m.data[4*3 + 2] +
          b.w * m.data[4*3 + 3];
    return r;
}

static inline mat4{# t #} lerp(mat4{# t #} a, mat4{# t #} b, {# et #} t)
{
    mat4{# t #} m;
    for (u32 i = 0; i < 16; i++)
        m.data[i] = lerp(a.data[i], b.data[i], t);
    return m;
}

inline mat4{# t #}::mat4{# t #}(std::initializer_list<{# et #}> fs) {
    auto it = fs.begin();
    const {# et #} *in = &*it;
    for (u32 i = 0; i < 4; i++) {
        for (u32 j = 0; j < 4; j++) {
            data[i + 4*j] = in[j + 4*i];
        }
    }
}
inline mat4{# t #} mat4{# t #}::id() {
    mat4{# t #} m = { 1, 0, 0, 0,
               0, 1, 0, 0,
               0, 0, 1, 0,
               0, 0, 0, 1 };
    return m;
}
inline mat4{# t #} mat4{# t #}::proj({# et #} n, {# et #} f, {# et #} fov, u32 w, u32 h) {
    {# et #} e = 1.f / tan(fov * 0.5f);
    {# et #} a = (({# et #})h)/w;
    mat4{# t #} m = { e, 0,   0, 0,
               0, e/a, 0, 0,
               0, 0, -(f+n)/(f-n), -2*f*n/(f-n),
               0, 0, -1, 0 };
    return m;
}
inline mat4{# t #} mat4{# t #}::ortho({# et #} n, {# et #} f, {# et #} e, u32 w, u32 h) {
    {# et #} a = (({# et #})h)/w;

    mat4{# t #} m = { e, 0, 0, 0,
               0, e/a, 0, 0,
               0, 0, -2.0f/(f-n), -(f+n)/(f-n),
               0, 0, 0, 1 };
    return m;
}


#if 0

static inline vec4{# vt #} lu_solve(mat4{# t #} M, vec4{# vt #} rhs) {
    {# et #} lu[16];
    for (i32 i = 0; i < 16; i++)
        lu[i] = 0;
    {# et #} sum = 0;
    for (i32 i = 0; i < 4; i++) {
        for (i32 j = i; j < 4; j++) {
            sum = 0;
            for (i32 k = 0; k < i; k++) {
                sum += lu[i + 4*k] * lu[k + 4*j];
            }
            lu[i + 4*j] = M.data[i + 4*j] - sum;
            for (i32 j = i + 1; j < 4; j++) {
                sum = 0;
                for (i32 k = 0; k < i; k++)
                    sum += lu[j + 4*k] * lu[k + 4*i];
                lu[j + 4*i] = (1.0 / lu[i +4*i]) * (M.data[j + 4*i] - sum);
            }
        }
    }
            
     // find solution of Ly = b
    {# et #} y[4] = {0,0,0,0};
    for (i32 i = 0; i < 4; i++) {
        sum = 0;
        for (i32 k = 0; k < i; k++)
            sum += lu[i + 4*k] * y[k];
        y[i] = rhs[i] - sum;
    }
    // find solution of Ux = y
    {# et #} x[4] = {0,0,0,0};
    for (i32 i = 3; i >= 0; i--) {
        sum = 0;
        for (i32 k = i + 1; k < 4; k++)
            sum += lu[i + 4*k] * x[k];
        x[i] = (1.0 / lu[i + 4*i]) * (y[i] - sum);
    }
    return vec4{# vt #} { x[0], x[1], x[2], x[3] };
}

#endif

static inline vec4{# vt #} gauss_solve(mat4{# t #} M, vec4{# vt #} rhs) {
    for (u32 i = 0; i < 4; i++) {
        {# et #} max_v = abs(M.data[i + 4*i]);
        u32 max_row = i;
        for (u32 k = i+1; k < 4; k++) {
            {# et #} v = abs(M.data[k + 4*i]);
            if (v > max_v) {
                max_v = v;
                max_row = k;
            }
        }
        
        for (u32 k = i; k < 4; k++) {
            {# et #} tmp = M.data[max_row + 4*k];
            M.data[max_row + 4*k] = M.data[i + 4*k];
            M.data[i + 4*k] = tmp;
        }
        
        {# et #} tmp = rhs[max_row];
        rhs[max_row] = rhs[i];
        rhs[i] = tmp;
        
        for (u32 k = i+1; k < 4; k++) {
            {# et #} v = - M.data[k + 4*i] / M.data[i + 4*i];
            M.data[k + 4*i] = 0;
            for (u32 j = i+1; j < 4; j++) {
                M.data[k + 4*j] += v * M.data[i + 4*j];
            }
            rhs[k] += v * rhs[i];
        }
    }

    vec4{# vt #} x;
    for (i32 i=3; i>=0; i--) {
        x[i] = rhs[i]/M.data[i + 4*i];
        for (i32 k=i-1;k>=0; k--) {
            rhs[k] -= M.data[k + 4*i] * x[i];
        }
    }
    return x;
}

#if 0
static inline vec3{# vt #} lu_solve(mat4{# t #} M, vec3{# vt #} rhs) {
    vec4{# vt #} lhs4 = lu_solve(M, vec(rhs, ({# et #})1.0));
    return lhs4.xyz()/lhs4.w;
}
#endif


static inline vec3{# vt #} inv_apply(mat4{# t #} M, vec3{# vt #} rhs) {
    vec4{# vt #} lhs4 = gauss_solve(M, vec(rhs, ({# et #})1.0));
    return lhs4.xyz()/lhs4.w;
}

static inline vec4{# vt #} inv_apply(mat4{# t #} M, vec4{# vt #} rhs) {
    return gauss_solve(M, rhs);
}

// from mesa

static inline mat4{# t #} inv(mat4{# t #} in)
{
    f64 inv[16];
    {# et #} *m_32 = in.data;
    f64 m[16];
    for (u32 i = 0; i < 16; i++)
        m[i] = m_32[i];

    inv[0] = m[5]  * m[10] * m[15] - 
             m[5]  * m[11] * m[14] - 
             m[9]  * m[6]  * m[15] + 
             m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] - 
             m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] + 
              m[4]  * m[11] * m[14] + 
              m[8]  * m[6]  * m[15] - 
              m[8]  * m[7]  * m[14] - 
              m[12] * m[6]  * m[11] + 
              m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] - 
             m[4]  * m[11] * m[13] - 
             m[8]  * m[5] * m[15] + 
             m[8]  * m[7] * m[13] + 
             m[12] * m[5] * m[11] - 
             m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] + 
               m[4]  * m[10] * m[13] +
               m[8]  * m[5] * m[14] - 
               m[8]  * m[6] * m[13] - 
               m[12] * m[5] * m[10] + 
               m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] + 
              m[1]  * m[11] * m[14] + 
              m[9]  * m[2] * m[15] - 
              m[9]  * m[3] * m[14] - 
              m[13] * m[2] * m[11] + 
              m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] - 
             m[0]  * m[11] * m[14] - 
             m[8]  * m[2] * m[15] + 
             m[8]  * m[3] * m[14] + 
             m[12] * m[2] * m[11] - 
             m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] + 
              m[0]  * m[11] * m[13] + 
              m[8]  * m[1] * m[15] - 
              m[8]  * m[3] * m[13] - 
              m[12] * m[1] * m[11] + 
              m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] - 
              m[0]  * m[10] * m[13] - 
              m[8]  * m[1] * m[14] + 
              m[8]  * m[2] * m[13] + 
              m[12] * m[1] * m[10] - 
              m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] - 
             m[1]  * m[7] * m[14] - 
             m[5]  * m[2] * m[15] + 
             m[5]  * m[3] * m[14] + 
             m[13] * m[2] * m[7] - 
             m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] + 
              m[0]  * m[7] * m[14] + 
              m[4]  * m[2] * m[15] - 
              m[4]  * m[3] * m[14] - 
              m[12] * m[2] * m[7] + 
              m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] - 
              m[0]  * m[7] * m[13] - 
              m[4]  * m[1] * m[15] + 
              m[4]  * m[3] * m[13] + 
              m[12] * m[1] * m[7] - 
              m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] + 
               m[0]  * m[6] * m[13] + 
               m[4]  * m[1] * m[14] - 
               m[4]  * m[2] * m[13] - 
               m[12] * m[1] * m[6] + 
               m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] + 
              m[1] * m[7] * m[10] + 
              m[5] * m[2] * m[11] - 
              m[5] * m[3] * m[10] - 
              m[9] * m[2] * m[7] + 
              m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] - 
             m[0] * m[7] * m[10] - 
             m[4] * m[2] * m[11] + 
             m[4] * m[3] * m[10] + 
             m[8] * m[2] * m[7] - 
             m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] + 
               m[0] * m[7] * m[9] + 
               m[4] * m[1] * m[11] - 
               m[4] * m[3] * m[9] - 
               m[8] * m[1] * m[7] + 
               m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] - 
              m[0] * m[6] * m[9] - 
              m[4] * m[1] * m[10] + 
              m[4] * m[2] * m[9] + 
              m[8] * m[1] * m[6] - 
              m[8] * m[2] * m[5];

    f64 det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
    //assert(abs(det) > 1e-5);
    //det = 1.f / det;

    for (u32 i = 0; i < 16; i++)
        in.data[i] = ({# et #})(inv[i] / det);

    return in;
}



{| end |}

static inline f32 inner_prod(quat a, quat b) {
    return dot(a.p, b.p) + a.w*b.w;
}

static inline aff operator*(aff a, scale s)
{
    return aff { {a.i.translation, a.i.rotation}, hadamard(a.scale, s.v) };
}

static inline aff operator*(translate t, aff a)
{
    return aff { { a.i.translation + t.v, a.i.rotation }, a.scale };
}

static inline vec3f div_by_w(vec4f v) {
    return v.xyz() / v.w;
}

static inline mat4 operator*(mat4 m, translate t)
{
    for (u32 i = 0; i < 4; i++) {
        f32 dot = 0;
        dot += m.data[i + 4*0] * t.v.x;
        dot += m.data[i + 4*1] * t.v.y;
        dot += m.data[i + 4*2] * t.v.z;
        m.data[i + 4*3] += dot;
    }
    return m;
}

static inline mat4 operator*(mat4 m, scale s)
{
    for (u32 i = 0; i < 4; i++) {
        m.data[i + 4*0] *= s.v.x;
        m.data[i + 4*1] *= s.v.y;
        m.data[i + 4*2] *= s.v.z;
    }
    return m;
}

static inline vec3f operator*(scale s, vec3f v)
{
    return hadamard(s.v, v);
}

static inline mat4 operator*(mat4 m, isom i)
{
    return m * translate { i.translation } * i.rotation;
}

static inline mat4 operator*(mat4 m, aff a)
{
    return m * a.i * scale { a.scale };
}

static inline bool operator==(mat4 a, mat4 b) {
    return memcmp(a.data, b.data, 16*sizeof(f32)) == 0;
}

static inline vec3f ray_plane_intersect(Ray ray, vec3f plane_o, vec3f plane_n)
{
    f32 s = dot(plane_o - ray.orig, plane_n) / dot(ray.dir, plane_n);
    return ray.orig + s*ray.dir;
}

static inline vec3f axis(u32 i)
{
    assert(i <= 2);
    vec3f a[] = { vec3f::X, vec3f::Y, vec3f::Z };
    return a[i];
}

static inline scale inv(scale s)
{
    return scale { { 1.f/s.v.x, 1.f/s.v.y, 1.f/s.v.z } };
}

static inline bool operator==(quat a, quat b)
{
    return a.p == b.p && a.w == b.w;
}

static inline bool operator==(isom a, isom b)
{
    return a.translation == b.translation && a.rotation == b.rotation;
}

static inline bool operator==(aff a, aff b)
{
    return a.i == b.i && a.scale == b.scale;
}

static inline bool almost_equal(vec3f a, vec3f b, f32 eps)
{
    return norm(a - b) <= eps;
}

static inline bool almost_equal(quat a, quat b, f32 eps)
{
    return norm(a - b) <= eps;
}

static inline bool almost_equal(isom a, isom b, f32 eps)
{
    return almost_equal(a.translation, b.translation, eps) &&
        almost_equal(a.rotation, b.rotation, eps);
}

static inline bool almost_equal(aff a, aff b, f32 eps)
{
    return almost_equal(a.i, b.i, eps) &&
        almost_equal(a.scale, b.scale, eps);
}

mat4 inv(mat4);
quat inv(quat);
vec2f line_line_intersection(vec2f v0, vec2f v1, vec2f u0, vec2f u1, f32 *vp, f32 *up);
vec3f line_line_cpa(vec3f o1, vec3f d1, vec3f o2, vec3f d2);
vec3f line_ray_cpa(vec3f o, vec3f d, Ray ray);
void to_euler(quat q, f32 *px, f32 *py, f32 *pz);
quat from_euler(f32 x, f32 y, f32 z);
quat quat_from_src_dst(vec3f src, vec3f dst);
mat4 operator*(mat4 m, rotate_axis r);

static inline isom linear(isom i) {
    return { vec3f::O, i.rotation };
}
static inline aff linear(aff a) {
    return { linear(a.i), a.scale };
}

static inline isom inv(isom i)
{
    quat ir = inv(i.rotation);
    return isom { -(ir * i.translation), ir };
}

static inline vec3f inv_apply(aff a, vec3f v)
{
    return hadamard(inv(a.i) * v, vec3f { 1.0f / a.scale.x, 1.0f / a.scale.y, 1.0f / a.scale.z });
}


f32 srgb_to_lin(f32 s);
vec3f hsl_to_rgb(vec3f hsl);
vec3f rgb_to_hsl(vec3f rgb);

struct color {
    static vec3f from_srgb888(u32 v) {
        vec3f c = {
            ((f32)((v & 0xff0000) >> 16))/255,
            ((f32)((v & 0x00ff00) >>  8))/255,
            ((f32)((v & 0x0000ff) >>  0))/255
        };
        c.x = srgb_to_lin(c.x);
        c.y = srgb_to_lin(c.y);
        c.z = srgb_to_lin(c.z);
        return c;
    }
};

inline mat4 transpose(const mat4& m)
{
    mat4 r;
    for (i32 i = 0; i < 4; i++) {
        for (i32 j = 0; j < 4; j++) {
            r.data[i + 4*j] = m.data[j + 4*i];
        }
    }
    return r;
}

inline bool is_affine(const mat4& m)
{
    return
        (m.data[4 + 4*0] == 0.f) &
        (m.data[4 + 4*1] == 0.f) &
        (m.data[4 + 4*2] == 0.f) &
        (m.data[4 + 4*2] == 1.f);
}

inline f32 random_f01();
inline i32 random_i32(i32,i32);

f32 line_line_cpa_parameter(vec3f o1, vec3f d1, vec3f o2, vec3f d2);

template<typename T> vec3f barycentric_coordinates(T p, T a, T b, T c);
template<typename T> bool is_point_inside_triangle(T p, T a, T b, T c, bool inclusive=false);

enum struct axis_type {
    X,
    Y,
    Z
};

// technically a Frame, but its a weird name
// TODO should be a mat3
struct Basis {
    vec3f O, X, Y, Z;
};

mat4 lookat(vec3f pos, vec3f target, vec3f up);

// a.x * b.y - a.y * b.x
// with less catastrophic cancellation
inline f32 det_kahan(vec2f a, vec2f b)
{
    f32 w = a.y * b.x;
    f32 e = std::fma(-a.y, b.x, w);
    f32 f = std::fma(a.x, b.y, -w);
    f32 x = f + e;
    return x;
}
f32 signed_angle_between(vec3f e1, vec3f e2, vec3f n);
vec3f perspective_lerp(vec3f A, vec3f B, f32 Aw, f32 Bw, f32 t);
vec4f inv_apply(mat4 M, vec4f rhs);
vec3f inv_apply(mat4 M, vec3f rhs);

inline vec3f clamp(vec3f a, vec3f m, vec3f M)
{
    return {clamp(a.x, m.x, M.x),
            clamp(a.y, m.y, M.y),
            clamp(a.z, m.z, M.z) };
}

inline f32 smoothstep(f32 left_edge, f32 right_edge, f32 t)
{
    f32 s = clamp((t - left_edge) / (right_edge - left_edge), 0.0, 1.0);
    return s * s * (3.f - 2.f * s);
}
