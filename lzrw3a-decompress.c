/*
 * Copyright 2023, Nick Botticelli. <nick.s.botticelli@gmail.com>
 *
 * This file is part of lzrw3a-decompress.
 *
 * lzrw3a-decompress is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * lzrw3a-decompress is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with lzrw3a-decompress. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>

#include "COMPRESS.h"

void printUsage(char *path) {
    fprintf(stderr,
            "Usage:\n"
            "%s <input file> <output file>\n",
            path
    );
}

int main(int argc, char *argv[]) {
    int result;
    char *progPath = argv[0];
    char *inputPath = argv[1];
    char *outputPath = argv[2];
    FILE *file;
    uint32_t fileSize;
    UBYTE *fileData;
    FILE *outFile;
    UBYTE *outData;
    uint32_t outSize;
    UBYTE *workingMem;
    
    if (argc != 3) {
        printUsage(progPath);
        return EXIT_FAILURE;
    }
    
    // Open input file
    file = fopen(inputPath, "rb");
    if (file == NULL) {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }
    
    // Get input file size
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    rewind(file);

    // Read input file into memory
    fileData = malloc(fileSize + 1);
    fread(fileData, fileSize, 1, file);
    fclose(file);
    
    // Allocate output buffer (upper bound is 9x input file size; this seems very inefficient)
    outData = malloc(fileSize * 9);
    
    // Decompress input data
    workingMem = malloc(lzrw3_req_mem());
    lzrw3a_compress(COMPRESS_ACTION_DECOMPRESS, workingMem, fileData, fileSize, outData, &outSize);
    
    // Create output file
    outFile = fopen(outputPath, "wb");
    if (outFile == NULL) {
        perror("Error writing output file");
        result = EXIT_FAILURE;
        goto END;
    }
    
    // Write output data to output file
    fwrite(outData, outSize, 1, outFile);
    fclose(outFile);
    
    printf("Decompressed %d bytes to \"%s\".\n", outSize, outputPath);
    result = EXIT_SUCCESS;
    
END:
    // Cleanup
    free(fileData);
    free(outData);
    free(workingMem);
    
    return result;
}
