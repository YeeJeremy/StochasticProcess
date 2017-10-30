// Copyright 2017 <jeremyyee@outlook.com.au>
// Nested simulation using input disturbances
////////////////////////////////////////////////////////////////////////////////

#include <RcppArmadillo.h>

// Subsmulation disturbances using input disturbances
//[[Rcpp::export]]
arma::cube SubsimDisturb(const arma::cube& path,
                         Rcpp::NumericVector disturb_) {
  // Extract parameters
  const std::size_t n_dec = path.n_slices;
  const std::size_t n_path = path.n_rows;
  const std::size_t n_dim = path.n_cols;
  const arma::ivec s_dims = disturb_.attr("dim");
  const std::size_t n_subsim = s_dims(2);
  const arma::cube disturb(disturb_.begin(), n_dim, n_dim * n_subsim * n_path,
                           n_dec - 1, false);
  // Perfrom the subsimulation
  // Store output as cube because field<cube> less efficient
  // Slices 0:(n_path - 1) is for the first time, and so on.
  // Will transform this to 4-d array in R
  std::size_t ll;
  arma::cube subsim(n_subsim, n_dim, n_path * (n_dec - 1));
#pragma omp parallel for private(ll)
  for (std::size_t tt = 0; tt < n_dec - 1; tt++) {
    for (std::size_t pp = 0; pp < n_path; pp++) {
      for (std::size_t ss = 0; ss < n_subsim; ss++) {
        ll = n_subsim * pp + ss;
        subsim.slice(n_path * tt + pp).row(ss) = path.slice(tt).row(pp) *
            arma::trans(disturb.slice(tt).cols(n_dim * ll, n_dim * (ll + 1) - 1));
      }
    }
  }
  // Process this to 4-D array in R
  return subsim;
}
