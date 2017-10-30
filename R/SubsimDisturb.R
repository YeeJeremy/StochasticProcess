## Copyright 2017 <Jeremy Yee> <jeremyyee@outlook.com.au>
## Subsim Disturb
################################################################################

SubsimDisturb <- function(path, disturb) {
    output <- .Call('_StochasticProcess_SubsimDisturb', PACKAGE = 'StochasticProcess', path, disturb)
    ## Put output into correct format
    n_dec <- dim(path)[3]
    n_dim <- dim(path)[2]
    n_path <- dim(path)[1]
    n_subsim <- dim(disturb)[3]
    dimens <- c(n_subsim, n_dim, n_path, n_dec - 1)
    output <- array(output, dim = dimens)
    return(output)   
}
