def k_center(S, k):
  C = [ S[0] ]
  is_chosen = { 0: True }
  for i in range(k - 1):
    max_distance = 0
    max_index = 0
    for j in range(|S|):
      if is_chosen[j]:
        continue
      current_distance = Infinity
      for candidate in C:
        current_distance = min(current_distance, d(candidate, S[j]))
      if current_distance > max_distance:
        max_distance = current_distance
        max_index = j
    is_chosen[max_index] = True
    C.append(S[max_index])
  return C
