double data_real[256];
double data_imag[256];
double coef_real[256];
double coef_imag[256];
int main()
{
  int i;
  int j=0, k;

  for (i=0 ; i<256 ; i++){
  *(data_real + j) = 0.66*j;
  *(data_imag + j) = 1;
  *(coef_real + j) = 2;
  *(coef_imag + j) = 2;
  j++;
  }

  double *coef_real_ptr = coef_real, *coef_imag_ptr = coef_imag;
  double *A_real;
  double *A_imag;
  double *B_real;
  double *B_imag;
  double temp_real;
  double temp_imag;
  double Ar;
  double Ai;
  double Br;
  double Bi;
  double Wr;
  double Wi;
  int groupsPerStage = 1;
  int buttersPerGroup = 256 / 2;
  for (i = 0; i < 8; i++) {
    A_real = data_real;
    A_imag = data_imag;
    B_real = data_real + buttersPerGroup;
    B_imag = data_imag + buttersPerGroup;
    j = 0;
    do {
      Wr = *coef_real_ptr++;
      Wi = *coef_imag_ptr++;
      k = 0;
      do {
        Ar = *A_real;
        Ai = *A_imag;
        Br = *B_real;
        Bi = *B_imag;
        temp_real = Wr * Br - Wi * Bi;
        temp_imag = Wi * Br + Wr * Bi;
        *A_real++ = Ar + temp_real;
        *B_real++ = Ar - temp_real;
        *A_imag++ = Ai + temp_imag;
        *B_imag++ = Ai - temp_imag;
        k++;
      } while (k < buttersPerGroup);
      A_real += buttersPerGroup;
      A_imag += buttersPerGroup;
      B_real += buttersPerGroup;
      B_imag += buttersPerGroup;
      j++;
    } while (j < groupsPerStage);
    groupsPerStage <<= 1;
    buttersPerGroup >>= 1;
  }

  double sum = 0;
  for (i=0 ; i<255 ; i++) sum+=111.1 *data_real[i];
  return sum;
}

