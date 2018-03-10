cbuffer ubo {
    matrix camproj;
}

struct VOut {
    float4 position : SV_POSITION;
};

float3 update_comp(float3 v, uint axis, float val) {
    if(axis == 0)
        v.x = val;
    else if(axis == 1)
        v.y = val;
    else
        v.z = val;

    return v;
}

VOut VS(uint InstanceId : SV_VertexID) {
    const uint side_size = CHUNK_DIM_X;
    const uint line_per_side = side_size+1;

    uint id = InstanceId;
    uint line_idx = id%2;
    id /= 2;
    uint axis = id%3;
    id /= 3;
    uint x = id%line_per_side;
    id /= line_per_side;
    uint y = id%line_per_side;
    id /= line_per_side;

    float3 position = 0;
    /* position[(axis+1)%3] = x; */
    /* position[(axis+2)%3] = y; */
    position = update_comp(position, (axis+1)%3, x);
    position = update_comp(position, (axis+2)%3, y);
    position -= side_size/2;

    if(line_idx == 1)
        /* position[axis] += side_size; */
        position = update_comp(position, axis, position[axis] + side_size);

    VOut output;
    output.position = mul(camproj, float4(position, 1));
    return output;
}

float4 PS(float4 p : SV_POSITION) : SV_TARGET {
    return float4(0.f, 0.f, 0.f, 1.f);
}
