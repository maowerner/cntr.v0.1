#include <Gamma.h>

namespace {
std::complex<double> const I(0.0, 1.0);
}

namespace LapH {

/*! Look-up table for gamma matrices.

  For every Gamma structure (currently 0-15) the four non-zero values are specified.

  @todo Refactor that into physical quantum number class along with momentum
  */
std::vector<gamma_lookup> make_gamma() {
  std::vector<gamma_lookup> gamma(16);

  // gamma_0
  gamma[0].row[0] = 2;
  gamma[0].value[0] = 1;
  gamma[0].row[1] = 3;
  gamma[0].value[1] = 1;
  gamma[0].row[2] = 0;
  gamma[0].value[2] = 1;
  gamma[0].row[3] = 1;
  gamma[0].value[3] = 1;

  // gamma_1
  gamma[1].row[0] = 3;
  gamma[1].value[0] = -I;
  gamma[1].row[1] = 2;
  gamma[1].value[1] = -I;
  gamma[1].row[2] = 1;
  gamma[1].value[2] = I;
  gamma[1].row[3] = 0;
  gamma[1].value[3] = I;

  // gamma_2
  gamma[2].row[0] = 3;
  gamma[2].value[0] = -1;
  gamma[2].row[1] = 2;
  gamma[2].value[1] = 1;
  gamma[2].row[2] = 1;
  gamma[2].value[2] = 1;
  gamma[2].row[3] = 0;
  gamma[2].value[3] = -1;

  // gamma_3
  gamma[3].row[0] = 2;
  gamma[3].value[0] = -I;
  gamma[3].row[1] = 3;
  gamma[3].value[1] = I;
  gamma[3].row[2] = 0;
  gamma[3].value[2] = I;
  gamma[3].row[3] = 1;
  gamma[3].value[3] = -I;

  // unity
  gamma[4].row[0] = 0;
  gamma[4].value[0] = 1;
  gamma[4].row[1] = 1;
  gamma[4].value[1] = 1;
  gamma[4].row[2] = 2;
  gamma[4].value[2] = 1;
  gamma[4].row[3] = 3;
  gamma[4].value[3] = 1;

  // gamma_5
  gamma[5].row[0] = 0;
  gamma[5].value[0] = 1;
  gamma[5].row[1] = 1;
  gamma[5].value[1] = 1;
  gamma[5].row[2] = 2;
  gamma[5].value[2] = -1;
  gamma[5].row[3] = 3;
  gamma[5].value[3] = -1;

  // gamma_0 * gamma_5
  gamma[6].row[0] = 2;
  gamma[6].value[0] = -1;
  gamma[6].row[1] = 3;
  gamma[6].value[1] = -1;
  gamma[6].row[2] = 0;
  gamma[6].value[2] = 1;
  gamma[6].row[3] = 1;
  gamma[6].value[3] = 1;

  // gamma_1 * gamma_5
  gamma[7].row[0] = 3;
  gamma[7].value[0] = I;
  gamma[7].row[1] = 2;
  gamma[7].value[1] = I;
  gamma[7].row[2] = 1;
  gamma[7].value[2] = I;
  gamma[7].row[3] = 0;
  gamma[7].value[3] = I;

  // gamma_2 * gamma_5
  gamma[8].row[0] = 3;
  gamma[8].value[0] = 1;
  gamma[8].row[1] = 2;
  gamma[8].value[1] = -1;
  gamma[8].row[2] = 1;
  gamma[8].value[2] = 1;
  gamma[8].row[3] = 0;
  gamma[8].value[3] = -1;

  // gamma_3 * gamma_5
  gamma[9].row[0] = 2;
  gamma[9].value[0] = I;
  gamma[9].row[1] = 3;
  gamma[9].value[1] = -I;
  gamma[9].row[2] = 0;
  gamma[9].value[2] = I;
  gamma[9].row[3] = 1;
  gamma[9].value[3] = -I;

  // gamma_0 * gamma_1
  gamma[10].row[0] = 1;
  gamma[10].value[0] = I;
  gamma[10].row[1] = 0;
  gamma[10].value[1] = I;
  gamma[10].row[2] = 3;
  gamma[10].value[2] = -I;
  gamma[10].row[3] = 2;
  gamma[10].value[3] = -I;

  // gamma_0 * gamma_2
  gamma[11].row[0] = 1;
  gamma[11].value[0] = 1;
  gamma[11].row[1] = 0;
  gamma[11].value[1] = -1;
  gamma[11].row[2] = 3;
  gamma[11].value[2] = -1;
  gamma[11].row[3] = 2;
  gamma[11].value[3] = 1;

  // gamma_0 * gamma_3
  gamma[12].row[0] = 0;
  gamma[12].value[0] = I;
  gamma[12].row[1] = 1;
  gamma[12].value[1] = -I;
  gamma[12].row[2] = 2;
  gamma[12].value[2] = -I;
  gamma[12].row[3] = 3;
  gamma[12].value[3] = I;

  // gamma_0 * gamma_1 * gamma_5
  gamma[13].row[0] = 1;
  gamma[13].value[0] = I;
  gamma[13].row[1] = 0;
  gamma[13].value[1] = I;
  gamma[13].row[2] = 3;
  gamma[13].value[2] = I;
  gamma[13].row[3] = 2;
  gamma[13].value[3] = I;

  // gamma_0 * gamma_2 * gamma_5
  gamma[14].row[0] = 1;
  gamma[14].value[0] = 1;
  gamma[14].row[1] = 0;
  gamma[14].value[1] = -1;
  gamma[14].row[2] = 3;
  gamma[14].value[2] = 1;
  gamma[14].row[3] = 2;
  gamma[14].value[3] = -1;

  // gamma_0 * gamma_3 * gamma_5
  gamma[15].row[0] = 0;
  gamma[15].value[0] = I;
  gamma[15].row[1] = 1;
  gamma[15].value[1] = -I;
  gamma[15].row[2] = 2;
  gamma[15].value[2] = I;
  gamma[15].row[3] = 3;
  gamma[15].value[3] = -I;

  return gamma;
}

}