def edit_dp(x, y):
  n = len(x)
  m = len(y)
  dp_table = [[0 for _ in range(m + 1)] for _ in range(n + 1)]
  for i in range(n):
    for j in range(m):
      if x[i] == y[j]:
        dp_table[i + 1][j + 1] = dp[i][j]
      else:
        dp_table[i + 1][j + 1] = 1 + min(dp[i - 1][j], dp[i][j - 1])
  return dp_table[n][m]
