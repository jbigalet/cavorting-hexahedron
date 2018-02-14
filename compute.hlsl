struct Vertex {
    float3 position;
    float3 color;
};

struct Triangle {
    Vertex e[3];
};

AppendStructuredBuffer<Triangle> uav : register(u0);



cbuffer ubo {
    float time;
}



/* void append_vertex(float3 p, float3 c) { */
/*     Vertex v; */
/*     v.position = p; */
/*     v.color = c; */
/*     uav.Append(v); */
/* } */

void append_triangle(float3 a, float3 b, float3 c, float3 color) {
    /* append_vertex(a, color); */
    /* append_vertex(b, color); */
    /* append_vertex(c, color); */
    Triangle t;
    t.e[0].position = b;
    t.e[1].position = a;
    t.e[2].position = c;

    t.e[0].color = color;
    t.e[1].color = color;
    t.e[2].color = color;

    uav.Append(t);
}

void append_aabb(float3 corner, float size=1.f, float3 color=float3(1, 0, 0)) {
    append_triangle(corner + float3(0.f, size, size), corner + float3(0.f, 0.f, size), corner + float3(size, size, size), color); // Z+
    append_triangle(corner + float3(size, size, size), corner + float3(0.f, 0.f, size), corner + float3(size, 0.f, size), color);

    append_triangle(corner + float3(0.f, 0.f, 0.f), corner + float3(0.f, size, 0.f), corner + float3(size, size, 0.f), color); // Z-
    append_triangle(corner + float3(0.f, 0.f, 0.f), corner + float3(size, size, 0.f), corner + float3(size, 0.f, 0.f), color);

    append_triangle(corner + float3(size, 0.f, size), corner + float3(size, 0.f, 0.f), corner + float3(size, size, size), color); // X+
    append_triangle(corner + float3(size, size, size), corner + float3(size, 0.f, 0.f), corner + float3(size, size, 0.f), color);

    append_triangle(corner + float3(0.f, 0.f, 0.f), corner + float3(0.f, 0.f, size), corner + float3(0.f, size, size), color); // X-
    append_triangle(corner + float3(0.f, 0.f, 0.f), corner + float3(0.f, size, size), corner + float3(0.f, size, 0.f), color);

    append_triangle(corner + float3(0.f, size, 0.f), corner + float3(0.f, size, size), corner + float3(size, size, size), color); // Y+
    append_triangle(corner + float3(0.f, size, 0.f), corner + float3(size, size, size), corner + float3(size, size, 0.f), color);

    append_triangle(corner + float3(0.f, 0.f, size), corner + float3(0.f, 0.f, 0.f), corner + float3(size, 0.f, size), color); // Y-
    append_triangle(corner + float3(size, 0.f, size), corner + float3(0.f, 0.f, 0.f), corner + float3(size, 0.f, 0.f), color);
}




static const float dim = 128.f;
static const float size = 10.f;
static const float voxel_size = size/dim;

static const float radius = 0.4;
static const float radius2 = radius*radius;



float metaball(float3 pos, float3 center) {
    float3 d = pos-center;
    return 1.f/dot(d, d);
}


float val(float3 pos) {
    float3 p = pos/dim - float3(0.5, 0.5, 0.5);
    /* return dot(p,p) <= radius2 ? 1.f : 0.f; */

    float m = metaball(p, float3(0,0,0));

    float s = fmod(time/5, 1.f)-0.5;
    m += metaball(p, float3(s,s,s));

    m += metaball(p, float3(fmod(time/2, 1.f)-0.5, -(fmod(time/3, 1.f)-0.5), fmod(time/4.f, 1.f)-0.5));

    m += metaball(p, float3(cos(time/4)/2, sin(time/3)/2, cos(-time/6)/2));

    return m >= 50.f ? 1.f : 0.f;
}


bool should_draw(float3 pos) {
    /* return val(pos) > 0.5f; */

    bool inside = val(pos) > 0.5f;
    for(uint x=0 ; x<=1 ; x++)
        for(uint y=0 ; y<=1 ; y++)
            for(uint z=0 ; z<=1 ; z++)
                if(inside != (val(pos+float3(x,y,z))))
                    return true;
    return false;
}


[numthreads(1, 1, 1)]
void main(uint3 id: SV_DispatchThreadID) {
    /* append_vertex(float3(0, 0, 1), float3(1, 0, 0)); */
    /* append_vertex(float3(1, 1, 1), float3(0, 1, 0)); */
    /* append_vertex(float3(0, 1, 1), float3(0, 0, 1)); */

    if(should_draw(id))
        append_aabb((id/dim - float3(0.5, 0.5, 0.5))*size, voxel_size);
}


