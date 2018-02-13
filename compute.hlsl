struct Vertex {
    float3 position;
    float3 color;
};

AppendStructuredBuffer<Vertex> uav : register(u0);


void append_vertex(float3 p, float3 c) {
    Vertex v;
    v.position = p;
    v.color = c;
    uav.Append(v);
}


[numthreads(1, 1, 1)]
void main(uint3 id: SV_DispatchThreadID) {
    append_vertex(float3(0, 0, 1), float3(1, 0, 0));
    append_vertex(float3(1, 1, 1), float3(0, 1, 0));
    append_vertex(float3(0, 1, 1), float3(0, 0, 1));
}
