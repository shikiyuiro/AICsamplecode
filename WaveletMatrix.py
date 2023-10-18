class WaveletMatrix:
    def __init__(self, arg, siz = 32) -> None:
        self.siz = siz
        sarg = arg
        self.mat = [[0 for _ in range(len(sarg)+1)] for __ in range(siz)]
        for h in reversed(range(siz)):
            for i in range(len(sarg)):
                self.mat[siz-1-h][i+1] = self.mat[siz-1-h][i] + ((sarg[i] >> h) & 1)
            def skey(a):
                return (1 - ((a >> h) & 1))
            sarg = sorted(sarg, key=skey)
        self.mat.append(sarg)

    def range_freq(self, L, R, lower, upper): #半開区間[L, R) における lower以上, upper未満 の値の個数を返します
        return self.__range_freq(L, R, upper) - self.__range_freq(L, R, lower)
    
    def range_Kth_min(self, L, R, K): #半開区間[L, R) における、K番目に小さい値を返します (K=0 にて最小値と一致します)
        return self.__range_Kth_min(L, R, K)
    
    def range_successor(self, L, R, value): #半開区間[L, R) における、valueより真に大きい値のうち最小のものを返します
        C = self.__range_freq(L, R, value+1)
        return self.__range_Kth_min(L, R, C) if C != R - L else -1
    
    def range_predecessor(self, L, R, value): #半開区間[L, R) における、valueより真に小さい値のうち最大のものを返します
        C = self.__range_freq(L, R, value)
        return self.__range_Kth_min(L, R, C-1) if C != 0 else -1


    #以下、内部関数

    def __range_freq(self, L, R, upper):
        if(upper >= (1<<self.siz)): return R - L
        if(upper < 0): return 0
        res = 0
        for h in reversed(range(self.siz)):
            [L1, R1, L0, R0] = self.__decomp(L, R, self.siz-1-h)
            if(upper & (1 << h)):
                res += R0 - L0
                [L, R] = [L1, R1]
            else:
                [L, R] = [L0, R0]
            print(L, R)
        return res

    def __range_Kth_min(self, L, R, K):
        res = 0
        for h in reversed(range(self.siz)):
            [L1, R1, L0, R0] = self.__decomp(L, R, self.siz-1-h)
            if(K < R0 - L0):
                [L, R] = [L0, R0]
            else:
                K -= R0 - L0
                res = res | (1 << h)
                [L, R] = [L1, R1]
        return res

    def __decomp(self, L, R, h):
        return [self.mat[h][L], self.mat[h][R], self.mat[h][-1] + (L - self.mat[h][L]), self.mat[h][-1] + (R - self.mat[h][R])]
    
    #内部関数ここまで

#https://judge.yosupo.jp/submission/166927
N, Q = map(int, input().split())
A = [int(x) for x in input().split()]
W = WaveletMatrix(A)
for _ in range(Q):
    L, R, K = map(int, input().split())
    print(W.range_Kth_min(L, R, K))