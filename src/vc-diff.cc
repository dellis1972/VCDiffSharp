#include "vc-diff.h"
#ifdef WIN32
#include <fcntl.h>
#include <io.h>
#endif  // WIN32
#include <stdio.h>
#include <vector>
#include <string>
#include <google/vcencoder.h>
#include <google/format_extension_flags.h>

bool fileSize(FILE* file, size_t* file_size) {
  long initial_position = ftell(file);
  if (fseek(file, 0, SEEK_END) != 0) {
    return false;
  }
  *file_size = static_cast<size_t>(ftell(file));
  if (fseek(file, initial_position, SEEK_SET) != 0) {
    return false;
  }
  return true;
}

bool ReadInput(FILE* input_file_, std::vector<char>* input_buffer_, size_t* bytes_read) {
  // Read from file or stdin
  *bytes_read = fread(&input_buffer_[0], 1, input_buffer_->size(), input_file_);
  if (ferror(input_file_)) {
    return false;
  }
  return true;
}

bool WriteOutput(FILE* output_file_, const std::string& output) {
  if (!output.empty()) {
    // Some new output has been generated and is ready to be written
    // to the output file or to stdout.
    fwrite(output.data(), 1, output.size(), output_file_);
    if (ferror(output_file_)) {
      return false;
    }
  }
  return true;
}

int VC_DIFF_CALL Encode (char * sourceFile, char * targetFile, char * patchFile)
{
    FILE* input_file_;
    size_t input_file_size = 0U;
    size_t buffer_size = 0U;
    FILE* source_file_;
    size_t source_file_size = 0U;
    FILE* output_file_;
    std::vector<char> buffer;
    std::vector<char> dictionary_;

    source_file_ = fopen (sourceFile, "rb");
    if (!source_file_) {
        return -1;
    }

    if (!fileSize (source_file_, &source_file_size)) {
        return -1;
    }

    dictionary_.resize(source_file_size);
    if (source_file_size > 0) {
        if (fread(&dictionary_[0], 1, source_file_size, source_file_) != source_file_size) {
            fclose(source_file_);
            dictionary_.clear();
            return -1;
        }
    }
    fclose(source_file_);

    open_vcdiff::HashedDictionary * hashed_dictionary_ = new open_vcdiff::HashedDictionary(&dictionary_[0], dictionary_.size());
    open_vcdiff::VCDiffFormatExtensionFlags format_flags = open_vcdiff::VCD_STANDARD_FORMAT;
    open_vcdiff::VCDiffStreamingEncoder encoder(hashed_dictionary_, format_flags, true);

    input_file_ = fopen (targetFile, "rb");
    if (!input_file_) {
        return -1;
    }

    if (!fileSize (input_file_, &input_file_size)) {
        fclose (input_file_);
        return -1;
    }

    buffer_size = static_cast<size_t>(1024);
    if (input_file_size < buffer_size) {
      buffer_size = input_file_size;
    }

    output_file_ = fopen(patchFile, "wb");
    if (!output_file_) {
      fclose (input_file_);
      return -1;
    }

    buffer.resize(buffer_size);

    std::string output;
    size_t input_size = 0;
    size_t output_size = 0;
    if (!encoder.StartEncoding (&output)) {
      fclose (input_file_);
      fclose (output_file_);
      return -1;
    }

    do {
      size_t bytes_read = 0;
      if (!WriteOutput(output_file_, output) || !ReadInput(input_file_, &buffer, &bytes_read)) {
        fclose (input_file_);
        fclose (output_file_);
        return false;
      }
      output_size += output.size();
      output.clear();
      if (bytes_read > 0) {
        input_size += bytes_read;
        if (!encoder.EncodeChunk(&buffer[0], bytes_read, &output)) {
          fclose (input_file_);
          fclose (output_file_);
          return false;
        }
      }
    } while (!feof(input_file_));

    encoder.FinishEncoding(&output);
    if (!WriteOutput(output_file_, output)) {
      fclose (input_file_);
      fclose (output_file_);
      return false;
    }
    output_size += output.size();
    output.clear();

    fclose (output_file_);
    output_file_ = NULL;

    fclose (input_file_);
    input_file_ = NULL;
    
    return 0;
}

int VC_DIFF_CALL Decode (char * targetFile, char * patchFile, char * outputFile)
{
    return 0;
}