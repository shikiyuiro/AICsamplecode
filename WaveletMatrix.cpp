class WaveletMatrix{
    using i64 = int64_t;
public:
    explicit WaveletMatrix() = default;
    WaveletMatrix(vector<i64> &_v){ internal_WaveletMatrix(_v);};
    i64 range_freq(i64 L, i64 R, i64 lower, i64 upper){ return internal_range_freq(L, R, upper) - internal_range_freq(L, R, lower);}
    i64 range_Kthmin(i64 L, i64 R, i64 K){ return internal_range_Kthmin(L, R, K);}
    i64 range_Kthmax(i64 L, i64 R, i64 K){ return internal_range_Kthmin(L, R, R - L - K - 1);}
    i64 range_successor(i64 L, i64 R, i64 value){ i64 C = internal_range_freq(L, R, value + 1); return C == R - L ? -1 : internal_range_Kthmin(L, R, C);}
    i64 range_predecessor(i64 L, i64 R, i64 value){ i64 C = internal_range_freq(L, R, value); return C == 0 ? -1 : internal_range_Kthmin(L, R, C - 1);}
private:
    vector<vector<i64>> Matrix;
    i64 bitsize;
    i64 prefixsize;
    void internal_WaveletMatrix(vector<i64> v){
        for(auto &e : v) assert(0 <= e and e < INT64_MAX);
        i64 v_max = *max_element(v.begin(), v.end()) + 1;
        bitsize = 64 - __builtin_clzll(v_max);
        prefixsize = v.size()+1;
        Matrix.resize(bitsize, vector<i64>(prefixsize, 0));
        for(i64 h = bitsize - 1; h >= 0; h--){
            for(i64 i = 0; i < v.size(); i++){
                Matrix[h][i+1] = Matrix[h][i] + (v[i] >> h & 1);
            }
            stable_sort(v.begin(), v.end(), [&](auto a, auto b){ return (a >> h & 1) > (b >> h & 1);});
        }
    };
    i64 internal_range_freq(i64 L, i64 R, i64 upper){
        if((uint64_t)upper >= ((uint64_t)1 << bitsize)) return R - L;
        if(upper < 0) return 0;
        i64 res = 0;
        for(i64 h = bitsize - 1; h >= 0; h--){
            auto [L1, R1, L0, R0] = decomp(L, R, h);
            if(upper & ((i64)1 << h)){
                res += R0 - L0;
                tie(L, R) = {L1, R1};
            }else{
                tie(L, R) = {L0, R0};
            }
        }
        return res;
    };
    i64 internal_range_Kthmin(i64 L, i64 R, i64 K){
        i64 res = 0;
        for(i64 h = bitsize - 1; h >= 0; h--){
            auto [L1, R1, L0, R0] = decomp(L, R, h);
            if(K < R0 - L0){
                tie(L, R) = {L0, R0};
            }else{
                K -= (R0 - L0);
                res |= (i64)1 << h;
                tie(L, R) = {L1, R1};
            }
        }
        return res;
    }
    tuple<i64,i64,i64,i64> decomp(i64 L, i64 R, i64 h){
        return tuple<i64,i64,i64,i64>({
            Matrix[h][L],
            Matrix[h][R],
            Matrix[h].back() + (L - Matrix[h][L]),
            Matrix[h].back() + (R - Matrix[h][R])
        });
    }
    //https://judge.yosupo.jp/submission/167370
};
