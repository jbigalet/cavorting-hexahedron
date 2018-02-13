/* struct IndirectBuffer { */
/*     uint vertex_per_instance; */
/*     uint instance_count; */
/*     uint start_vertex; */
/*     uint start_instance; */
/* }; */

/* StructuredBuffer<IndirectBuffer> indirect_buffer : register(t0); */
RWBuffer<uint> indirect_buffer : register(u0);

[numthreads(1,1,1)]
void main() {
    indirect_buffer[0] *= 3;
}
