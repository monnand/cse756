double data_real[1024], data_imag[1024];
double coef_real[1024];
double coef_imag[1024];

int main()
{
  int i, j, k;

  double temp_real;
  double temp_imag;
  double Wr;
  double Wi;

  for (i=0 ; i<1024 ; i++){
    data_real[i] = 0.33*i;  
    data_imag[i] = 1;
    coef_real[i] = 1;
    coef_imag[i] = 1;
  }

  int groupsPerStage = 1;
  int buttersPerGroup = 1024 / 2;

  for (i = 0; i < 10; ++i) {
    for (j = 0; j < groupsPerStage; ++j) {
      Wr = coef_real[(1<<i)-1+j];
      Wi = coef_imag[(1<<i)-1+j];
      for (k = 0; k < buttersPerGroup; ++k) {
        temp_real = Wr * data_real[2*j*buttersPerGroup+buttersPerGroup+k] -
                    Wi * data_imag[2*j*buttersPerGroup+buttersPerGroup+k];
        temp_imag = Wi * data_real[2*j*buttersPerGroup+buttersPerGroup+k] +
                    Wr * data_imag[2*j*buttersPerGroup+buttersPerGroup+k];
        data_real[2*j*buttersPerGroup+buttersPerGroup+k] =
                    data_real[2*j*buttersPerGroup+k] - temp_real;
        data_real[2*j*buttersPerGroup+k] += temp_real;
        data_imag[2*j*buttersPerGroup+buttersPerGroup+k] =
                    data_imag[2*j*buttersPerGroup+k] - temp_imag;
        data_imag[2*j*buttersPerGroup+k] += temp_imag;
      }
    }
    groupsPerStage <<= 1;
    buttersPerGroup >>= 1;
  }

  double sum = 0.0;
  for (i=0 ; i<1023 ; i++) sum+=11.1 * data_real[i]; 
  return sum;
}
