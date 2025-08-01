#include "compressor.h"

static hash_entry_t *hash_table[HASH_TABLE_SIZE];

uint32_t hash_function(const unsigned char *data) {
  if (!data)
    return 0;

  uint32_t hash = 0;
  for (int i = 0; i < MIN_MATCH_LENGTH && data[i]; i++) {
    hash = ((hash << 8) + data[i]) % HASH_TABLE_SIZE;
  }
  return hash;
}

static void insert_hash(uint32_t pos, const unsigned char *data) {
  uint32_t hash = hash_function(data + pos);

  hash_entry_t *entry = malloc(sizeof(hash_entry_t));
  if (!entry)
    return;

  entry->position = pos;
  entry->next = hash_table[hash];
  hash_table[hash] = entry;
}

static void clear_hash_table(void) {
  for (int i = 0; i < HASH_TABLE_SIZE; i++) {
    hash_entry_t *entry = hash_table[i];
    while (entry) {
      hash_entry_t *next = entry->next;
      free(entry);
      entry = next;
    }
    hash_table[i] = NULL;
  }
}

lz77_match_t find_longest_match(const unsigned char *data, size_t pos,
                                size_t data_size,
                                hash_entry_t **hash_table_param) {
  (void)hash_table_param; // Suppress unused parameter warning
  lz77_match_t match = {0, 0, 0};

  if (pos >= data_size || pos + MIN_MATCH_LENGTH > data_size) {
    if (pos < data_size) {
      match.next_char = data[pos];
    }
    return match;
  }

  uint32_t hash = hash_function(data + pos);
  hash_entry_t *entry = hash_table[hash];

  uint16_t best_length = 0;
  uint16_t best_distance = 0;

  size_t window_start = (pos > WINDOW_SIZE) ? pos - WINDOW_SIZE : 0;

  while (entry) {
    uint32_t match_pos = entry->position;

    // Check if position is within sliding window
    if (match_pos < window_start || match_pos >= pos) {
      entry = entry->next;
      continue;
    }

    // Find match length
    uint16_t length = 0;
    size_t max_length = (data_size - pos > MAX_MATCH_LENGTH) ? MAX_MATCH_LENGTH
                                                             : data_size - pos;

    while (length < max_length &&
           data[match_pos + length] == data[pos + length]) {
      length++;
    }

    if (length >= MIN_MATCH_LENGTH && length > best_length) {
      best_length = length;
      best_distance = (uint16_t)(pos - match_pos);

      // Early exit for maximum length match
      if (length == MAX_MATCH_LENGTH) {
        break;
      }
    }

    entry = entry->next;
  }

  match.length = best_length;
  match.distance = best_distance;
  match.next_char =
      (pos + best_length < data_size) ? data[pos + best_length] : 0;

  return match;
}

int lz77_compress(const unsigned char *input, size_t input_size,
                  unsigned char **output, size_t *output_size, int level) {
  if (!input || input_size == 0 || !output || !output_size) {
    return -1;
  }

  // Clear hash table
  clear_hash_table();

  // Estimate output size (worst case: slight expansion)
  size_t estimated_size = input_size + (input_size / 8) + 1024;
  bit_buffer_t *bit_buf = create_bit_buffer(estimated_size);
  if (!bit_buf) {
    return -1;
  }

  size_t pos = 0;

  while (pos < input_size) {
    // Insert current position into hash table
    if (pos + MIN_MATCH_LENGTH <= input_size) {
      insert_hash(pos, input);
    }

    lz77_match_t match = find_longest_match(input, pos, input_size, NULL);

    if (match.length >= MIN_MATCH_LENGTH) {
      // Write match: flag(1) + distance(15) + length(8) + next_char(8)
      write_bits(bit_buf, 1, 1); // Match flag
      write_bits(bit_buf, match.distance, 15);
      write_bits(bit_buf, match.length - MIN_MATCH_LENGTH, 8);

      pos += match.length;

      // Insert intermediate positions into hash table for better compression
      if (level > LEVEL_FAST) {
        for (uint16_t i = 1;
             i < match.length &&
             pos - match.length + i + MIN_MATCH_LENGTH <= input_size;
             i++) {
          insert_hash(pos - match.length + i, input);
        }
      }
    } else {
      // Write literal: flag(0) + character(8)
      write_bits(bit_buf, 0, 1); // Literal flag
      write_bits(bit_buf, input[pos], 8);
      pos++;
    }

    // Limit hash table growth for memory efficiency
    if (pos % 4096 == 0 && level <= LEVEL_NORMAL) {
      // Periodically clear old entries to manage memory
      // This is a simplified approach; a more sophisticated implementation
      // would use a sliding window hash table
    }
  }

  // Pad the last byte if necessary
  if (bit_buf->bit_pos > 0) {
    write_bits(bit_buf, 0, 8 - bit_buf->bit_pos);
  }

  // Prepare output
  *output_size = bit_buf->byte_pos;
  *output = malloc(*output_size);
  if (!*output) {
    free_bit_buffer(bit_buf);
    clear_hash_table();
    return -1;
  }

  memcpy(*output, bit_buf->buffer, *output_size);

  // Cleanup
  free_bit_buffer(bit_buf);
  clear_hash_table();

  return 0;
}

int lz77_decompress(const unsigned char *input, size_t input_size,
                    unsigned char **output, size_t *output_size) {
  if (!input || input_size == 0 || !output || !output_size) {
    return -1;
  }

  // Create bit buffer for input
  bit_buffer_t *bit_buf = create_bit_buffer(input_size);
  if (!bit_buf) {
    return -1;
  }
  memcpy(bit_buf->buffer, input, input_size);

  // Estimate output size (assume 2x expansion as initial guess)
  size_t output_capacity = input_size * 2;
  *output = malloc(output_capacity);
  if (!*output) {
    free_bit_buffer(bit_buf);
    return -1;
  }

  size_t output_pos = 0;

  while (bit_buf->byte_pos < input_size || bit_buf->bit_pos > 0) {
    // Ensure we have enough output space
    if (output_pos + MAX_MATCH_LENGTH >= output_capacity) {
      output_capacity *= 2;
      unsigned char *new_output = realloc(*output, output_capacity);
      if (!new_output) {
        free(*output);
        *output = NULL;
        free_bit_buffer(bit_buf);
        return -1;
      }
      *output = new_output;
    }

    // Check if we can read at least 1 bit
    if (bit_buf->byte_pos >= input_size)
      break;

    uint32_t flag = read_bits(bit_buf, 1);

    if (flag == 0) {
      // Literal byte
      if (bit_buf->byte_pos >= input_size && bit_buf->bit_pos + 8 > 8) {
        break; // Not enough bits for literal
      }
      uint32_t literal = read_bits(bit_buf, 8);
      (*output)[output_pos++] = (unsigned char)literal;
    } else {
      // Match
      if (bit_buf->byte_pos + 3 > input_size) {
        break; // Not enough bits for match
      }

      uint32_t distance = read_bits(bit_buf, 15);
      uint32_t length = read_bits(bit_buf, 8) + MIN_MATCH_LENGTH;

      // Validate match parameters
      if (distance == 0 || distance > output_pos || length > MAX_MATCH_LENGTH) {
        free(*output);
        *output = NULL;
        free_bit_buffer(bit_buf);
        return -1;
      }

      // Copy match
      size_t match_start = output_pos - distance;
      for (uint32_t i = 0; i < length; i++) {
        (*output)[output_pos++] = (*output)[match_start + i];
      }
    }
  }

  // Trim output to actual size
  if (output_pos < output_capacity) {
    unsigned char *trimmed = realloc(*output, output_pos);
    if (trimmed) {
      *output = trimmed;
    }
  }

  *output_size = output_pos;
  free_bit_buffer(bit_buf);

  return 0;
}