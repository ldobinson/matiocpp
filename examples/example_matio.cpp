#include <matio.h>
#include <iostream>
#include <fstream>

// Compile with: g++ example_matio.cpp -o example_matio -lmatio
// Then just execute it with: ./example_matio

bool exists(const char *matFilename);

int main(void) {

  Mat_LogInit("example");

  mat_t *mat;
  matvar_t *matvar1, *matvar2;

  const char *varName1 = "vec1";
  const char *varName2 = "vec2";
  int resDel;

  size_t dims[2] = {1,250};
  const char *matFilename = "test.mat";

  double a[250] = {43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0, 43.5, 5432.434, 3.32, 0.0001, 88834.0};

  matvar1 = Mat_VarCreate(varName1,MAT_C_DOUBLE,MAT_T_DOUBLE,2,dims,a,0);
  matvar2 = Mat_VarCreate(varName2,MAT_C_DOUBLE,MAT_T_DOUBLE,2,dims,a,0);

  if (exists(matFilename)) {
    std::cout << "Mat file already exists, opening: " << matFilename << std::endl;
    mat = Mat_Open(matFilename,MAT_ACC_RDWR);

    std::cout << "Trying to rewrite existing variables without deleting (it will fail with a warning message: -E- : Variable already exists):" << std::endl;
    Mat_VarWrite(mat, matvar1, MAT_COMPRESSION_ZLIB);
    Mat_VarWrite(mat, matvar2, MAT_COMPRESSION_ZLIB);

    std::cout << "Trying to rewrite existing variables after deleting (it will work fine)" << std::endl;
    resDel = Mat_VarDelete(mat, varName1);
    resDel = Mat_VarDelete(mat, varName2);
    Mat_VarWrite(mat, matvar1, MAT_COMPRESSION_ZLIB);
    Mat_VarWrite(mat, matvar2, MAT_COMPRESSION_ZLIB);
  }
  else {
    std::cout << "Mat file does not exist, creating: " << matFilename << std::endl;
    mat = Mat_CreateVer(matFilename, NULL, MAT_FT_DEFAULT);

    std::cout << "Writing variables" << std::endl;
    Mat_VarWrite(mat, matvar1, MAT_COMPRESSION_ZLIB);
    Mat_VarWrite(mat, matvar2, MAT_COMPRESSION_ZLIB);
  }

  Mat_VarFree(matvar1);
  Mat_VarFree(matvar2);
  Mat_Close(mat);

  return 0;
}

bool exists(const char *matFilename) {
  if (FILE *file = fopen(matFilename, "r")) {
    fclose(file);
    return true;
  }
  else {
    return false;
  }
}
