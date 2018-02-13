struct Vertex {
    float3 position;
    float3 color;
};

struct Triangle {
    Vertex e[3];
};

StructuredBuffer<Triangle> data : register(t0);


cbuffer ubo {
    matrix camproj;
}

struct VOut {
    float4 position : SV_POSITION;
    float3 color : COLOR;
};

/* VOut VS(float3 position : POSITION, float3 color : COLOR) { */
VOut VS(uint InstanceId : SV_VertexID) {
/* VOut VS(float3 position : POSITION) { */

    Vertex v = data[InstanceId/3].e[InstanceId%3];

    float3 position = v.position;
    float3 color = v.color;

    /* color = float3(1,0,0); */
    /* position = float3(0,0,0); */
    /* if(InstanceId == 0) */
    /*     position.x = 1; */
    /* if(InstanceId == 1) { */
    /*     position.x = 1; */
    /*     position.y = 1; */
    /* } */
    /* if(InstanceId == 2) { */
    /*     position.x = 1; */
    /*     position.z = 1; */
    /* } */

    VOut output;
    output.position = mul(camproj, float4(position, 1));
    /* output.position = float4(position, 1); */
    output.color = color;
    /* if(output.position.z < 3) output.color.b = 1; */
    return output;
}

float4 PS(float4 position : SV_POSITION, float3 color : COLOR) : SV_TARGET {
/* float4 PS(float4 position : SV_POSITION) : SV_TARGET { */
    /* return float4(1,0,0,1); */
    /* return float4(color, 1.f); */

    float c = position.z / position.w;
    /* float c = real_pos.z < 0.99 ? 1 : 0; */
    /* float c = real_pos.z; */
    c = 1 - pow(1-c, 10.f);
    /* float c = z == 0 ? 1 : 0; */
    return float4(float3(c,c,c), 1.f);
}
