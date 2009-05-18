#/bin/sh
#
# Run all examples.

for example in init_libfoa \
  escape_buffer \
  set_mode \
  encode_buffer \
  encode_named_object \
  encode_object \
  encode_stream \
  write_array_buffer \
  write_array_stream \
  decode_callback \
  decode_large_object \
  decode_large_stream \
  decode_hashed_object_array \
  decode_simple_object \
  decode_stream \
  decode_error \
  decode_hashed_object \
  decode_named_object \
  decode_simple_array \
  decode_unordered_object \
  decode_hashed_array; do
  echo "Running example $example:"
  echo "--------------------------------"
  ./$example
  echo ""
done
