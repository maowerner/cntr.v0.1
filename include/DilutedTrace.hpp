#pragma once

#include "DilutedFactor.hpp"
#include "KahanAccumulator.hpp"
#include "typedefs.hpp"

struct DilutedTrace {
  using Data = Complex;

  Data data;
  UsedRnd used_rnd_ids;
};

struct DilutedTraces {
  std::vector<DilutedTrace> traces;
  bool ignore_imag;
};

inline Complex operator+(DilutedTrace const &df, Complex const &c) {
  return c + df.data;
}

inline Complex operator+(Complex const &c, DilutedTrace const &df) {
  return df + c;
}

inline Accumulator<Complex> operator+(DilutedTrace const &df,
                                      Accumulator<Complex> const &c) {
  return c + df.data;
}

inline Accumulator<Complex> operator+(Accumulator<Complex> const &c,
                                      DilutedTrace const &df) {
  return df + c;
}

/** Map from DiagramIndex.id to DilutedTrace for all random index combinations */
using DilutedTracesMap = std::map<ssize_t, std::vector<DilutedTrace>>;

Complex inner_product(DilutedTraces const &left_vec, DilutedTraces const &right_vec);

Complex inner_product(DilutedTraces const &left_vec,
                      DilutedTraces const &middle_vec,
                      DilutedTraces const &right_vec);

std::vector<DilutedTrace> factor_to_trace(std::vector<DilutedFactor> const &left_vec,
                                          std::vector<DilutedFactor> const &right_vec);

std::vector<DilutedTrace> factor_to_trace(std::vector<DilutedFactor> const &vec);
