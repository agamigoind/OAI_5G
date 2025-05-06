add_library(ldpc_segment OBJECT
  nr_rate_matching.c
  nrLDPC_coding_segment_decoder.c
  nrLDPC_coding_segment_encoder.c
)

add_library(ldpc_segment_sequential_write OBJECT
  nr_rate_matching.c
  nrLDPC_coding_segment_decoder.c
  nrLDPC_coding_segment_encoder_sequential_write.c
)

#ensure that the T header files are generated before targets depending on them
if (${T_TRACER})
  add_dependencies(ldpc_segment generate_T)
  add_dependencies(ldpc_segment_sequential_write generate_T)
endif (${T_TRACER})
