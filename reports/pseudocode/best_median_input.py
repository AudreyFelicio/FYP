def best_median_input(S):
  best_input = None
  best_distance = Infinity
  for s1 in S:
    total_distance = 0
    for s2 in S:
      total_distance += d(s1, s2)
    if total_distance < best_distance:
      best_distance = total_distance
      best_input = s1
  return best_input
