def tabu_search():
  determine initial candidate solution s
  while temination criteria is False:
    determine set N' of non-tabu neighbours of s
    for s' in N':
      if s' is an improving candidate solution:
        with probability P ignore this and continue to the next neighbour
        update tabu table based on s'
        s = s'
        break
