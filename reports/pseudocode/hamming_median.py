def hamming_median(S, n):
  median_string = ""
  for i in range(n):
    dictionary = {}
    maximum_count = 0
    for s in S:
      dictionary[s[i]] += 1
      maximum_count = max(maximum_count, dictionary[s[i]])
    for key, value in dictionary:
      if value == maximum_count:
        median_string += key
        break
  return median_string
