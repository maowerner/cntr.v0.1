#include "QuarkLineBlock2.h"

#include <utility>
#include <iostream>

namespace {
std::complex<double> const I(0.0, 1.0);
}

template <DilutedFactorType qlt>
DilutedFactorFactory<qlt>::DilutedFactorFactory(
    RandomVector const &random_vector,
    Perambulator const &perambulator,
    OperatorFactory const &_meson_operator,
    ssize_t const dilT,
    ssize_t const dilE,
    ssize_t const nev,
    typename DilutedFactorTypeTraits<qlt>::type const &_quarkline_indices)
    : peram(perambulator),
      rnd_vec(random_vector),
      meson_operator(_meson_operator),
      dilT(dilT),
      dilE(dilE),
      nev(nev),
      quarkline_indices(_quarkline_indices) {}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// rvdaggervr is calculated by multiplying vdaggerv with the same quantum
// numbers with random vectors from right and left.
template <>
void DilutedFactorFactory<DilutedFactorType::Q0>::build(Key const &time_key) {
  int const eigenspace_dirac_size = dilD * dilE;

  auto const t1 = time_key[0];
  
  double local_timer;

  for (int operator_key = 0; operator_key < ssize(quarkline_indices); ++operator_key) {
    auto const &op = quarkline_indices[operator_key];
    const ssize_t gamma_id = op.gamma[0];

    local_timer = omp_get_wtime();

    const Eigen::MatrixXcd & vdv = meson_operator.return_vdaggerv(op.id_vdaggerv, t1);
    
    local_timer = omp_get_wtime() - local_timer;
    std::cout << "[DilutedFactorFactory::Q0] return_vdaggerv Thread " <<
      omp_get_thread_num() << " Timing " << local_timer << " seconds " << std::endl;

    ssize_t rnd_counter = 0;
    int check = -1;
    Eigen::MatrixXcd M;  // Intermediate memory

    /*! Dilution of columns */
    for (const auto &rnd_id : op.rnd_vec_ids) {
      if (check != rnd_id.second) {  // this avoids recomputation
        local_timer = omp_get_wtime();
        /*! Should be 4*nev rows, but there is always just one entry not zero */
        M = Eigen::MatrixXcd::Zero(nev, 4 * dilE);

        for (ssize_t vec_i = 0; vec_i < nev; vec_i++) {
          for (ssize_t block = 0; block < 4; block++) {
            ssize_t blk = block + (vec_i + nev * t1) * 4;
            if( op.need_vdaggerv_daggering == false ){
              M.col(vec_i % dilE + dilE * block).noalias() +=
                rnd_vec(rnd_id.second, blk) * vdv.col(vec_i);
            } else {
              M.col(vec_i % dilE + dilE * block).noalias() +=
                rnd_vec(rnd_id.second, blk) * vdv.adjoint().col(vec_i);
            }
          }
        }
        local_timer = omp_get_wtime() - local_timer;
        std::cout << "[DilutedFactorFactory::Q0] VdaggerV*r Thread " <<
          omp_get_thread_num() << " Timing " << local_timer << " seconds " << std::endl;
      }

      local_timer = omp_get_wtime();
      /*! Dilution of rows and creating a sparse matrix from smaller blocks */
      Eigen::MatrixXcd matrix =
          Eigen::MatrixXcd::Zero(eigenspace_dirac_size, eigenspace_dirac_size);
      for (ssize_t block = 0; block < 4; block++) {
        const Complex value = gamma_vec[gamma_id].value[block];
        const ssize_t gamma_index = gamma_vec[gamma_id].row[block];
        for (ssize_t vec_i = 0; vec_i < nev; vec_i++) {
          ssize_t blk = gamma_index + (vec_i + nev * t1) * dilD;
          matrix.block(vec_i % dilE + dilE * gamma_index, block * dilE, 1, dilE).noalias() +=
              value * M.block(vec_i, block * dilE, 1, dilE) *
              std::conj(rnd_vec(rnd_id.first, blk));
        }
      }
      local_timer = omp_get_wtime() - local_timer;
      std::cout << "[DilutedFactorFactory::Q0] r*VdaggerV*r Thread " <<
        omp_get_thread_num() << " Timing " << local_timer << " seconds " << std::endl;

      check = rnd_id.second;
      rnd_counter++;
      
      Ql[time_key][{operator_key}].push_back(
          {matrix, std::make_pair(rnd_id.first, rnd_id.second), {}});
    }
  }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template <>
void DilutedFactorFactory<DilutedFactorType::Q1>::build(Key const &time_key) {
  int const eigenspace_dirac_size = dilD * dilE;

  int const t1 = time_key[0];
  int const b2 = time_key[1];

  double local_timer;

  for (int operator_key = 0; operator_key < ssize(quarkline_indices); ++operator_key) {
    auto const &op = quarkline_indices[operator_key];
    for (auto const &rnd_id : op.rnd_vec_ids) {
      auto const gamma_id = op.gamma[0];

      local_timer = omp_get_wtime();
      const Eigen::MatrixXcd & vdv = meson_operator.return_vdaggerv(op.id_vdaggerv, t1);
      local_timer = omp_get_wtime() - local_timer;
      std::cout << "[DilutedFactorFactory::Q1] return_vdaggerv Thread " <<
        omp_get_thread_num() << " Timing " << local_timer << " seconds " << std::endl;

      local_timer = omp_get_wtime();
      Eigen::MatrixXcd rvdaggerv = Eigen::MatrixXcd::Zero(eigenspace_dirac_size, nev);
      for (ssize_t vec_i = 0; vec_i < nev; ++vec_i) {
        for (ssize_t block = 0; block < dilD; block++) {
          ssize_t blk = block + vec_i * dilD + dilD * nev * t1;
          if(op.need_vdaggerv_daggering == false){
            rvdaggerv.row(vec_i % dilE + dilE * block).noalias() +=
                std::conj(rnd_vec(rnd_id.first, blk)) * vdv.row(vec_i);
          } else {
            rvdaggerv.row(vec_i % dilE + dilE * block).noalias() +=
                std::conj(rnd_vec(rnd_id.first, blk)) * vdv.adjoint().row(vec_i);
          }
        }
      }
      local_timer = omp_get_wtime() - local_timer;
      std::cout << "[DilutedFactorFactory::Q1] r*VdaggerV Thread " <<
        omp_get_thread_num() << " Timing " << local_timer << " seconds " << std::endl;

      local_timer = omp_get_wtime();
      Eigen::MatrixXcd matrix =
          Eigen::MatrixXcd::Zero(eigenspace_dirac_size, eigenspace_dirac_size);
      for (int row = 0; row < dilD; row++) {
        for (int col = 0; col < dilD; col++) {
          matrix.block(row * dilE, col * dilE, dilE, dilE).noalias() =
              gamma_vec[gamma_id].value[row] * rvdaggerv.block(row * dilE, 0, dilE, nev) *
              peram[rnd_id.second].block((t1 * dilD + gamma_vec[gamma_id].row[row]) * nev,
                                         (b2 * dilD + col) * dilE,
                                         nev,
                                         dilE);
        }
      }
      local_timer = omp_get_wtime() - local_timer;
      std::cout << "[DilutedFactorFactory::Q1] r*VdaggerV*Peram*r Thread " <<
        omp_get_thread_num() << " Timing " << local_timer << " seconds " << std::endl;

      Ql[time_key][{operator_key}].push_back(
          {matrix, std::make_pair(rnd_id.first, rnd_id.second), {}});
    }
  }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
template <>
void DilutedFactorFactory<DilutedFactorType::Q2>::build(Key const &time_key) {
  int const eigenspace_dirac_size = dilD * dilE;

  auto const b1 = time_key[0];
  auto const t1 = time_key[1];
  auto const b2 = time_key[2];

  double local_timer;

  for (int operator_key = 0; operator_key < ssize(quarkline_indices); ++operator_key) {
    auto const &op = quarkline_indices[operator_key];
    ssize_t rnd_counter = 0;
    int check = -1;

    Eigen::MatrixXcd M = Eigen::MatrixXcd::Zero(dilD * dilE, 4 * nev);

    for (const auto &rnd_id : op.rnd_vec_ids) {
      if (check != rnd_id.first) {  // this avoids recomputation
        local_timer = omp_get_wtime();
        for (int row = 0; row < dilD; row++) {
          for (int col = 0; col < 4; col++) {
            if (!op.need_vdaggerv_daggering)
              M.block(col * dilE, row * nev, dilE, nev).noalias() =
                  peram[rnd_id.first]
                      .block((t1 * 4 + row) * nev, (b1 * dilD + col) * dilE, nev, dilE)
                      .adjoint() *
                  meson_operator.return_vdaggerv(op.id_vdaggerv, t1);
            else
              M.block(col * dilE, row * nev, dilE, nev).noalias() =
                  peram[rnd_id.first]
                      .block((t1 * 4 + row) * nev, (b1 * dilD + col) * dilE, nev, dilE)
                      .adjoint() *
                  meson_operator.return_vdaggerv(op.id_vdaggerv, t1).adjoint();
            // gamma_5 trick
            if (((row + col) == 3) || (abs(row - col) > 1))
              M.block(col * dilE, row * nev, dilE, nev) *= -1.;
          }
        }
        local_timer = omp_get_wtime() - local_timer;
        std::cout << "[DilutedFactorFactory::Q2] Peram_dagger*VdaggerV Thread " <<
          omp_get_thread_num() << " Timing " << local_timer << " seconds " << std::endl;
      }
     
      local_timer = omp_get_wtime(); 
      Eigen::MatrixXcd matrix =
          Eigen::MatrixXcd::Zero(eigenspace_dirac_size, eigenspace_dirac_size);

      const ssize_t gamma_id = op.gamma[0];

      for (ssize_t block_dil = 0; block_dil < dilD; block_dil++) {
        const Complex value = gamma_vec[gamma_id].value[block_dil];
        const ssize_t gamma_index = gamma_vec[gamma_id].row[block_dil];
        for (int row = 0; row < dilD; row++) {
          for (int col = 0; col < dilD; col++) {
            matrix.block(row * dilE, col * dilE, dilE, dilE).noalias() +=
                value * M.block(row * dilE, block_dil * nev, dilE, nev) *
                peram[rnd_id.second].block(
                    (t1 * 4 + gamma_index) * nev, (b2 * dilD + col) * dilE, nev, dilE);
          }
        }
      }
      local_timer = omp_get_wtime() - local_timer;
      std::cout << "[DilutedFactorFactory::Q2] r*VdaggerV*Peram*r Thread " <<
        omp_get_thread_num() << " Timing " << local_timer << " seconds " << std::endl;
      
      check = rnd_id.first;
      rnd_counter++;

      auto const rnd_pair = std::make_pair(rnd_id.first, rnd_id.second);

      Ql[time_key][{operator_key}].push_back({matrix, rnd_pair, {}});
    }
  }
}

template class DilutedFactorFactory<DilutedFactorType::Q0>;
template class DilutedFactorFactory<DilutedFactorType::Q1>;
template class DilutedFactorFactory<DilutedFactorType::Q2>;
