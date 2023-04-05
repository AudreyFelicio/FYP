def iterated_local_search():
  determine an initial candidate solution s
  perform subsidiary local search on s
  While termination criteria is False:
    r = s
    perform perturbation on s
    perform subsidiary local search on s

    if s is worse than r:
      revert s back to r
