struct Vertex {
    float3 position;
    float3 color;
};

struct Triangle {
    Vertex e[3];
};

AppendStructuredBuffer<Triangle> uav : register(u0);


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


[numthreads(1, 1, 1)]
void main(uint3 id: SV_DispatchThreadID) {
    /* append_vertex(float3(0, 0, 1), float3(1, 0, 0)); */
    /* append_vertex(float3(1, 1, 1), float3(0, 1, 0)); */
    /* append_vertex(float3(0, 1, 1), float3(0, 0, 1)); */
    float3 pos = id;
    float3 p = pos - float3(8, 8, 8);
    const float radius = 8.f;
    const float radius2 = radius*radius;
    if(dot(p, p) <= radius2)
        append_aabb(float3(-8, -8, -16) + id);
}


