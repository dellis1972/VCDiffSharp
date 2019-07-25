#include "vc-diff.h"
#ifdef WIN32
#include <fcntl.h>
#include <io.h>
#endif  // WIN32
#include <stdio.h>
#include <vector>
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

int VC_DIFF_CALL Encode (char * sourceFile, char * targetFile, char * patchFile)
{
    FILE* input_file_;
    size_t input_file_size = 0U;
    size_t buffer_size = 0U;
    FILE* source_file_;
    size_t source_file_size = 0U;
    FILE* output_file_;
    std::vector<char>* buffer;
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
        return -1;
    }

    buffer_size = static_cast<size_t>(1024);
    if (input_file_size < buffer_size) {
      buffer_size = input_file_size;
    }

    output_file_ = fopen(patchFile, "wb");
    if (!output_file_) {
      return -1;
    }

    buffer->resize(buffer_size);



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