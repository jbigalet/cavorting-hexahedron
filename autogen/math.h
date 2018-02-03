
// forward declaration for the vec cast operators


struct vec2f;

struct vec3f;

struct vec4f;



struct vec2i;

struct vec3i;

struct vec4i;



struct vec2d;

struct vec3d;

struct vec4d;




static inline f32 clamp(f32 v, f32 m, f32 M) {
    return max(min(v, M), m);
}

static inline i32 clamp(i32 v, i32 m, i32 M) {
    return max(min(v, M), m);
}





    
    
struct vec2b {

    
        bool x;
    
        bool y;
    

    

    
    static const vec2b X,Y,O;

    // forward declaration of cast operators
    

    // in-place operators
    

    bool& operator[](i32 i) {
        assert(i < 2);
        return (&x)[i];
    }
    bool operator[](i32 i) const {
        assert(i < 2);
        return (&x)[i];
    }

    std::string to_string() {
        std::ostringstream stream;
        stream << "vec(" << x << "," << y << ")";
        return stream.str();
    }

};  // end of struct vec2b



WEAK const vec2b vec2b::X { 1, 0 };

WEAK const vec2b vec2b::Y { 0, 1 };

WEAK const vec2b vec2b::O { 0, 0 };



// not in-place operators of vec2b

static inline bool operator==(const vec2b& a, const vec2b& b) {
    return (a.x == b.x) && (a.y == b.y);
}

static inline bool operator!=(const vec2b& a, const vec2b& b) {
    return !(a == b);
}


static inline bool all(const vec2b& v) {
    return v.x && v.y;
}

static inline bool any(const vec2b& v) {
    return v.x || v.y;
}





// constructors
// TODO clean me up =)

static inline vec2b vec(bool z0, bool z1) { return vec2b { z0, z1 }; }

static inline vec2b vec(vec2b z0) { return vec2b { z0.x, z0.y }; }




    
    
struct vec3b {

    
        bool x;
    
        bool y;
    
        bool z;
    

    
        
            vec2b xy() const { return vec2b { x,y };}
        
            vec2b xz() const { return vec2b { x,z };}
        
            vec2b yz() const { return vec2b { y,z };}
        
    

    
    static const vec3b X,Y,Z,O;

    // forward declaration of cast operators
    

    // in-place operators
    

    bool& operator[](i32 i) {
        assert(i < 3);
        return (&x)[i];
    }
    bool operator[](i32 i) const {
        assert(i < 3);
        return (&x)[i];
    }

    std::string to_string() {
        std::ostringstream stream;
        stream << "vec(" << x << "," << y << "," << z << ")";
        return stream.str();
    }

};  // end of struct vec3b



WEAK const vec3b vec3b::X { 1, 0, 0 };

WEAK const vec3b vec3b::Y { 0, 1, 0 };

WEAK const vec3b vec3b::Z { 0, 0, 1 };

WEAK const vec3b vec3b::O { 0, 0, 0 };



// not in-place operators of vec3b

static inline bool operator==(const vec3b& a, const vec3b& b) {
    return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
}

static inline bool operator!=(const vec3b& a, const vec3b& b) {
    return !(a == b);
}


static inline bool all(const vec3b& v) {
    return v.x && v.y && v.z;
}

static inline bool any(const vec3b& v) {
    return v.x || v.y || v.z;
}





// constructors
// TODO clean me up =)

static inline vec3b vec(bool z0, bool z1, bool z2) { return vec3b { z0, z1, z2 }; }

static inline vec3b vec(bool z0, vec2b z1) { return vec3b { z0, z1.x, z1.y }; }

static inline vec3b vec(vec2b z0, bool z1) { return vec3b { z0.x, z0.y, z1 }; }

static inline vec3b vec(vec3b z0) { return vec3b { z0.x, z0.y, z0.z }; }




    
    
struct vec4b {

    
        bool x;
    
        bool y;
    
        bool z;
    
        bool w;
    

    
        
            vec2b xy() const { return vec2b { x,y };}
        
            vec2b xz() const { return vec2b { x,z };}
        
            vec2b xw() const { return vec2b { x,w };}
        
            vec2b yz() const { return vec2b { y,z };}
        
            vec2b yw() const { return vec2b { y,w };}
        
            vec2b zw() const { return vec2b { z,w };}
        
    
        
            vec3b xyz() const { return vec3b { x,y,z };}
        
            vec3b xyw() const { return vec3b { x,y,w };}
        
            vec3b xzw() const { return vec3b { x,z,w };}
        
            vec3b yzw() const { return vec3b { y,z,w };}
        
    

    
    static const vec4b X,Y,Z,W,O;

    // forward declaration of cast operators
    

    // in-place operators
    

    bool& operator[](i32 i) {
        assert(i < 4);
        return (&x)[i];
    }
    bool operator[](i32 i) const {
        assert(i < 4);
        return (&x)[i];
    }

    std::string to_string() {
        std::ostringstream stream;
        stream << "vec(" << x << "," << y << "," << z << "," << w << ")";
        return stream.str();
    }

};  // end of struct vec4b



WEAK const vec4b vec4b::X { 1, 0, 0, 0 };

WEAK const vec4b vec4b::Y { 0, 1, 0, 0 };

WEAK const vec4b vec4b::Z { 0, 0, 1, 0 };

WEAK const vec4b vec4b::W { 0, 0, 0, 1 };

WEAK const vec4b vec4b::O { 0, 0, 0, 0 };



// not in-place operators of vec4b

static inline bool operator==(const vec4b& a, const vec4b& b) {
    return (a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w);
}

static inline bool operator!=(const vec4b& a, const vec4b& b) {
    return !(a == b);
}


static inline bool all(const vec4b& v) {
    return v.x && v.y && v.z && v.w;
}

static inline bool any(const vec4b& v) {
    return v.x || v.y || v.z || v.w;
}





// constructors
// TODO clean me up =)

static inline vec4b vec(bool z0, bool z1, bool z2, bool z3) { return vec4b { z0, z1, z2, z3 }; }

static inline vec4b vec(bool z0, bool z1, vec2b z2) { return vec4b { z0, z1, z2.x, z2.y }; }

static inline vec4b vec(bool z0, vec2b z1, bool z2) { return vec4b { z0, z1.x, z1.y, z2 }; }

static inline vec4b vec(bool z0, vec3b z1) { return vec4b { z0, z1.x, z1.y, z1.z }; }

static inline vec4b vec(vec2b z0, bool z1, bool z2) { return vec4b { z0.x, z0.y, z1, z2 }; }

static inline vec4b vec(vec2b z0, vec2b z1) { return vec4b { z0.x, z0.y, z1.x, z1.y }; }

static inline vec4b vec(vec3b z0, bool z1) { return vec4b { z0.x, z0.y, z0.z, z1 }; }

static inline vec4b vec(vec4b z0) { return vec4b { z0.x, z0.y, z0.z, z0.w }; }








    
    
struct vec2f {

    
        f32 x;
    
        f32 y;
    

    

    
    static const vec2f X,Y,O;

    // forward declaration of cast operators
    
    operator vec2i() const;
    
    operator vec2d() const;
    

    // in-place operators
    
        
        vec2f& operator+=(const vec2f& v) {
            
                x += v.x;
            
                y += v.y;
            
            return *this;
        }
        
        vec2f& operator-=(const vec2f& v) {
            
                x -= v.x;
            
                y -= v.y;
            
            return *this;
        }
        

        
        vec2f& operator*=(const f32 f) {
            
            x *= f;
            
            y *= f;
            
            return *this;
        }
        
        vec2f& operator/=(const f32 f) {
            
            x /= f;
            
            y /= f;
            
            return *this;
        }
        
    

    f32& operator[](i32 i) {
        assert(i < 2);
        return (&x)[i];
    }
    f32 operator[](i32 i) const {
        assert(i < 2);
        return (&x)[i];
    }

    std::string to_string() {
        std::ostringstream stream;
        stream << "vec(" << x << "," << y << ")";
        return stream.str();
    }

};  // end of struct vec2f



WEAK const vec2f vec2f::X { 1, 0 };

WEAK const vec2f vec2f::Y { 0, 1 };

WEAK const vec2f vec2f::O { 0, 0 };



// not in-place operators of vec2f

static inline bool operator==(const vec2f& a, const vec2f& b) {
    return (a.x == b.x) && (a.y == b.y);
}

static inline bool operator!=(const vec2f& a, const vec2f& b) {
    return !(a == b);
}





static inline vec2f operator+(vec2f a, const vec2f& b) {
    a += b;
    return a;
}

static inline vec2f operator-(vec2f a, const vec2f& b) {
    a -= b;
    return a;
}



static inline vec2f operator*(vec2f v, const f32 f) {
    v *= f;
    return v;
}

static inline vec2f operator/(vec2f v, const f32 f) {
    v /= f;
    return v;
}


static inline vec2f operator*(const f32 f, vec2f v) {
    return v*f;
}

static inline vec2f operator-(const vec2f& v) {
    return vec2f { -v.x, -v.y };
}


    static inline vec2b operator<(const vec2f& a, const vec2f& b) {
        return vec2b{ (a.x < b.x), (a.y < b.y) };
    }

    static inline vec2b operator<=(const vec2f& a, const vec2f& b) {
        return vec2b{ (a.x <= b.x), (a.y <= b.y) };
    }

    static inline vec2b operator>(const vec2f& a, const vec2f& b) {
        return vec2b{ (a.x > b.x), (a.y > b.y) };
    }

    static inline vec2b operator>=(const vec2f& a, const vec2f& b) {
        return vec2b{ (a.x >= b.x), (a.y >= b.y) };
    }


static inline vec2f hadamard(const vec2f& a, const vec2f& b) {
    return vec2f { a.x*b.x, a.y*b.y };
}

static inline f32 dot(const vec2f& a, const vec2f& b) {
    return a.x*b.x + a.y*b.y;
}

static inline f32 norm2(const vec2f& v) {
    return dot(v,v);
}

static inline f32 norm(const vec2f& v) {
    return sqrt(norm2(v));
}

inline f32 manhattan(const vec2f& v) {
    return abs(v.x) + abs(v.y);
}

static inline vec2f normalized(const vec2f& v) {
    f32 n = norm2(v);
    //assert(n > 1e-8);
    return v/sqrt(n);
}


static inline vec2f round(const vec2f& v) {
    return vec2f { round(v.x), round(v.y) };
}


static inline vec2f lerp(const vec2f& a, const vec2f& b, f32 t) {
    return (1.f - t)*a + t*b;
}


static inline vec2f constant_speed_lerp(const vec2f& a, const vec2f& b, f32 dt) {
    return a + (b-a) * clamp(dt / norm(b-a), 0.f, 1.f);
}


static inline vec2f flush_to_zero(vec2f v, f32 epsilon) {
    if(norm2(v) <= epsilon*epsilon)
        return vec2f::O;
    return v;
}

static inline bool hasnan(const vec2f& v) {
    return isnan(v.x) || isnan(v.y);
}



namespace std {
    
    static inline vec2f min(const vec2f &a, const vec2f &b) {
        return vec2f{ min(a.x, b.x), min(a.y, b.y) };
    }
    
    static inline vec2f max(const vec2f &a, const vec2f &b) {
        return vec2f{ max(a.x, b.x), max(a.y, b.y) };
    }
    

    static inline vec2f abs(const vec2f &v) {
        return vec2f{ abs(v.x), abs(v.y) };
    }
}




// constructors
// TODO clean me up =)

static inline vec2f vec(f32 z0, f32 z1) { return vec2f { z0, z1 }; }

static inline vec2f vec(vec2f z0) { return vec2f { z0.x, z0.y }; }




    
    
struct vec3f {

    
        f32 x;
    
        f32 y;
    
        f32 z;
    

    
        
            vec2f xy() const { return vec2f { x,y };}
        
            vec2f xz() const { return vec2f { x,z };}
        
            vec2f yz() const { return vec2f { y,z };}
        
    

    
    static const vec3f X,Y,Z,O;

    // forward declaration of cast operators
    
    operator vec3i() const;
    
    operator vec3d() const;
    

    // in-place operators
    
        
        vec3f& operator+=(const vec3f& v) {
            
                x += v.x;
            
                y += v.y;
            
                z += v.z;
            
            return *this;
        }
        
        vec3f& operator-=(const vec3f& v) {
            
                x -= v.x;
            
                y -= v.y;
            
                z -= v.z;
            
            return *this;
        }
        

        
        vec3f& operator*=(const f32 f) {
            
            x *= f;
            
            y *= f;
            
            z *= f;
            
            return *this;
        }
        
        vec3f& operator/=(const f32 f) {
            
            x /= f;
            
            y /= f;
            
            z /= f;
            
            return *this;
        }
        
    

    f32& operator[](i32 i) {
        assert(i < 3);
        return (&x)[i];
    }
    f32 operator[](i32 i) const {
        assert(i < 3);
        return (&x)[i];
    }

    std::string to_string() {
        std::ostringstream stream;
        stream << "vec(" << x << "," << y << "," << z << ")";
        return stream.str();
    }

};  // end of struct vec3f



WEAK const vec3f vec3f::X { 1, 0, 0 };

WEAK const vec3f vec3f::Y { 0, 1, 0 };

WEAK const vec3f vec3f::Z { 0, 0, 1 };

WEAK const vec3f vec3f::O { 0, 0, 0 };



// not in-place operators of vec3f

static inline bool operator==(const vec3f& a, const vec3f& b) {
    return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
}

static inline bool operator!=(const vec3f& a, const vec3f& b) {
    return !(a == b);
}





static inline vec3f operator+(vec3f a, const vec3f& b) {
    a += b;
    return a;
}

static inline vec3f operator-(vec3f a, const vec3f& b) {
    a -= b;
    return a;
}



static inline vec3f operator*(vec3f v, const f32 f) {
    v *= f;
    return v;
}

static inline vec3f operator/(vec3f v, const f32 f) {
    v /= f;
    return v;
}


static inline vec3f operator*(const f32 f, vec3f v) {
    return v*f;
}

static inline vec3f operator-(const vec3f& v) {
    return vec3f { -v.x, -v.y, -v.z };
}


    static inline vec3b operator<(const vec3f& a, const vec3f& b) {
        return vec3b{ (a.x < b.x), (a.y < b.y), (a.z < b.z) };
    }

    static inline vec3b operator<=(const vec3f& a, const vec3f& b) {
        return vec3b{ (a.x <= b.x), (a.y <= b.y), (a.z <= b.z) };
    }

    static inline vec3b operator>(const vec3f& a, const vec3f& b) {
        return vec3b{ (a.x > b.x), (a.y > b.y), (a.z > b.z) };
    }

    static inline vec3b operator>=(const vec3f& a, const vec3f& b) {
        return vec3b{ (a.x >= b.x), (a.y >= b.y), (a.z >= b.z) };
    }


static inline vec3f hadamard(const vec3f& a, const vec3f& b) {
    return vec3f { a.x*b.x, a.y*b.y, a.z*b.z };
}

static inline f32 dot(const vec3f& a, const vec3f& b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

static inline f32 norm2(const vec3f& v) {
    return dot(v,v);
}

static inline f32 norm(const vec3f& v) {
    return sqrt(norm2(v));
}

inline f32 manhattan(const vec3f& v) {
    return abs(v.x) + abs(v.y) + abs(v.z);
}

static inline vec3f normalized(const vec3f& v) {
    f32 n = norm2(v);
    //assert(n > 1e-8);
    return v/sqrt(n);
}


static inline vec3f round(const vec3f& v) {
    return vec3f { round(v.x), round(v.y), round(v.z) };
}


static inline vec3f lerp(const vec3f& a, const vec3f& b, f32 t) {
    return (1.f - t)*a + t*b;
}


static inline vec3f constant_speed_lerp(const vec3f& a, const vec3f& b, f32 dt) {
    return a + (b-a) * clamp(dt / norm(b-a), 0.f, 1.f);
}


static inline vec3f flush_to_zero(vec3f v, f32 epsilon) {
    if(norm2(v) <= epsilon*epsilon)
        return vec3f::O;
    return v;
}

static inline bool hasnan(const vec3f& v) {
    return isnan(v.x) || isnan(v.y) || isnan(v.z);
}



namespace std {
    
    static inline vec3f min(const vec3f &a, const vec3f &b) {
        return vec3f{ min(a.x, b.x), min(a.y, b.y), min(a.z, b.z) };
    }
    
    static inline vec3f max(const vec3f &a, const vec3f &b) {
        return vec3f{ max(a.x, b.x), max(a.y, b.y), max(a.z, b.z) };
    }
    

    static inline vec3f abs(const vec3f &v) {
        return vec3f{ abs(v.x), abs(v.y), abs(v.z) };
    }
}




// constructors
// TODO clean me up =)

static inline vec3f vec(f32 z0, f32 z1, f32 z2) { return vec3f { z0, z1, z2 }; }

static inline vec3f vec(f32 z0, vec2f z1) { return vec3f { z0, z1.x, z1.y }; }

static inline vec3f vec(vec2f z0, f32 z1) { return vec3f { z0.x, z0.y, z1 }; }

static inline vec3f vec(vec3f z0) { return vec3f { z0.x, z0.y, z0.z }; }




    
    
struct vec4f {

    
        f32 x;
    
        f32 y;
    
        f32 z;
    
        f32 w;
    

    
        
            vec2f xy() const { return vec2f { x,y };}
        
            vec2f xz() const { return vec2f { x,z };}
        
            vec2f xw() const { return vec2f { x,w };}
        
            vec2f yz() const { return vec2f { y,z };}
        
            vec2f yw() const { return vec2f { y,w };}
        
            vec2f zw() const { return vec2f { z,w };}
        
    
        
            vec3f xyz() const { return vec3f { x,y,z };}
        
            vec3f xyw() const { return vec3f { x,y,w };}
        
            vec3f xzw() const { return vec3f { x,z,w };}
        
            vec3f yzw() const { return vec3f { y,z,w };}
        
    

    
    static const vec4f X,Y,Z,W,O;

    // forward declaration of cast operators
    
    operator vec4i() const;
    
    operator vec4d() const;
    

    // in-place operators
    
        
        vec4f& operator+=(const vec4f& v) {
            
                x += v.x;
            
                y += v.y;
            
                z += v.z;
            
                w += v.w;
            
            return *this;
        }
        
        vec4f& operator-=(const vec4f& v) {
            
                x -= v.x;
            
                y -= v.y;
            
                z -= v.z;
            
                w -= v.w;
            
            return *this;
        }
        

        
        vec4f& operator*=(const f32 f) {
            
            x *= f;
            
            y *= f;
            
            z *= f;
            
            w *= f;
            
            return *this;
        }
        
        vec4f& operator/=(const f32 f) {
            
            x /= f;
            
            y /= f;
            
            z /= f;
            
            w /= f;
            
            return *this;
        }
        
    

    f32& operator[](i32 i) {
        assert(i < 4);
        return (&x)[i];
    }
    f32 operator[](i32 i) const {
        assert(i < 4);
        return (&x)[i];
    }

    std::string to_string() {
        std::ostringstream stream;
        stream << "vec(" << x << "," << y << "," << z << "," << w << ")";
        return stream.str();
    }

};  // end of struct vec4f



WEAK const vec4f vec4f::X { 1, 0, 0, 0 };

WEAK const vec4f vec4f::Y { 0, 1, 0, 0 };

WEAK const vec4f vec4f::Z { 0, 0, 1, 0 };

WEAK const vec4f vec4f::W { 0, 0, 0, 1 };

WEAK const vec4f vec4f::O { 0, 0, 0, 0 };



// not in-place operators of vec4f

static inline bool operator==(const vec4f& a, const vec4f& b) {
    return (a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w);
}

static inline bool operator!=(const vec4f& a, const vec4f& b) {
    return !(a == b);
}





static inline vec4f operator+(vec4f a, const vec4f& b) {
    a += b;
    return a;
}

static inline vec4f operator-(vec4f a, const vec4f& b) {
    a -= b;
    return a;
}



static inline vec4f operator*(vec4f v, const f32 f) {
    v *= f;
    return v;
}

static inline vec4f operator/(vec4f v, const f32 f) {
    v /= f;
    return v;
}


static inline vec4f operator*(const f32 f, vec4f v) {
    return v*f;
}

static inline vec4f operator-(const vec4f& v) {
    return vec4f { -v.x, -v.y, -v.z, -v.w };
}


    static inline vec4b operator<(const vec4f& a, const vec4f& b) {
        return vec4b{ (a.x < b.x), (a.y < b.y), (a.z < b.z), (a.w < b.w) };
    }

    static inline vec4b operator<=(const vec4f& a, const vec4f& b) {
        return vec4b{ (a.x <= b.x), (a.y <= b.y), (a.z <= b.z), (a.w <= b.w) };
    }

    static inline vec4b operator>(const vec4f& a, const vec4f& b) {
        return vec4b{ (a.x > b.x), (a.y > b.y), (a.z > b.z), (a.w > b.w) };
    }

    static inline vec4b operator>=(const vec4f& a, const vec4f& b) {
        return vec4b{ (a.x >= b.x), (a.y >= b.y), (a.z >= b.z), (a.w >= b.w) };
    }


static inline vec4f hadamard(const vec4f& a, const vec4f& b) {
    return vec4f { a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w };
}

static inline f32 dot(const vec4f& a, const vec4f& b) {
    return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}

static inline f32 norm2(const vec4f& v) {
    return dot(v,v);
}

static inline f32 norm(const vec4f& v) {
    return sqrt(norm2(v));
}

inline f32 manhattan(const vec4f& v) {
    return abs(v.x) + abs(v.y) + abs(v.z) + abs(v.w);
}

static inline vec4f normalized(const vec4f& v) {
    f32 n = norm2(v);
    //assert(n > 1e-8);
    return v/sqrt(n);
}


static inline vec4f round(const vec4f& v) {
    return vec4f { round(v.x), round(v.y), round(v.z), round(v.w) };
}


static inline vec4f lerp(const vec4f& a, const vec4f& b, f32 t) {
    return (1.f - t)*a + t*b;
}


static inline vec4f constant_speed_lerp(const vec4f& a, const vec4f& b, f32 dt) {
    return a + (b-a) * clamp(dt / norm(b-a), 0.f, 1.f);
}


static inline vec4f flush_to_zero(vec4f v, f32 epsilon) {
    if(norm2(v) <= epsilon*epsilon)
        return vec4f::O;
    return v;
}

static inline bool hasnan(const vec4f& v) {
    return isnan(v.x) || isnan(v.y) || isnan(v.z) || isnan(v.w);
}



namespace std {
    
    static inline vec4f min(const vec4f &a, const vec4f &b) {
        return vec4f{ min(a.x, b.x), min(a.y, b.y), min(a.z, b.z), min(a.w, b.w) };
    }
    
    static inline vec4f max(const vec4f &a, const vec4f &b) {
        return vec4f{ max(a.x, b.x), max(a.y, b.y), max(a.z, b.z), max(a.w, b.w) };
    }
    

    static inline vec4f abs(const vec4f &v) {
        return vec4f{ abs(v.x), abs(v.y), abs(v.z), abs(v.w) };
    }
}




// constructors
// TODO clean me up =)

static inline vec4f vec(f32 z0, f32 z1, f32 z2, f32 z3) { return vec4f { z0, z1, z2, z3 }; }

static inline vec4f vec(f32 z0, f32 z1, vec2f z2) { return vec4f { z0, z1, z2.x, z2.y }; }

static inline vec4f vec(f32 z0, vec2f z1, f32 z2) { return vec4f { z0, z1.x, z1.y, z2 }; }

static inline vec4f vec(f32 z0, vec3f z1) { return vec4f { z0, z1.x, z1.y, z1.z }; }

static inline vec4f vec(vec2f z0, f32 z1, f32 z2) { return vec4f { z0.x, z0.y, z1, z2 }; }

static inline vec4f vec(vec2f z0, vec2f z1) { return vec4f { z0.x, z0.y, z1.x, z1.y }; }

static inline vec4f vec(vec3f z0, f32 z1) { return vec4f { z0.x, z0.y, z0.z, z1 }; }

static inline vec4f vec(vec4f z0) { return vec4f { z0.x, z0.y, z0.z, z0.w }; }








    
    
struct vec2i {

    
        i32 x;
    
        i32 y;
    

    

    
    static const vec2i X,Y,O;

    // forward declaration of cast operators
    
    operator vec2f() const;
    
    operator vec2d() const;
    

    // in-place operators
    
        
        vec2i& operator+=(const vec2i& v) {
            
                x += v.x;
            
                y += v.y;
            
            return *this;
        }
        
        vec2i& operator-=(const vec2i& v) {
            
                x -= v.x;
            
                y -= v.y;
            
            return *this;
        }
        

        
        vec2i& operator*=(const f32 f) {
            
            x *= f;
            
            y *= f;
            
            return *this;
        }
        
        vec2i& operator/=(const f32 f) {
            
            x /= f;
            
            y /= f;
            
            return *this;
        }
        
    

    i32& operator[](i32 i) {
        assert(i < 2);
        return (&x)[i];
    }
    i32 operator[](i32 i) const {
        assert(i < 2);
        return (&x)[i];
    }

    std::string to_string() {
        std::ostringstream stream;
        stream << "vec(" << x << "," << y << ")";
        return stream.str();
    }

};  // end of struct vec2i



WEAK const vec2i vec2i::X { 1, 0 };

WEAK const vec2i vec2i::Y { 0, 1 };

WEAK const vec2i vec2i::O { 0, 0 };



// not in-place operators of vec2i

static inline bool operator==(const vec2i& a, const vec2i& b) {
    return (a.x == b.x) && (a.y == b.y);
}

static inline bool operator!=(const vec2i& a, const vec2i& b) {
    return !(a == b);
}





static inline vec2i operator+(vec2i a, const vec2i& b) {
    a += b;
    return a;
}

static inline vec2i operator-(vec2i a, const vec2i& b) {
    a -= b;
    return a;
}



static inline vec2i operator*(vec2i v, const f32 f) {
    v *= f;
    return v;
}

static inline vec2i operator/(vec2i v, const f32 f) {
    v /= f;
    return v;
}


static inline vec2i operator*(const f32 f, vec2i v) {
    return v*f;
}

static inline vec2i operator-(const vec2i& v) {
    return vec2i { -v.x, -v.y };
}


    static inline vec2b operator<(const vec2i& a, const vec2i& b) {
        return vec2b{ (a.x < b.x), (a.y < b.y) };
    }

    static inline vec2b operator<=(const vec2i& a, const vec2i& b) {
        return vec2b{ (a.x <= b.x), (a.y <= b.y) };
    }

    static inline vec2b operator>(const vec2i& a, const vec2i& b) {
        return vec2b{ (a.x > b.x), (a.y > b.y) };
    }

    static inline vec2b operator>=(const vec2i& a, const vec2i& b) {
        return vec2b{ (a.x >= b.x), (a.y >= b.y) };
    }


static inline vec2i hadamard(const vec2i& a, const vec2i& b) {
    return vec2i { a.x*b.x, a.y*b.y };
}

static inline f32 dot(const vec2i& a, const vec2i& b) {
    return a.x*b.x + a.y*b.y;
}

static inline f32 norm2(const vec2i& v) {
    return dot(v,v);
}

static inline f32 norm(const vec2i& v) {
    return sqrt(norm2(v));
}

inline f32 manhattan(const vec2i& v) {
    return abs(v.x) + abs(v.y);
}

static inline vec2i normalized(const vec2i& v) {
    f32 n = norm2(v);
    //assert(n > 1e-8);
    return v/sqrt(n);
}




namespace std {
    
    static inline vec2i min(const vec2i &a, const vec2i &b) {
        return vec2i{ min(a.x, b.x), min(a.y, b.y) };
    }
    
    static inline vec2i max(const vec2i &a, const vec2i &b) {
        return vec2i{ max(a.x, b.x), max(a.y, b.y) };
    }
    

    static inline vec2i abs(const vec2i &v) {
        return vec2i{ abs(v.x), abs(v.y) };
    }
}




// constructors
// TODO clean me up =)

static inline vec2i vec(i32 z0, i32 z1) { return vec2i { z0, z1 }; }

static inline vec2i vec(vec2i z0) { return vec2i { z0.x, z0.y }; }




    
    
struct vec3i {

    
        i32 x;
    
        i32 y;
    
        i32 z;
    

    
        
            vec2i xy() const { return vec2i { x,y };}
        
            vec2i xz() const { return vec2i { x,z };}
        
            vec2i yz() const { return vec2i { y,z };}
        
    

    
    static const vec3i X,Y,Z,O;

    // forward declaration of cast operators
    
    operator vec3f() const;
    
    operator vec3d() const;
    

    // in-place operators
    
        
        vec3i& operator+=(const vec3i& v) {
            
                x += v.x;
            
                y += v.y;
            
                z += v.z;
            
            return *this;
        }
        
        vec3i& operator-=(const vec3i& v) {
            
                x -= v.x;
            
                y -= v.y;
            
                z -= v.z;
            
            return *this;
        }
        

        
        vec3i& operator*=(const f32 f) {
            
            x *= f;
            
            y *= f;
            
            z *= f;
            
            return *this;
        }
        
        vec3i& operator/=(const f32 f) {
            
            x /= f;
            
            y /= f;
            
            z /= f;
            
            return *this;
        }
        
    

    i32& operator[](i32 i) {
        assert(i < 3);
        return (&x)[i];
    }
    i32 operator[](i32 i) const {
        assert(i < 3);
        return (&x)[i];
    }

    std::string to_string() {
        std::ostringstream stream;
        stream << "vec(" << x << "," << y << "," << z << ")";
        return stream.str();
    }

};  // end of struct vec3i



WEAK const vec3i vec3i::X { 1, 0, 0 };

WEAK const vec3i vec3i::Y { 0, 1, 0 };

WEAK const vec3i vec3i::Z { 0, 0, 1 };

WEAK const vec3i vec3i::O { 0, 0, 0 };



// not in-place operators of vec3i

static inline bool operator==(const vec3i& a, const vec3i& b) {
    return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
}

static inline bool operator!=(const vec3i& a, const vec3i& b) {
    return !(a == b);
}





static inline vec3i operator+(vec3i a, const vec3i& b) {
    a += b;
    return a;
}

static inline vec3i operator-(vec3i a, const vec3i& b) {
    a -= b;
    return a;
}



static inline vec3i operator*(vec3i v, const f32 f) {
    v *= f;
    return v;
}

static inline vec3i operator/(vec3i v, const f32 f) {
    v /= f;
    return v;
}


static inline vec3i operator*(const f32 f, vec3i v) {
    return v*f;
}

static inline vec3i operator-(const vec3i& v) {
    return vec3i { -v.x, -v.y, -v.z };
}


    static inline vec3b operator<(const vec3i& a, const vec3i& b) {
        return vec3b{ (a.x < b.x), (a.y < b.y), (a.z < b.z) };
    }

    static inline vec3b operator<=(const vec3i& a, const vec3i& b) {
        return vec3b{ (a.x <= b.x), (a.y <= b.y), (a.z <= b.z) };
    }

    static inline vec3b operator>(const vec3i& a, const vec3i& b) {
        return vec3b{ (a.x > b.x), (a.y > b.y), (a.z > b.z) };
    }

    static inline vec3b operator>=(const vec3i& a, const vec3i& b) {
        return vec3b{ (a.x >= b.x), (a.y >= b.y), (a.z >= b.z) };
    }


static inline vec3i hadamard(const vec3i& a, const vec3i& b) {
    return vec3i { a.x*b.x, a.y*b.y, a.z*b.z };
}

static inline f32 dot(const vec3i& a, const vec3i& b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

static inline f32 norm2(const vec3i& v) {
    return dot(v,v);
}

static inline f32 norm(const vec3i& v) {
    return sqrt(norm2(v));
}

inline f32 manhattan(const vec3i& v) {
    return abs(v.x) + abs(v.y) + abs(v.z);
}

static inline vec3i normalized(const vec3i& v) {
    f32 n = norm2(v);
    //assert(n > 1e-8);
    return v/sqrt(n);
}




namespace std {
    
    static inline vec3i min(const vec3i &a, const vec3i &b) {
        return vec3i{ min(a.x, b.x), min(a.y, b.y), min(a.z, b.z) };
    }
    
    static inline vec3i max(const vec3i &a, const vec3i &b) {
        return vec3i{ max(a.x, b.x), max(a.y, b.y), max(a.z, b.z) };
    }
    

    static inline vec3i abs(const vec3i &v) {
        return vec3i{ abs(v.x), abs(v.y), abs(v.z) };
    }
}




// constructors
// TODO clean me up =)

static inline vec3i vec(i32 z0, i32 z1, i32 z2) { return vec3i { z0, z1, z2 }; }

static inline vec3i vec(i32 z0, vec2i z1) { return vec3i { z0, z1.x, z1.y }; }

static inline vec3i vec(vec2i z0, i32 z1) { return vec3i { z0.x, z0.y, z1 }; }

static inline vec3i vec(vec3i z0) { return vec3i { z0.x, z0.y, z0.z }; }




    
    
struct vec4i {

    
        i32 x;
    
        i32 y;
    
        i32 z;
    
        i32 w;
    

    
        
            vec2i xy() const { return vec2i { x,y };}
        
            vec2i xz() const { return vec2i { x,z };}
        
            vec2i xw() const { return vec2i { x,w };}
        
            vec2i yz() const { return vec2i { y,z };}
        
            vec2i yw() const { return vec2i { y,w };}
        
            vec2i zw() const { return vec2i { z,w };}
        
    
        
            vec3i xyz() const { return vec3i { x,y,z };}
        
            vec3i xyw() const { return vec3i { x,y,w };}
        
            vec3i xzw() const { return vec3i { x,z,w };}
        
            vec3i yzw() const { return vec3i { y,z,w };}
        
    

    
    static const vec4i X,Y,Z,W,O;

    // forward declaration of cast operators
    
    operator vec4f() const;
    
    operator vec4d() const;
    

    // in-place operators
    
        
        vec4i& operator+=(const vec4i& v) {
            
                x += v.x;
            
                y += v.y;
            
                z += v.z;
            
                w += v.w;
            
            return *this;
        }
        
        vec4i& operator-=(const vec4i& v) {
            
                x -= v.x;
            
                y -= v.y;
            
                z -= v.z;
            
                w -= v.w;
            
            return *this;
        }
        

        
        vec4i& operator*=(const f32 f) {
            
            x *= f;
            
            y *= f;
            
            z *= f;
            
            w *= f;
            
            return *this;
        }
        
        vec4i& operator/=(const f32 f) {
            
            x /= f;
            
            y /= f;
            
            z /= f;
            
            w /= f;
            
            return *this;
        }
        
    

    i32& operator[](i32 i) {
        assert(i < 4);
        return (&x)[i];
    }
    i32 operator[](i32 i) const {
        assert(i < 4);
        return (&x)[i];
    }

    std::string to_string() {
        std::ostringstream stream;
        stream << "vec(" << x << "," << y << "," << z << "," << w << ")";
        return stream.str();
    }

};  // end of struct vec4i



WEAK const vec4i vec4i::X { 1, 0, 0, 0 };

WEAK const vec4i vec4i::Y { 0, 1, 0, 0 };

WEAK const vec4i vec4i::Z { 0, 0, 1, 0 };

WEAK const vec4i vec4i::W { 0, 0, 0, 1 };

WEAK const vec4i vec4i::O { 0, 0, 0, 0 };



// not in-place operators of vec4i

static inline bool operator==(const vec4i& a, const vec4i& b) {
    return (a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w);
}

static inline bool operator!=(const vec4i& a, const vec4i& b) {
    return !(a == b);
}





static inline vec4i operator+(vec4i a, const vec4i& b) {
    a += b;
    return a;
}

static inline vec4i operator-(vec4i a, const vec4i& b) {
    a -= b;
    return a;
}



static inline vec4i operator*(vec4i v, const f32 f) {
    v *= f;
    return v;
}

static inline vec4i operator/(vec4i v, const f32 f) {
    v /= f;
    return v;
}


static inline vec4i operator*(const f32 f, vec4i v) {
    return v*f;
}

static inline vec4i operator-(const vec4i& v) {
    return vec4i { -v.x, -v.y, -v.z, -v.w };
}


    static inline vec4b operator<(const vec4i& a, const vec4i& b) {
        return vec4b{ (a.x < b.x), (a.y < b.y), (a.z < b.z), (a.w < b.w) };
    }

    static inline vec4b operator<=(const vec4i& a, const vec4i& b) {
        return vec4b{ (a.x <= b.x), (a.y <= b.y), (a.z <= b.z), (a.w <= b.w) };
    }

    static inline vec4b operator>(const vec4i& a, const vec4i& b) {
        return vec4b{ (a.x > b.x), (a.y > b.y), (a.z > b.z), (a.w > b.w) };
    }

    static inline vec4b operator>=(const vec4i& a, const vec4i& b) {
        return vec4b{ (a.x >= b.x), (a.y >= b.y), (a.z >= b.z), (a.w >= b.w) };
    }


static inline vec4i hadamard(const vec4i& a, const vec4i& b) {
    return vec4i { a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w };
}

static inline f32 dot(const vec4i& a, const vec4i& b) {
    return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}

static inline f32 norm2(const vec4i& v) {
    return dot(v,v);
}

static inline f32 norm(const vec4i& v) {
    return sqrt(norm2(v));
}

inline f32 manhattan(const vec4i& v) {
    return abs(v.x) + abs(v.y) + abs(v.z) + abs(v.w);
}

static inline vec4i normalized(const vec4i& v) {
    f32 n = norm2(v);
    //assert(n > 1e-8);
    return v/sqrt(n);
}




namespace std {
    
    static inline vec4i min(const vec4i &a, const vec4i &b) {
        return vec4i{ min(a.x, b.x), min(a.y, b.y), min(a.z, b.z), min(a.w, b.w) };
    }
    
    static inline vec4i max(const vec4i &a, const vec4i &b) {
        return vec4i{ max(a.x, b.x), max(a.y, b.y), max(a.z, b.z), max(a.w, b.w) };
    }
    

    static inline vec4i abs(const vec4i &v) {
        return vec4i{ abs(v.x), abs(v.y), abs(v.z), abs(v.w) };
    }
}




// constructors
// TODO clean me up =)

static inline vec4i vec(i32 z0, i32 z1, i32 z2, i32 z3) { return vec4i { z0, z1, z2, z3 }; }

static inline vec4i vec(i32 z0, i32 z1, vec2i z2) { return vec4i { z0, z1, z2.x, z2.y }; }

static inline vec4i vec(i32 z0, vec2i z1, i32 z2) { return vec4i { z0, z1.x, z1.y, z2 }; }

static inline vec4i vec(i32 z0, vec3i z1) { return vec4i { z0, z1.x, z1.y, z1.z }; }

static inline vec4i vec(vec2i z0, i32 z1, i32 z2) { return vec4i { z0.x, z0.y, z1, z2 }; }

static inline vec4i vec(vec2i z0, vec2i z1) { return vec4i { z0.x, z0.y, z1.x, z1.y }; }

static inline vec4i vec(vec3i z0, i32 z1) { return vec4i { z0.x, z0.y, z0.z, z1 }; }

static inline vec4i vec(vec4i z0) { return vec4i { z0.x, z0.y, z0.z, z0.w }; }








    
    
struct vec2d {

    
        f64 x;
    
        f64 y;
    

    

    
    static const vec2d X,Y,O;

    // forward declaration of cast operators
    
    operator vec2f() const;
    
    operator vec2i() const;
    

    // in-place operators
    
        
        vec2d& operator+=(const vec2d& v) {
            
                x += v.x;
            
                y += v.y;
            
            return *this;
        }
        
        vec2d& operator-=(const vec2d& v) {
            
                x -= v.x;
            
                y -= v.y;
            
            return *this;
        }
        

        
        vec2d& operator*=(const f32 f) {
            
            x *= f;
            
            y *= f;
            
            return *this;
        }
        
        vec2d& operator/=(const f32 f) {
            
            x /= f;
            
            y /= f;
            
            return *this;
        }
        
    

    f64& operator[](i32 i) {
        assert(i < 2);
        return (&x)[i];
    }
    f64 operator[](i32 i) const {
        assert(i < 2);
        return (&x)[i];
    }

    std::string to_string() {
        std::ostringstream stream;
        stream << "vec(" << x << "," << y << ")";
        return stream.str();
    }

};  // end of struct vec2d



WEAK const vec2d vec2d::X { 1, 0 };

WEAK const vec2d vec2d::Y { 0, 1 };

WEAK const vec2d vec2d::O { 0, 0 };



// not in-place operators of vec2d

static inline bool operator==(const vec2d& a, const vec2d& b) {
    return (a.x == b.x) && (a.y == b.y);
}

static inline bool operator!=(const vec2d& a, const vec2d& b) {
    return !(a == b);
}





static inline vec2d operator+(vec2d a, const vec2d& b) {
    a += b;
    return a;
}

static inline vec2d operator-(vec2d a, const vec2d& b) {
    a -= b;
    return a;
}



static inline vec2d operator*(vec2d v, const f32 f) {
    v *= f;
    return v;
}

static inline vec2d operator/(vec2d v, const f32 f) {
    v /= f;
    return v;
}


static inline vec2d operator*(const f32 f, vec2d v) {
    return v*f;
}

static inline vec2d operator-(const vec2d& v) {
    return vec2d { -v.x, -v.y };
}


    static inline vec2b operator<(const vec2d& a, const vec2d& b) {
        return vec2b{ (a.x < b.x), (a.y < b.y) };
    }

    static inline vec2b operator<=(const vec2d& a, const vec2d& b) {
        return vec2b{ (a.x <= b.x), (a.y <= b.y) };
    }

    static inline vec2b operator>(const vec2d& a, const vec2d& b) {
        return vec2b{ (a.x > b.x), (a.y > b.y) };
    }

    static inline vec2b operator>=(const vec2d& a, const vec2d& b) {
        return vec2b{ (a.x >= b.x), (a.y >= b.y) };
    }


static inline vec2d hadamard(const vec2d& a, const vec2d& b) {
    return vec2d { a.x*b.x, a.y*b.y };
}

static inline f32 dot(const vec2d& a, const vec2d& b) {
    return a.x*b.x + a.y*b.y;
}

static inline f32 norm2(const vec2d& v) {
    return dot(v,v);
}

static inline f32 norm(const vec2d& v) {
    return sqrt(norm2(v));
}

inline f32 manhattan(const vec2d& v) {
    return abs(v.x) + abs(v.y);
}

static inline vec2d normalized(const vec2d& v) {
    f32 n = norm2(v);
    //assert(n > 1e-8);
    return v/sqrt(n);
}




namespace std {
    
    static inline vec2d min(const vec2d &a, const vec2d &b) {
        return vec2d{ min(a.x, b.x), min(a.y, b.y) };
    }
    
    static inline vec2d max(const vec2d &a, const vec2d &b) {
        return vec2d{ max(a.x, b.x), max(a.y, b.y) };
    }
    

    static inline vec2d abs(const vec2d &v) {
        return vec2d{ abs(v.x), abs(v.y) };
    }
}




// constructors
// TODO clean me up =)

static inline vec2d vec(f64 z0, f64 z1) { return vec2d { z0, z1 }; }

static inline vec2d vec(vec2d z0) { return vec2d { z0.x, z0.y }; }




    
    
struct vec3d {

    
        f64 x;
    
        f64 y;
    
        f64 z;
    

    
        
            vec2d xy() const { return vec2d { x,y };}
        
            vec2d xz() const { return vec2d { x,z };}
        
            vec2d yz() const { return vec2d { y,z };}
        
    

    
    static const vec3d X,Y,Z,O;

    // forward declaration of cast operators
    
    operator vec3f() const;
    
    operator vec3i() const;
    

    // in-place operators
    
        
        vec3d& operator+=(const vec3d& v) {
            
                x += v.x;
            
                y += v.y;
            
                z += v.z;
            
            return *this;
        }
        
        vec3d& operator-=(const vec3d& v) {
            
                x -= v.x;
            
                y -= v.y;
            
                z -= v.z;
            
            return *this;
        }
        

        
        vec3d& operator*=(const f32 f) {
            
            x *= f;
            
            y *= f;
            
            z *= f;
            
            return *this;
        }
        
        vec3d& operator/=(const f32 f) {
            
            x /= f;
            
            y /= f;
            
            z /= f;
            
            return *this;
        }
        
    

    f64& operator[](i32 i) {
        assert(i < 3);
        return (&x)[i];
    }
    f64 operator[](i32 i) const {
        assert(i < 3);
        return (&x)[i];
    }

    std::string to_string() {
        std::ostringstream stream;
        stream << "vec(" << x << "," << y << "," << z << ")";
        return stream.str();
    }

};  // end of struct vec3d



WEAK const vec3d vec3d::X { 1, 0, 0 };

WEAK const vec3d vec3d::Y { 0, 1, 0 };

WEAK const vec3d vec3d::Z { 0, 0, 1 };

WEAK const vec3d vec3d::O { 0, 0, 0 };



// not in-place operators of vec3d

static inline bool operator==(const vec3d& a, const vec3d& b) {
    return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
}

static inline bool operator!=(const vec3d& a, const vec3d& b) {
    return !(a == b);
}





static inline vec3d operator+(vec3d a, const vec3d& b) {
    a += b;
    return a;
}

static inline vec3d operator-(vec3d a, const vec3d& b) {
    a -= b;
    return a;
}



static inline vec3d operator*(vec3d v, const f32 f) {
    v *= f;
    return v;
}

static inline vec3d operator/(vec3d v, const f32 f) {
    v /= f;
    return v;
}


static inline vec3d operator*(const f32 f, vec3d v) {
    return v*f;
}

static inline vec3d operator-(const vec3d& v) {
    return vec3d { -v.x, -v.y, -v.z };
}


    static inline vec3b operator<(const vec3d& a, const vec3d& b) {
        return vec3b{ (a.x < b.x), (a.y < b.y), (a.z < b.z) };
    }

    static inline vec3b operator<=(const vec3d& a, const vec3d& b) {
        return vec3b{ (a.x <= b.x), (a.y <= b.y), (a.z <= b.z) };
    }

    static inline vec3b operator>(const vec3d& a, const vec3d& b) {
        return vec3b{ (a.x > b.x), (a.y > b.y), (a.z > b.z) };
    }

    static inline vec3b operator>=(const vec3d& a, const vec3d& b) {
        return vec3b{ (a.x >= b.x), (a.y >= b.y), (a.z >= b.z) };
    }


static inline vec3d hadamard(const vec3d& a, const vec3d& b) {
    return vec3d { a.x*b.x, a.y*b.y, a.z*b.z };
}

static inline f32 dot(const vec3d& a, const vec3d& b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

static inline f32 norm2(const vec3d& v) {
    return dot(v,v);
}

static inline f32 norm(const vec3d& v) {
    return sqrt(norm2(v));
}

inline f32 manhattan(const vec3d& v) {
    return abs(v.x) + abs(v.y) + abs(v.z);
}

static inline vec3d normalized(const vec3d& v) {
    f32 n = norm2(v);
    //assert(n > 1e-8);
    return v/sqrt(n);
}




namespace std {
    
    static inline vec3d min(const vec3d &a, const vec3d &b) {
        return vec3d{ min(a.x, b.x), min(a.y, b.y), min(a.z, b.z) };
    }
    
    static inline vec3d max(const vec3d &a, const vec3d &b) {
        return vec3d{ max(a.x, b.x), max(a.y, b.y), max(a.z, b.z) };
    }
    

    static inline vec3d abs(const vec3d &v) {
        return vec3d{ abs(v.x), abs(v.y), abs(v.z) };
    }
}




// constructors
// TODO clean me up =)

static inline vec3d vec(f64 z0, f64 z1, f64 z2) { return vec3d { z0, z1, z2 }; }

static inline vec3d vec(f64 z0, vec2d z1) { return vec3d { z0, z1.x, z1.y }; }

static inline vec3d vec(vec2d z0, f64 z1) { return vec3d { z0.x, z0.y, z1 }; }

static inline vec3d vec(vec3d z0) { return vec3d { z0.x, z0.y, z0.z }; }




    
    
struct vec4d {

    
        f64 x;
    
        f64 y;
    
        f64 z;
    
        f64 w;
    

    
        
            vec2d xy() const { return vec2d { x,y };}
        
            vec2d xz() const { return vec2d { x,z };}
        
            vec2d xw() const { return vec2d { x,w };}
        
            vec2d yz() const { return vec2d { y,z };}
        
            vec2d yw() const { return vec2d { y,w };}
        
            vec2d zw() const { return vec2d { z,w };}
        
    
        
            vec3d xyz() const { return vec3d { x,y,z };}
        
            vec3d xyw() const { return vec3d { x,y,w };}
        
            vec3d xzw() const { return vec3d { x,z,w };}
        
            vec3d yzw() const { return vec3d { y,z,w };}
        
    

    
    static const vec4d X,Y,Z,W,O;

    // forward declaration of cast operators
    
    operator vec4f() const;
    
    operator vec4i() const;
    

    // in-place operators
    
        
        vec4d& operator+=(const vec4d& v) {
            
                x += v.x;
            
                y += v.y;
            
                z += v.z;
            
                w += v.w;
            
            return *this;
        }
        
        vec4d& operator-=(const vec4d& v) {
            
                x -= v.x;
            
                y -= v.y;
            
                z -= v.z;
            
                w -= v.w;
            
            return *this;
        }
        

        
        vec4d& operator*=(const f32 f) {
            
            x *= f;
            
            y *= f;
            
            z *= f;
            
            w *= f;
            
            return *this;
        }
        
        vec4d& operator/=(const f32 f) {
            
            x /= f;
            
            y /= f;
            
            z /= f;
            
            w /= f;
            
            return *this;
        }
        
    

    f64& operator[](i32 i) {
        assert(i < 4);
        return (&x)[i];
    }
    f64 operator[](i32 i) const {
        assert(i < 4);
        return (&x)[i];
    }

    std::string to_string() {
        std::ostringstream stream;
        stream << "vec(" << x << "," << y << "," << z << "," << w << ")";
        return stream.str();
    }

};  // end of struct vec4d



WEAK const vec4d vec4d::X { 1, 0, 0, 0 };

WEAK const vec4d vec4d::Y { 0, 1, 0, 0 };

WEAK const vec4d vec4d::Z { 0, 0, 1, 0 };

WEAK const vec4d vec4d::W { 0, 0, 0, 1 };

WEAK const vec4d vec4d::O { 0, 0, 0, 0 };



// not in-place operators of vec4d

static inline bool operator==(const vec4d& a, const vec4d& b) {
    return (a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w);
}

static inline bool operator!=(const vec4d& a, const vec4d& b) {
    return !(a == b);
}





static inline vec4d operator+(vec4d a, const vec4d& b) {
    a += b;
    return a;
}

static inline vec4d operator-(vec4d a, const vec4d& b) {
    a -= b;
    return a;
}



static inline vec4d operator*(vec4d v, const f32 f) {
    v *= f;
    return v;
}

static inline vec4d operator/(vec4d v, const f32 f) {
    v /= f;
    return v;
}


static inline vec4d operator*(const f32 f, vec4d v) {
    return v*f;
}

static inline vec4d operator-(const vec4d& v) {
    return vec4d { -v.x, -v.y, -v.z, -v.w };
}


    static inline vec4b operator<(const vec4d& a, const vec4d& b) {
        return vec4b{ (a.x < b.x), (a.y < b.y), (a.z < b.z), (a.w < b.w) };
    }

    static inline vec4b operator<=(const vec4d& a, const vec4d& b) {
        return vec4b{ (a.x <= b.x), (a.y <= b.y), (a.z <= b.z), (a.w <= b.w) };
    }

    static inline vec4b operator>(const vec4d& a, const vec4d& b) {
        return vec4b{ (a.x > b.x), (a.y > b.y), (a.z > b.z), (a.w > b.w) };
    }

    static inline vec4b operator>=(const vec4d& a, const vec4d& b) {
        return vec4b{ (a.x >= b.x), (a.y >= b.y), (a.z >= b.z), (a.w >= b.w) };
    }


static inline vec4d hadamard(const vec4d& a, const vec4d& b) {
    return vec4d { a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w };
}

static inline f32 dot(const vec4d& a, const vec4d& b) {
    return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}

static inline f32 norm2(const vec4d& v) {
    return dot(v,v);
}

static inline f32 norm(const vec4d& v) {
    return sqrt(norm2(v));
}

inline f32 manhattan(const vec4d& v) {
    return abs(v.x) + abs(v.y) + abs(v.z) + abs(v.w);
}

static inline vec4d normalized(const vec4d& v) {
    f32 n = norm2(v);
    //assert(n > 1e-8);
    return v/sqrt(n);
}




namespace std {
    
    static inline vec4d min(const vec4d &a, const vec4d &b) {
        return vec4d{ min(a.x, b.x), min(a.y, b.y), min(a.z, b.z), min(a.w, b.w) };
    }
    
    static inline vec4d max(const vec4d &a, const vec4d &b) {
        return vec4d{ max(a.x, b.x), max(a.y, b.y), max(a.z, b.z), max(a.w, b.w) };
    }
    

    static inline vec4d abs(const vec4d &v) {
        return vec4d{ abs(v.x), abs(v.y), abs(v.z), abs(v.w) };
    }
}




// constructors
// TODO clean me up =)

static inline vec4d vec(f64 z0, f64 z1, f64 z2, f64 z3) { return vec4d { z0, z1, z2, z3 }; }

static inline vec4d vec(f64 z0, f64 z1, vec2d z2) { return vec4d { z0, z1, z2.x, z2.y }; }

static inline vec4d vec(f64 z0, vec2d z1, f64 z2) { return vec4d { z0, z1.x, z1.y, z2 }; }

static inline vec4d vec(f64 z0, vec3d z1) { return vec4d { z0, z1.x, z1.y, z1.z }; }

static inline vec4d vec(vec2d z0, f64 z1, f64 z2) { return vec4d { z0.x, z0.y, z1, z2 }; }

static inline vec4d vec(vec2d z0, vec2d z1) { return vec4d { z0.x, z0.y, z1.x, z1.y }; }

static inline vec4d vec(vec3d z0, f64 z1) { return vec4d { z0.x, z0.y, z0.z, z1 }; }

static inline vec4d vec(vec4d z0) { return vec4d { z0.x, z0.y, z0.z, z0.w }; }









// vec cast operator implementations


    
    

    
inline vec2f::operator vec2i() const {
        return vec2i { (i32)x, (i32)y };
    }
    
inline vec2f::operator vec2d() const {
        return vec2d { (f64)x, (f64)y };
    }
    

    
    

    
inline vec3f::operator vec3i() const {
        return vec3i { (i32)x, (i32)y, (i32)z };
    }
    
inline vec3f::operator vec3d() const {
        return vec3d { (f64)x, (f64)y, (f64)z };
    }
    

    
    

    
inline vec4f::operator vec4i() const {
        return vec4i { (i32)x, (i32)y, (i32)z, (i32)w };
    }
    
inline vec4f::operator vec4d() const {
        return vec4d { (f64)x, (f64)y, (f64)z, (f64)w };
    }
    



    
    

    
inline vec2i::operator vec2f() const {
        return vec2f { (f32)x, (f32)y };
    }
    
inline vec2i::operator vec2d() const {
        return vec2d { (f64)x, (f64)y };
    }
    

    
    

    
inline vec3i::operator vec3f() const {
        return vec3f { (f32)x, (f32)y, (f32)z };
    }
    
inline vec3i::operator vec3d() const {
        return vec3d { (f64)x, (f64)y, (f64)z };
    }
    

    
    

    
inline vec4i::operator vec4f() const {
        return vec4f { (f32)x, (f32)y, (f32)z, (f32)w };
    }
    
inline vec4i::operator vec4d() const {
        return vec4d { (f64)x, (f64)y, (f64)z, (f64)w };
    }
    



    
    

    
inline vec2d::operator vec2f() const {
        return vec2f { (f32)x, (f32)y };
    }
    
inline vec2d::operator vec2i() const {
        return vec2i { (i32)x, (i32)y };
    }
    

    
    

    
inline vec3d::operator vec3f() const {
        return vec3f { (f32)x, (f32)y, (f32)z };
    }
    
inline vec3d::operator vec3i() const {
        return vec3i { (i32)x, (i32)y, (i32)z };
    }
    

    
    

    
inline vec4d::operator vec4f() const {
        return vec4f { (f32)x, (f32)y, (f32)z, (f32)w };
    }
    
inline vec4d::operator vec4i() const {
        return vec4i { (i32)x, (i32)y, (i32)z, (i32)w };
    }
    








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



struct mat4;

struct mat4d;


struct mat4 {
    f32 data[16];
    mat4() {}
    
    
    
    
    static mat4 cast(const mat4d&);
    
    
    mat4(std::initializer_list<f32> fs);
    static mat4 id();
    static mat4 proj(f32 n, f32 f, f32 fov, u32 w, u32 h);
    static mat4 ortho(f32 n, f32 f, f32 e, u32 w, u32 h);
};

struct mat4d {
    f64 data[16];
    mat4d() {}
    
    
    static mat4d cast(const mat4&);
    
    
    
    
    mat4d(std::initializer_list<f64> fs);
    static mat4d id();
    static mat4d proj(f64 n, f64 f, f64 fov, u32 w, u32 h);
    static mat4d ortho(f64 n, f64 f, f64 e, u32 w, u32 h);
};




// AABBs



// forward declaration for aabb cast constructor


class aabb2f;

class aabb3f;

class aabb4f;



class aabb2i;

class aabb3i;

class aabb4i;







class aabb2f {
public:
    vec2f corner;
    vec2f dim;

    aabb2f() : corner(vec2f()), dim(vec2f()) {}
    aabb2f(const vec2f corner, const vec2f dim) : corner(corner), dim(dim) {}

    // forward declaration of cast constructors
    
    aabb2f(const aabb2i& r);
    

    bool contains(const vec2f p) const {
        vec2f diff = p-corner;
        return all(diff <= dim) && all(vec2f() <= diff);
    }

    bool intersects(const aabb2f& b) {
        return all(
                std::min(opposite_corner(), b.opposite_corner())
              > std::max(corner, b.corner)
        );
    }

    aabb2f& operator&=(const aabb2f& b) {
        vec2f opposite = std::min(opposite_corner(), b.opposite_corner());
        corner = std::max(corner, b.corner);
        if(any(opposite < corner))
            dim = vec2f::O;
        else
            dim = opposite-corner;
        return *this;
    }

    aabb2f& operator|=(const aabb2f& b) {
        vec2f opposite = std::max(opposite_corner(), b.opposite_corner());
        corner = std::min(corner, b.corner);
        dim = opposite-corner;
        return *this;
    }

    f32 right() const {
        return corner.x + dim.x;
    }

    f32 bottom() const {
        return corner.y + dim.y;
    }

    vec2f center() const {
        return corner + dim/2.f;
    };

    vec2f opposite_corner() const {
        return corner + dim;
    }

    aabb2f with_margin(vec2f margin) {
        return aabb2f(
            corner + margin,
            dim - 2*margin
        );
    }

    f32 volume() const {
        return dim.x*dim.y;
    }

    // TODO for every size
    
    aabb2f clip(const aabb2f& scissor) {
        vec2f top_left = vec2f {
            clamp(corner.x, scissor.corner.x, scissor.right()),
            clamp(corner.y, scissor.corner.y, scissor.bottom())
        };
        vec2f bottom_right = vec2f {
            clamp(right(), scissor.corner.x, scissor.right()),
            clamp(bottom(), scissor.corner.y, scissor.bottom())
        };
        return aabb2f(top_left, bottom_right-top_left);
    }
    

    std::string to_string() {
        std::ostringstream stream;
        stream << "AABB(" << corner.to_string() << ", " << dim.to_string() << ")";
        return stream.str();
    }
};

static inline bool operator==(const aabb2f& a, const aabb2f& b) {
    return a.corner == b.corner && a.dim == b.dim;
}

static inline bool operator!=(const aabb2f& a, const aabb2f& b) {
    return !(a == b);
}


static inline aabb2f operator&(aabb2f a, const aabb2f& b) {
    a &= b;
    return a;
}

static inline aabb2f operator|(aabb2f a, const aabb2f& b) {
    a |= b;
    return a;
}


static inline bool hasnan(const aabb2f& aabb) {
    return hasnan(aabb.dim) || hasnan(aabb.corner);
}

static inline aabb2f bounding_box(std::vector<vec2f> points) {
    aabb2f bb;
    if(points.size() > 0)
        bb.corner = points[0];
    for(u32 i=1 ; i<points.size() ; i++)
        bb |= aabb2f(points[i], vec2f());
    return bb;
}




class aabb3f {
public:
    vec3f corner;
    vec3f dim;

    aabb3f() : corner(vec3f()), dim(vec3f()) {}
    aabb3f(const vec3f corner, const vec3f dim) : corner(corner), dim(dim) {}

    // forward declaration of cast constructors
    
    aabb3f(const aabb3i& r);
    

    bool contains(const vec3f p) const {
        vec3f diff = p-corner;
        return all(diff <= dim) && all(vec3f() <= diff);
    }

    bool intersects(const aabb3f& b) {
        return all(
                std::min(opposite_corner(), b.opposite_corner())
              > std::max(corner, b.corner)
        );
    }

    aabb3f& operator&=(const aabb3f& b) {
        vec3f opposite = std::min(opposite_corner(), b.opposite_corner());
        corner = std::max(corner, b.corner);
        if(any(opposite < corner))
            dim = vec3f::O;
        else
            dim = opposite-corner;
        return *this;
    }

    aabb3f& operator|=(const aabb3f& b) {
        vec3f opposite = std::max(opposite_corner(), b.opposite_corner());
        corner = std::min(corner, b.corner);
        dim = opposite-corner;
        return *this;
    }

    f32 right() const {
        return corner.x + dim.x;
    }

    f32 bottom() const {
        return corner.y + dim.y;
    }

    vec3f center() const {
        return corner + dim/2.f;
    };

    vec3f opposite_corner() const {
        return corner + dim;
    }

    aabb3f with_margin(vec3f margin) {
        return aabb3f(
            corner + margin,
            dim - 2*margin
        );
    }

    f32 volume() const {
        return dim.x*dim.y*dim.z;
    }

    // TODO for every size
    

    std::string to_string() {
        std::ostringstream stream;
        stream << "AABB(" << corner.to_string() << ", " << dim.to_string() << ")";
        return stream.str();
    }
};

static inline bool operator==(const aabb3f& a, const aabb3f& b) {
    return a.corner == b.corner && a.dim == b.dim;
}

static inline bool operator!=(const aabb3f& a, const aabb3f& b) {
    return !(a == b);
}


static inline aabb3f operator&(aabb3f a, const aabb3f& b) {
    a &= b;
    return a;
}

static inline aabb3f operator|(aabb3f a, const aabb3f& b) {
    a |= b;
    return a;
}


static inline bool hasnan(const aabb3f& aabb) {
    return hasnan(aabb.dim) || hasnan(aabb.corner);
}

static inline aabb3f bounding_box(std::vector<vec3f> points) {
    aabb3f bb;
    if(points.size() > 0)
        bb.corner = points[0];
    for(u32 i=1 ; i<points.size() ; i++)
        bb |= aabb3f(points[i], vec3f());
    return bb;
}




class aabb4f {
public:
    vec4f corner;
    vec4f dim;

    aabb4f() : corner(vec4f()), dim(vec4f()) {}
    aabb4f(const vec4f corner, const vec4f dim) : corner(corner), dim(dim) {}

    // forward declaration of cast constructors
    
    aabb4f(const aabb4i& r);
    

    bool contains(const vec4f p) const {
        vec4f diff = p-corner;
        return all(diff <= dim) && all(vec4f() <= diff);
    }

    bool intersects(const aabb4f& b) {
        return all(
                std::min(opposite_corner(), b.opposite_corner())
              > std::max(corner, b.corner)
        );
    }

    aabb4f& operator&=(const aabb4f& b) {
        vec4f opposite = std::min(opposite_corner(), b.opposite_corner());
        corner = std::max(corner, b.corner);
        if(any(opposite < corner))
            dim = vec4f::O;
        else
            dim = opposite-corner;
        return *this;
    }

    aabb4f& operator|=(const aabb4f& b) {
        vec4f opposite = std::max(opposite_corner(), b.opposite_corner());
        corner = std::min(corner, b.corner);
        dim = opposite-corner;
        return *this;
    }

    f32 right() const {
        return corner.x + dim.x;
    }

    f32 bottom() const {
        return corner.y + dim.y;
    }

    vec4f center() const {
        return corner + dim/2.f;
    };

    vec4f opposite_corner() const {
        return corner + dim;
    }

    aabb4f with_margin(vec4f margin) {
        return aabb4f(
            corner + margin,
            dim - 2*margin
        );
    }

    f32 volume() const {
        return dim.x*dim.y*dim.z*dim.w;
    }

    // TODO for every size
    

    std::string to_string() {
        std::ostringstream stream;
        stream << "AABB(" << corner.to_string() << ", " << dim.to_string() << ")";
        return stream.str();
    }
};

static inline bool operator==(const aabb4f& a, const aabb4f& b) {
    return a.corner == b.corner && a.dim == b.dim;
}

static inline bool operator!=(const aabb4f& a, const aabb4f& b) {
    return !(a == b);
}


static inline aabb4f operator&(aabb4f a, const aabb4f& b) {
    a &= b;
    return a;
}

static inline aabb4f operator|(aabb4f a, const aabb4f& b) {
    a |= b;
    return a;
}


static inline bool hasnan(const aabb4f& aabb) {
    return hasnan(aabb.dim) || hasnan(aabb.corner);
}

static inline aabb4f bounding_box(std::vector<vec4f> points) {
    aabb4f bb;
    if(points.size() > 0)
        bb.corner = points[0];
    for(u32 i=1 ; i<points.size() ; i++)
        bb |= aabb4f(points[i], vec4f());
    return bb;
}








class aabb2i {
public:
    vec2i corner;
    vec2i dim;

    aabb2i() : corner(vec2i()), dim(vec2i()) {}
    aabb2i(const vec2i corner, const vec2i dim) : corner(corner), dim(dim) {}

    // forward declaration of cast constructors
    
    aabb2i(const aabb2f& r);
    

    bool contains(const vec2i p) const {
        vec2i diff = p-corner;
        return all(diff <= dim) && all(vec2i() <= diff);
    }

    bool intersects(const aabb2i& b) {
        return all(
                std::min(opposite_corner(), b.opposite_corner())
              > std::max(corner, b.corner)
        );
    }

    aabb2i& operator&=(const aabb2i& b) {
        vec2i opposite = std::min(opposite_corner(), b.opposite_corner());
        corner = std::max(corner, b.corner);
        if(any(opposite < corner))
            dim = vec2i::O;
        else
            dim = opposite-corner;
        return *this;
    }

    aabb2i& operator|=(const aabb2i& b) {
        vec2i opposite = std::max(opposite_corner(), b.opposite_corner());
        corner = std::min(corner, b.corner);
        dim = opposite-corner;
        return *this;
    }

    i32 right() const {
        return corner.x + dim.x;
    }

    i32 bottom() const {
        return corner.y + dim.y;
    }

    vec2i center() const {
        return corner + dim/2.f;
    };

    vec2i opposite_corner() const {
        return corner + dim;
    }

    aabb2i with_margin(vec2i margin) {
        return aabb2i(
            corner + margin,
            dim - 2*margin
        );
    }

    i32 volume() const {
        return dim.x*dim.y;
    }

    // TODO for every size
    
    aabb2i clip(const aabb2i& scissor) {
        vec2i top_left = vec2i {
            clamp(corner.x, scissor.corner.x, scissor.right()),
            clamp(corner.y, scissor.corner.y, scissor.bottom())
        };
        vec2i bottom_right = vec2i {
            clamp(right(), scissor.corner.x, scissor.right()),
            clamp(bottom(), scissor.corner.y, scissor.bottom())
        };
        return aabb2i(top_left, bottom_right-top_left);
    }
    

    std::string to_string() {
        std::ostringstream stream;
        stream << "AABB(" << corner.to_string() << ", " << dim.to_string() << ")";
        return stream.str();
    }
};

static inline bool operator==(const aabb2i& a, const aabb2i& b) {
    return a.corner == b.corner && a.dim == b.dim;
}

static inline bool operator!=(const aabb2i& a, const aabb2i& b) {
    return !(a == b);
}


static inline aabb2i operator&(aabb2i a, const aabb2i& b) {
    a &= b;
    return a;
}

static inline aabb2i operator|(aabb2i a, const aabb2i& b) {
    a |= b;
    return a;
}


static inline bool hasnan(const aabb2i& aabb) {
    return hasnan(aabb.dim) || hasnan(aabb.corner);
}

static inline aabb2i bounding_box(std::vector<vec2i> points) {
    aabb2i bb;
    if(points.size() > 0)
        bb.corner = points[0];
    for(u32 i=1 ; i<points.size() ; i++)
        bb |= aabb2i(points[i], vec2i());
    return bb;
}




class aabb3i {
public:
    vec3i corner;
    vec3i dim;

    aabb3i() : corner(vec3i()), dim(vec3i()) {}
    aabb3i(const vec3i corner, const vec3i dim) : corner(corner), dim(dim) {}

    // forward declaration of cast constructors
    
    aabb3i(const aabb3f& r);
    

    bool contains(const vec3i p) const {
        vec3i diff = p-corner;
        return all(diff <= dim) && all(vec3i() <= diff);
    }

    bool intersects(const aabb3i& b) {
        return all(
                std::min(opposite_corner(), b.opposite_corner())
              > std::max(corner, b.corner)
        );
    }

    aabb3i& operator&=(const aabb3i& b) {
        vec3i opposite = std::min(opposite_corner(), b.opposite_corner());
        corner = std::max(corner, b.corner);
        if(any(opposite < corner))
            dim = vec3i::O;
        else
            dim = opposite-corner;
        return *this;
    }

    aabb3i& operator|=(const aabb3i& b) {
        vec3i opposite = std::max(opposite_corner(), b.opposite_corner());
        corner = std::min(corner, b.corner);
        dim = opposite-corner;
        return *this;
    }

    i32 right() const {
        return corner.x + dim.x;
    }

    i32 bottom() const {
        return corner.y + dim.y;
    }

    vec3i center() const {
        return corner + dim/2.f;
    };

    vec3i opposite_corner() const {
        return corner + dim;
    }

    aabb3i with_margin(vec3i margin) {
        return aabb3i(
            corner + margin,
            dim - 2*margin
        );
    }

    i32 volume() const {
        return dim.x*dim.y*dim.z;
    }

    // TODO for every size
    

    std::string to_string() {
        std::ostringstream stream;
        stream << "AABB(" << corner.to_string() << ", " << dim.to_string() << ")";
        return stream.str();
    }
};

static inline bool operator==(const aabb3i& a, const aabb3i& b) {
    return a.corner == b.corner && a.dim == b.dim;
}

static inline bool operator!=(const aabb3i& a, const aabb3i& b) {
    return !(a == b);
}


static inline aabb3i operator&(aabb3i a, const aabb3i& b) {
    a &= b;
    return a;
}

static inline aabb3i operator|(aabb3i a, const aabb3i& b) {
    a |= b;
    return a;
}


static inline bool hasnan(const aabb3i& aabb) {
    return hasnan(aabb.dim) || hasnan(aabb.corner);
}

static inline aabb3i bounding_box(std::vector<vec3i> points) {
    aabb3i bb;
    if(points.size() > 0)
        bb.corner = points[0];
    for(u32 i=1 ; i<points.size() ; i++)
        bb |= aabb3i(points[i], vec3i());
    return bb;
}




class aabb4i {
public:
    vec4i corner;
    vec4i dim;

    aabb4i() : corner(vec4i()), dim(vec4i()) {}
    aabb4i(const vec4i corner, const vec4i dim) : corner(corner), dim(dim) {}

    // forward declaration of cast constructors
    
    aabb4i(const aabb4f& r);
    

    bool contains(const vec4i p) const {
        vec4i diff = p-corner;
        return all(diff <= dim) && all(vec4i() <= diff);
    }

    bool intersects(const aabb4i& b) {
        return all(
                std::min(opposite_corner(), b.opposite_corner())
              > std::max(corner, b.corner)
        );
    }

    aabb4i& operator&=(const aabb4i& b) {
        vec4i opposite = std::min(opposite_corner(), b.opposite_corner());
        corner = std::max(corner, b.corner);
        if(any(opposite < corner))
            dim = vec4i::O;
        else
            dim = opposite-corner;
        return *this;
    }

    aabb4i& operator|=(const aabb4i& b) {
        vec4i opposite = std::max(opposite_corner(), b.opposite_corner());
        corner = std::min(corner, b.corner);
        dim = opposite-corner;
        return *this;
    }

    i32 right() const {
        return corner.x + dim.x;
    }

    i32 bottom() const {
        return corner.y + dim.y;
    }

    vec4i center() const {
        return corner + dim/2.f;
    };

    vec4i opposite_corner() const {
        return corner + dim;
    }

    aabb4i with_margin(vec4i margin) {
        return aabb4i(
            corner + margin,
            dim - 2*margin
        );
    }

    i32 volume() const {
        return dim.x*dim.y*dim.z*dim.w;
    }

    // TODO for every size
    

    std::string to_string() {
        std::ostringstream stream;
        stream << "AABB(" << corner.to_string() << ", " << dim.to_string() << ")";
        return stream.str();
    }
};

static inline bool operator==(const aabb4i& a, const aabb4i& b) {
    return a.corner == b.corner && a.dim == b.dim;
}

static inline bool operator!=(const aabb4i& a, const aabb4i& b) {
    return !(a == b);
}


static inline aabb4i operator&(aabb4i a, const aabb4i& b) {
    a &= b;
    return a;
}

static inline aabb4i operator|(aabb4i a, const aabb4i& b) {
    a |= b;
    return a;
}


static inline bool hasnan(const aabb4i& aabb) {
    return hasnan(aabb.dim) || hasnan(aabb.corner);
}

static inline aabb4i bounding_box(std::vector<vec4i> points) {
    aabb4i bb;
    if(points.size() > 0)
        bb.corner = points[0];
    for(u32 i=1 ; i<points.size() ; i++)
        bb |= aabb4i(points[i], vec4i());
    return bb;
}







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




    
    inline aabb2f::aabb2f(const aabb2i& r) {
        corner = (vec2f)r.corner;
        dim = (vec2f)r.dim;
    }
    



    
    inline aabb3f::aabb3f(const aabb3i& r) {
        corner = (vec3f)r.corner;
        dim = (vec3f)r.dim;
    }
    



    
    inline aabb4f::aabb4f(const aabb4i& r) {
        corner = (vec4f)r.corner;
        dim = (vec4f)r.dim;
    }
    





    
    inline aabb2i::aabb2i(const aabb2f& r) {
        corner = (vec2i)r.corner;
        dim = (vec2i)r.dim;
    }
    



    
    inline aabb3i::aabb3i(const aabb3f& r) {
        corner = (vec3i)r.corner;
        dim = (vec3i)r.dim;
    }
    



    
    inline aabb4i::aabb4i(const aabb4f& r) {
        corner = (vec4i)r.corner;
        dim = (vec4i)r.dim;
    }
    




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



    
    
    
    
inline mat4 mat4::cast(const mat4d& other) {
    mat4 m;
    for (u32 i = 0; i < 16; i++) {
        m.data[i] = (f32)other.data[i];
    }
    return m;
}
    
    


static inline mat4 operator*(mat4 a, mat4 b)
{
    mat4 m;
    for (u32 i = 0; i < 4; i++) {
        for (u32 j = 0; j < 4; j++) {
            f32 dot = 0;
            for (u32 k = 0; k < 4; k++)
                dot += a.data[i + 4*k] * b.data[k + 4*j];
            m.data[i + 4*j] = dot;
        }
    }
    return m;
}

static inline mat4 operator*(mat4 m, quat q)
{
    mat4 qm = {
        1 - 2*q.p.y*q.p.y - 2*q.p.z*q.p.z, 2*q.p.x*q.p.y - 2*q.p.z*q.w, 2*q.p.x*q.p.z + 2*q.p.y*q.w, 0,
        2*q.p.x*q.p.y + 2*q.p.z*q.w, 1 - 2*q.p.x*q.p.x - 2*q.p.z*q.p.z, 2*q.p.y*q.p.z - 2*q.p.x*q.w, 0,
        2*q.p.x*q.p.z - 2*q.p.y*q.w, 2*q.p.y*q.p.z + 2*q.p.x*q.w, 1 - 2*q.p.x*q.p.x - 2*q.p.y*q.p.y, 0,
        0, 0, 0, 1
    };
    return m * qm;
    
}

static inline mat4 operator-(mat4 a, mat4 b)
{
    mat4 m;
    for (u32 i = 0; i < 4; i++)
        for (u32 j = 0; j < 4; j++)
            m.data[i+4*j] = a.data[i+4*j] - b.data[i+4*j];
    return m;
}


static inline vec3f operator*(mat4 m, vec3f b)
{
    vec3f r;
    f32 w;
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

static inline vec4f operator*(mat4 m, vec4f b)
{
    vec4f r;
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

static inline vec3f trans_product(mat4 m, vec3f b) {
    vec3f r;
    f32 w;
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

static inline vec4f trans_product(mat4 m, vec4f b) {
    vec4f r;
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

static inline mat4 lerp(mat4 a, mat4 b, f32 t)
{
    mat4 m;
    for (u32 i = 0; i < 16; i++)
        m.data[i] = lerp(a.data[i], b.data[i], t);
    return m;
}

inline mat4::mat4(std::initializer_list<f32> fs) {
    auto it = fs.begin();
    const f32 *in = &*it;
    for (u32 i = 0; i < 4; i++) {
        for (u32 j = 0; j < 4; j++) {
            data[i + 4*j] = in[j + 4*i];
        }
    }
}
inline mat4 mat4::id() {
    mat4 m = { 1, 0, 0, 0,
               0, 1, 0, 0,
               0, 0, 1, 0,
               0, 0, 0, 1 };
    return m;
}
inline mat4 mat4::proj(f32 n, f32 f, f32 fov, u32 w, u32 h) {
    f32 e = 1.f / tan(fov * 0.5f);
    f32 a = ((f32)h)/w;
    mat4 m = { e, 0,   0, 0,
               0, e/a, 0, 0,
               0, 0, -(f+n)/(f-n), -2*f*n/(f-n),
               0, 0, -1, 0 };
    return m;
}
inline mat4 mat4::ortho(f32 n, f32 f, f32 e, u32 w, u32 h) {
    f32 a = ((f32)h)/w;

    mat4 m = { e, 0, 0, 0,
               0, e/a, 0, 0,
               0, 0, -2.0f/(f-n), -(f+n)/(f-n),
               0, 0, 0, 1 };
    return m;
}


#if 0

static inline vec4f lu_solve(mat4 M, vec4f rhs) {
    f32 lu[16];
    for (i32 i = 0; i < 16; i++)
        lu[i] = 0;
    f32 sum = 0;
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
    f32 y[4] = {0,0,0,0};
    for (i32 i = 0; i < 4; i++) {
        sum = 0;
        for (i32 k = 0; k < i; k++)
            sum += lu[i + 4*k] * y[k];
        y[i] = rhs[i] - sum;
    }
    // find solution of Ux = y
    f32 x[4] = {0,0,0,0};
    for (i32 i = 3; i >= 0; i--) {
        sum = 0;
        for (i32 k = i + 1; k < 4; k++)
            sum += lu[i + 4*k] * x[k];
        x[i] = (1.0 / lu[i + 4*i]) * (y[i] - sum);
    }
    return vec4f { x[0], x[1], x[2], x[3] };
}

#endif

static inline vec4f gauss_solve(mat4 M, vec4f rhs) {
    for (u32 i = 0; i < 4; i++) {
        f32 max_v = abs(M.data[i + 4*i]);
        u32 max_row = i;
        for (u32 k = i+1; k < 4; k++) {
            f32 v = abs(M.data[k + 4*i]);
            if (v > max_v) {
                max_v = v;
                max_row = k;
            }
        }
        
        for (u32 k = i; k < 4; k++) {
            f32 tmp = M.data[max_row + 4*k];
            M.data[max_row + 4*k] = M.data[i + 4*k];
            M.data[i + 4*k] = tmp;
        }
        
        f32 tmp = rhs[max_row];
        rhs[max_row] = rhs[i];
        rhs[i] = tmp;
        
        for (u32 k = i+1; k < 4; k++) {
            f32 v = - M.data[k + 4*i] / M.data[i + 4*i];
            M.data[k + 4*i] = 0;
            for (u32 j = i+1; j < 4; j++) {
                M.data[k + 4*j] += v * M.data[i + 4*j];
            }
            rhs[k] += v * rhs[i];
        }
    }

    vec4f x;
    for (i32 i=3; i>=0; i--) {
        x[i] = rhs[i]/M.data[i + 4*i];
        for (i32 k=i-1;k>=0; k--) {
            rhs[k] -= M.data[k + 4*i] * x[i];
        }
    }
    return x;
}

#if 0
static inline vec3f lu_solve(mat4 M, vec3f rhs) {
    vec4f lhs4 = lu_solve(M, vec(rhs, (f32)1.0));
    return lhs4.xyz()/lhs4.w;
}
#endif


static inline vec3f inv_apply(mat4 M, vec3f rhs) {
    vec4f lhs4 = gauss_solve(M, vec(rhs, (f32)1.0));
    return lhs4.xyz()/lhs4.w;
}

static inline vec4f inv_apply(mat4 M, vec4f rhs) {
    return gauss_solve(M, rhs);
}

// from mesa

static inline mat4 inv(mat4 in)
{
    f64 inv[16];
    f32 *m_32 = in.data;
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
        in.data[i] = (f32)(inv[i] / det);

    return in;
}





    
    
inline mat4d mat4d::cast(const mat4& other) {
    mat4d m;
    for (u32 i = 0; i < 16; i++) {
        m.data[i] = (f64)other.data[i];
    }
    return m;
}
    
    
    
    


static inline mat4d operator*(mat4d a, mat4d b)
{
    mat4d m;
    for (u32 i = 0; i < 4; i++) {
        for (u32 j = 0; j < 4; j++) {
            f64 dot = 0;
            for (u32 k = 0; k < 4; k++)
                dot += a.data[i + 4*k] * b.data[k + 4*j];
            m.data[i + 4*j] = dot;
        }
    }
    return m;
}

static inline mat4d operator*(mat4d m, quat q)
{
    mat4d qm = {
        1 - 2*q.p.y*q.p.y - 2*q.p.z*q.p.z, 2*q.p.x*q.p.y - 2*q.p.z*q.w, 2*q.p.x*q.p.z + 2*q.p.y*q.w, 0,
        2*q.p.x*q.p.y + 2*q.p.z*q.w, 1 - 2*q.p.x*q.p.x - 2*q.p.z*q.p.z, 2*q.p.y*q.p.z - 2*q.p.x*q.w, 0,
        2*q.p.x*q.p.z - 2*q.p.y*q.w, 2*q.p.y*q.p.z + 2*q.p.x*q.w, 1 - 2*q.p.x*q.p.x - 2*q.p.y*q.p.y, 0,
        0, 0, 0, 1
    };
    return m * qm;
    
}

static inline mat4d operator-(mat4d a, mat4d b)
{
    mat4d m;
    for (u32 i = 0; i < 4; i++)
        for (u32 j = 0; j < 4; j++)
            m.data[i+4*j] = a.data[i+4*j] - b.data[i+4*j];
    return m;
}


static inline vec3d operator*(mat4d m, vec3d b)
{
    vec3d r;
    f64 w;
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

static inline vec4d operator*(mat4d m, vec4d b)
{
    vec4d r;
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

static inline vec3d trans_product(mat4d m, vec3d b) {
    vec3d r;
    f64 w;
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

static inline vec4d trans_product(mat4d m, vec4d b) {
    vec4d r;
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

static inline mat4d lerp(mat4d a, mat4d b, f64 t)
{
    mat4d m;
    for (u32 i = 0; i < 16; i++)
        m.data[i] = lerp(a.data[i], b.data[i], t);
    return m;
}

inline mat4d::mat4d(std::initializer_list<f64> fs) {
    auto it = fs.begin();
    const f64 *in = &*it;
    for (u32 i = 0; i < 4; i++) {
        for (u32 j = 0; j < 4; j++) {
            data[i + 4*j] = in[j + 4*i];
        }
    }
}
inline mat4d mat4d::id() {
    mat4d m = { 1, 0, 0, 0,
               0, 1, 0, 0,
               0, 0, 1, 0,
               0, 0, 0, 1 };
    return m;
}
inline mat4d mat4d::proj(f64 n, f64 f, f64 fov, u32 w, u32 h) {
    f64 e = 1.f / tan(fov * 0.5f);
    f64 a = ((f64)h)/w;
    mat4d m = { e, 0,   0, 0,
               0, e/a, 0, 0,
               0, 0, -(f+n)/(f-n), -2*f*n/(f-n),
               0, 0, -1, 0 };
    return m;
}
inline mat4d mat4d::ortho(f64 n, f64 f, f64 e, u32 w, u32 h) {
    f64 a = ((f64)h)/w;

    mat4d m = { e, 0, 0, 0,
               0, e/a, 0, 0,
               0, 0, -2.0f/(f-n), -(f+n)/(f-n),
               0, 0, 0, 1 };
    return m;
}


#if 0

static inline vec4d lu_solve(mat4d M, vec4d rhs) {
    f64 lu[16];
    for (i32 i = 0; i < 16; i++)
        lu[i] = 0;
    f64 sum = 0;
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
    f64 y[4] = {0,0,0,0};
    for (i32 i = 0; i < 4; i++) {
        sum = 0;
        for (i32 k = 0; k < i; k++)
            sum += lu[i + 4*k] * y[k];
        y[i] = rhs[i] - sum;
    }
    // find solution of Ux = y
    f64 x[4] = {0,0,0,0};
    for (i32 i = 3; i >= 0; i--) {
        sum = 0;
        for (i32 k = i + 1; k < 4; k++)
            sum += lu[i + 4*k] * x[k];
        x[i] = (1.0 / lu[i + 4*i]) * (y[i] - sum);
    }
    return vec4d { x[0], x[1], x[2], x[3] };
}

#endif

static inline vec4d gauss_solve(mat4d M, vec4d rhs) {
    for (u32 i = 0; i < 4; i++) {
        f64 max_v = abs(M.data[i + 4*i]);
        u32 max_row = i;
        for (u32 k = i+1; k < 4; k++) {
            f64 v = abs(M.data[k + 4*i]);
            if (v > max_v) {
                max_v = v;
                max_row = k;
            }
        }
        
        for (u32 k = i; k < 4; k++) {
            f64 tmp = M.data[max_row + 4*k];
            M.data[max_row + 4*k] = M.data[i + 4*k];
            M.data[i + 4*k] = tmp;
        }
        
        f64 tmp = rhs[max_row];
        rhs[max_row] = rhs[i];
        rhs[i] = tmp;
        
        for (u32 k = i+1; k < 4; k++) {
            f64 v = - M.data[k + 4*i] / M.data[i + 4*i];
            M.data[k + 4*i] = 0;
            for (u32 j = i+1; j < 4; j++) {
                M.data[k + 4*j] += v * M.data[i + 4*j];
            }
            rhs[k] += v * rhs[i];
        }
    }

    vec4d x;
    for (i32 i=3; i>=0; i--) {
        x[i] = rhs[i]/M.data[i + 4*i];
        for (i32 k=i-1;k>=0; k--) {
            rhs[k] -= M.data[k + 4*i] * x[i];
        }
    }
    return x;
}

#if 0
static inline vec3d lu_solve(mat4d M, vec3d rhs) {
    vec4d lhs4 = lu_solve(M, vec(rhs, (f64)1.0));
    return lhs4.xyz()/lhs4.w;
}
#endif


static inline vec3d inv_apply(mat4d M, vec3d rhs) {
    vec4d lhs4 = gauss_solve(M, vec(rhs, (f64)1.0));
    return lhs4.xyz()/lhs4.w;
}

static inline vec4d inv_apply(mat4d M, vec4d rhs) {
    return gauss_solve(M, rhs);
}

// from mesa

static inline mat4d inv(mat4d in)
{
    f64 inv[16];
    f64 *m_32 = in.data;
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
        in.data[i] = (f64)(inv[i] / det);

    return in;
}





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
